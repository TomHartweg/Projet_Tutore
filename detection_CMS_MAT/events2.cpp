#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

#include "quadriv.h" //Classe des quadi-impulsion = quadirv: P[0<=i<=3]
#include "particule.h"//Classe des particules = particule: ID, impulsion, mass
#include "particule.cc" //Methodes des particules
#include "event.h" //Classe des evenements = event: nevent, part[0<=i<=3]
#include "event.cc" //Methodes des evenements
#include "histogramme.h" //Classe des histogrammes
#include "histogramme.cc" //Methodes des histogrammes

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

int main() {
//récupération données
  ifstream entree("parametre.in");
  ifstream data("listevents");
  ofstream histogramme_txt("histogramme.dat");
  ofstream sortie("part_decay.out");


  //Start by determining number of data points : count the lines
	int n_pts = 0;
	string line; //To read lines (--> count them), need to store them into a string

  //creation de l'evenenment
  quadriv q0(0,0,0,0),q1(0,0,0,0),q2(0,0,0,0),q3(0,0,0,0);
  particule part0(0,&q0,0),part1(0,&q1,0),part2(0,&q2,0),part3(0,&q3,0);
  event currentevent(0,&part0,&part1,&part2,&part3);
  //dimensions du detecteur CMS (en metres) par default
  double R=1,H=1;
  entree>>R>>H; //lecture des parametres du detecteur
  //dimensions du detecteur MATHUSA
  double DX,DY,DZ,X,Y,Z;
  entree>>DX>>DY>>DZ>>X>>Y>>Z;
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


	while(getline(data,line)) //Read each line until end of file
	{
    if (line == "<event>")
    {
      //nombre d'évènement
      n_pts++;  //compte nombre d'évènement
      currentevent.setnevent(n_pts);
      int position; //variable pour la lecture

      for (int i=0 ; i<4 ; i++){ // i = numéro de la particule
        getline(data,line); //saut d'une ligne
        position = data.tellg(); //enregistrement de la position du lecteur
        //attribution ID
        double ID;
        data>>ID; //lecture
        currentevent.getppart(i)->setID(ID);

        //lecture quadri-impulsion
        data.seekg(position+32); //déplacement à la position des données
        double tablect[4];
        data>>tablect[0]>>tablect[1]>>tablect[2]>>tablect[3]; //lecture

        //attribution masse
        double mass;
        data>>mass; //lecture
        currentevent.getppart(i)->setmass(mass);

        //enregistrement du quadriv
        for( int j=0; j<4; j++) {currentevent.getppart(i)->getpimpulsion()->setcoord(j,tablect[j]);};
        //  cout<<(currentevent.getppart(i)->gamma())<<endl;
      }
          for(double k=ctmin; k<=ctmax; k+=ctpas){
            int evdetect=0; // nombre de particules detectee durant l'evenement
            exponential_distribution<double> exp_dist (1/k);
            for(int i=0; i<4; i++){
              if(currentevent.getppart(i)->gamma()>=1){//permet de ne sélectionner que les neutralinos
                //if (currentevent.getppart(i)->detectCMS(R,H,exp_dist(generator))){evdetect++;} //detection par CMS
                if (currentevent.getppart(i)->detectMAT(DX,DY,DZ,X,Y,Z,exp_dist(generator))){evdetect++;} //detection par MATHUSLA
              }//test + remplissage de l'histo
            }
            if(evdetect==0){histo_detect0.fill(k);}
            if(evdetect==1){histo_detect1.fill(k);}
            if(evdetect==2){histo_detect2.fill(k);}
      }
    }
	}
	//Get back to beginning of file after we've read it
	data.clear();
	data.seekg(0, ios::beg);
  //cout<<n_pts<<endl;
  //cout<<currentevent.getppart(0)->getID()<<" " <<currentevent.getppart(0)->getmass()<<endl;

//sortie du nombre d'evenements dans lequel il y a 0,1,2 decay de particule
sortie<<"#"<<"ctau "<<"nb_ev_ 0_decay"<<" "<<"nb_evt_1_decay"<<" "<<"nb_evt_2_decay"<<endl;
  for(int i=0;i<Nbins-1;i++){
    sortie<<ctmin+i*ctpas<<" "<<histo_detect0.getbins(i)<<" "<<histo_detect1.getbins(i)<<" "<<histo_detect2.getbins(i)<<endl;
  }
  cout<<"il y a eu "<<histo_detect0.getNerror()+histo_detect1.getNerror()+histo_detect2.getNerror()<<" erreur(s) de remplissage"<<endl;
  cout<<"nombre d'evenements: " <<n_pts<<endl;
 //cerr<<"Nerror beta*gamma : "<<histo_gamma.getNerror()<<"; Nerror fly distance : "<<histo_flydist.getNerror()<<endl;

 //system("gnuplot command.gnu");

 return 0;
 }
