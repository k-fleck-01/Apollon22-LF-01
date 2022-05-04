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
#include "G4Trap.hh"
#include "G4UnionSolid.hh"

#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"

#include "G4RunManager.hh"
#include "G4GDMLParser.hh"

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
    G4Material* g4Helium    = nist->FindOrBuildMaterial("G4_He");
    G4Material* g4Lead      = nist->FindOrBuildMaterial("G4_Pb");
    G4Material* g4Aluminium = nist->FindOrBuildMaterial("G4_Al");
    G4Material* g4Iron      = nist->FindOrBuildMaterial("G4_Fe");
    G4Material* g4YAG       = nist->FindOrBuildMaterial("YAG");
    G4Material* g4Kapton    = nist->FindOrBuildMaterial("G4_KAPTON");
    G4Material* g4Tungsten  = nist->FindOrBuildMaterial("G4_W");
    G4Material* g4Gadox     = nist->FindOrBuildMaterial("G4_GADOLINIUM_OXYSULFIDE");
    G4Material* g4Cr39      = nist->FindOrBuildMaterial("G4_CR39");

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

    //*************************************************************************
    //*************************************************************************
    // VACUUM CHAMBER GEOMETRY
    //*************************************************************************
    //*************************************************************************

    // Chamber volume
    G4Box* solidChamberOuter = new G4Box("chamberOuter",
                                    1250.*mm/2.,
                                    1134.*mm/2.,
                                    2150.*mm/2.);
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
                                         1190.*mm/2.,
                                         1074.*mm/2.,
                                         2090.*mm/2.);
    G4LogicalVolume* logicChamberInner = new G4LogicalVolume(solidChamberInner, g4Vacuum, "lChamberInner");
    G4VPhysicalVolume* physChamberInner = new G4PVPlacement(0, 
                                                            G4ThreeVector(),
                                                            logicChamberInner,
                                                            "ChamberInner",
                                                            logicChamberOuter,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    const G4double relToChamberWall = -2090.*mm/2.;

    // Interior dividing wall
    G4Box* solidInnerDivide = new G4Box("innerDivide",
                                        1190.*mm/2.,
                                        1074.*mm/2.,
                                        10.*mm/2.);
    G4LogicalVolume* logicInnerDivide = new G4LogicalVolume(solidInnerDivide, g4Aluminium, "lInnerDivide");
    G4VPhysicalVolume* physInnerDivide = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., relToChamberWall + 5.*mm + 704.*mm),
                                                           logicInnerDivide,
                                                           "InnerDivide",
                                                           logicChamberInner,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    G4Tubs* solidDivideHole = new G4Tubs("divideHole",
                                       0.,
                                       52.*mm/2.,
                                       10.*mm/2.,
                                       0.,
                                       360.*deg);
    G4LogicalVolume* logicDivideHole = new G4LogicalVolume(solidDivideHole, g4Vacuum, "lDivideHole");
    G4VPhysicalVolume* physDivideHole = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logicDivideHole,
                                                          "DivideHole",
                                                          logicInnerDivide,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    // Gas cell
    G4Box* solidGasCellOuter = new G4Box("gasCellOuter",
                                          40.*mm/2.,
                                          50.*mm/2.,
                                          60.*mm/2.);

    G4LogicalVolume* logicGasCellOuter = new G4LogicalVolume(solidGasCellOuter, g4Aluminium, "lGasCellOuter");
    G4VPhysicalVolume* physGasCellOuter = new G4PVPlacement(0,
                                                            G4ThreeVector(0., 0., relToChamberWall + 30.0*mm + 940.0*mm),
                                                            logicGasCellOuter,
                                                            "GasCellOuter",
                                                            logicChamberInner,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    G4Box* solidGasCellInner = new G4Box("gasCellInner",
                                          39.9*mm/2.,
                                          30.*mm/2.,
                                          40.*mm/2.);

    G4LogicalVolume* logicGasCellInner = new G4LogicalVolume(solidGasCellInner, g4Helium, "lGasCellInner");
    G4VPhysicalVolume* physGasCellInner = new G4PVPlacement(0,
                                                            G4ThreeVector(),
                                                            logicGasCellInner,
                                                            "GasCellInner",
                                                            logicGasCellOuter,
                                                            false,
                                                            0,
                                                            checkOverlaps);

    G4Box* solidGasCellSlit = new G4Box("gasCellSlit",
                                        15.0*mm/2.,
                                        24.0*mm/2.,
                                        10.*mm/2.);

    G4LogicalVolume* logicGasCellSlit = new G4LogicalVolume(solidGasCellSlit, g4Helium, "lGasCellSlit");
    G4VPhysicalVolume* physGasCellSlitFront = new G4PVPlacement(0,
                                                                G4ThreeVector(0., 0., -25.*mm),
                                                                logicGasCellSlit,
                                                                "GasCellSlitFront",
                                                                logicGasCellOuter,
                                                                false,
                                                                0,
                                                                checkOverlaps);
    G4VPhysicalVolume* physGasCellSlitBack = new G4PVPlacement(0,
                                                                G4ThreeVector(0., 0., 25.*mm),
                                                                logicGasCellSlit,
                                                                "GasCellSlitBack",
                                                                logicGasCellOuter,
                                                                false,
                                                                0,
                                                                checkOverlaps);

    // Converter wedge
    G4Trap* solidWedge = new G4Trap("wedge",  // Full lengths are used in G4Trap - right angular trapezoid
                                    20.0*mm,  // Depth of wedge (along z)
                                    24.4*mm,  // Length along y
                                    50.0*mm,  // Widest part along x
                                    25.0*mm); // Shortest side along x

    G4LogicalVolume* logicWedge = new G4LogicalVolume(solidWedge, g4Aluminium, "lWedge");

    // Rotation matrix to correctly align wedge
    G4RotationMatrix* rotMatrix = new G4RotationMatrix();
    rotMatrix->rotateX(90.*deg);
    rotMatrix->rotateY(180.*deg);
    G4VPhysicalVolume* physWedge = new G4PVPlacement(rotMatrix,
                                                     G4ThreeVector(10.*mm, 0., relToChamberWall + 12.2*mm + 1018.*mm),
                                                     logicWedge,
                                                     "Wedge",
                                                     logicChamberInner,
                                                     false,
                                                     0,
                                                     checkOverlaps);
    
    // Emittance mask
    G4Box* solidMask = new G4Box("mask",
                                 40.*mm/2.,
                                 30.*mm/2.,
                                 5.*mm/2.);

    G4LogicalVolume* logicMask = new G4LogicalVolume(solidMask, g4Iron, "lMask");
    G4VPhysicalVolume* physMask = new G4PVPlacement(0,
                                                    G4ThreeVector(0., 0., relToChamberWall + 2.5*mm + 1158.9*mm),
                                                    logicMask,
                                                    "Mask",
                                                    logicChamberInner,
                                                    false,
                                                    0,
                                                    checkOverlaps);

    // Aluminium plate
    G4Box* solidPlate = new G4Box("plate",
                                  100.*mm/2.,
                                  100.*mm/2.,
                                  3.*mm/2.);

    G4LogicalVolume* logicPlate = new G4LogicalVolume(solidPlate, g4Aluminium, "lPlate");
    G4VPhysicalVolume* physPlate = new G4PVPlacement(0,
                                                     G4ThreeVector(0., 0., relToChamberWall + 1.5*mm + 1171.4*mm),
                                                     logicPlate,
                                                     "Plate",
                                                     logicChamberInner,
                                                     false,
                                                     0,
                                                     checkOverlaps);

    G4Tubs* solidPlateHole = new G4Tubs("hole",
                                        0.,        // inner radius
                                        3.5*mm/2., // outer radius
                                        3.*mm/2.,  // half length
                                        0.,        // starting azimuth angle
                                        360.*deg); // angular segment

    G4LogicalVolume* logicPlateHole = new G4LogicalVolume(solidPlateHole, g4Vacuum, "lPlateHole");
    G4VPhysicalVolume* physPlateHole = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         logicPlateHole,
                                                         "PlateHole",
                                                         logicPlate,
                                                         false,
                                                         0,
                                                         checkOverlaps);

    // Lead walls
    G4Box* solidLeadWallFront = new G4Box("leadWallFront",
                                          475.*mm/2.,
                                          285.*mm/2.,
                                          100.*mm/2.);

    G4LogicalVolume* logicLeadWallFront = new G4LogicalVolume(solidLeadWallFront, g4Lead, "lLeadWallFront");
    G4VPhysicalVolume* physLeadWallFront = new G4PVPlacement(0,
                                                             G4ThreeVector(0., 0., relToChamberWall + 50.*mm + 1189.9*mm),
                                                             logicLeadWallFront,
                                                             "LeadWallFront",
                                                             logicChamberInner,
                                                             false,
                                                             0,
                                                             checkOverlaps);

    G4Box* solidLeadWallRear = new G4Box("leadWallRear",
                                          855.*mm/2.,
                                          285.*mm/2.,
                                          100.*mm/2.);

    G4LogicalVolume* logicLeadWallRear = new G4LogicalVolume(solidLeadWallRear, g4Lead, "lLeadWallRear");
    G4VPhysicalVolume* physLeadWallRear = new G4PVPlacement(0,
                                                             G4ThreeVector(0., 0., relToChamberWall + 50.*mm + 1339.9*mm),
                                                             logicLeadWallRear,
                                                             "LeadWallRear",
                                                             logicChamberInner,
                                                             false,
                                                             0,
                                                             checkOverlaps);
                                        
    // Vacuum chamber magnet
    G4Box* solidMagCore = new G4Box("magCore",
                                    360.*mm/2.,
                                    350.*mm/2.,
                                    270.*mm/2.);
    G4LogicalVolume* logicMagCore = new G4LogicalVolume(solidMagCore, g4Iron, "lMagCore");
    G4VPhysicalVolume* physMagCore = new G4PVPlacement(0,
                                                       G4ThreeVector(0., 0., relToChamberWall + 135.*mm + 1525.*mm),
                                                       logicMagCore,
                                                       "MagCore",
                                                       logicChamberInner,
                                                       false,
                                                       0,
                                                       checkOverlaps);

    G4Box* solidMagGap = new G4Box("magGap",
                                   242.*mm/2.,
                                   18.*mm/2.,
                                   270.*mm/2.);
    G4LogicalVolume* logicMagGap  = new G4LogicalVolume(solidMagGap, g4Vacuum, "lMagGap");
    G4VPhysicalVolume* physMagGap = new G4PVPlacement(0,
                                                      G4ThreeVector(-59.*mm, 0., 0.),
                                                      logicMagGap,
                                                      "MagGap",
                                                      logicMagCore,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    // Kapton window for chamber
    G4Box* solidWindow = new G4Box("window",
                                   945.*mm/2.,
                                   40.*mm/2.,
                                   30.*mm/2.);

    G4LogicalVolume* logicWindow = new G4LogicalVolume(solidWindow, g4Kapton, "lWindow");
    G4VPhysicalVolume* physWindow = new G4PVPlacement(0,
                                                      G4ThreeVector(0., 0., 1060.*mm),
                                                      logicWindow,
                                                      "Window",
                                                      logicChamberOuter,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    //*************************************************************************
    //*************************************************************************
    // GAMMA SPECTROMETER GEOMETRY
    //*************************************************************************
    //*************************************************************************
    const G4double relToChamberExit = -1425.*mm;

    //*************************************************************************
    //*************************************************************************
    // MUON SPECTROMETRY GEOMETRY
    //*************************************************************************
    //*************************************************************************

    // Cr-39 stacks
    G4Box* solidStack = new G4Box("stack",
                                  50.*mm/2.,
                                  50.*mm/2.,
                                  63.*mm/2);
    G4LogicalVolume* logicStack = new G4LogicalVolume(solidStack, g4Air, "lStack");

    // Single layer of stack
    G4Box* solidStackLayer = new G4Box("StackLayer",
                                 50.*mm/2.,
                                 50.*mm/2.,
                                 21.*mm/2.);
    G4LogicalVolume* logicStackLayer = new G4LogicalVolume(solidStackLayer, g4Air, "lStackLayer");
    G4VPhysicalVolume* physStackLayer = new G4PVReplica("StackLayer",     // name
                                                        logicStackLayer,  // logical volume
                                                        logicStack,       // mother logical volume
                                                        kZAxis,           // axis of replication
                                                        3,                // no. of replicas
                                                        21.*mm);         // width of replica
    
    G4Box* solidCr39 = new G4Box("cr39",
                                 50.*mm/2.,
                                 50.*mm/2.,
                                 0.5*mm/2.);
    G4LogicalVolume* logicCr39 = new G4LogicalVolume(solidCr39, g4Cr39, "lCr39");
    G4VPhysicalVolume* physCr39 = new G4PVPlacement(0,
                                                    G4ThreeVector(0., 0., -21.*mm/2. + 0.5*mm/2.),
                                                    logicCr39,
                                                    "Cr39",
                                                    logicStackLayer,
                                                    false,
                                                    0,
                                                    checkOverlaps);

    G4Box* solidCr39Gap = new G4Box("cr39Gap",
                                    50.*mm/2.,
                                    50.*mm/2.,
                                    20.5*mm/2.);
    G4LogicalVolume* logicCr39Gap = new G4LogicalVolume(solidCr39Gap, g4Air, "lCr39Gap");
    G4VPhysicalVolume* physCr39Gap = new G4PVPlacement(0, 
                                                       G4ThreeVector(0., 0., 0.5*mm/2.),
                                                       logicCr39Gap,
                                                       "Cr39Gap",
                                                       logicStackLayer,
                                                       false,
                                                       0,
                                                       checkOverlaps);

    
    // Placement of stacks
    G4VPhysicalVolume* physStack1 = new G4PVPlacement(0,
                                                      G4ThreeVector(61.*mm, 0., relToChamberExit + 1431.*mm),
                                                      logicStack,
                                                      "Stack1",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4VPhysicalVolume* physStack2 = new G4PVPlacement(0,
                                                      G4ThreeVector(0., 0., relToChamberExit + 1431.*mm),
                                                      logicStack,
                                                      "Stack2",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4VPhysicalVolume* physStack3 = new G4PVPlacement(0,
                                                      G4ThreeVector(-61.*mm, 0., relToChamberExit + 1431.*mm),
                                                      logicStack,
                                                      "Stack3",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    
    
    // Assign magnetic fields to logical volumes
    //fLogicChamberMagField = logicMagFld;
    //fLogicSpecMagField    = logicSpecMagGap;

    // Print material table
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Exporting geometry to GDML
    G4GDMLParser* gdmlParser = new G4GDMLParser();
    gdmlParser->Write("apollon_g4geometry.gdml", physWorld);
    delete gdmlParser;
    
    return physWorld;

}

G4VPhysicalVolume* DetectorConstruction::Construct() {

    return DefineVolumes();
}

void DetectorConstruction::ConstructSDandField() {
/*
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
*/
}


void DetectorConstruction::SetMagnetStrength(G4double val) {

    fMagnetStrength = val;
    G4RunManager::GetRunManager()->ReinitializeGeometry();

}
