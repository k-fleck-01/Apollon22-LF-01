#ifndef _EVENT_ACTION_H_
#define _EVENT_ACTION_H_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for EventAction class
// Last edited: 11/02/2022
//

#include "G4UserEventAction.hh"
#include "G4Event.hh"

class RunAction;

class EventAction : public G4UserEventAction {
    public:
        EventAction(RunAction*);
        ~EventAction();

    public:
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
        void AddEdep(G4double);
        G4double GetEdep() const;

    private:
        G4double fEdep;
};

#endif