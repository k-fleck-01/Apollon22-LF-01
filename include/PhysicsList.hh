#ifndef PHYSICS_LIST_H
#define PHYSICS_LIST_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PhysicsList class
// Last edited: 13/02/2022
//

#include "G4VModularPhysicsList.hh"

class PhysListEmStandard;

class PhysicsList : public G4VModularPhysicsList {
    public:
        PhysicsList();
        ~PhysicsList();

    public:
        virtual void ConstructParticle();
        virtual void ConstructProcess();

    private:
        PhysListEmStandard* fEmPhysicsList;

};

#endif