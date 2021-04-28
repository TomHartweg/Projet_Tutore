#include <iostream> // std::cout, std::fixed
#include <iomanip> // std::setprecision
#include <math.h>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>
#include <ctime>
using namespace std;

int main() {
  ifstream data;
  ifstream parametre("parametre.in");
  ofstream result("result_mean.dat");

  int nb_fichier = 200;
  int size_data;


  double ctmin, ctmax, ctpas;
  string line;
  getline(parametre, line);
  getline(parametre, line);
  parametre>>ctmin>>ctmax>>ctpas;
  size_data = int((ctmax-ctmin)/ctpas);
  std::vector<double> moyMAT(size_data,0);
  std::vector<double> sigMAT(size_data,0);
  std::vector<double> moyCMS(size_data,0);
  std::vector<double> sigCMS(size_data,0);

  std::vector<std::vector<double>> dataMAT(nb_fichier);
  std::vector<std::vector<double>> dataCMS(nb_fichier);



  for(int i=0; i<nb_fichier; i++){
    dataMAT[i].resize(size_data,0);
    dataCMS[i].resize(size_data,0);
  }

  float a;
  //copie des donnees dans un vecteur de vecteur
  for(int i=0; i<nb_fichier; i++){

    string nom_de_fichier = "donnees/part_decay_"+to_string(i+1)+".out";
    data.open(nom_de_fichier, ifstream::in);
    cout<<nom_de_fichier<<endl;
    //data.open("part_decay_1.out");
    getline(data, line);
    for(int j=0;j<size_data;j++){data>>a>>dataMAT[i][j]>>dataCMS[i][j];}
    data.clear();
    data.close();
  }

  //analyse des donnees
  for(int i=0;i<size_data;i++){
    for(int j=0; j<nb_fichier; j++){
      moyMAT[i]+=dataMAT[j][i];
      moyCMS[i]+=dataCMS[j][i];
    }
    moyMAT[i]/=double(nb_fichier);
    moyCMS[i]/=double(nb_fichier);
    for(int j=0; j<nb_fichier; j++){
      sigMAT[i]+=(dataMAT[j][i]-moyMAT[i])*(dataMAT[j][i]-moyMAT[i]);
      sigCMS[i]+=(dataCMS[j][i]-moyCMS[i])*(dataCMS[j][i]-moyCMS[i]);
    }
    sigMAT[i]=sqrt(sigMAT[i]/double(nb_fichier));
    sigCMS[i]=sqrt(sigCMS[i]/double(nb_fichier));
  }

  result<<"# ctau  moyMAT  sigMAT  moyCMS  sigCMS"<<endl;
  for(int i=0; i<size_data; i++){
    result<<setprecision(10)<<ctmin+i*ctpas<<" "<<moyMAT[i]<<" "<<sigMAT[i]/sqrt(nb_fichier)<<" "<<moyCMS[i]<<" "<<sigCMS[i]/sqrt(nb_fichier)<<endl;
  }
  return 0;
}
