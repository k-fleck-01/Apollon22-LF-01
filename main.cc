//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Main file for the Apollon simulation
// Last edited: 11/02/2022
//
#include <iostream>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv) {

    // Initialise the run manager
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
	G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 4);
	if (argc == 3) nThreads = G4UIcommand::ConvertToInt(argv[2]);
	runManager->SetNumberOfThreads(nThreads);
	G4cout << "===== profiler is started with " << runManager->GetNumberOfThreads() << " threads =====" << G4endl;
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization);

    // Initialise UI and visualisation managers
	G4UIExecutive* ui = new G4UIExecutive(argc, argv);
	G4VisManager* vis = new G4VisExecutive();
	
	vis->Initialize();
	
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	UImanager->ApplyCommand("/control/execute run.mac");
	ui->SessionStart();

	delete runManager;
	return 0;
}