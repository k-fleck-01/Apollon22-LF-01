//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PhysListEmExtended class - an extension of the standard EM
// physics list to include gamma conversion and e+e- annihilation to muons.
//
// Created: 22/05/2022
// Last edited: 22/05/2022
//
#include "PhysListEmExtended.hh"
#include "PhysListMessenger.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4KleinNishinaModel.hh"
#include "G4GammaConversionToMuons.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4AnnihiToMuPair.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4MscStepLimitType.hh"

#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4BuilderType.hh"

#include "G4SystemOfUnits.hh"
#include "G4ProcessTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmExtended::PhysListEmExtended(const G4String& name)
    :   G4VPhysicsConstructor(name)
{
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetVerbose(1);
  param->SetMinEnergy(10 * eV);
  param->SetMaxEnergy(10 * TeV);
  param->SetLowestElectronEnergy(100 * eV);
  param->SetNumberOfBinsPerDecade(20);
  param->SetMscStepLimitType(fUseSafetyPlus);
  param->SetFluo(true);
  param->SetAuger(false);
  param->SetPixe(false);
  SetPhysicsType(bElectromagnetic);

  fPhysListMessenger = new PhysListMessenger(this); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmExtended::~PhysListEmExtended()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmExtended::ConstructProcess()
{
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

    // Add standard EM Processes
    //
    auto particleIterator = GetParticleIterator();
    particleIterator->reset();
    while ((*particleIterator)()) {
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();

        if (particleName == "gamma") {

            ////ph->RegisterProcess(new G4RayleighScattering, particle);      
            ph->RegisterProcess(new G4PhotoElectricEffect, particle);
            G4ComptonScattering* cs = new G4ComptonScattering;
            cs->SetEmModel(new G4KleinNishinaModel());
            ph->RegisterProcess(cs, particle);
            ph->RegisterProcess(new G4GammaConversion, particle);
            ph->RegisterProcess(new G4GammaConversionToMuons(), particle);

        }
        else if (particleName == "e-") {

            ph->RegisterProcess(new G4eMultipleScattering(), particle);
            //            
            G4eIonisation* eIoni = new G4eIonisation();
            eIoni->SetStepFunction(0.1, 100 * um);
            ph->RegisterProcess(eIoni, particle);
            //
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);

        }
        else if (particleName == "e+") {

            ph->RegisterProcess(new G4eMultipleScattering(), particle);
            //     
            G4eIonisation* eIoni = new G4eIonisation();
            eIoni->SetStepFunction(0.1, 100 * um);
            ph->RegisterProcess(eIoni, particle);
            //
            ph->RegisterProcess(new G4eBremsstrahlung(), particle);
            //
            ph->RegisterProcess(new G4eplusAnnihilation(), particle);
            ph->RegisterProcess(new G4AnnihiToMuPair(), particle);

        }
        else if (particleName == "mu+" ||
            particleName == "mu-") {

            ph->RegisterProcess(new G4MuMultipleScattering(), particle);
            G4MuIonisation* muIoni = new G4MuIonisation();
            muIoni->SetStepFunction(0.1, 50 * um);
            ph->RegisterProcess(muIoni, particle);
            ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
            ph->RegisterProcess(new G4MuPairProduction(), particle);

        }
        else if (particleName == "proton" ||
            particleName == "pi-" ||
            particleName == "pi+") {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            G4hIonisation* hIoni = new G4hIonisation();
            hIoni->SetStepFunction(0.1, 20 * um);
            ph->RegisterProcess(hIoni, particle);
            ph->RegisterProcess(new G4hBremsstrahlung(), particle);
            ph->RegisterProcess(new G4hPairProduction(), particle);

        }
        else if (particleName == "alpha" ||
            particleName == "He3") {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetStepFunction(0.1, 1 * um);
            ph->RegisterProcess(ionIoni, particle);
            ph->RegisterProcess(new G4NuclearStopping(), particle);

        }
        else if (particleName == "GenericIon") {

            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            G4ionIonisation* ionIoni = new G4ionIonisation();
            ionIoni->SetEmModel(new G4IonParametrisedLossModel());
            ionIoni->SetStepFunction(0.1, 1 * um);
            ph->RegisterProcess(ionIoni, particle);
            ph->RegisterProcess(new G4NuclearStopping(), particle);

        }
        else if ((!particle->IsShortLived()) &&
            (particle->GetPDGCharge() != 0.0) &&
            (particle->GetParticleName() != "chargedgeantino")) {

            //all others charged particles except geantino
            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4hIonisation(), particle);
        }
    }

    // Deexcitation
    //
    G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
    G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmExtended::SetMuonScaleFactor(G4double scale) {

    G4ProcessTable* theProcessTable = G4ProcessTable::GetProcessTable();

    G4GammaConversionToMuons* GammaToMuPairProcess  = 
                    (G4GammaConversionToMuons*) theProcessTable->FindProcess("GammaToMuPair", "gamma");
    G4AnnihiToMuPair* AnnihiToMuPairProcess = 
                    (G4AnnihiToMuPair*) theProcessTable->FindProcess("AnnihiToMuPair", "e+");

    if (GammaToMuPairProcess) GammaToMuPairProcess->SetCrossSecFactor(scale);
    if (AnnihiToMuPairProcess) AnnihiToMuPairProcess->SetCrossSecFactor(scale);

    G4cout << "G4GammaConversionToMuons and G4AnnihiToMuPair processes haven been scaled by: " << 
            scale << G4endl;
}