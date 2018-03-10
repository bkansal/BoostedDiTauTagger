# include <iostream>
# include <fstream>
# include <string>
# include <sys/types.h>
# include <unistd.h>
# include <utility>
# include <vector>
# include <sys/wait.h>

# include <CLHEP/Vector/LorentzVector.h>

# include "Pythia8/Pythia.h"
# include "Pythia8Plugins/HepMC2.h"


// Total events = nUnit*nEvent
int nEvent = 1000;
int nUnit = 10;

const int strLen = 500;

std::string commonUnitName = "test";
std::string hepmcOutDir = "hepmc_out";
std::string delphesOutDir = "delphes_out";


// Maximum parallel processes
int nChildMax = 4;
int nChildRunning = 0;


int main()
{    
    // Pythia options
    Pythia8::Pythia pythia;
    
    // Pythia8 configuration file
    pythia.readFile("pythia8config/pythia8config_ZJet.txt");
    
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
            
            // Set Random number seed
            pythia.readString("Random:seed = " + std::to_string(iUnit+1));
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
            for(int iEvent = 0; iEvent < nEvent; iEvent++)
            {
                if (!pythia.next())
                {
                    continue;
                }
                
                // Units will be as chosen for HepMC build; but can be changed by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
                HepMC::GenEvent *hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::CM);
                ToHepMC.fill_next_event(pythia, hepmcevt);
                
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
