#ifndef TRANSFORMATION_CC_
#define TRANSFORMATION_CC_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

#include "fonctions.h"

std::vector<double> car_to_cms (std::vector<double>& vect_car){

  if(vect_car.size()!=3){
    cerr<<"tentative de transformation d'un mauvais vecteur "<<endl;
    return std::vector<double>(3,0);
  }
  std::vector<double> vect_cms(3);//creation du vecteur dans la base CMS
  double r,z;
  double epsilon=1e-10;//ajout de epsilon pour eviter les divisions par 0
  r=epsilon+sqrt(vect_car[0]*vect_car[0]+vect_car[1]*vect_car[1]);
  vect_cms[0]=r; //distance au faisceau rho

  if(vect_car[0]>=0){
    vect_cms[1] = atan(vect_car[1]/(vect_car[0]+epsilon)); //angle azimutal Phi
  }
  else {vect_cms[1] = M_PI-atan(vect_car[1]/(vect_car[0]+epsilon));}

  z=vect_car[2];

  vect_cms[2]=-log(sqrt((z*z)/(r*r)+1)-z/r); //pseudo-vitesse eta

  return(vect_cms);

}

#endif
