#ifndef _ACTION_INITIALIZATION_H_
#define _ACTION_INITIALIZATION_H_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for ActionInitialization class
// Last edited: 11/02/2022
//

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
    public:
        ActionInitialization();
        ~ActionInitialization();

    public:
        virtual void BuildForMaster() const;
        virtual void Build() const;
};

#endif