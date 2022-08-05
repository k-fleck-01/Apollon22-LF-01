//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PrimaryGeneratorMessenger class
// Last edited: 04/08/2022
//

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* pgAction) :
        G4UImessenger(),
        fAction(pgAction) {
            fPrimaryDir = new G4UIdirectory("/primary/");
            fPrimaryDir->SetGuidance("Control of primary parameters");

            fImportBeamCmd = new G4UIcmdWithAString("/primary/importBeam", this);
            fImportBeamCmd->SetGuidance("Import beam from file");
            fImportBeamCmd->SetParameterName("fileName", false);
            fImportBeamCmd->AvailableForStates(G4State_PreInit);

            fSpectrumInputCmd = new G4UIcmdWithAnInteger("/primary/setSpectrumInput", this);
            fSpectrumInputCmd->SetGuidance("Sets beam mode to use default or from file");
            fSpectrumInputCmd->SetParameterName("spectrumFlag", false);
            fSpectrumInputCmd->SetRange("spectrumFlag > 0 && spectrumFlag < 4");
            fSpectrumInputCmd->AvailableForStates(G4State_PreInit);
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
    delete fPrimaryDir;
    delete fImportBeamCmd;
    delete fSpectrumInputCmd;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if(command == fImportBeamCmd) fAction->SetInputBeamFile(newValue);
    if(command == fSpectrumInputCmd) fAction->SetInputSpectrumFlag(fSpectrumInputCmd->GetNewIntValue(newValue));
}