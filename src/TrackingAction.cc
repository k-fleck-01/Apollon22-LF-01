// *
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for TrackingAction class
// Last edited: 16/02/2022
// *

#include "TrackingAction.hh"
#include "RunAction.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "G4EmProcessSubType.hh"
#include "G4SystemOfUnits.hh"

#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"

TrackingAction::TrackingAction(RunAction*) : G4UserTrackingAction()
{}

TrackingAction::~TrackingAction()
{}

void TrackingAction::PreUserTrackingAction(const G4Track*) 
{}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {

    // Get detector ID; return if not in a sensitive volume
    const G4StepPoint* preStepPoint = track->GetStep()->GetPreStepPoint();
    const G4String& logicName = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    G4int ldet = 0;
    if (logicName == "lYagScreen") {
        ldet = 1000;
    }
    else if (logicName == "lStack") {
        ldet = 2000;
    }
    else if (logicName == "lLanexSheet") {
        ldet = 3000;
    }
    else {
        return;
    }

    G4int detid = ldet + preStepPoint->GetPhysicalVolume()->GetCopyNo() + 1;    
    G4int trackid = track->GetTrackID();
    G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
    G4int procid = 2000;
    G4ThreeVector primaryVertex = track->GetVertexPosition();
    G4ThreeVector endVertex = track->GetPosition();
    G4double kEnergy = track->GetVertexKineticEnergy()/MeV;

    // Creator process ID
    if (trackid != 1) {
        const G4String& creatorProcess = track->GetCreatorProcess()->GetProcessName();
        G4int id = 0;
        if (creatorProcess == "CoulombScat") id = fCoulombScattering;
        if (creatorProcess == "eIoni") id = fIonisation; 
        if (creatorProcess == "eBrem") id = fBremsstrahlung;
        if (creatorProcess == "muBrems") id = 16; //fMuBremsstrahlung;
        if (creatorProcess == "ppCharged") id = fPairProdByCharged;
        if (creatorProcess == "annihil") id = fAnnihilation; 
        if (creatorProcess == "AnnihiToMuPair") id = fAnnihilationToMuMu;
        if (creatorProcess == "hAnnihil") id = fAnnihilationToHadrons;
        if (creatorProcess == "nuclearStopping") id = fNuclearStopping;
        if (creatorProcess == "eGeneral") id = fElectronGeneralProcess;

        if (creatorProcess == "msc") id = fMultipleScattering;
        
        if (creatorProcess == "Rayl") id = fRayleigh;
        if (creatorProcess == "phot") id = fPhotoElectricEffect;
        if (creatorProcess == "compt") id = fComptonScattering;
        if (creatorProcess == "conv") id = fGammaConversion;
        if (creatorProcess == "GammaToMuPair") id = fGammaConversionToMuMu;
        if (creatorProcess == "gGeneral") id = fGammaGeneralProcess;
        
        if (creatorProcess == "Cherenkov") id = fCerenkov;
        if (creatorProcess == "Scintillation") id = fScintillation;
        if (creatorProcess == "SynRad") id = fSynchrotronRadiation;
        if (creatorProcess == "TR") id = fTransitionRadiation;

        procid = 2000 + id;
    }

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    G4RunManager* runManager = G4RunManager::GetRunManager();

    analysisManager->FillNtupleIColumn(2, 0, runManager->GetCurrentEvent()->GetEventID());
    analysisManager->FillNtupleIColumn(2, 1, trackid);
    analysisManager->FillNtupleIColumn(2, 2, pdg);
    analysisManager->FillNtupleIColumn(2, 3, detid);
    analysisManager->FillNtupleIColumn(2, 4, procid);
    analysisManager->FillNtupleDColumn(2, 5, primaryVertex.x()/mm);
    analysisManager->FillNtupleDColumn(2, 6, primaryVertex.y()/mm);
    analysisManager->FillNtupleDColumn(2, 7, primaryVertex.z()/mm);
    analysisManager->FillNtupleDColumn(2, 8, endVertex.x()/mm);
    analysisManager->FillNtupleDColumn(2, 9, endVertex.y()/mm);
    analysisManager->FillNtupleDColumn(2, 10, endVertex.z()/mm);
    analysisManager->FillNtupleDColumn(2, 11, kEnergy);
    analysisManager->AddNtupleRow(2);

}
