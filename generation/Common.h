# ifndef Common_H
# define Common_H


# include <iostream>
# include <fstream>
# include <string>
# include <utility>
# include <vector>

# include "CLHEP/Vector/Vector/LorentzVector.h"


namespace Common
{
    void parseFileByLine(std::string fileName, std::vector <std::string> *v_line)
    {
        std::ifstream lineStream;
        lineStream.open(fileName);
        
        std::string line;
        
        while(std::getline(lineStream, line))
        {
            v_line->push_back(line);
        }
    }
}


# endif
