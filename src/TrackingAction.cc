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

    // If track originated in GRS converter, record track information
    G4String vertexName = track->GetLogicalVolumeAtVertex()->GetName();
    if (vertexName != "lGSpecConverter") return;

    // Get information at end of track
    const G4StepPoint* preStepPoint = track->GetStep()->GetPreStepPoint();

    G4int trackid = track->GetTrackID();
    G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
    G4ThreeVector primaryVertex = track->GetVertexPosition();
    G4ThreeVector endVertex = track->GetPosition();
    G4double kEnergy = track->GetVertexKineticEnergy()/MeV;

    G4String endVertexName = preStepPoint->GetPhysicalVolume()->GetName();
    G4int detid = 0;
    if (endVertexName == "PhosphorLayer") detid += 1000;

    // Creator process ID
    G4int procid = 2000;
    if (trackid != 1) {
        const G4String& creatorProcess = track->GetCreatorProcess()->GetProcessName();
        if (creatorProcess == "CoulombScat") procid += fCoulombScattering;
        if (creatorProcess == "eIoni") procid += fIonisation; 
        if (creatorProcess == "eBrem") procid += fBremsstrahlung;
        if (creatorProcess == "muBrems") procid += 16; //fMuBremsstrahlung;
        if (creatorProcess == "ppCharged") procid += fPairProdByCharged;
        if (creatorProcess == "annihil") procid += fAnnihilation; 
        if (creatorProcess == "AnnihiToMuPair") procid += fAnnihilationToMuMu;
        if (creatorProcess == "hAnnihil") procid += fAnnihilationToHadrons;
        if (creatorProcess == "nuclearStopping") procid += fNuclearStopping;
        if (creatorProcess == "eGeneral") procid += fElectronGeneralProcess;

        if (creatorProcess == "msc") procid += fMultipleScattering;
        
        if (creatorProcess == "Rayl") procid += fRayleigh;
        if (creatorProcess == "phot") procid += fPhotoElectricEffect;
        if (creatorProcess == "compt") procid += fComptonScattering;
        if (creatorProcess == "conv") procid += fGammaConversion;
        if (creatorProcess == "GammaToMuPair") procid += fGammaConversionToMuMu;
        if (creatorProcess == "gGeneral") procid += fGammaGeneralProcess;
        
        if (creatorProcess == "Cherenkov") procid += fCerenkov;
        if (creatorProcess == "Scintillation") procid += fScintillation;
        if (creatorProcess == "SynRad") procid += fSynchrotronRadiation;
        if (creatorProcess == "TR") procid += fTransitionRadiation;
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
