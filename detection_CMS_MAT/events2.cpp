#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
#include <ctime>
using namespace std;


#include "particule.h"//Classe des particules = particule: ID, impulsion, mass
#include "event.h" //Classe des evenements = event: nevent, part[0<=i<=3]
#include "histogramme.h" //Classe des histogrammes
#include "transformation.h" //transformation coordonnées cartésiennes à CMS


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

int main() {
  //évaluation du temps de run
  clock_t t_init = clock();

//récupération données
  ifstream entree("parametre.in");
  ifstream data("unweighted_events.lhe");
  ofstream histogramme_txt("histogramme.dat");
  ofstream histogramme_decay("part_decay.out");
  ofstream histogramme_mass("histogramme_mass.out");


  //Start by determining number of data points : count the lines
	int n_pts = 0;
	string line; //To read lines (--> count them), need to store them into a string

  //creation de l'evenenment
  event currentevent(0);
  currentevent.addpart(10);
  //dimensions du detecteur CMS (en metres) par default
  double R=1,H=1;
  entree>>R>>H; //lecture des parametres du detecteur
  //dimensions du detecteur MATHUSLA
  double DX,DY,DZ,X,Y,Z,epsilon; //longueur,hauteur,largeur, position x y z, hauteur de la bande de detection
  entree>>DX>>DY>>DZ>>X>>Y>>Z>>epsilon;
  //bornes de ct en m et pas par default
  double ctmin=0;
  double ctmax=0.1;
  double ctpas=0.01;
  entree >>ctmin>>ctmax>>ctpas; //lecture des parametres de ctau


  //distib proba temps de vie
  int seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator (seed);
  //creation histogramme

  int Nbins = floor((ctmax-ctmin)/ctpas)+1;
  histogramme histo_detect0(ctmin-ctpas/2,ctmax+ctpas/2,Nbins);
  histogramme histo_detect1(ctmin-ctpas/2,ctmax+ctpas/2,Nbins);
  histogramme histo_detect2(ctmin-ctpas/2,ctmax+ctpas/2,Nbins);

  double m_min=1.89502e+02,m_max=1.89508e+02,m_nbin=500;
  histogramme histo_mass(m_min,m_max,m_nbin);


	while(getline(data,line)) //Read each line until end of file
	{
    if (line == "<event>")
    {
      //nombre d'évènement
      n_pts++;  //compte nombre d'évènement;
      currentevent.setnevent(n_pts) ;
      int position; //variable pour la lecture

      for (int i=0 ; i<10 ; i++){ // i = numéro de la particule
        getline(data,line); //saut d'une ligne
        position = data.tellg(); //enregistrement de la position du lecteur
        //attribution ID
        double ID;
        data>>ID; //lecture
        currentevent.getpart(i).setID(ID);

        //lecture quadri-impulsion
        data.seekg(position+32); //déplacement à la position des données
        double tablect[4];
        data>>tablect[0]>>tablect[1]>>tablect[2]>>tablect[3]; //lecture

        //attribution masse
        double mass;
        data>>mass; //lecture
        currentevent.getpart(i).setmass(mass);

        //enregistrement du quadriv
        currentevent.getpart(i).setimpulsion(tablect[0],tablect[1],tablect[2],tablect[3]);
        //  cout<<(currentevent.getpart(i).gamma())<<endl;
      }
      for(int i=0; i<10; i++){ // i= numero de la particule dans l'evenement
        if(currentevent.getpart(i).getmass()>=10){ //on se sert de la masse pour savoir si la particule est un neutralino (masse>=10 GeV <=> oui)
          histo_mass.fill(currentevent.getpart(i).getmass());
          for(double k=ctmin; k<=ctmax; k+=ctpas){

            exponential_distribution<float> exp_dist (1/k);
            double c_tau=exp_dist(generator);

            //TEST DE DETECTION DES PARTICULES FILLES DU NEUTRALINO PAR LE DETECTEUR MATHUSLA
            if(currentevent.getpart(i).detectMAT(DX,DY,DZ,X,Y,Z,c_tau)==true){
              //copie des particules produites
              particule muon,electron;
              if(i==2){
                electron=currentevent.getpart(4);
                muon=currentevent.getpart(5);
              }
              else if (i==3){
                electron=currentevent.getpart(7);
                muon=currentevent.getpart(8);
              }

              if(electron.getdirectioncar(1)>0 && muon.getdirectioncar(1)>0){
                double x,y,z;// coordonees de la particule mere i au moment de sa desintegration
                x=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(0);
                y=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(1);
                z=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(2);

                double delta_x_neutralino=X-x;//par  rapport au centre de gravite de la zone de desintegration
                double delta_y_neutralino=Y+DY/2.0+epsilon-y; //par rapport au point le plus haut de la zone de detection
                double delta_z_neutralino=Z-z;//par  rapport au centre de gravite de la zone de desintegration


                bool muon_detect=true,electron_detect=true;
                int j=0;
                while((muon_detect||electron_detect) && j<5){ //boucle sur les cinqs plans de détection
                    //decalages des particules filles par rapport au point de desintegration du neutralino au moment de passer a l'horizontale des detecteurs
                    double delta_x_electron=(delta_y_neutralino-j*epsilon/4)/electron.getdirectioncar(1)*electron.getdirectioncar(0);
                    double delta_z_electron=(delta_y_neutralino-j*epsilon/4)/electron.getdirectioncar(1)*electron.getdirectioncar(2);
                    double delta_x_muon=(delta_y_neutralino-j*epsilon/4)/muon.getdirectioncar(1)*muon.getdirectioncar(0);
                    double delta_z_muon=(delta_y_neutralino-j*epsilon/4)/muon.getdirectioncar(1)*muon.getdirectioncar(2);

                  //test du passage du muon a travers la zone de detection j
                    if(((abs(delta_x_muon+delta_x_neutralino)<DX/2)&(abs(delta_z_muon+delta_z_neutralino)<DZ/2)) == false){muon_detect=false;}
                    else if(fmod(delta_x_muon+delta_x_neutralino,10)<1){muon_detect=false;}
                    else if(fmod(delta_z_muon+delta_z_neutralino,10)<1){muon_detect=false;}

                    //test du passage de l'electron a travers la zone de detection j
                    if(((abs(delta_x_electron+delta_x_neutralino)<DX/2)&(abs(delta_z_electron+delta_z_neutralino)<DZ/2)) == false){electron_detect=false;}
                    else if(fmod(delta_x_electron+delta_x_neutralino,10)<1){electron_detect=false;}
                    else if(fmod(delta_z_electron+delta_z_neutralino,10)<1){electron_detect=false;}
                    j++;
                  }
                if (muon_detect){
                  histo_detect0.fill(k);
                }
                if (electron_detect){
                  histo_detect1.fill(k);
                }
                if(electron_detect && muon_detect){
                  histo_detect2.fill(k);
                }
              }
            }

            //test de detection CMS
            if(currentevent.getpart(i).detectCMS(R,H,c_tau)==true){

            }
          }
        }
      }
    }
	}
	//Get back to beginning of file after we've read it
	data.clear();
	data.seekg(0, ios::beg);
  //cout<<n_pts<<endl;
  //cout<<currentevent.getpart(0)->getID()<<" " <<currentevent.getpart(0)->getmass()<<endl;

//sortie du nombre d'evenements dans lequel il y a 0,1,2 decay de particule
histogramme_decay<<"#"<<"ctau "<<"nb_ev_ 0_decay"<<" "<<"nb_evt_1_decay"<<" "<<"nb_evt_2_decay"<<endl;
  for(int i=0;i<Nbins-1;i++){
    histogramme_decay<<ctmin+i*ctpas<<" "<<histo_detect0.getbins(i)<<" "<<histo_detect1.getbins(i)<<" "<<histo_detect2.getbins(i)<<endl;
  }

  for(int i=0; i<m_nbin; i++){
    histogramme_mass<<setprecision(15)<<m_min+i*(m_max-m_min)/m_nbin<<" "<<histo_mass.getbins(i)<<endl;
  }

  cout<<"il y a eu "<<histo_mass.getNerror()<<" erreur(s) de remplissage"<<endl;
  cout<<"nombre d'evenements: " <<n_pts<<endl;
 //cerr<<"Nerror beta*gamma : "<<histo_gamma.getNerror()<<"; Nerror fly distance : "<<histo_flydist.getNerror()<<endl;

 //system("gnuplot command.gnu");
 clock_t t_final = clock();
 double t_cpu = (float)(t_final - t_init) / CLOCKS_PER_SEC;
 cout<<"temps de run : "<<t_cpu<<endl;

 return 0;
 }
