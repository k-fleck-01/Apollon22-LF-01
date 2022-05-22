#ifndef RUN_ACTION_H
#define RUN_ACTION_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for RunAction class
// Last edited: 11/02/2022
//
#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction {
    public:
        RunAction();
        ~RunAction();

    public:
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

};

#endif