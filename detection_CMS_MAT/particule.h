#ifndef PARTICULE_H_
#define PARTICULE_H_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
using namespace std;


//-------------------------------------
//CLASSE DES PARTICULES :
//-------------------------------------

class particule {
  private :
  //variables
  int ID; //ID de la particule
  std::vector<double> impulsion; //Vecteur quadri-impulsion //est un pointeur vers un quadrivecteur
  double mass; // masse de la particule
 public:
   //constructeurs
   particule();//constucteur par default
   particule(int, vector<double> ,double); //constructeur a partir de pointeur de quadrivecteur
  //mutateurs
  void setID(int n){ID=n;};
  void setmass(double m){mass=m;};
  void setimpulsion(double,double,double,double);
  //accesseurs
  double getpimpulsion(int i){return (impulsion[i]);};
  double getmass(){return mass;};
  int getID(){return ID;};
  double gamma();
  //autres methodes
  bool detectCMS(double,double,double);//verifie la desintegration de la particule dans un cylindre selon z de dimension R,H pour un ctau donne
  bool detectMAT(double,double,double,double,double,double,double);//verifie la desintegration de la particule dans un pave de dimension DX DY DZ et position X Y Z pour un ctau donne
};

#endif
