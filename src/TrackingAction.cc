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

#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"

TrackingAction::TrackingAction(RunAction*) : G4UserTrackingAction()
{}

TrackingAction::~TrackingAction()
{}

void TrackingAction::PreUserTrackingAction(const G4Track*) 
{}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {

    G4int trackid = track->GetTrackID();
    G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
    G4ThreeVector primaryVertex = track->GetVertexPosition();
    G4ThreeVector endVertex = track->GetPosition();

    if (!trackid) {
        G4String creatorProcess = track->GetCreatorProcess()->GetProcessName();
        G4cout << "Creation process " << creatorProcess << G4endl;
    }
    else {
        G4cout << "Primary track! No process name" << G4endl;
    }

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    G4RunManager* runManager = G4RunManager::GetRunManager();

    analysisManager->FillNtupleIColumn(2, 0, runManager->GetCurrentEvent()->GetEventID());
    analysisManager->FillNtupleIColumn(2, 1, trackid);
    analysisManager->FillNtupleIColumn(2, 2, pdg);
    analysisManager->FillNtupleIColumn(2, 3, -1); // procid
    analysisManager->FillNtupleDColumn(2, 4, primaryVertex.x());
    analysisManager->FillNtupleDColumn(2, 5, primaryVertex.y());
    analysisManager->FillNtupleDColumn(2, 6, primaryVertex.z());
    analysisManager->FillNtupleDColumn(2, 7, endVertex.x());
    analysisManager->FillNtupleDColumn(2, 8, endVertex.y());
    analysisManager->FillNtupleDColumn(2, 9, endVertex.z());
    analysisManager->AddNtupleRow(2);

}