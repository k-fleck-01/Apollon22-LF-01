#ifndef DETECTOR_MESSENGER_H
#define DETECTOR_MESSENGER_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for DetectorConstructionMessenger class
// Last edited: 13/02/2022

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger : public G4UImessenger {
    public:
        DetectorMessenger(DetectorConstruction*);
        ~DetectorMessenger();

    public:
        virtual void SetNewValue(G4UIcommand*, G4String);

    private:
        DetectorConstruction* fDetector;
        G4UIdirectory*        fSpecDir;
        G4UIcmdWithADoubleAndUnit* fMagnetStrengthCmd;


};

#endif