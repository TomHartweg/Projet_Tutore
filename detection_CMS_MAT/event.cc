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
    for (int i=0; i<4; i++) p_part[i]=NULL;
  }
  //constructeur a partir d'un indice et de 4 adresses de particules
  event::event(int n,particule* a, particule* b, particule* c , particule* d){
    nevent = n;
    p_part[0]= a;
    p_part[1] = b;
    p_part[2] = c;
    p_part[3] = d;
  }

#endif
