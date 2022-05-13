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
    analysisManager->CreateNtupleDColumn(0, "vtxx");
    analysisManager->CreateNtupleDColumn(0, "vtxy");
    analysisManager->CreateNtupleDColumn(0, "vtxz");
    analysisManager->CreateNtupleDColumn(0, "edep");
    analysisManager->CreateNtupleDColumn(0, "energy");
    analysisManager->CreateNtupleIColumn(0, "pdg");
    analysisManager->CreateNtupleIColumn(0, "procid");
    analysisManager->CreateNtupleIColumn(0, "detid");
    analysisManager->FinishNtuple(0);

    analysisManager->CreateNtuple("Events", "Events");
    analysisManager->CreateNtupleIColumn(1, "evid");
    analysisManager->CreateNtupleDColumn(1, "edep");
    analysisManager->FinishNtuple(1);

    analysisManager->CreateNtuple("Tracks", "Tracks");
    analysisManager->CreateNtupleIColumn(2, "evid");
    analysisManager->CreateNtupleIColumn(2, "trackid");
    analysisManager->CreateNtupleIColumn(2, "pdg");
    analysisManager->CreateNtupleIColumn(2, "detid");
    analysisManager->CreateNtupleIColumn(2, "procid");
    analysisManager->CreateNtupleDColumn(2, "vtxx");
    analysisManager->CreateNtupleDColumn(2, "vtxy");
    analysisManager->CreateNtupleDColumn(2, "vtxz");
    analysisManager->CreateNtupleDColumn(2, "endx");
    analysisManager->CreateNtupleDColumn(2, "endy");
    analysisManager->CreateNtupleDColumn(2, "endz");
    analysisManager->CreateNtupleDColumn(2, "kEnergy");
    analysisManager->FinishNtuple(2);

    analysisManager->CreateNtuple("Primaries", "Primaries");
    analysisManager->CreateNtupleDColumn(3, "x");
    analysisManager->CreateNtupleDColumn(3, "y");
    analysisManager->CreateNtupleDColumn(3, "z");
    analysisManager->CreateNtupleDColumn(3, "E");
    analysisManager->CreateNtupleDColumn(3, "theta");
    analysisManager->CreateNtupleDColumn(3, "phi");
    analysisManager->FinishNtuple(3);

    analysisManager->CreateNtuple("Bdx", "Bdx");
    analysisManager->CreateNtupleIColumn(4, "eventid");
    analysisManager->CreateNtupleIColumn(4, "pdg");
    analysisManager->CreateNtupleIColumn(4, "detid");
    analysisManager->CreateNtupleIColumn(4, "procid");
    analysisManager->CreateNtupleDColumn(4, "x");
    analysisManager->CreateNtupleDColumn(4, "y");
    analysisManager->CreateNtupleDColumn(4, "z");
    analysisManager->CreateNtupleDColumn(4, "vtxx");
    analysisManager->CreateNtupleDColumn(4, "vtxy");
    analysisManager->CreateNtupleDColumn(4, "vtxz");
    analysisManager->CreateNtupleDColumn(4, "px");
    analysisManager->CreateNtupleDColumn(4, "py");
    analysisManager->CreateNtupleDColumn(4, "pz");
    analysisManager->CreateNtupleDColumn(4, "energy");
    analysisManager->CreateNtupleDColumn(4, "theta");
    analysisManager->CreateNtupleDColumn(4, "fluence");
    analysisManager->FinishNtuple(4);

    return;
}

void RunAction::EndOfRunAction(const G4Run*) {

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

    analysisManager->Write();
    analysisManager->CloseFile();

    return;
}