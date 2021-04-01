#ifndef HISTOGRAMME_CC_
#define HISTOGRAMME_CC_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

#include "histogramme.h"

//-------------------------------------
//METHODES DES HISTOGRAMMES :
//-------------------------------------


histogramme::histogramme(float Min, float Max, int N){
  max=Max;
  min=Min;
  Nbin=N;
  Nerror=0;

  binsize=(float(max-min)/float(Nbin)); //calcul de la taille des classes

  bins = new int[Nbin]; //création du tableau servant à récupérer les effectifs

  for(int i=0; i<Nbin; i++){
    bins[i]=0;
  }
}

histogramme::~histogramme(){
  delete[] bins;
}

void histogramme::fill(float dat_in){
if((dat_in >= max) || (dat_in < min)){
  cerr<<"histogramme : tentative de remplissage de l'histogramme avec une valeur hors bornes"<<endl;
  Nerror++;
}
else{
    int j; //indice de la classe dans laquelle doit être mise dat_in
  j = int((dat_in-min)/binsize);
   bins[j]++;
}
}

int histogramme::getbins(int i){
  if ((i >= Nbin) || (i < 0)){
    cerr<<"histogramme : tentative de lecture d'une classe inexistante"<<endl;
  }
  else return(bins[i]);
}

float histogramme::getbinsize(){
  return(binsize);
}

int histogramme::getNerror(){
  return(Nerror);
}

#endif
