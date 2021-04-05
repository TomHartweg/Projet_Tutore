#ifndef EVENT_CC_
#define EVENT_CC_

#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

#include "event.h"
#include "particule.h"

//-------------------------------------
//METHODES DES EVENEMENTS :
//-------------------------------------

//constucteurs
  //constructeur par default
  event::event(){
    nevent=0;
  }
  //constructeur a partir d'un indice et de 4 adresses de particules
  event::event(int n){
    nevent = n;
  }

//mutateurs
void event::addpart(particule& temp_part){
  part.resize(part.size()+1);
  part[part.size()-1]=temp_part;
}
void event::setpart(int numpart, particule& temp_part){
  part[numpart]=temp_part;
}
#endif
