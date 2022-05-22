#ifndef TRACKING_ACTION_H
#define TRACKING_ACTION_H 1
// *
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for TrackingAction class
// Last edited: 16/02/2022
// *

#include "G4UserTrackingAction.hh"

#include "G4ThreeVector.hh"

class RunAction;
class G4Track;

class TrackingAction : public G4UserTrackingAction {
    public:
        TrackingAction(RunAction*);
        ~TrackingAction();

    public:
        virtual void PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);

};

#endif