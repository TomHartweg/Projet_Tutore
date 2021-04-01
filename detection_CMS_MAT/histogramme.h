#ifndef HISTOGRAMME_H_
#define HISTOGRAMME_H_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

//-------------------------------------
//CLASSE DES HISTOGRAMMES :
//-------------------------------------

class histogramme{
private:
  float max; //maximum de l'étendue
  float min; //minimum de l'étendue
  //l'étendue est alors : [min;max[
  int Nbin; //nombre de classes requises
  float binsize; //taille des classes
  int* bins; //pointeur pour un tableau
  int Nerror;
public:
  //constructeur
  histogramme(float,float,int);
  //destructeur
  ~histogramme();
  //remplissage
  void fill(float);
  //accesseurs
  int getbins(int);
  float getbinsize();
  int getNerror();
};

#endif
