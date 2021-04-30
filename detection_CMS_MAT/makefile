all: transformation.o particule.o event.o histogramme.o Mean.exe
	g++ -Os transformation.o particule.o event.o histogramme.o events2.cpp -o events2.exe
particule.o:
	g++ -c particule.cc
event.o:
	g++ -c event.cc
histogramme.o:
	g++ -c histogramme.cc
transformation.o:
	g++ -c transformation.cc
Mean.exe:
	g++ DATAs/Mean.cpp -o DATAs/Mean.exe
clean:
	rm *.o
