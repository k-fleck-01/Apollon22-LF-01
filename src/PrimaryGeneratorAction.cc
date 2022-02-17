//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PrimaryGeneratorAction class
// Last edited: 11/02/2022
//

#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {

    // Generate one particle per event
    fParticleGun = new G4ParticleGun(1);

    // Setting ParticleGun defaults
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
	fParticleGun->SetParticleDefinition(particle); // Setting particle type

	G4ThreeVector momentum(0., 0., 1.);
	fParticleGun->SetParticleMomentumDirection(momentum); // Setting momentum direction

}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    // Generates a primary particle with random position about centre
    // from a flat energy distribution

    G4double radius = 0.1*cm;
    G4double angle =  2*3.1415926545*G4UniformRand();

    G4double x0 = radius*cos(angle);
    G4double y0 = radius*sin(angle);
    G4double z0 = -18.0*cm;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    // Sampling energy from a flat distribution
    G4double Elower = 0.;
    G4double Eupper = 2.*GeV;

    G4double energy = (Eupper - Elower)*G4UniformRand() + Elower;
    fParticleGun->SetParticleEnergy(energy);

    fParticleGun->GeneratePrimaryVertex(anEvent);
    
}