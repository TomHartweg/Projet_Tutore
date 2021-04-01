#ifndef QUADRIV_H_
#define QUADRIV_H_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

//-------------------------------------
//CLASSE DES QUADRIVECTEURS :
//-------------------------------------

class quadriv{
  private:
  //variables
  double P[4];
  public:
  //constucteurs
  quadriv(); //constructeur par default
  quadriv(double,double,double,double); //constructeur a partir des 4 coordonnees
  //accesseur
  double* getcoord(int indice){return &(P[indice]);};
  //mutateur
  void setcoord(int indice,double valeur){P[indice]=valeur;}
};
//constucteurs de quadriv
  //constructeur par default
  quadriv::quadriv(){
    for (int i=0; i<4; i++){
      P[i]=0;
    }
  }
  //constructeur a partir des 4 coordonnees
  quadriv::quadriv(double Px,double Py,double Pz,double Energie){
    P[0]=Px;
    P[1]=Py;
    P[2]=Pz;
    P[3]=Energie;
  }

#endif
