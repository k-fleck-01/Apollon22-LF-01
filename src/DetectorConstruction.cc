//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for DetectorConstruction class.
//

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"
//#include "G4GDMLParser.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fDetectorMessenger(0), 
                      fLogicChamberMagField(0), fLogicSpecMagField(0), fMagnetStrength(0.6*tesla) {

    DefineMaterials();
    fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::DefineMaterials() {
    //
    // Defining necessary materials
    //
    G4NistManager* nist = G4NistManager::Instance();
    G4bool isotopes = false;

    // YAG (yttrium aluminium garnet)
    // Compositional information taken from: 
    // https://www.americanelements.com/cerium-doped-yttrium-aluminum-garnet-163584-80-3
    G4Element* elY  = nist->FindOrBuildElement("Y", isotopes);
    G4Element* elAl = nist->FindOrBuildElement("Al", isotopes);
    G4Element* elO  = nist->FindOrBuildElement("O", isotopes);

    G4Material* g4YAG = new G4Material("YAG", 4.6*g/cm3, 3, kStateSolid);
    g4YAG->AddElement(elY,  0.15);
    g4YAG->AddElement(elAl, 0.25);
    g4YAG->AddElement(elO, 0.6);

}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
    G4bool checkOverlaps = true;

    // Creating list of materials
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* g4Vacuum    = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* g4Air       = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* g4Lead      = nist->FindOrBuildMaterial("G4_Pb");
    G4Material* g4Aluminium = nist->FindOrBuildMaterial("G4_Al");
    G4Material* g4Iron      = nist->FindOrBuildMaterial("G4_Fe");
    G4Material* g4YAG       = nist->FindOrBuildMaterial("YAG");
    G4Material* g4Kapton    = nist->FindOrBuildMaterial("G4_KAPTON");
    G4Material* g4Tungsten  = nist->FindOrBuildMaterial("G4_W");
    G4Material* g4Gadox     = nist->FindOrBuildMaterial("G4_GADOLINIUM_OXYSULFIDE");

    //
    // Defining geometry
    //
    // World volume
    G4double worldSize = 5.*m;
    G4Box* solidWorld = new G4Box("world",
                                  worldSize,
                                  worldSize,
                                  worldSize);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, g4Air, "lWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,               // rotation matrix
                                                     G4ThreeVector(), // position in mother volume
                                                     logicWorld,      // associated logical volume
                                                     "World",         // string identifier
                                                     0,               // associated mother volume - zero for world
                                                     false,           // set to false
                                                     0,               // copy number
                                                     checkOverlaps);  // check for overlaps with existing volumes

    // Chamber volume
    G4Box* solidChamberOuter = new G4Box("chamberOuter",
                                    625.*mm,
                                    567.*mm,
                                    1075.*mm);
    G4LogicalVolume* logicChamberOuter = new G4LogicalVolume(solidChamberOuter, g4Aluminium, "lChamberOuter");
    G4VPhysicalVolume* physChamberOuter = new G4PVPlacement(0,
                                                            G4ThreeVector(0., 0., -2500.*mm),
                                                            logicChamberOuter,
                                                            "ChamberOuter",
                                                            logicWorld,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    G4Box* solidChamberInner = new G4Box("chamberInner",
                                         575.*mm,
                                         517.*mm,
                                         1025.*mm);
    G4LogicalVolume* logicChamberInner = new G4LogicalVolume(solidChamberInner, g4Vacuum, "lChamberInner");
    G4VPhysicalVolume* physChamberInner = new G4PVPlacement(0, 
                                                            G4ThreeVector(),
                                                            logicChamberInner,
                                                            "ChamberInner",
                                                            logicChamberOuter,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    
    
    // Assign magnetic fields to logical volumes
    fLogicChamberMagField = logicMagFld;
    fLogicSpecMagField    = logicSpecMagGap;

    // Print material table
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Exporting geometry to GDML
    //G4GDMLParser* gdmlParser = new G4GDMLParser();
    //gdmlParser->Write("apollon_g4geometry.gdml", physWorld);
    delete gdmlParser;
    
    return physWorld;

}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    return DefineVolumes();
}

void DetectorConstruction::ConstructSDandField() {

    G4SDManager* SDManager = G4SDManager::GetSDMpointer();

    SensitiveDetector* sd = new SensitiveDetector("sd");
    SDManager->AddNewDetector(sd);
    
    SetSensitiveDetector("lDetScreen", sd, true);    // YAG detector
    SetSensitiveDetector("lTarget", sd, true);       // Tungsten target
    SetSensitiveDetector("lWindow", sd, true);       // Kapton window
    SetSensitiveDetector("lLanexScreen", sd, true);  // Spectrometer LANEX screen

    // Add magnetic fields
    G4MagneticField* chamberMagField = new G4UniformMagField(G4ThreeVector(0., -1.7*tesla, 0.));
    G4FieldManager* localChamberFldManager = new G4FieldManager(chamberMagField);
    localChamberFldManager->SetDetectorField(chamberMagField);
    localChamberFldManager->CreateChordFinder(chamberMagField);
    fLogicChamberMagField->SetFieldManager(localChamberFldManager, true);

    G4MagneticField* specMagField = new G4UniformMagField(G4ThreeVector(0., fMagnetStrength, 0.));
    G4FieldManager* localSpecFldManager = new G4FieldManager(specMagField);
    localSpecFldManager->SetDetectorField(specMagField);
    localSpecFldManager->CreateChordFinder(specMagField);
    fLogicSpecMagField->SetFieldManager(localSpecFldManager, true);

}


void DetectorConstruction::SetMagnetStrength(G4double val) {

    fMagnetStrength = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}
