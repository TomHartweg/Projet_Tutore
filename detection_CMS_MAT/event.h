#ifndef EVENT_H_
#define EVENT_H_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

#include "particule.h"

//-------------------------------------
//CLASSE DES EVENEMENTS :
//-------------------------------------

class event{
private:
  int nevent;
  particule* p_part[4];
public:
  //constructeur
  event(int,particule*,particule*,particule*,particule*);
  event();
  //accesseurs
  int getnevent(){return(nevent);};
  particule* getppart(int numpart){return p_part[numpart];};
  //mutateurs
  void setnevent(int numevent){nevent=numevent;};
  void setppart(int numpart,particule* ppart){p_part[numpart]=ppart;};
};

#endif
