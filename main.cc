//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Main file for the Apollon simulation
// Last edited: 14/02/2022
//
#include <iostream>

#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"

#include "DetectorConstruction.hh"
#include "QGSP_BERT_EXT.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv) {

	// Detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;
	if(argc == 1) ui = new G4UIExecutive(argc, argv);

    // Initialise the run manager
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
	G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 16);
	if (argc == 3) nThreads = G4UIcommand::ConvertToInt(argv[2]);
	runManager->SetNumberOfThreads(nThreads);
	G4cout << "===== Simulation has started with " << runManager->GetNumberOfThreads() << " threads =====" << G4endl;
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new QGSP_BERT_EXT);
    runManager->SetUserInitialization(new ActionInitialization);

	G4VisManager* visManager = nullptr;
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if(ui) { // interactive mode
		visManager = new G4VisExecutive;
		visManager->Initialize();
		UImanager->ApplyCommand("/control/execute run.mac");
		ui->SessionStart();
		delete ui;
	} else { // batch mode
		G4String command = "/control/execute  ";
		G4String filename = argv[1];
		UImanager->ApplyCommand(command + filename);
	}

    delete visManager;
	delete runManager;
	return 0;
}