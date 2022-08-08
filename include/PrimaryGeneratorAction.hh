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
class FileReader;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGeneratorAction(DetectorConstruction*);
        ~PrimaryGeneratorAction();

    public:
        virtual void GeneratePrimaries(G4Event*);

        void SetInputSpectrumFlag(int flag) { fSpectrumId = flag; }
        void SetInputBeamFile(G4String&);

    private:
        G4ParticleGun* fParticleGun;
        DetectorConstruction* fDetectorConstruction;
        PrimaryGeneratorMessenger* fMessenger;
        FileReader* fFileReader;

        G4int fBeamMode;
        G4int fSpectrumId;
        
};

#endif