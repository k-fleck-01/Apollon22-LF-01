//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for BDCrossing class
// Last edited: 11/05/2022
//
#include <cmath>

#include "BDCrossing.hh"

#include "G4VProcess.hh"
#include "G4EmProcessSubType.hh"

BDCrossing::BDCrossing(): fPdg(0), fDetid(0), fVertex(0), fPosition(0),
             fEnergy(0.), fMomentum(0), fAngle(0.), fFluence(0.), fProcid(0)
{}

BDCrossing::~BDCrossing()
{}

//
//

void BDCrossing::SetPDG(G4int pdg) { fPdg = pdg; }
void BDCrossing::SetDetID(G4int detid) { fDetid = detid; }
void BDCrossing::SetVertex(G4ThreeVector vtx) { fVertex = vtx; }
void BDCrossing::SetPosition(G4ThreeVector pos) { fPosition = pos; }
void BDCrossing::SetEnergy(G4double energy) { fEnergy = energy; }
void BDCrossing::SetMomentum(G4ThreeVector momentum) {fMomentum = momentum; }
void BDCrossing::SetAngle(G4ThreeVector surfNorm) {
    G4ThreeVector pUnit = fMomentum/fMomentum.mag();
    fAngle = pUnit.polarAngle(-surfNorm);
 }
void BDCrossing::SetFluence(G4ThreeVector surfNorm, G4double areaS) { 
    G4double cosTheta = fMomentum.cosTheta(-surfNorm);
    if (std::abs(cosTheta) < 1.e-7) {
        fFluence = 0.;
    }
    else {
        fFluence = 1./cosTheta/areaS;
    } 
}
void BDCrossing::SetCreatorProcess(G4int procid) { fProcid = procid; }

//
//

G4int BDCrossing::GetPDG() const { return fPdg; }
G4int BDCrossing::GetDetID() const { return fDetid; }
G4ThreeVector BDCrossing::GetVertex() const { return fVertex; }
G4ThreeVector BDCrossing::GetPosition() const { return fPosition; }
G4double BDCrossing::GetEnergy() const { return fEnergy; }
G4ThreeVector BDCrossing::GetMomentum() const { return fMomentum; }
G4double BDCrossing::GetAngle() const { return fAngle; }
G4double BDCrossing::GetFluence() const { return fFluence; }
G4int BDCrossing::GetProcessID() const { return fProcid; }