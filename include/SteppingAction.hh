#ifndef STEPPING_ACTION_H
#define STEPPING_ACTION_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for SteppingAction class
// Last edited: 11/02/2022
//

#include "G4UserSteppingAction.hh"

class EventAction;
class G4Step;

class SteppingAction : public G4UserSteppingAction {
    public:
        SteppingAction(EventAction*);
        ~SteppingAction();

    public:
        virtual void UserSteppingAction(const G4Step*);

    private:
        EventAction* fEventAction;
};

#endif