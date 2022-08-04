#ifndef PRIMARY_GENERATOR_ACTION_H
#define PRIMARY_GENERATOR_ACTION_H 1

//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PrimaryGeneratorAction class
// Last edited: 11/02/2022
//

#include <vector>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGeneratorAction(DetectorConstruction*);
        ~PrimaryGeneratorAction();

    public:
        virtual void GeneratePrimaries(G4Event*);

        void SetInputSpectrumFlag(int flag) { fSpectrumId = flag; }
        void SetInputBeamFile(G4String& fname) { ImportBeamFromFile(fname); fBeamMode = 1; };

    private:
        G4ParticleGun* fParticleGun;
        DetectorConstruction* fDetectorConstruction;
        //PrimaryGeneratorMessenger* fMessenger;

        G4int fBeamMode;
        G4int fTrackEntries;
        G4int fSpectrumId;

        // Importing beam from file
        std::vector<G4ThreeVector> fImportPos;
        std::vector<G4double> fImportEnergy;
        std::vector<G4double> fImportDiv;

        void ImportBeamFromFile(G4String&);
        G4ThreeVector GetImportBeamPos(int evid) const { return fImportPos[evid]; };
        G4double GetImportBeamEnergy(int evid) const { return fImportEnergy[evid]; };
        G4double GetImportBeamDiv(int evid) const { return fImportDiv[evid]; };
        


};

#endif