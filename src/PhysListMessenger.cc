//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PhysListMessenger class
// Last edited: 21/02/2022
//

#include "PhysListMessenger.hh"
#include "PhysListEmExtended.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithADouble.hh"

PhysListMessenger::PhysListMessenger(PhysListEmExtended* physlist) : G4UImessenger(), 
                                                        fPhysListEmExtended(physlist) {

    fDirectory = new G4UIdirectory("/physlist/");
    fDirectory->SetGuidance("Directory containing commands to apply cross section biasing to muon production.");

    fSetMuonFactorCmd = new G4UIcmdWithADouble("/physlist/SetMuonScaleFactor", this);
    fSetMuonFactorCmd->SetGuidance("Sets the scaling factor to bias the muon production cross sections.");
    fSetMuonFactorCmd->SetParameterName("scaleFactor", false);
    fSetMuonFactorCmd->SetRange("scaleFactor>0.");
    fSetMuonFactorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSetMuonFactorCmd->SetToBeBroadcasted(false);
}

PhysListMessenger::~PhysListMessenger() {
    delete fPhysListEmExtended;
    delete fDirectory;
    delete fSetMuonFactorCmd;
}

void PhysListMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    if (command == fSetMuonFactorCmd) { 
        fPhysListEmExtended->SetMuonScaleFactor(fSetMuonFactorCmd->GetNewDoubleValue(newValue));
    }

}

