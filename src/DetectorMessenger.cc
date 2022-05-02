//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for DetectorMessenger class
// Last edited: 13/02/2022

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det) : G4UImessenger(), fDetector(Det) {

    fSpecDir = new G4UIdirectory("/spectrometer/");
    fSpecDir->SetGuidance("Control of gamma spectrometer magnet field strength.");

    fMagnetStrengthCmd = new G4UIcmdWithADoubleAndUnit("/spectrometer/SetMagnetStrength", this);
    fMagnetStrengthCmd->SetGuidance("Set strength of magnetic field.");
    fMagnetStrengthCmd->SetParameterName("MagnetStrength", false);
    fMagnetStrengthCmd->SetRange("MagnetStrength>0.");
    fMagnetStrengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fMagnetStrengthCmd->SetToBeBroadcasted(false);


}

DetectorMessenger::~DetectorMessenger() {

    delete fSpecDir;
    delete fMagnetStrengthCmd;

}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    if(command == fMagnetStrengthCmd) fDetector->SetMagnetStrength(fMagnetStrengthCmd->GetNewDoubleValue(newValue));
}