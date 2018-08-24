#ifndef CLHEP_VERSION_H
#define CLHEP_VERSION_H 1
//
// ClhepVersion.h
//
// Sample Usage:
//   std::string version = CLHEP::Version::String()
//   int mjr = CLHEP::Version::Major();
//

#include <string>

namespace CLHEP {

  struct Version {

    static std::string String() { return "2.1.3.1"; }
    static int Major()          { return 2; }
    static int SubMajor()       { return 1; }
    static int Minor()          { return 3; }
    static int SubMinor()       { return 1; }

  };

} // namespace

#endif  // CLHEP_VERSION_H
