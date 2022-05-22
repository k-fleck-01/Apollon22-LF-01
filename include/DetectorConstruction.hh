#ifndef DETECTOR_CONSTRUCTION_H
#define DETECTOR_CONSTRUCTION_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for DetectorConstruction class
//

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction {
    public:
        DetectorConstruction();
        ~DetectorConstruction();

    public:
        virtual G4VPhysicalVolume* Construct();
        virtual void ConstructSDandField();

        void DefineMaterials();
        G4VPhysicalVolume* DefineVolumes();
        virtual void SetMagnetStrength(G4double);

    private:
        DetectorMessenger* fDetectorMessenger;
        G4LogicalVolume* fLogicChamberMagField;
        G4LogicalVolume* fLogicSpecMagField;

        G4double fMagnetStrength;

};

#endif