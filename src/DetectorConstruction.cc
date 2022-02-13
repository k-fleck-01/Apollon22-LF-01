//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for DetectorConstruction class
//

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fLogicMagField(0)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    G4bool checkOverlaps = true;
    //
    // Defining necessary materials
    //
    G4NistManager* nist = G4NistManager::Instance();
    // Vacuum
    G4Material* g4Vacuum = new G4Material("G4_GALACTIC",
									       1.,
									       1.008*g/mole,
									       1.e-25*g/cm3,
									       kStateGas,
									       2.73*kelvin,
									       3.e-18*pascal);
    // Lead
    G4Material* g4Lead = nist->FindOrBuildMaterial("G4_Pb");
    // Aluminium
    G4Material* g4Aluminium = nist->FindOrBuildMaterial("G4_Al");
    // Iron
    G4Material* g4Iron = nist->FindOrBuildMaterial("G4_Fe");
    // YAG (yttrium aluminium garnet)
    // Compositional information taken from: https://www.americanelements.com/cerium-doped-yttrium-aluminum-garnet-163584-80-3
 /*   G4Element* elY  = nist->FindOrBuildElement("G4_Y");
    G4Element* elAl = nist->FindOrBuildElement("G4_Al");
    G4Element* elO  = nist->FindOrBuildElement("G4_O");
    G4Material* g4YAG = new G4Material("YAG", 4.6*g/cm3, 3);
    g4YAG->AddElement(elY,  0.15);
    g4YAG->AddElement(elAl, 0.25);
    g4YAG->AddElement(elO,  0.60);
*/
    G4Material* g4gadox = nist->FindOrBuildMaterial("G4_GADOLINIUM_OXYSULFIDE");
    // Kapton
    G4Material* g4Kapton = nist->FindOrBuildMaterial("G4_KAPTON");
    //
    // Defining geometry
    //
    // World volume
    G4double worldSize = 5.*m;
    G4Box* solidWorld = new G4Box("world",
                                  worldSize,
                                  worldSize,
                                  worldSize);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, g4Vacuum, "lWorld");
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
                                    62.*cm,
                                    51.*cm,
                                    86.*cm);
    G4LogicalVolume* logicChamberOuter = new G4LogicalVolume(solidChamberOuter, g4Aluminium, "lChamberOuter");
    G4VPhysicalVolume* physChamberOuter = new G4PVPlacement(0,
                                                            G4ThreeVector(0.,15.*cm, 18.*cm),
                                                            logicChamberOuter,
                                                            "ChamberOuter",
                                                            logicWorld,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    G4Box* solidChamberInner = new G4Box("chamberInner",
                                         60.*cm,
                                         49.*cm,
                                         84.*cm);
    G4LogicalVolume* logicChamberInner = new G4LogicalVolume(solidChamberInner, g4Vacuum, "lChamberInner");
    G4VPhysicalVolume* physChamberInner = new G4PVPlacement(0, 
                                                            G4ThreeVector(),
                                                            logicChamberInner,
                                                            "ChamberInner",
                                                            logicChamberOuter,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    // Target volume
    G4Box* solidTarget = new G4Box("target",
                                   5.*cm,
                                   5.*cm,
                                   0.5*cm);
    G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, g4Lead, "lTarget");
    G4VPhysicalVolume* physTarget = new G4PVPlacement(0,
                                                      G4ThreeVector(0., -15.*cm, -16.4*cm),
                                                      logicTarget,
                                                      "Target",
                                                      logicChamberInner,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    // Lead block volumes
    G4Box* solidSmallLead1 = new G4Box("smallLead1",
                                      10.*cm,
                                      5.5*cm,
                                      5.*cm);
    G4Box* solidSmallLead2 = new G4Box("smallLead2",
                                       5.*cm,
                                       2.5*cm,
                                       5.*cm);
    G4UnionSolid* solidLeadCross     = new G4UnionSolid("solidSmallLead1+solidSmallLead2", solidSmallLead1, solidSmallLead2);
    G4LogicalVolume* logicLeadCross  = new G4LogicalVolume(solidLeadCross, g4Lead, "lLeadCross");
    G4VPhysicalVolume* physLeadCross = new G4PVPlacement(0,
                                                         G4ThreeVector(0., -15.*cm, 2.1*cm),
                                                         logicLeadCross,
                                                         "LeadCross",
                                                         logicChamberInner,
                                                         false,
                                                         0,
                                                         checkOverlaps);

    // Lead wall volume
    G4Box* solidLeadWall = new G4Box("leadWall",
                                     39.5*cm,
                                     16.*cm,
                                     5.*cm);
    G4LogicalVolume* logicLeadWall = new G4LogicalVolume(solidLeadWall, g4Lead, "lLeadWall");
    G4VPhysicalVolume* physLeadWall = new G4PVPlacement(0,
                                                        G4ThreeVector(0., -15.*cm, 17.1*cm),
                                                        logicLeadWall,
                                                        "LeadWall",
                                                        logicChamberInner,
                                                        false,
                                                        0,
                                                        checkOverlaps);

    // Lead insert volumes
    G4Box* solidInsert = new G4Box("insert",
                                   0.255*cm,
                                   0.255*cm,
                                   5*cm);
    G4LogicalVolume* logicInsert = new G4LogicalVolume(solidInsert, g4Vacuum, "lInsert");

    G4VPhysicalVolume* physBlockInsert = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logicInsert,
                                                          "BlockInsert",
                                                          logicLeadCross,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    G4VPhysicalVolume* physWallInsert = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logicInsert,
                                                          "WallInsert",
                                                          logicLeadWall,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    // Dipole magnet
    G4Box* solidMagCore = new G4Box("magCore",
                                    18.0*cm,
                                    17.5*cm,
                                    12.5*cm);
    G4LogicalVolume* logicMagCore = new G4LogicalVolume(solidMagCore, g4Iron, "lMagCore");
    G4VPhysicalVolume* physMagCore = new G4PVPlacement(0,
                                                       G4ThreeVector(3.8*cm, -15.*cm, 36.6*cm),
                                                       logicMagCore,
                                                       "MagCore",
                                                       logicChamberInner,
                                                       false,
                                                       0,
                                                       checkOverlaps);

    G4Box* solidMagGap = new G4Box("magGap",
                                   12.1*cm,
                                   0.85*cm,
                                   12.5*cm);
    G4LogicalVolume* logicMagGap  = new G4LogicalVolume(solidMagGap, g4Vacuum, "lMagGap");
    G4VPhysicalVolume* physMagGap = new G4PVPlacement(0,
                                                      G4ThreeVector(-5.9*cm, 0., 0.),
                                                      logicMagGap,
                                                      "MagGap",
                                                      logicMagCore,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4Box* solidMagFld = new G4Box("magFld",
                                   5.0*cm,
                                   0.85*cm,
                                   12.5*cm);
    G4LogicalVolume* logicMagFld  = new G4LogicalVolume(solidMagFld, g4Vacuum, "lMagFld");
    G4VPhysicalVolume* physMagFld = new G4PVPlacement(0,
                                                      G4ThreeVector(4.1*cm, 0., 0.),
                                                      logicMagFld,
                                                      "MagFld",
                                                      logicMagGap,
                                                      false,
                                                      0,
                                                      checkOverlaps);
    

    // Scintillator screen
    G4Box* solidDetScreen = new G4Box("detScreen",
                                      10.0*cm,
                                      3.8*cm,
                                      0.005*cm);
    G4LogicalVolume* logicDetScreen = new G4LogicalVolume(solidDetScreen, g4gadox, "lDetScreen");
    G4VPhysicalVolume* physDetScreen = new G4PVPlacement(0,
                                                         G4ThreeVector(14.*cm, -15.*cm, 83.805*cm),
                                                         logicDetScreen,
                                                         "DetScreen",
                                                         logicChamberInner,
                                                         false,
                                                         0,
                                                         checkOverlaps);

    // Kapton window
    G4Box* solidWindow = new G4Box("window",
                                   25.*cm,
                                   3.8*cm,
                                   1.*cm);
    G4LogicalVolume* logicWindow = new G4LogicalVolume(solidWindow, g4Kapton, "lWindow");
    G4VPhysicalVolume* physWindow = new G4PVPlacement(0,
                                                      G4ThreeVector(0., -15.*cm, 85.*cm),
                                                      logicWindow,
                                                      "Window",
                                                      logicChamberOuter,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    fLogicMagField = logicMagFld;

    return physWorld;
}

void DetectorConstruction::ConstructSDandField() {

    G4SDManager* SDManager = G4SDManager::GetSDMpointer();

    SensitiveDetector* sd = new SensitiveDetector("sd");
    SDManager->AddNewDetector(sd);
    
    SetSensitiveDetector("lDetScreen", sd, true); // YAG detector
    SetSensitiveDetector("lTarget", sd, true);    // Tungsten target
    SetSensitiveDetector("lWindow", sd, true);    // Kapton window

    // Add magnetic field
    G4MagneticField* magfield = new G4UniformMagField(G4ThreeVector(0., -1.7*tesla, 0.));
    G4FieldManager* localFldManager = new G4FieldManager(magfield);
    fLogicMagField->SetFieldManager(localFldManager, true);

    return;
}