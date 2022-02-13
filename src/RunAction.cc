//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for RunAction class
// Last edited: 11/02/2022
//

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RootAnalysisManager.hh"

RunAction::RunAction() : G4UserRunAction() 
{}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*) {

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    analysisManager->OpenFile("apollon_out.root");

    analysisManager->CreateNtuple("Hits", "Hits");
    analysisManager->CreateNtupleIColumn(0, "evid");
    analysisManager->CreateNtupleDColumn(0, "x");
    analysisManager->CreateNtupleDColumn(0, "y");
    analysisManager->CreateNtupleDColumn(0, "z");
    analysisManager->CreateNtupleDColumn(0, "edep");
    analysisManager->CreateNtupleDColumn(0, "eKin");
    analysisManager->CreateNtupleIColumn(0, "pdg");
    analysisManager->CreateNtupleIColumn(0, "procid");
    analysisManager->CreateNtupleIColumn(0, "detid");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("Events", "Events");
    analysisManager->CreateNtupleIColumn(1, "evid");
    analysisManager->CreateNtupleDColumn(1, "edep");
    analysisManager->FinishNtuple(1);

    return;
}

void RunAction::EndOfRunAction(const G4Run*) {

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

    return;
}