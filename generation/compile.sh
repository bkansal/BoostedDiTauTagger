#!/bin/bash

FILE=$1

if [[ $FILE = *"."* ]]; then
    
    echo "Enter filename without extension."
    exit

fi

ROOT_FLAGS=`/opt/apps/root_v5.34.36/bin/root-config --libs --cflags --ldflags`
# CLHEP_FLAGS=`/home/work/bhumika/work/shivani/CLHEP/2.2.0.8/CLHEP/clhep-config --include --ldflags --libs`
PYTHIA8_FLAGS=`/home/work/bhumika/work/shivani/Pythia8/pythia8219/bin/pythia8-config --cflags --ldflags`
HEPMC_FLAGS="-I/home/work/bhumika/work/shivani/HepMC269/install/include -L/home/work/bhumika/work/shivani/HepMC269/install/lib -lHepMC -Wl,-rpath=/home/work/bhumika/work/shivani/HepMC269/install/lib"
#LHAPDF_FLAGS=`lhapdf-config --cflags --libs`
FASTJET_FLAGS=`/home/work/bhumika/work/shivani/fastjet/fastjet-3.3.0/fastjet-config --cxxflags --libs`

# g++ -std=c++0x $FILE.cc -o $FILE $ROOT_FLAGS $CLHEP_FLAGS $PYTHIA8_FLAGS $FASTJET_FLAGS $HEPMC_FLAGS -ldl
#g++ -std=c++11 $FILE.cc -o $FILE $ROOT_FLAGS $PYTHIA8_FLAGS $FASTJET_FLAGS $HEPMC_FLAGS -ldl
g++ -std=c++11 $FILE.cc -o $FILE $ROOT_FLAGS $PYTHIA8_FLAGS $FASTJET_FLAGS $HEPMC_FLAGS -ldl
