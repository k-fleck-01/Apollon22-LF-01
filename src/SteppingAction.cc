//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for SteppingAction class
// Last edited: 11/02/2022
//

#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4Step.hh"

SteppingAction::SteppingAction(EventAction* eventAction) : G4UserSteppingAction(), fEventAction(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep) {
    G4double edep = aStep->GetTotalEnergyDeposit()/CLHEP::keV;
    fEventAction->AddEdep(edep);
}
