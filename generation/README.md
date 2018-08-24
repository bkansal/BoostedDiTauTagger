1. In runDelphes.sh set the value of DELPHES_PATH to your Delphes path.

2. Place your Pythia8 config files in pythis8config directory but i am not changing anything here .(Normally there are two files    pythia8config_ZJet.txt and pythia8config_lhe-showering_test.txt , Since we are using lhe file as input from madgraph, we will need only second file) .

3. Place the lhe file in lhe/lhe-showering_test/ and delete the existing file (if not needed) (I am using tt_50000evts.lhe) 

4. Change the name of lhe file in sourceFiles/lhe-showering_test.txt

4. Use compile.sh to compile. Make sure the paths are correctly set in compile.sh. Syntax: ./compile.sh \<.cc file without extension>

3. Compile and run generator.cc to generate. The code is pretty well commented, and should be easy enough to follow.
