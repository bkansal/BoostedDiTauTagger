#!/bin/bash

FILE=$1

ROOT_FLAGS=`root-config --libs --cflags --ldflags`
#CLHEP_FLAGS=`clhep-config --include --ldflags --libs`
PYTHIA8_FLAGS=`pythia8-config --cflags --ldflags`
HEPMC_FLAGS="-I/opt/HepMC-2.06.09/include -L/opt/HepMC-2.06.09/lib -lHepMC -Wl,-rpath=/opt/HepMC-2.06.09/lib"
#LHAPDF_FLAGS=`lhapdf-config --cflags --libs`
FASTJET_FLAGS=`fastjet-config --cxxflags --libs`

#g++ -std=c++11 $FILE.cc -o $FILE $ROOT_FLAGS $CLHEP_FLAGS $PYTHIA8_FLAGS $FASTJET_FLAGS $HEPMC_FLAGS -ldl
g++ -std=c++11 $FILE.cc -o $FILE $ROOT_FLAGS $PYTHIA8_FLAGS $FASTJET_FLAGS $HEPMC_FLAGS -ldl
