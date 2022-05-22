#ifndef PHYS_LIST_EM_EXTENDED_H
#define PHYS_LIST_EM_EXTENDED_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for PhysListEmExtended class - an extension of the standard EM
// physics list to include gamma conversion and e+e- annihilation to muons.
//
// Created: 22/05/2022
// Last edited: 22/05/2022
//
#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class PhysListMessenger;

class PhysListEmExtended : public G4VPhysicsConstructor
{
  public: 
    PhysListEmExtended(const G4String& name = "extended");
   ~PhysListEmExtended();

  public: 
    // This method is dummy for physics
    virtual void ConstructParticle() {};
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    virtual void ConstructProcess();

    virtual void SetMuonScaleFactor(G4double);

  private:
    PhysListMessenger* fPhysListMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
