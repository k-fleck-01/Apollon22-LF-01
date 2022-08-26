//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PrimaryGeneratorAction class
// Last edited: 11/02/2022
//
#include <string>
#include <cstdio>

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "G4RootAnalysisManager.hh"
#include "G4RootAnalysisReader.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "FileReader.hh"

#include "PrimaryGeneratorAction.hh"

namespace { G4Mutex rootPrimGenMutex = G4MUTEX_INITIALIZER; }
FileReader* PrimaryGeneratorAction::fFileReader = 0;

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det) : 
        G4VUserPrimaryGeneratorAction(),  
        fParticleGun(0),
        fDetectorConstruction(det),
        fBeamMode(0),
        fSpectrumId(-1) {
    
    // Generate one particle per event
    fParticleGun = new G4ParticleGun(1);

    // Setting ParticleGun defaults
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName="e-");
	fParticleGun->SetParticleDefinition(particle); // Setting particle type
    
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(1.*GeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -2575.*mm));

    fMessenger = new PrimaryGeneratorMessenger(this);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
    delete fMessenger;

    G4AutoLock lock(&rootPrimGenMutex);
    if(fFileReader) {delete fFileReader; fFileReader = 0; }
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // Select generation based on beam mode
    switch(fBeamMode) {
        case 0:
            // Default beam
            break;
        case 1:
            // Beam imported from file
            G4int evid = anEvent->GetEventID();
            long long int trackEntries = fFileReader->GetTrackEntries();
            if(evid > trackEntries) {
                // Event ID greater than number of sampled particles
                fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("geantino"));
                fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
                break;
            }

            G4ThreeVector pPos = G4ThreeVector(0., 0., -2575.*mm) + fFileReader->GetImportBeamPos(evid);
            fParticleGun->SetParticlePosition(pPos);
            fParticleGun->SetParticleEnergy(fFileReader->GetImportBeamEnergy(evid));

            G4double theta = fFileReader->GetImportBeamDiv(evid);
            G4double phi   = 2.*3.14159265*G4UniformRand();
            G4ThreeVector momentum_dir(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
            fParticleGun->SetParticleMomentumDirection(momentum_dir.unit());
            break;
    }
    
    fParticleGun->GeneratePrimaryVertex(anEvent);

    // Adding primary information to tree
    G4double x0 = fParticleGun->GetParticlePosition().x();
    G4double y0 = fParticleGun->GetParticlePosition().y();
    G4double z0 = fParticleGun->GetParticlePosition().z();
    G4double energy = fParticleGun->GetParticleEnergy();
    G4ThreeVector pvec = fParticleGun->GetParticleMomentumDirection();
    G4double phi = atan2(pvec.y(), pvec.x());
    G4double theta = acos(pvec.z());

    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(3, 0, x0/mm);
    analysisManager->FillNtupleDColumn(3, 1, y0/mm);
    analysisManager->FillNtupleDColumn(3, 2, z0/mm);
    analysisManager->FillNtupleDColumn(3, 3, energy/MeV);
    analysisManager->FillNtupleDColumn(3, 4, theta/mrad);
    analysisManager->FillNtupleDColumn(3, 5, phi/rad);
    analysisManager->AddNtupleRow(3);

}

void PrimaryGeneratorAction::SetInputBeamFile(G4String& fname) { 
    G4AutoLock lock(&rootPrimGenMutex);
    if (G4Threading::G4GetThreadId() == 0) {
        if(!fFileReader) fFileReader = new FileReader(fname, fSpectrumId); 
        fBeamMode = 1; 
    }
}
