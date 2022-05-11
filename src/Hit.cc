//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for Hit class
//

#include "Hit.hh"

#include "G4ThreeVector.hh"

Hit::Hit() : G4VHit(), fEdep(0.), fEnergy(0.), fPosition(0), fVertexPosition(0),
             fParticleType(-1), fProcess(-1), fDetid(-1)
{}

Hit::~Hit()
{}

G4double Hit::GetEdep() const {
    return fEdep;
}

G4double Hit::GetEnergy() const {
    return fEnergy;
}

G4ThreeVector Hit::GetPosition() const {
    return fPosition;
}

G4ThreeVector Hit::GetVertexPosition() const {
    return fVertexPosition;
}

G4int Hit::GetParticleType() const {
    return fParticleType;
}

G4int Hit::GetProcess() const {
    return fProcess;
}

G4int Hit::GetDetectorID() const {
    return fDetid;
}

void Hit::AddEdep(G4double edep) {
    fEdep = edep;
}

void Hit::AddEnergy(G4double ekin) {
    fEnergy = ekin;
}

void Hit::AddPosition(G4ThreeVector pos) {
    fPosition = pos;
}

void Hit::AddVertexPosition(G4ThreeVector vpos) {
    fVertexPosition = vpos;
}

void Hit::AddParticleType(G4int pdg) {
    fParticleType = pdg; 
}
void Hit::AddProcess(G4int procid) {
    fProcess = procid;
}
void Hit::AddDetectorID(G4int detid) {
    fDetid = detid;
}