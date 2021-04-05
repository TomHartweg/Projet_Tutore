#ifndef PARTICULE_CC_
#define PARTICULE_CC_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

#include "particule.h"

//-------------------------------------
//METHODES DES PARTICULES :
//-------------------------------------

//constucteurs
  //constructeur par default
  particule::particule(){
    ID=0;
    mass=0;
    impulsion.resize(4);
    }
  //constructeur a partir de pointeur de quadrivecteur
  particule::particule(int numparticule,vector<double> P,double masse){
      ID=numparticule;
      impulsion.resize(4);
      P.resize(4);
      impulsion=P;
      mass=masse;
  }
//accesseurs et mutateurs des particules
void particule::setimpulsion(double px, double py, double pz, double E){
  impulsion[0]=px;
  impulsion[1]=py;
  impulsion[2]=pz;
  impulsion[3]=E;
}
//autres methodes
  //calcul du facteur de Lorentz gamma de la particule
  double particule::gamma(){
    if (mass!=0){
      double E=impulsion[3];
      return E/mass;
    }
    else{
      return 0;
    }
  }
  //test de detection de la particule par le detecteur CMS
  bool particule::detectCMS(double R, double H, double ct){
    bool test=false; //est ce que la particule se desintegre dans le cylindre
    double px=impulsion[0];
    double py=impulsion[1];
    double pz=impulsion[2];
    double norme=sqrt(px*px+py*py+pz*pz);
    double _gamma = gamma();
    double bg = sqrt(_gamma*_gamma-1);
    double deplx=px/norme*bg*ct;//distance de desintegration selon x
    double deply=py/norme*bg*ct;//distance de desintegration selon y
    double deplz=pz/norme*bg*ct;//distance de desintegration selon z
    //cylindre de rayon R et hauteur H
    double rayon=sqrt(deplx*deplx+deply*deply); //distance de desintegration par rapport a Oz
    if(rayon<R & deplz<H/2){test=true;}
    return test;
  }
  //test de detection de la particule par le detecteur MATHUSLA
  bool particule::detectMAT(double DX, double DY, double DZ,double X,double Y, double Z, double ct){
    bool test=false; //est ce que la particule se desintegre dans le volume du pave
    double px=impulsion[0];
    double py=impulsion[1];
    double pz=impulsion[2];
    double norme=sqrt(px*px+py*py+pz*pz);
    double _gamma = gamma();
    double bg = sqrt(_gamma*_gamma-1);
    double deplx=px/norme*bg*ct;//distance de desintegration selon x
    double deply=py/norme*bg*ct;//distance de desintegration selon y
    double deplz=pz/norme*bg*ct;//distance de desintegration selon z
    //pave de cote DX,,DY,DZ centre en X,Y,Z
    bool testx=(deplx>X-DX/2)&(deplx<X+DX/2);
    bool testy=(deply>Y-DY/2)&(deply<Y+DY/2);
    bool testz=(deplz>Z-DZ/2)&(deplz<Z+DZ/2);
    if(testx&testy&testz){test=true;}
    return test;
  }

  //surcharge opérateur =
  void particule::operator=(particule& temp_part){
    ID=temp_part.getID();
    mass=temp_part.getmass();
    for (int i=0; i<4; i++){
      impulsion[i]=temp_part.getpimpulsion(i);
    }
  }

#endif
