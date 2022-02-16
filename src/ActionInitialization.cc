//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for ActionInitialization class
// Last edited: 11/02/2022
//

#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() 
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const {
	
	RunAction* runAction = new RunAction();
	SetUserAction(runAction);
	return;
}

void ActionInitialization::Build() const {

    PrimaryGeneratorAction* primaryGenerator = new PrimaryGeneratorAction();
	SetUserAction(primaryGenerator);
	
	RunAction* runAction = new RunAction();
	SetUserAction(runAction);

	EventAction* eventAction = new EventAction(runAction);
	SetUserAction(eventAction);

	SteppingAction* steppingAction = new SteppingAction(eventAction);
	SetUserAction(steppingAction);

	TrackingAction* trackingAction = new TrackingAction(runAction);
	SetUserAction(trackingAction);

    return;
}
