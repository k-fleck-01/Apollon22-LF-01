#ifndef _FILE_READER_HH_ 
#define _FILE_READER_HH_ 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for FileReader class
// Last edited: 08/08/2022
//
#include <vector>

#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

class FileReader {
    public:
        FileReader(G4String&, G4int&);
        ~FileReader();
    
    public:
        void ImportBeamFromFile();
        long long int GetTrackEntries() const { return fTrackEntries; }
        G4ThreeVector GetImportBeamPos(int evid) const { return fImportPos[evid]; }
        G4double      GetImportBeamEnergy(int evid) const { return fImportEnergy[evid]; }
        G4double      GetImportBeamDiv(int evid) const { return fImportDiv[evid]; }

    private:
        G4String fFileName;
        G4int fSpectrumId;
        long long int fTrackEntries;
        std::vector<G4ThreeVector> fImportPos;
        std::vector<G4double> fImportEnergy;
        std::vector<G4double> fImportDiv;
};

#endif