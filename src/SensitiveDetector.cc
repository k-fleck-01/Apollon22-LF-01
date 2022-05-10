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

    // Get detector ID
    const G4String& logicName = preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    G4int ldet = 0;
    if (logicName == "lYagScreen") {
        ldet = 1000;
    }
    else if (logicName == "lStack") {
        ldet = 2000;
    }
    else if (logicName == "lLanexSheet") {
        ldet = 3000;
    }
    G4int detid = ldet + preStepPoint->GetPhysicalVolume()->GetCopyNo() + 1;

    // Position of hit
    G4ThreeVector prePosition = preStepPoint->GetPosition();
    G4ThreeVector postPosition = postStepPoint->GetPosition();
    G4ThreeVector position = prePosition + G4UniformRand() * (postPosition - prePosition); // Energy deposition occurs at 
                                                                                           // a random point along step.
    

    // Energy deposition of hit
    G4double edep = aStep->GetTotalEnergyDeposit()/MeV; // Energy deposition in MeV.
    if (edep == 0.) return false;

    G4Track* track = aStep->GetTrack();
    // PDG code of particle causing hit
    G4int pdgCode = track->GetParticleDefinition()->GetPDGEncoding();
    

    // Vertex position of track causing hit
    G4ThreeVector vpos = track->GetVertexPosition();

    // Kinetic energy if particle is entering detector
    G4double kinEnergy = -1;
    const G4VProcess* currentProcess = preStepPoint->GetProcessDefinedStep();
    if(currentProcess != 0) {
        const G4String& procName = currentProcess->GetProcessName();
        const G4String& thisVolume = track->GetVolume()->GetName();
        const G4String& volumePos = track->GetNextVolume()->GetName();
        if (procName == "Transportation" && volumePos == thisVolume) kinEnergy = track->GetKineticEnergy();
    }

    // Adding Hit information
    aHit->AddPosition(position);
    aHit->AddEdep(edep);
    aHit->AddParticleType(pdgCode);
    aHit->AddVertexPosition(vpos);
    aHit->AddKineticEnergy(kinEnergy);
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

        G4int pdg          = hit->GetParticleType();
        G4int procid       = hit->GetProcess();
        G4int detid        = hit->GetDetectorID();
        G4double x         = hit->GetPosition().x();
        G4double y         = hit->GetPosition().y();
        G4double z         = hit->GetPosition().z();
        G4double vtxx      = hit->GetVertexPosition().x();
        G4double vtxy      = hit->GetVertexPosition().y();
        G4double vtxz      = hit->GetVertexPosition().z();
        G4double edep      = hit->GetEdep();
        G4double kinEnergy = hit->GetKineticEnergy();

        analysisManager->FillNtupleIColumn(0, 0, runManager->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleDColumn(0, 1, x/cm);
        analysisManager->FillNtupleDColumn(0, 2, y/cm);
        analysisManager->FillNtupleDColumn(0, 3, z/cm);
        analysisManager->FillNtupleDColumn(0, 4, vtxx/cm);
        analysisManager->FillNtupleDColumn(0, 5, vtxy/cm);
        analysisManager->FillNtupleDColumn(0, 6, vtxz/cm);
        analysisManager->FillNtupleDColumn(0, 7, edep/MeV);
        analysisManager->FillNtupleDColumn(0, 8, kinEnergy/MeV);
        analysisManager->FillNtupleIColumn(0, 9, pdg);
        analysisManager->FillNtupleIColumn(0, 10, procid);
        analysisManager->FillNtupleIColumn(0, 11, detid);
        analysisManager->AddNtupleRow(0);
    }

}
