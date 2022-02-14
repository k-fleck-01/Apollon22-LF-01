//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for SensitiveDetector class
// Last edited: 07/02/2022
//

#include "SensitiveDetector.hh"
#include "Hit.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name), fHitCollection(0), fHCID(0) {
    collectionName.insert("HitCollection");
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE) {

    fHitCollection = new HitCollection(GetName(), collectionName[0]);
	fHCID = -1;
	if (fHCID < 0) fHCID = GetCollectionID(0);
	HCE->AddHitsCollection(fHCID, fHitCollection);

}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* aTouchableHist) {

    G4StepPoint* preStepPoint  = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

    class Hit* aHit = new class Hit(); // class keyword needs to be added as 'Hit'
                                       // is also an inline function within this scope.

    // Position of hit
    G4ThreeVector prePosition = preStepPoint->GetPosition();
    G4ThreeVector postPosition = postStepPoint->GetPosition();
    G4ThreeVector position = prePosition + G4UniformRand() * (postPosition - prePosition); // Energy deposition occurs at 
                                                                                           // a random point along step.
    aHit->AddPosition(position);

    // Energy deposition of hit
    G4double edep = aStep->GetTotalEnergyDeposit()/MeV; // Energy deposition in MeV.
    if (edep == 0.) return false;
    aHit->AddEdep(edep);

    G4Track* track = aStep->GetTrack();
    // PDG code of particle causing hit
    G4int pdgCode = track->GetParticleDefinition()->GetPDGEncoding();
    aHit->AddParticleType(pdgCode);

    // Kinetic energy if particle is entering detector
    G4double kinEnergy;
    if(aStep->IsFirstStepInVolume()) {
        kinEnergy = track->GetKineticEnergy()/MeV;
    } else {
        kinEnergy = -1.*MeV;
    }
    aHit->AddKineticEnergy(kinEnergy);

    // ID of detector hit occurs in
    const G4String& vName = preStepPoint->GetPhysicalVolume()->GetName();
    G4int detid = -1;
    if (vName == "Target") {
        detid = 0;
    } else if (vName == "DetScreen") {
        detid = 1;
    } else if (vName == "Window") {
        detid = 2;
    } else if (vName == "LanexScreen") {
        detid = 3;
    }
    aHit->AddDetectorID(detid);

    fHitCollection->insert(aHit);

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE) {
    
    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();
    G4RunManager* runManager = G4RunManager::GetRunManager();

    G4int nhits = fHitCollection->entries();
    for (G4int ii = 0; ii < nhits; ++ii) {
        auto hit = (*fHitCollection)[ii];

        G4int pdg = hit->GetParticleType();
        G4int procid = hit->GetCreatorProcess();
        G4int detid = hit->GetDetectorID();
        G4double x = hit->GetPosition().x();
        G4double y = hit->GetPosition().y();
        G4double z = hit->GetPosition().z();
        G4double edep = hit->GetEdep();
        G4double kinEnergy = hit->GetKineticEnergy();

        analysisManager->FillNtupleIColumn(0, 0, runManager->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleDColumn(0, 1, x);
        analysisManager->FillNtupleDColumn(0, 2, y);
        analysisManager->FillNtupleDColumn(0, 3, z);
        analysisManager->FillNtupleDColumn(0, 4, edep);
        analysisManager->FillNtupleDColumn(0, 5, kinEnergy);
        analysisManager->FillNtupleIColumn(0, 6, pdg);
        analysisManager->FillNtupleIColumn(0, 7, procid);
        analysisManager->FillNtupleIColumn(0, 8, detid);
        analysisManager->AddNtupleRow(0);
    }

}