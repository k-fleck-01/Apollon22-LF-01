#ifndef _PRIMARY_GENERATOR_MESSENGER_HH_ 
#define _PRIMARY_GENERATOR_MESSENGER_HH_ 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PrimaryGeneratorMessenger class
// Last edited: 04/08/2022
//
#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class PrimaryGeneratorMessenger : public G4UImessenger {
    public:
        PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
        ~PrimaryGeneratorMessenger();

    public:
        virtual void SetNewValue(G4UIcommand*, G4String);

    private:
        PrimaryGeneratorAction* fAction;
        G4UIdirectory* fPrimaryDir;
        G4UIcmdWithAString* fImportBeamCmd;
        G4UIcmdWithAnInteger* fSpectrumInputCmd;
};

#endif