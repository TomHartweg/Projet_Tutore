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
#include "fonctions.h" //transformation coordonnées cartésiennes à CMS


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

int main() {
  //évaluation du temps de run
  clock_t t_init = clock();

//récupération données
  ifstream entree("parametre.in");

  ofstream histogramme_decay("part_decay.out");
  ofstream histogramme_decay2("part_decay2.out");
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
  uniform_real_distribution<double> unif_dist(0.0,1.0);
  exponential_distribution<float> exp_dist (1);

  //creation histogramme
  int Nbins = floor((ctmax-ctmin)/ctpas)+1;
  histogramme histo_detect_mat(ctmin-ctpas/2,ctmax+ctpas/2,Nbins);
  histogramme histo_detect_cms(ctmin-ctpas/2,ctmax+ctpas/2,Nbins);

  double m_min=1e+02,m_max=2e+02,m_nbin=500;
  histogramme histo_mass(m_min,m_max,m_nbin);



  ifstream data;
  for (int num_dat=0; num_dat<4; num_dat++){

    cout<<"lecture du fichier n°"<<num_dat<<endl;

    if(num_dat==0){data.open("unweighted_events_0.lhe", ifstream::in);}
    else if(num_dat==1){data.open("unweighted_events_1.lhe", ifstream::in);}
    else if(num_dat==2){data.open("unweighted_events_2.lhe", ifstream::in);}
    else if(num_dat==3){data.open("unweighted_events_3.lhe", ifstream::in);}

    data.clear();
  	data.seekg(0, ios::beg);

  	while(getline(data,line)) //Read each line until end of file
  	{
      if (line == "<event>")
      {
        //nombre d'évènement
        n_pts++;  //compte nombre d'évènement;
        currentevent.setnevent(n_pts) ;
        int position; //variable pour la lecture

        //LECTURE DES PARTICULES ET CREATION DE L'EVENEMENT
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

              double c_tau_norm=exp_dist(generator);


              for(double k=ctmin; k<=ctmax; k+=ctpas){
              double c_tau = c_tau_norm*k;
              //double c_tau=k;

              //TEST DE DETECTION DES PARTICULES FILLES DU NEUTRALINO PAR LE DETECTEUR MATHUSLA
              if(currentevent.getpart(i).detectMAT(DX,DY,DZ,X,Y,Z,c_tau)==true){
                //copie des particules produites
                particule muon1,muon2;
                if(i==2){
                  muon2=currentevent.getpart(4);
                  muon1=currentevent.getpart(5);
                }
                else if (i==3){
                  muon2=currentevent.getpart(7);
                  muon1=currentevent.getpart(8);
                }

                if(muon2.getdirectioncar(1)>0 && muon1.getdirectioncar(1)>0){
                  double x,y,z;// coordonees de la particule mere i au moment de sa desintegration
                  x=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(0);
                  y=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(1);
                  z=c_tau*currentevent.getpart(i).getbg()*currentevent.getpart(i).getdirectioncar(2);

                  double delta_x_neutralino=X-x;//par  rapport au centre de gravite de la zone de desintegration
                  double delta_y_neutralino=Y+DY/2.0+epsilon-y; //par rapport au point le plus haut de la zone de detection
                  double delta_z_neutralino=Z-z;//par  rapport au centre de gravite de la zone de desintegration


                  bool muon1_detect=true,muon2_detect=true;
                  int j=0;
                  while((muon1_detect||muon2_detect) && j<5){ //boucle sur les cinqs plans de détection
                      //decalages des particules filles par rapport au point de desintegration du neutralino au moment de passer a l'horizontale des detecteurs
                      double delta_x_muon2=(delta_y_neutralino-j*epsilon/4)/muon2.getdirectioncar(1)*muon2.getdirectioncar(0);
                      double delta_z_muon2=(delta_y_neutralino-j*epsilon/4)/muon2.getdirectioncar(1)*muon2.getdirectioncar(2);
                      double delta_x_muon1=(delta_y_neutralino-j*epsilon/4)/muon1.getdirectioncar(1)*muon1.getdirectioncar(0);
                      double delta_z_muon1=(delta_y_neutralino-j*epsilon/4)/muon1.getdirectioncar(1)*muon1.getdirectioncar(2);

                    //test du passage du muon1 a travers la zone de detection j
                      if(((abs(delta_x_muon1+delta_x_neutralino)<DX/2)&(abs(delta_z_muon1+delta_z_neutralino)<DZ/2)) == false){muon1_detect=false;}
                      else if(fmod(delta_x_muon1+delta_x_neutralino,10)<1){muon1_detect=false;}
                      else if(fmod(delta_z_muon1+delta_z_neutralino,10)<1){muon1_detect=false;}

                      //test du passage du muon2 a travers la zone de detection j
                      if(((abs(delta_x_muon2+delta_x_neutralino)<DX/2)&(abs(delta_z_muon2+delta_z_neutralino)<DZ/2)) == false){muon2_detect=false;}
                      else if(fmod(delta_x_muon2+delta_x_neutralino,10)<1){muon2_detect=false;}
                      else if(fmod(delta_z_muon2+delta_z_neutralino,10)<1){muon2_detect=false;}
                      j++;
                    }
                  if(muon2_detect && muon1_detect){
                    histo_detect_mat.fill(k);
                  }
                }

              }

              //TEST DE DETECTION DES PARTICULES FILLES DU NEUTRALINO PAR LE DETECTEUR CMS
              if(currentevent.getpart(i).detectCMS(R,H,c_tau)==true){
                std::vector<double> coord_car_neutralino (3);
                for (int ii=0; ii<3; ii++){coord_car_neutralino[ii]=currentevent.getpart(i).getdirectioncar(ii)*currentevent.getpart(i).getbg()*c_tau;} //point de desintegration du neutralino

                vector<double> coord_cms_neutralino(3);
                coord_cms_neutralino = car_to_cms(coord_car_neutralino);
                double eta=abs(coord_cms_neutralino[2]);

                bool pt_mort=((eta<=1.5) || (eta>=1.7)); //exclu les particules se desintegrant dans le point mort du CMS
                if((eta<=2.5) && (coord_cms_neutralino[0]<=0.6) && pt_mort==true){
                  double a1,a2,a3,a4,a5,a6,d;
                  a1=-0.003775463;
                  a2=2.62108e-5;
                  a3=-1.05512e-07;
                  a4=2.28586e-10;
                  a5=-2.54327e-13;
                  a6=1.13477e-16;

                  d = coord_cms_neutralino[0]*1000.0;

                  double rand_val = unif_dist(generator);

                  if(rand_val < a1*d+a2*pow(d,2)+a3*pow(d,3)+a4*pow(d,4)+a5*pow(d,5)+a6*pow(d,6)+1){histo_detect_cms.fill(k);}
                }

              }
            }
          }
        }
      }
  	}

    data.clear();
    data.close();
  }
	//Get back to beginning of file after we've read it




//Creation d'un histogramme de pas plus important pour MATHUSLA
int mult=10; //facteur de réduction de nombre de classes
histogramme histo_mat_mean(ctmin,ctmax+ctpas/2,floor(Nbins/mult)+1);

//sortie du nombre d'evenements dans lequel il y a 0,1,2 decay de particule
histogramme_decay<<"#"<<" ctau  MAT  CMS"<<endl;
  for(int i=0;i<Nbins-1;i++){
    histogramme_decay<<left<<setw(5)<<setfill(' ')<<setprecision(10)<<ctmin+i*ctpas<<left<<setw(10)<<setfill(' ')<<setprecision(10)<<double(histo_detect_mat.getbins(i))/(2*n_pts)<<left<<setw(8)<<setfill(' ')<<setprecision(10)<<double(histo_detect_cms.getbins(i))/(2*n_pts)<<endl;

    for (int j=1; j<=histo_detect_mat.getbins(i); j++ ){
      histo_mat_mean.fill(ctmin+i*ctpas);
    }
  }

  for(int i=0; i<m_nbin; i++){
    histogramme_mass<<setprecision(15)<<m_min+i*(m_max-m_min)/m_nbin<<" "<<histo_mass.getbins(i)<<endl;
  }
  histogramme_decay2<<"ctau nb decay"<<endl;
  for(int i=0; i<(Nbins/mult)+1;i++){
    histogramme_decay2<<setprecision(10)<<ctmin+i*(ctmax+ctpas/2-ctmin)/(floor(Nbins/mult)+1)<<"   "<<histo_mat_mean.getbins(i)<<endl;
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
