CXX      = g++

CXXFLAGS= $(shell root-config --cflags)
#LIBS    = $(shell root-config --libs) ## temp: some libs not found for root-v6, hence explicitly declaring LIBS in next line. 
LIBS = -L$(ROOTSYS)/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic 

SOURCES = NtupleVariables.cc AnalyzeBoostedTopPol.cc 
HEADERS = NtupleVariables.h AnalyzeBoostedTopPol.h 
OBJECTS = $(SOURCES:.cc=.o)

EXECUTABLE = analyzeBoostedTopPol

all: $(SOURCES) $(EXECUTABLE)

%.o: %.cc $(HEADERS)
	@echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking $(EXECUTABLE) ..."
	@echo "@$(CXX) $(LIBS) $(OBJECTS) -o $@"
	@$(CXX) -o $@ $^ $(LIBS) 
	@echo "done"

# Specifying the object files as intermediates deletes them automatically after the build process.
.INTERMEDIATE:  $(OBJECTS)

# The default target, which gives instructions, can be called regardless of whether or not files need to be updated.
.PHONY : clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

###
NtupleVariables.o: NtupleVariables.h
AnalyzeBoostedTopPol.o:NtupleVariables.h AnalyzeBoostedTopPol.h
