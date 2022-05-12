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
                      fLogicChamberMagField(0), fLogicSpecMagField(0), fMagnetStrength(1.*tesla) {

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
    G4Material* g4Tantalum  = nist->FindOrBuildMaterial("G4_Ta");
    G4Material* g4Steel     = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    G4Material* g4YAG       = nist->FindOrBuildMaterial("YAG");
    G4Material* g4Kapton    = nist->FindOrBuildMaterial("G4_KAPTON");
    G4Material* g4Perspex   = nist->FindOrBuildMaterial("G4_PLEXIGLASS");  // Perspex trademark - PMMA or Plexiglass is generic name
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
    G4LogicalVolume* logicChamberOuter = new G4LogicalVolume(solidChamberOuter, g4Steel, "lChamberOuter");
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

    constexpr G4double relToChamberWall = -2090.*mm/2.;

    // Interior dividing wall
    G4Box* solidInnerDivide = new G4Box("innerDivide",
                                        1190.*mm/2.,
                                        1074.*mm/2.,
                                        10.*mm/2.);
    G4LogicalVolume* logicInnerDivide = new G4LogicalVolume(solidInnerDivide, g4Steel, "lInnerDivide");
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

    G4LogicalVolume* logicGasCellOuter = new G4LogicalVolume(solidGasCellOuter, g4Steel, "lGasCellOuter");
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

    G4LogicalVolume* logicWedge = new G4LogicalVolume(solidWedge, g4Tantalum, "lWedge");

    // Rotation matrix to correctly align wedge
    G4RotationMatrix* wedgeRotMatrix = new G4RotationMatrix();
    wedgeRotMatrix->rotateX(90.*deg);
    wedgeRotMatrix->rotateY(180.*deg);
    G4VPhysicalVolume* physWedge = new G4PVPlacement(wedgeRotMatrix,
                                                     G4ThreeVector(10.*mm, 0., relToChamberWall + 12.2*mm + 1018.*mm),
                                                     logicWedge,
                                                     "Wedge",
                                                     logicChamberInner,
                                                     false,
                                                     0,
                                                     checkOverlaps);


    // Kapton sheet
    G4Box* solidKaptonSheet = new G4Box("kaptonSheet",
                                        25.*mm/2.,
                                        110.*mm/2.,
                                        1.*mm/2.);
    G4LogicalVolume* logicKaptonSheet = new G4LogicalVolume(solidKaptonSheet, g4Kapton, "lKaptonSheet");

    G4RotationMatrix* kSheetRotMatrix = new G4RotationMatrix();
    kSheetRotMatrix->rotateY(-45.*deg);
    G4VPhysicalVolume* physKaptonSheet = new G4PVPlacement(kSheetRotMatrix,
                                                           G4ThreeVector(0., 0., relToChamberWall + 0.5*mm + 1009.5*mm),
                                                           logicKaptonSheet,
                                                           "KaptonSheet",
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

    G4Box* solidMaskLayerSet = new G4Box("maskLayerSet",
                                 40.*mm/2.,
                                 25.5*mm/2.,
                                 5.*mm/2.);
    G4LogicalVolume* logicMaskLayerSet = new G4LogicalVolume(solidMaskLayerSet, g4Iron, "lMaskLayerSet");
    G4VPhysicalVolume* physMaskLayerSet = new G4PVPlacement(0,
                                                    G4ThreeVector(0., 0.25*mm, 0.),
                                                    logicMaskLayerSet,
                                                    "MaskLayerSet",
                                                    logicMask,
                                                    false,
                                                    0,
                                                    checkOverlaps);

    G4Box* solidMaskLayer = new G4Box("maskLayer",
                                        24.8*mm/2.,
                                        1.7*mm/2.,
                                        5.*mm/2.);
    G4LogicalVolume* logicMaskLayer = new G4LogicalVolume(solidMaskLayer, g4Iron, "lMaskLayer");
    G4VPhysicalVolume* physMaskLayer = new G4PVReplica("MaskLayer",
                                                       logicMaskLayer,
                                                       logicMaskLayerSet,
                                                       kYAxis,
                                                       15,
                                                       1.7*mm);

    G4Box* solidMaskFeSlit = new G4Box("maskFeSlit",
                                       24.8*mm/2.,
                                       0.85*mm/2.,
                                       5.*mm/2.);
    G4LogicalVolume* logicMaskFeSlit = new G4LogicalVolume(solidMaskFeSlit, g4Iron, "lMaskFeSlit");
    G4VPhysicalVolume* physMaskFeSlit = new G4PVPlacement(0,
                                                          G4ThreeVector(0., 0.425*mm, 0.),
                                                          logicMaskFeSlit,
                                                          "MaskFeSlit",
                                                          logicMaskLayer,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    G4Box* solidMaskSlit = new G4Box("maskSlit",
                                       24.8*mm/2.,
                                       0.85*mm/2.,
                                       5.*mm/2.);
    G4LogicalVolume* logicMaskSlit = new G4LogicalVolume(solidMaskSlit, g4Vacuum, "lMaskSlit");
    G4VPhysicalVolume* physMaskSlit = new G4PVPlacement(0,
                                                          G4ThreeVector(0., -0.425*mm, 0.),
                                                          logicMaskSlit,
                                                          "MaskSlit",
                                                          logicMaskLayer,
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
                                                             G4ThreeVector(0., 0., relToChamberWall + 50.*mm + 1319.9*mm),
                                                             logicLeadWallRear,
                                                             "LeadWallRear",
                                                             logicChamberInner,
                                                             false,
                                                             0,
                                                             checkOverlaps);

    G4Tubs* solidLeadHole = new G4Tubs("leadHole",
                                       0.,
                                       5.*mm,
                                       100.*mm/2.,
                                       0.,
                                       360.*deg);
    G4LogicalVolume* logicLeadHole = new G4LogicalVolume(solidLeadHole, g4Vacuum, "lLeadHole");
    G4VPhysicalVolume* physLeadHoleFront = new G4PVPlacement(0,
                                                             G4ThreeVector(),
                                                             logicLeadHole,
                                                             "LeadHoleFront",
                                                             logicLeadWallFront,
                                                             false,
                                                             0,
                                                             checkOverlaps);
    G4VPhysicalVolume* physLeadHoleRear = new G4PVPlacement(0,
                                                             G4ThreeVector(),
                                                             logicLeadHole,
                                                             "LeadHoleRear",
                                                             logicLeadWallRear,
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
                                                       G4ThreeVector(0., 0., relToChamberWall + 135.*mm + 1463.9*mm),
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

    G4Box* solidMagField = new G4Box("magField",
                                     100.*mm/2.,
                                     18.*mm/2.,
                                     270.*mm/2.);
    G4LogicalVolume* logicMagField = new G4LogicalVolume(solidMagField, g4Vacuum, "lMagField");
    G4VPhysicalVolume* physMagField = new G4PVPlacement(0,
                                                        G4ThreeVector(91.*mm - 50.*mm, 0., 0.),
                                                        logicMagField,
                                                        "MagField",
                                                        logicMagGap,
                                                        false,
                                                        0,
                                                        checkOverlaps);

    // YAG screen
    G4Box* solidYagScreen = new G4Box("yagScreen",
                                      100.*mm/2.,
                                      30.3*mm/2.,
                                      2.*mm);
    G4LogicalVolume* logicYagScreen = new G4LogicalVolume(solidYagScreen, g4YAG, "lYagScreen");

    G4RotationMatrix* yagScreenRotMatrix = new G4RotationMatrix();
    yagScreenRotMatrix->rotateX(-45.*deg);
    G4VPhysicalVolume* physYagScreenUpper = new G4PVPlacement(yagScreenRotMatrix,
                                                              G4ThreeVector(50.*mm + 50.*mm, 0., relToChamberWall + 0.1*mm + 1883.9*mm),
                                                              logicYagScreen,
                                                              "YagScreenUpper",
                                                              logicChamberInner,
                                                              false,
                                                              0,
                                                              checkOverlaps);
    G4VPhysicalVolume* physYagScreenLower = new G4PVPlacement(yagScreenRotMatrix,
                                                              G4ThreeVector(-50.*mm - 50.*mm, 0., relToChamberWall + 0.1*mm + 1883.9*mm),
                                                              logicYagScreen,
                                                              "YagScreenLower",
                                                              logicChamberInner,
                                                              false,
                                                              0,
                                                              checkOverlaps);
                                                    
    // YAG stand
    G4Trap* solidYagStand = new G4Trap("yagStand",  
                                       100.*mm,  // Depth of wedge (along z)
                                       290.*mm,  // Length along y
                                       195.*mm,  // Widest part along x
                                       66.7*mm); // Shortest side along x
    G4LogicalVolume* logicYagStand = new G4LogicalVolume(solidYagStand, g4Steel, "lYagStand");

    G4RotationMatrix* yagStandRotMatrix = new G4RotationMatrix();
    yagStandRotMatrix->rotateY(-90.*deg);
    G4VPhysicalVolume* physYagStandUpper = new G4PVPlacement(yagStandRotMatrix,
                                                             G4ThreeVector(50.*mm + 50.*mm, -12.2*mm -145.*mm, relToChamberWall + 33.35*mm + 1883.9*mm),
                                                             logicYagStand,
                                                             "YagStandUpper",
                                                             logicChamberInner,
                                                             false,
                                                             0,
                                                             checkOverlaps);
    G4VPhysicalVolume* physYagStandLower = new G4PVPlacement(yagStandRotMatrix,
                                                             G4ThreeVector(-50.*mm - 50.*mm, -12.2*mm -145.*mm, relToChamberWall + 33.35*mm + 1883.9*mm),
                                                             logicYagStand,
                                                             "YagStandLower",
                                                             logicChamberInner,
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

    // Perspex layer over kapton window
    G4Box* solidPerspex = new G4Box("perspex",
                                    945.*mm/2.,
                                    40.*mm/2.,
                                    4.*mm/2.);
    G4LogicalVolume* logicPerspex = new G4LogicalVolume(solidPerspex, g4Perspex, "lPerspex");
    G4VPhysicalVolume* physPerspex = new G4PVPlacement(0,
                                                       G4ThreeVector(0., 0., -1425.*mm + 2.*mm),
                                                       logicPerspex,
                                                       "Perspex",
                                                       logicWorld,
                                                       false,
                                                       0,
                                                       checkOverlaps);

    
    //*************************************************************************
    //*************************************************************************
    // MUON SPECTROMETRY GEOMETRY
    //*************************************************************************
    //*************************************************************************
    constexpr G4double relToChamberExit = -1425.*mm;

    // Lead shielding
    G4Box* solidLeadWallSpec = new G4Box("solidLeadWallSpec",
                                         475.*mm/2.,
                                         285.*mm/2.,
                                         100.*mm/2.);
    G4LogicalVolume* logicLeadWallSpec = new G4LogicalVolume(solidLeadWallSpec, g4Lead, "lLeadWallSpec");
    G4VPhysicalVolume* physLeadWallSpecLower = new G4PVPlacement(0,
                                                                 G4ThreeVector(-80.*mm -475.*mm/2. , 0., relToChamberExit + 50.*mm + 250.*mm),
                                                                 logicLeadWallSpec,
                                                                 "LeadWallSpecLower",
                                                                 logicWorld,
                                                                 false,
                                                                 0,
                                                                 checkOverlaps);
    G4VPhysicalVolume* physLeadWallSpecUpper = new G4PVPlacement(0,
                                                                 G4ThreeVector(80.*mm + 475.*mm/2., 0., relToChamberExit + 50.*mm + 250.*mm),
                                                                 logicLeadWallSpec,
                                                                 "LeadWallSpecUpper",
                                                                 logicWorld,
                                                                 false,
                                                                 0,
                                                                 checkOverlaps);

    G4Box* soildLeadShieldAdd = new G4Box("solidLeadShieldAdd",
                                          200.*mm/2.,
                                          285.*mm/2.,
                                          50.*mm/2.);
    G4LogicalVolume* logicLeadShieldAdd = new G4LogicalVolume(soildLeadShieldAdd, g4Lead, "lLeadShieldAdd");
    G4VPhysicalVolume* physLeadShieldAdd = new G4PVPlacement(0,
                                                             G4ThreeVector(-80.*mm -100.*mm, 0., relToChamberExit + 25.*mm + 350.*mm),
                                                             logicLeadShieldAdd,
                                                             "LeadShieldAdd",
                                                             logicWorld,
                                                             false,
                                                             0,
                                                             checkOverlaps);

    // Cr-39 stacks
    G4Box* solidStack = new G4Box("stack",
                                  50.*mm/2.,
                                  50.*mm/2.,
                                  10.*mm/2);
    G4LogicalVolume* logicStack = new G4LogicalVolume(solidStack, g4Cr39, "lStack");

    // Single layer of stack
    G4Box* solidCr39 = new G4Box("cr39",
                                 50.*mm/2.,
                                 50.*mm/2.,
                                 0.5*mm/2.);
    G4LogicalVolume* logicCr39 = new G4LogicalVolume(solidCr39, g4Cr39, "lCr39");
    G4VPhysicalVolume* physCr39 = new G4PVReplica("Cr39",     // name
                                                  logicCr39,  // logical volume
                                                  logicStack, // mother logical volume
                                                  kZAxis,     // axis of replication
                                                  20,         // no. of replicas
                                                  0.5*mm);    // width of replica


    
    // Placement of stacks
    G4VPhysicalVolume* physStack1 = new G4PVPlacement(0,
                                                      G4ThreeVector(-180.*mm -25.*mm, 25.*mm, relToChamberExit + 5.*mm + 400.*mm),
                                                      logicStack,
                                                      "Stack1",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4VPhysicalVolume* physStack2 = new G4PVPlacement(0,
                                                      G4ThreeVector(-180.*mm + 25.*mm, 25.*mm, relToChamberExit + 5.*mm + 400.*mm),
                                                      logicStack,
                                                      "Stack2",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4VPhysicalVolume* physStack3 = new G4PVPlacement(0,
                                                      G4ThreeVector(-180.*mm -25.*mm, -25.*mm, relToChamberExit + 5.*mm + 400.*mm),
                                                      logicStack,
                                                      "Stack3",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    G4VPhysicalVolume* physStack4 = new G4PVPlacement(0,
                                                      G4ThreeVector(-180.*mm +25.*mm, -25.*mm, relToChamberExit + 5.*mm + 400.*mm),
                                                      logicStack,
                                                      "Stack4",
                                                      logicWorld,
                                                      false,
                                                      0,
                                                      checkOverlaps);

    //*************************************************************************
    //*************************************************************************
    // GAMMA SPECTROMETER GEOMETRY
    //*************************************************************************
    //*************************************************************************

    // Spectrometer converter
    G4Box* solidGSpecConverter = new G4Box("gSpecConverter",
                                           20.*mm/2.,
                                           20.*mm/2.,
                                           225.*um/2.);
    G4LogicalVolume* logicGSpecConverter = new G4LogicalVolume(solidGSpecConverter, g4Tantalum, "lGSpecConverter");
    G4VPhysicalVolume* physGSpecConverter = new G4PVPlacement(0,
                                                              G4ThreeVector(0., 0., relToChamberExit + 112.5*um + 439.775*mm),
                                                              logicGSpecConverter,
                                                              "GSpecConverter",
                                                              logicWorld,
                                                              false,
                                                              0,
                                                              checkOverlaps);

    // Lead blocks
    G4Box* solidLeadBlock = new G4Box("leadBlock",
                                      50.*mm/2.,
                                      100.*mm/2.,
                                      100.*mm/2.);
    G4LogicalVolume* logicLeadBlock = new G4LogicalVolume(solidLeadBlock, g4Lead, "lLeadBlock");

    G4VPhysicalVolume* physLBFrontUpper = new G4PVPlacement(0,
                                                           G4ThreeVector(2.*mm + 25.*mm, 0., relToChamberExit + 50.*mm + 440.*mm),
                                                           logicLeadBlock,
                                                           "LBFrontUpper",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);
    G4VPhysicalVolume* physLBFrontLower = new G4PVPlacement(0,
                                                           G4ThreeVector(-2.*mm -25.*mm, 0., relToChamberExit + 50.*mm + 440.*mm),
                                                           logicLeadBlock,
                                                           "LBFrontLower",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    G4RotationMatrix* leadBlockRotMatrix = new G4RotationMatrix();
    leadBlockRotMatrix->rotateY(90.*deg);
    G4VPhysicalVolume* physLBBackLower = new G4PVPlacement(leadBlockRotMatrix,
                                                           G4ThreeVector(-20.*mm - 50.*mm, 0., relToChamberExit + 25*mm + 575.*mm),
                                                           logicLeadBlock,
                                                           "LBBackLower",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);
    G4VPhysicalVolume* physLBBackUpper = new G4PVPlacement(leadBlockRotMatrix,
                                                           G4ThreeVector(20.*mm + 50.*mm, 0., relToChamberExit + 25*mm + 575.*mm),
                                                           logicLeadBlock,
                                                           "LBBackUpper",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    // Rear collimator
    G4Box* solidCollimator = new G4Box("collimator",
                                       105.*mm/2.,
                                       100.*mm/2.,
                                       100.*mm/2.);
    G4LogicalVolume* logicCollimator = new G4LogicalVolume(solidCollimator, g4Lead, "lCollimator");
    G4VPhysicalVolume* physCollimator = new G4PVPlacement(0,
                                                          G4ThreeVector(0, 0, relToChamberExit + 50.*mm + 665.*mm),
                                                          logicCollimator,
                                                          "Collimator",
                                                          logicWorld,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    G4Box* solidCollimatorGap = new G4Box("collimatorGap",
                                          5.*mm/2.,
                                          10.*mm/2.,
                                          100.*mm/2.);
    G4LogicalVolume* logicCollimatorGap = new G4LogicalVolume(solidCollimatorGap, g4Air, "lCollimatorGap");
    G4VPhysicalVolume* physCollimatorGap = new G4PVPlacement(0,
                                                             G4ThreeVector(),
                                                             logicCollimatorGap,
                                                             "CollimatorGap",
                                                             logicCollimator,
                                                             false,
                                                             0,
                                                             checkOverlaps);
    
    // Gamma spectrometer dipole
    G4Box* solidGSpecMagnet = new G4Box("gSpecMagnet",
                                        80.*mm/2.,
                                        30.*mm/2.,
                                        50.*mm/2.);
    G4LogicalVolume* logicGSpecMagnet = new G4LogicalVolume(solidGSpecMagnet, g4Iron, "lGSpecMagnet");
    G4VPhysicalVolume* physGSpecMagnet = new G4PVPlacement(0,
                                                           G4ThreeVector(0., 0., relToChamberExit + 25.*mm + 770.*mm),
                                                           logicGSpecMagnet,
                                                           "GSpecMagnet",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    G4Box* solidGSpecMagGap = new G4Box("gSpecMagGap",
                                        70.*mm/2.,
                                        8.*mm/2.,
                                        50.*mm/2.);
    G4LogicalVolume* logicGSpecMagGap = new  G4LogicalVolume(solidGSpecMagGap, g4Air, "lGSpecMagGap");
    G4VPhysicalVolume* physGSpecMagGap = new G4PVPlacement(0,
                                                           G4ThreeVector(),
                                                           logicGSpecMagGap,
                                                           "GSpecMagGap",
                                                           logicGSpecMagnet,
                                                           false,
                                                           0,
                                                           checkOverlaps);

    // LANEX mount
    G4Box* solidLanexMount = new G4Box("lanexMount",
                                       315.*mm/2.,
                                       180.*mm/2.,
                                       10.*mm/2.);
    G4LogicalVolume* logicLanexMount = new G4LogicalVolume(solidLanexMount, g4Steel, "lLanexMount");
    G4VPhysicalVolume* physLanexMount = new G4PVPlacement(0,
                                                          G4ThreeVector(-15.*mm, 0., relToChamberExit + 5.*mm + 1470.*mm),
                                                          logicLanexMount,
                                                          "LanexMount",
                                                          logicWorld,
                                                          false,
                                                          0,
                                                          checkOverlaps);

    G4Box* solidLanexMountOpen = new G4Box("lanexMountOpen",
                                           300.*mm/2.,
                                           150.*mm/2.,
                                           10.*mm/2.);
    G4LogicalVolume* logicLanexMountOpen = new G4LogicalVolume(solidLanexMountOpen, g4Air, "lLanexMountOpent");
    G4VPhysicalVolume* physLanexMountOpen = new G4PVPlacement(0,
                                                              G4ThreeVector(15.*mm/2., 0., 0.),
                                                              logicLanexMountOpen,
                                                              "LanexMountOpen",
                                                              logicLanexMount,
                                                              false,
                                                              0,
                                                              checkOverlaps);

    G4Box* solidLanexSheet = new G4Box("lanexSheet",
                                       300.*mm/2.,
                                       150.*mm/2.,
                                       2.*mm/2.);
    G4LogicalVolume* logicLanexSheet = new G4LogicalVolume(solidLanexSheet, g4Gadox, "lLanexSheet");
    G4VPhysicalVolume* physLanexSheet = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          logicLanexSheet,
                                                          "LanexSheet",
                                                          logicLanexMountOpen,
                                                          false,
                                                          0,
                                                          checkOverlaps);
    
    // Assign magnetic fields to logical volumes
    fLogicChamberMagField = logicMagField;
    fLogicSpecMagField    = logicGSpecMagGap;

    // Print material table
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Exporting geometry to GDML
    G4GDMLParser* gdmlParser = new G4GDMLParser();
    //gdmlParser->Write("apollon_g4geometry_v4.2.gdml", physWorld);
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
    
    // Setting sensitive volumes
    SetSensitiveDetector("lYagScreen", sd, true);
    SetSensitiveDetector("lStack", sd, true);
    SetSensitiveDetector("lLanexSheet", sd, true);

    // Add magnetic fields
    G4MagneticField* chamberMagField = new G4UniformMagField(G4ThreeVector(0., 1.7*tesla, 0.));
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
