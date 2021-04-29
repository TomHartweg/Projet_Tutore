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
  ifstream parametre("../parametre.in");
  ofstream result("result_mean.dat");

  int nb_fichier = 100;
  int size_data;


  double ctmin, ctmax, ctpas;
  string line;
  getline(parametre, line);
  getline(parametre, line);
  parametre>>ctmin>>ctmax>>ctpas;
  size_data = int((ctmax-ctmin)/ctpas);
  std::vector<double> moyMAT(size_data,0);
  std::vector<double> sigMAT(size_data,0);
  std::vector<double> moyMAT_simple(size_data,0);
  std::vector<double> sigMAT_simple(size_data,0);
  std::vector<double> moyCMS(size_data,0);
  std::vector<double> sigCMS(size_data,0);
  std::vector<double> moyCMS_muon(size_data,0);
  std::vector<double> sigCMS_muon(size_data,0);

  std::vector<std::vector<double>> dataMAT(nb_fichier);
  std::vector<std::vector<double>> dataMAT_simple(nb_fichier);
  std::vector<std::vector<double>> dataCMS(nb_fichier);
  std::vector<std::vector<double>> dataCMS_muon(nb_fichier);



  for(int i=0; i<nb_fichier; i++){
    dataMAT[i].resize(size_data,0);
    dataMAT_simple[i].resize(size_data,0);
    dataCMS[i].resize(size_data,0);
    dataCMS_muon[i].resize(size_data,0);
  }

  float a;
  //copie des donnees dans un tableau
  for(int i=0; i<nb_fichier; i++){

    string nom_de_fichier = "donnees/part_decay_"+to_string(i+1)+".out";
    data.open(nom_de_fichier, ifstream::in);
    cout<<nom_de_fichier<<endl;
    //data.open("part_decay_1.out");
    getline(data, line);
    for(int j=0;j<size_data;j++){data>>a>>dataMAT[i][j]>>dataMAT_simple[i][j]>>dataCMS[i][j]>>dataCMS_muon[i][j];}
    data.clear();
    data.close();
  }

  //analyse des donnees
  for(int i=0;i<size_data;i++){
    for(int j=0; j<nb_fichier; j++){
      moyMAT[i]+=dataMAT[j][i];
      moyMAT_simple[i]+=dataMAT_simple[j][i];
      moyCMS[i]+=dataCMS[j][i];
      moyCMS_muon[i]+=dataCMS_muon[j][i];
    }
    moyMAT[i]/=double(nb_fichier);
    moyMAT_simple[i]/=double(nb_fichier);

    moyCMS[i]/=double(nb_fichier);
    moyCMS_muon[i]/=double(nb_fichier);

    for(int j=0; j<nb_fichier; j++){
      sigMAT[i]+=(dataMAT[j][i]-moyMAT[i])*(dataMAT[j][i]-moyMAT[i]);
      sigMAT_simple[i]+=(dataMAT_simple[j][i]-moyMAT_simple[i])*(dataMAT_simple[j][i]-moyMAT_simple[i]);
      sigCMS[i]+=(dataCMS[j][i]-moyCMS[i])*(dataCMS[j][i]-moyCMS[i]);
      sigCMS_muon[i]+=(dataCMS_muon[j][i]-moyCMS_muon[i])*(dataCMS_muon[j][i]-moyCMS_muon[i]);
    }
    sigMAT[i]=sqrt(sigMAT[i]/double(nb_fichier));
    sigMAT_simple[i]=sqrt(sigMAT_simple[i]/double(nb_fichier));
    sigCMS[i]=sqrt(sigCMS[i]/double(nb_fichier));
    sigCMS_muon[i]=sqrt(sigCMS_muon[i]/double(nb_fichier));
  }

  result<<"# ctau  moyMAT  sigMAT  moyCMS  sigCMS"<<endl;
  for(int i=0; i<size_data; i++){
    result<<setprecision(10)<<ctmin+i*ctpas<<" ";
    result<<moyMAT[i]<<" "<<sigMAT[i]<<" ";
    result<<moyMAT_simple[i]<<" "<<sigMAT_simple[i]<<" ";
    result<<moyCMS[i]<<" "<<sigCMS[i]<<" ";
    result<<moyCMS_muon[i]<<" "<<sigCMS_muon[i]<<" "<<endl;
  }
  return 0;
}
