#ifndef ACTION_INITIALIZATION_H
#define ACTION_INITIALIZATION_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for ActionInitialization class
// Last edited: 11/02/2022
//

#include "G4VUserActionInitialization.hh"

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization {
    public:
        ActionInitialization(DetectorConstruction*);
        ~ActionInitialization();

    public:
        virtual void BuildForMaster() const;
        virtual void Build() const;

    private:
        DetectorConstruction* fDetector;
};

#endif