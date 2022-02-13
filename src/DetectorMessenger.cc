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
    fSpecDir->SetGuidance("Control of gamma spectrometer geometry.");

    fTargetMagnetSepCmd = new G4UIcmdWithADoubleAndUnit("/spectrometer/SetTargetMagnetSep", this);
    fTargetMagnetSepCmd->SetGuidance("Set distance between target and magnet entrance.");
    fTargetMagnetSepCmd->SetParameterName("TargetMagnetSep", false);
    fTargetMagnetSepCmd->SetRange("TargetMagnetSep>0.");
    fTargetMagnetSepCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fTargetMagnetSepCmd->SetToBeBroadcasted(false);

    fMagnetLengthCmd = new G4UIcmdWithADoubleAndUnit("/spectrometer/SetMagnetLength", this);
    fMagnetLengthCmd->SetGuidance("Set length of magnet and active magnetic field.");
    fMagnetLengthCmd->SetParameterName("MagnetLength", false);
    fMagnetLengthCmd->SetRange("MagnetLength>0.");
    fMagnetLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fMagnetLengthCmd->SetToBeBroadcasted(false);

    fMagnetStrengthCmd = new G4UIcmdWithADoubleAndUnit("/spectrometer/SetMagnetStrength", this);
    fMagnetStrengthCmd->SetGuidance("Set strength of magnetic field.");
    fMagnetStrengthCmd->SetParameterName("MagnetStrength", false);
    fMagnetStrengthCmd->SetRange("MagnetStrength>0.");
    fMagnetStrengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fMagnetStrengthCmd->SetToBeBroadcasted(false);

    fMagnetDetSepCmd = new G4UIcmdWithADoubleAndUnit("/spectrometer/SetMagnetDetSep", this);
    fMagnetDetSepCmd->SetGuidance("Set distance between magnet exit and detector screen.");
    fMagnetDetSepCmd->SetParameterName("MagnetDetSep", false);
    fMagnetDetSepCmd->SetRange("MagnetDetSep>0.");
    fMagnetDetSepCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fMagnetDetSepCmd->SetToBeBroadcasted(false);

}

DetectorMessenger::~DetectorMessenger() {

    delete fSpecDir;
    delete fTargetMagnetSepCmd;
    delete fMagnetLengthCmd;
    delete fMagnetStrengthCmd;
    delete fMagnetDetSepCmd;

}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    if(command == fTargetMagnetSepCmd) fDetector->SetTargetMagnetSep(fTargetMagnetSepCmd->GetNewDoubleValue(newValue));
    if(command == fMagnetLengthCmd) fDetector->SetMagnetLength(fMagnetLengthCmd->GetNewDoubleValue(newValue));
    if(command == fMagnetStrengthCmd) fDetector->SetMagnetStrength(fMagnetStrengthCmd->GetNewDoubleValue(newValue));
    if(command == fMagnetDetSepCmd) fDetector->SetMagnetDetSep(fMagnetDetSepCmd->GetNewDoubleValue(newValue));

}