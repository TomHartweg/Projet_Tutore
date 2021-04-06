all: particule.o event.o histogramme.o
	g++  particule.o event.o histogramme.o events2.cpp -o events2.exe
particule.o:
	g++ -c particule.cc
event.o:
	g++ -c event.cc
histogramme.o:
	g++ -c histogramme.cc
clean:
	rm *.o
