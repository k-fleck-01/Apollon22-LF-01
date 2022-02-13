//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for EventAction class
// Last edited: 11/02/2022
//

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4RootAnalysisManager.hh"

EventAction::EventAction(RunAction*) : G4UserEventAction(), fEdep(0.)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event*) {

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    G4RunManager* runManager = G4RunManager::GetRunManager();

    analysisManager->FillNtupleIColumn(1, 0, runManager->GetCurrentEvent()->GetEventID());
    analysisManager->FillNtupleDColumn(1, 1, fEdep/MeV);
    analysisManager->AddNtupleRow(1);

    fEdep = 0.;
    return;
}

void EventAction::AddEdep(G4double edep) {
    fEdep += edep;
    return;
}

G4double EventAction::GetEdep() const {
    return fEdep;
}