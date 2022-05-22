//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for QGSP_BERT_EXT class - an extension of the QGSP_BERT physics
// list to include gamma conversion and e+e- annihilation to muons.
//
// Created: 22/05/2022
// Last edited: 22/05/2022
//
#include <iomanip>

#include <CLHEP/Units/SystemOfUnits.h>
#include "globals.hh"
#include "G4ios.hh"

#include "G4DecayPhysics.hh"
#include "PhysListEmExtended.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "QGSP_BERT_EXT.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"

QGSP_BERT_EXT::QGSP_BERT_EXT(G4int ver)
{
      if(ver > 0) {
        G4cout << "<<< Geant4 Physics List simulation engine: QGSP_BERT_EXT"<<G4endl;
    G4cout <<G4endl;
  }
  defaultCutValue = 0.7*CLHEP::mm;  
  SetVerboseLevel(ver);
  // EM Physics - uses extended list
  RegisterPhysics( new PhysListEmExtended() );
  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(ver) );
  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );
   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics(ver) );
  // Hadron Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BERT(ver));
  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(ver) );
  // Ion Physics
  RegisterPhysics( new G4IonPhysics(ver));
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut(ver));
}