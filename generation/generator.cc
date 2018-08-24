# include <iostream>
# include <fstream>
# include <string>
# include <sys/types.h>
# include <unistd.h>
# include <utility>
# include <vector>
# include <sys/wait.h>

// # include "/home/work/bhumika/work/shivani/CLHEP/2.1.3.1/CLHEP/Vector/Vector/LorentzVector.h"
# include "CLHEP/Vector/Vector/LorentzVector.h"

# include "/home/work/bhumika/work/shivani/Pythia8/pythia8219/include/Pythia8/Pythia.h"
# include "/home/work/bhumika/work/shivani/Pythia8/pythia8219/include/Pythia8Plugins/HepMC2.h"

# include "Common.h"

// Total events = nUnit*nEvent
// Ignore if using LHE files
int nEvent = 50000;
int nUnit = 1;

// Whether using LHE file or not
bool fromLHE = true;

// Text file containing the list of the LHe files to be used
std::string lheListFile = "sourceFiles/lhe-showering_test.txt";
std::vector <std::string> v_lheFile;

// Maximum number of events tp process per LHE file
// Set to -1 to process all
int nLHE_max = 10;

const int strLen = 50000;


// Output directories
std::string hepmcOutDir = "hepmc_out";
std::string delphesOutDir = "delphes_out";

// Process name
// Will create output directory inside hepmcOutDir and delphesOutDir with this name
std::string processName = "test-showering";

// Output file name
std::string commonUnitName = processName;


// Maximum parallel processes
// Setting a value greater than the number of cores won't help
int nChildMax = 3;
int nChildRunning = 0;


int main()
{
    hepmcOutDir += "/" + processName;
    delphesOutDir += "/" + processName;
    
    // Pythia options
    Pythia8::Pythia pythia;
    
    // Pythia8 configuration file
    if(fromLHE)
    {
        pythia.readFile("pythia8config/pythia8config_lhe-showering_test.txt");
        Common::parseFileByLine(lheListFile, &v_lheFile);
        
        nUnit = v_lheFile.size();
        printf("Will process %d LHE files. \n", nUnit);
    }
    
    else
    {
         pythia.readFile("pythia8config/pythia8config_ZJet.txt");
    }
    
    // Set explicitly, just in case
    pythia.readString("Random:setSeed = on");
    
    char command[500];
    
    // Make hepmc_out and delphes_out directories
    sprintf(command, "mkdir -p %s", hepmcOutDir.c_str());
    system(command);
    sprintf(command, "mkdir -p %s", delphesOutDir.c_str());
    system(command);
    
    char ouputRootFileName[strLen];
    sprintf(ouputRootFileName, "%s/%s_delphes_out", delphesOutDir.c_str(), commonUnitName.c_str());
    
    int status;
    pid_t pid;
    
    for(int iUnit = 0; iUnit < nUnit; iUnit++)
    {
        nChildRunning++;
        
        fflush(stdout);
        pid = fork();
        
        // Parent
        if(pid > 0)
        {
            // Limit the maximum number of parallely running children
            for(; nChildRunning >= nChildMax; nChildRunning--)
            {
                //printf("nChildRunning %d \n", nChildRunning);
                wait(&status);
            }
            
            //printf("Parent: %d, FID %d, PID %d, PPID %d \n", iUnit, pid, getpid(), getppid());
        }
        
        // Child
        else if(pid == 0)
        {
            //printf("Child: %d, FID %d, PID %d, PPID %d \n", iUnit, pid, getpid(), getppid());
            
            if(fromLHE)
            {
                //if(iUnit > 0)
                //{
                //    pythia.readString("Beams:newLHEFsameInit = on");
                //}
                
                // Read next LHE file
                pythia.readString("Beams:LHEF = " + v_lheFile.at(iUnit));
            }

            
            // Set Random number seed
            pythia.readString("Random:seed = " + std::to_string(iUnit+1));
            
            // Intialize Pythia
            pythia.init();
            
            char hepmcFileName[strLen], delphesFileName[strLen];
            
            // Generate output file names
            sprintf(hepmcFileName, "%s/%s_hepmc_out_%d.dat", hepmcOutDir.c_str(), commonUnitName.c_str(), iUnit+1);
            sprintf(delphesFileName, "%s_%d.root", ouputRootFileName, iUnit+1);
            
            // Interface for conversion from Pythia8::Event to HepMC event.
            HepMC::Pythia8ToHepMC ToHepMC;
            
            // Must be a pointer. Otherwise cannot delete, and the file is not closed properly, leading to an error in Delphes.
            HepMC::IO_GenEvent *ascii_io = new HepMC::IO_GenEvent(hepmcFileName, std::ios::out);
            
            // Event loop
            for(int iEvent = 0; ; iEvent++)
            {
                // Process nLHE_max events per LHE file
                if(fromLHE)
                {
                    if(nLHE_max > 0 && iEvent >= nLHE_max)
                    {
                        break;
                    }
                }
                
                // Process nEvent events per unit if not from LHE
                else
                {
                    if(iEvent >= nEvent)
                    {
                        break;
                    }
                }
                
                if(!pythia.next())
                {
                    // Break event loop if the LHE file's end is reached
                    if(fromLHE && pythia.info.atEndOfFile())
                    {
                        break;
                    }
                    
                    continue;
                }
                
                // Units will be as chosen for HepMC build; but can be changed by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
                HepMC::GenEvent *hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::CM);
                ToHepMC.fill_next_event(pythia.event, hepmcevt);
                
                // Write the HepMC event to file
                ascii_io->write_event(hepmcevt);
                
                delete hepmcevt;
            }
            
            pythia.stat();
            
            delete ascii_io;
            
            sprintf(command, "./runDelphes.sh %s %s", hepmcFileName, delphesFileName);
            printf("%s \n", command);
            system(command);
            
            break;
        }
        
        else
        {
            printf("Error: Could not create child. \n");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}
