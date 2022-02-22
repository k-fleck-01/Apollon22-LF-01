#ifndef _PHYSLIST_MESSENGER_H_
#define _PHYSLIST_MESSENGER_H_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PhysListMessenger class
// Last edited: 21/02/2022
//

#include "globals.hh"
#include "G4UImessenger.hh"

class PhysListEmStandard;
class G4UIdirectory;
class G4UIcmdWithADouble;

class PhysListMessenger : public G4UImessenger {
    public:
        PhysListMessenger(PhysListEmStandard*);
        ~PhysListMessenger();

    public:
        virtual void SetNewValue(G4UIcommand*, G4String);

    private:
        PhysListEmStandard* fPhysListEmStandard;
        G4UIdirectory* fDirectory;
        G4UIcmdWithADouble* fSetMuonFactorCmd;
};

#endif
