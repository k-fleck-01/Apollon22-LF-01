#ifndef PHYSLIST_MESSENGER_H
#define PHYSLIST_MESSENGER_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PhysListMessenger class
// Last edited: 21/02/2022
//

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysListEmExtended;
class G4UIdirectory;
class G4UIcmdWithADouble;

class PhysListMessenger : public G4UImessenger {
    public:
        PhysListMessenger(PhysListEmExtended*);
        ~PhysListMessenger();

    public:
        virtual void SetNewValue(G4UIcommand*, G4String);

    private:
        PhysListEmExtended* fPhysListEmExtended;
        G4UIdirectory* fDirectory;
        G4UIcmdWithADouble* fSetMuonFactorCmd;
};

#endif
