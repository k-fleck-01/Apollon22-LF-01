//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for DetectorConstruction class
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

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fDetectorMessenger(0), fLogicChamberMagField(0),
                     fLogicSpecMagField(0), fTargetMagnetSep(30.*cm), fMagnetLength(15.*cm), fMagnetStrength(1.75*tesla),
                     fMagnetDetSep(30.*cm) {

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
    // Vacuum
    G4Material* g4Vacuum = new G4Material("G4_GALACTIC",
									       1.,
									       1.008*g/mole,
									       1.e-25*g/cm3,
									       kStateGas,
									       2.73*kelvin,
									       3.e-18*pascal);

    // YAG (yttrium aluminium garnet)
    // Compositional information taken from: https://www.americanelements.com/cerium-doped-yttrium-aluminum-garnet-163584-80-3
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

    G4Material* g4Vacuum    = nist->FindOrBuildMaterial("G4_GALACTIC");
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
    G4LogicalVolume* logicDetScreen = new G4LogicalVolume(solidDetScreen, g4YAG, "lDetScreen");
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

    //
    // Gamma spectrometer geometry
    //

    // Converter 
    G4Box* solidConverter = new G4Box("converter",
                                      1.*cm,
                                      1.*cm,
                                      0.01*cm);
    G4LogicalVolume* logicConverter = new G4LogicalVolume(solidConverter, g4Tungsten, "lConverter");
    G4VPhysicalVolume* physConverter = new G4PVPlacement(0,
                                                         G4ThreeVector(0., 0., 149.99*cm),
                                                         logicConverter,
                                                         "Converter",
                                                         logicWorld,
                                                         false,
                                                         0,
                                                         checkOverlaps);

    // Collimators
    G4Tubs* solidCollimator = new G4Tubs("collimator",
                                         0.2*cm,
                                         2.5*cm,
                                         5.*cm,
                                         0.,           // Starting phi angle
                                         360.);        // End of angle segment
    G4LogicalVolume* logicCollimator = new G4LogicalVolume(solidCollimator, g4Lead, "lCollimator");
    G4VPhysicalVolume* physCollimator1 = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., 155.*cm),
                                                           logicCollimator,
                                                           "Collimator1",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);
    G4VPhysicalVolume* physCollimator2 = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., 175.*cm),
                                                           logicCollimator,
                                                           "Collimator2",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    // Magnet core (yoke)
    G4Box* solidSpecMagCore = new G4Box("specMagCore",
                                         6.*cm,
                                         4.*cm,
                                         fMagnetLength/2.);
    G4LogicalVolume* logicSpecMagCore = new G4LogicalVolume(solidSpecMagCore, g4Iron, "lSpecMagCore");
    G4VPhysicalVolume* physSpecMagCore = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., 149.99*cm + fTargetMagnetSep + fMagnetLength/2),
                                                           logicSpecMagCore,
                                                           "SpecMagCore",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    // Magnet void
    G4Box* solidSpecMagGap = new G4Box("specMagGap",
                                       4.*cm,
                                       2.*cm,
                                       fMagnetLength/2.);
    G4LogicalVolume* logicSpecMagGap = new G4LogicalVolume(solidSpecMagGap, g4Air, "lSpecMagGap");
    G4VPhysicalVolume* physSpecMagGap = new G4PVPlacement(0,
                                                          G4ThreeVector(2.*cm, 0., 0.),
                                                          logicSpecMagGap,
                                                          "SpecMagGap",
                                                          logicSpecMagCore,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    // LANEX screen (Gd2SO2)
    G4Box* solidLanexScreen = new G4Box("lanexScreen",
                                        20.*cm,
                                        5.*cm,
                                        0.25*mm);
    G4LogicalVolume* logicLanexScreen = new G4LogicalVolume(solidLanexScreen, g4Gadox, "lLanexScreen");
    G4VPhysicalVolume* physLanexScreen = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., 149.99*cm + fTargetMagnetSep + fMagnetLength + fMagnetDetSep),
                                                           logicLanexScreen,
                                                           "LanexScreen",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);
    
    // Assign magnetic fields to logical volumes
    fLogicChamberMagField = logicMagFld;
    fLogicSpecMagField    = logicSpecMagGap;

    // Print material table
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
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
    fLogicChamberMagField->SetFieldManager(localChamberFldManager, true);

    G4MagneticField* specMagField = new G4UniformMagField(G4ThreeVector(0., -1.*fMagnetStrength, 0.));
    G4FieldManager* localSpecFldManager = new G4FieldManager(specMagField);
    fLogicSpecMagField->SetFieldManager(localSpecFldManager, true);

    return;
}

void DetectorConstruction::SetTargetMagnetSep(G4double val) {

    fTargetMagnetSep = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void DetectorConstruction::SetMagnetLength(G4double val) {

    fMagnetLength = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void DetectorConstruction::SetMagnetStrength(G4double val) {

    fMagnetStrength = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}

void DetectorConstruction::SetMagnetDetSep(G4double val) {

    fMagnetDetSep = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}