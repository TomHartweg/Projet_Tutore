#ifndef EVENT_H_
#define EVENT_H_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <vector>
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
  std::vector<particule> part;
public:
  //constructeur
  event(int);
  event();
  //accesseurs
  int getnevent(){return(nevent);};
  particule& getppart(int numpart){return part[numpart];};
  //mutateurs
  void setnevent(int numevent){nevent=numevent;};
  void addpart(particule&);
  void addpart(int i){part.resize(part.size()+i);};
  void setpart(int, particule&);
};

#endif
