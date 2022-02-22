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

    // Adding selection cut so only particles entering spectrometer region
    // are recorded.

    constexpr G4double xmincut = -25.*cm;
    constexpr G4double xmaxcut = 25.*cm;
    constexpr G4double ymincut = -15.*cm;
    constexpr G4double ymaxcut = 15.*cm;
    constexpr G4double zmincut = 145.*cm;
    constexpr G4double zmaxcut = 250.*cm;

    G4bool trackFlag = false;
    G4double trackx = track->GetPosition().x();
    G4double tracky = track->GetPosition().y();
    G4double trackz = track->GetPosition().z();
    if ((trackx >= xmincut && trackx <= xmaxcut) &&
        (tracky >= ymincut && tracky <= ymaxcut) &&
        (trackz >= zmincut && trackz <= zmaxcut)) trackFlag = true;

    if (trackFlag) {
        G4int trackid = track->GetTrackID();
        G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4int procid = 2000;
        G4ThreeVector primaryVertex = track->GetVertexPosition();
        G4ThreeVector endVertex = track->GetPosition();
        G4double kEnergy = track->GetVertexKineticEnergy()/MeV;

        if (trackid != 1) {
            const G4String& creatorProcess = track->GetCreatorProcess()->GetProcessName();
            G4int id = 0;
            if (creatorProcess == "CoulombScat") id = fCoulombScattering;
            if (creatorProcess == "eIoni") id = fIonisation; 
            if (creatorProcess == "eBrem") id = fBremsstrahlung;
            if (creatorProcess == "muBrems") id = fMuBremsstrahlung;
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
        analysisManager->FillNtupleIColumn(2, 3, procid);
        analysisManager->FillNtupleDColumn(2, 4, primaryVertex.x()/cm);
        analysisManager->FillNtupleDColumn(2, 5, primaryVertex.y()/cm);
        analysisManager->FillNtupleDColumn(2, 6, primaryVertex.z()/cm);
        analysisManager->FillNtupleDColumn(2, 7, endVertex.x()/cm);
        analysisManager->FillNtupleDColumn(2, 8, endVertex.y()/cm);
        analysisManager->FillNtupleDColumn(2, 9, endVertex.z()/cm);
        analysisManager->FillNtupleDColumn(2, 10, kEnergy);
        analysisManager->AddNtupleRow(2);
    }
}
