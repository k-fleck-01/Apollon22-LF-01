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

#include "G4RootAnalysisManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {

    // Generate one particle per event
    fParticleGun = new G4ParticleGun(1);

    // Setting ParticleGun defaults
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
	fParticleGun->SetParticleDefinition(particle); // Setting particle type

}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    // Generates a primary particle with random position about centre
    // r0 small -> effective point source
    
    G4double r0 = 2.*nm; 
    G4double radius = r0*(2.0*G4UniformRand() - 1.0);
    G4double angle =  2*3.1415926545*G4UniformRand();

    G4double x0 = radius*cos(angle);
    G4double y0 = radius*sin(angle);
    G4double z0 = -2575.*mm;        // Centre of gas cell
    //G4double z0 = -2500.*mm;        // Matches Niall's simulations

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

/*
    // Sampling energy from a flat distribution
    G4double Elower = 0.;
    G4double Eupper = 2.*GeV;

    G4double energy = (Eupper - Elower)*G4UniformRand() + Elower;
    fParticleGun->SetParticleEnergy(energy);

    // Setting momentum direction (including beam divergence)
    G4double thetaMax = 1.*mrad;
    G4double theta = thetaMax*G4UniformRand();
    G4double phi   = 2.*3.1459265*G4UniformRand()*rad;
    G4ThreeVector momentum(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
	fParticleGun->SetParticleMomentumDirection(momentum); // Setting momentum direction
*/

    // Sampling from experimental spectrum
    G4double energy = this->SampleEnergyValue();
    fParticleGun->SetParticleEnergy(energy);

    G4double theta = pow((0.6/(energy/GeV)), 1.4)*mrad;
    G4double phi   = 2.*3.1459265*G4UniformRand()*rad;
    G4ThreeVector momentum(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
	fParticleGun->SetParticleMomentumDirection(momentum); // Setting momentum direction

    fParticleGun->GeneratePrimaryVertex(anEvent);

    // Adding primary information to tree
    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(3, 0, x0/mm);
    analysisManager->FillNtupleDColumn(3, 1, y0/mm);
    analysisManager->FillNtupleDColumn(3, 2, z0/mm);
    analysisManager->FillNtupleDColumn(3, 3, energy/MeV);
    analysisManager->FillNtupleDColumn(3, 4, theta/mrad);
    analysisManager->FillNtupleDColumn(3, 5, phi/rad);
    analysisManager->AddNtupleRow(3);

}

G4double PrimaryGeneratorAction::SampleEnergyValue() {

    // Following values taken from numerical fitting of
    // experimental spectrum.
    G4double fitCoefficients[6] = {0.0029295798536937375,
                                -1.966250866302375e-06,
                                -1.1132757750710963e-08,
                                2.0221298798397796e-11,
                                -1.2508534257254872e-14,
                                2.7354212401810846e-18};
    G4double scaleC = 3.1523775432003887;
    G4double energyMax = 1609.0090089999999*MeV;
    G4double energyMin = 200.*MeV;

    auto f = [fitCoefficients] (G4double xx) {
        G4double sum = 0.;
        for (int ii = 0; ii < 6; ++ii) {
            sum += fitCoefficients[ii]*pow(xx, ii);
        }
        return sum;
    };

    // Sampling energy using acceptance-rejection method
    G4bool acceptValue = false;
    G4int counter = 0;
    while (not acceptValue) {
        G4double uu = G4UniformRand();
        G4double xx = (energyMax - energyMin)*G4UniformRand() + energyMin;
        G4double rr = f(xx)*(energyMax - energyMin)/scaleC;

        if (uu <= rr) return xx;
        ++counter;

        if (counter > 10000) {
            G4cout << "Failed to sample a value - returning minium energy." << G4endl;
            break;
        }
    }
    return energyMin;
}