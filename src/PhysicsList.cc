//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PhysicsList class
// Last edited: 13/02/2022
//

#include "PhysicsList.hh"
#include "PhysListEmStandard.hh"

#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4ProcessManager.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
    SetVerboseLevel(1);
    fEmPhysicsList = new PhysListEmStandard("local");
    G4LossTableManager::Instance();

    SetDefaultCutValue(0.1*mm);
}

PhysicsList::~PhysicsList() {
    delete fEmPhysicsList;
}

void PhysicsList::ConstructParticle() {
	G4BosonConstructor      pBosonConstructor;      pBosonConstructor.ConstructParticle();
	G4LeptonConstructor     pLeptonConstructor;     pLeptonConstructor.ConstructParticle();
	G4MesonConstructor      pMesonConstructor;      pMesonConstructor.ConstructParticle();
	G4BaryonConstructor     pBaryonConstructor;     pBaryonConstructor.ConstructParticle();
	G4IonConstructor        pIonConstructor;        pIonConstructor.ConstructParticle();
	G4ShortLivedConstructor pShortLivedConstructor; pShortLivedConstructor.ConstructParticle();
}

void PhysicsList::ConstructProcess() {
	AddTransportation();
	fEmPhysicsList->ConstructProcess();
	G4EmParameters* param = G4EmParameters::Instance();
	param->SetBuildCSDARange(true);
	
}
