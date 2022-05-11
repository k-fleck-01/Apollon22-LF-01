//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for SensitiveDetector class
// Last edited: 07/02/2022
//

#include "SensitiveDetector.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name), fHitCollection(0), fBDXCollection(0),
                 fHCID(0), fBXCID(0) {
    collectionName.insert("HitCollection");
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE) {

    fHitCollection = new HitCollection(GetName(), collectionName[0]);
	fHCID = -1;
	if (fHCID < 0) fHCID = GetCollectionID(0);
	HCE->AddHitsCollection(fHCID, fHitCollection);

    fBDXCollection = new BDXCollection(GetName(), collectionName[0]);
    fBXCID = -1;
    if (fBXCID < 0) fBXCID = GetCollectionID(0);
    HCE->AddHitsCollection(fBXCID, fBDXCollection);
    
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* aTouchableHist) {

    G4StepPoint* preStepPoint  = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

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
    
    G4Track* track = aStep->GetTrack();
    // PDG code of particle causing hit
    G4int pdgCode = track->GetParticleDefinition()->GetPDGEncoding();
    
    // Vertex position of track causing hit
    G4ThreeVector vpos = track->GetVertexPosition();

    // Total energy of particle depositing energy
    G4double energy = track->GetTotalEnergy();

     //
    // Boundary crossing scoring 
    //
    if (aStep->IsFirstStepInVolume()) {
        BDCrossing * aBdx = new BDCrossing();

        // Getting volume information
        const G4VTouchable* theTouchable = preStepPoint->GetTouchable();
        G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(prePosition);
        G4ThreeVector surfNorm = theTouchable->GetSolid()->SurfaceNormal(localPosition);
        G4double areaS = 0.5*theTouchable->GetSolid()->GetSurfaceArea()/mm2;

        // Creator process
        G4int procid = 2000;
        if (track->GetCreatorProcess()) {
            procid += track->GetCreatorProcess()->GetProcessSubType();
        }

        aBdx->SetPDG(pdgCode);
        aBdx->SetDetID(detid);
        aBdx->SetVertex(vpos);
        aBdx->SetPosition(prePosition);
        aBdx->SetEnergy(energy);
        aBdx->SetMomentum(track->GetMomentum());
        aBdx->SetAngle(surfNorm);
        aBdx->SetFluence(surfNorm, areaS);
        aBdx->SetCreatorProcess(procid);
        fBDXCollection->insert(aBdx);

    }

    // Adding Hit information
    // Energy deposition of hit
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    class Hit* aHit = new class Hit(); // class keyword needs to be added as 'Hit'
                                       // is also an inline function within this scope.
    aHit->AddPosition(position);
    aHit->AddEdep(edep);
    aHit->AddParticleType(pdgCode);
    aHit->AddVertexPosition(vpos);
    aHit->AddEnergy(energy);
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
        G4double energy    = hit->GetEnergy();

        analysisManager->FillNtupleIColumn(0, 0, runManager->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleDColumn(0, 1, x/mm);
        analysisManager->FillNtupleDColumn(0, 2, y/mm);
        analysisManager->FillNtupleDColumn(0, 3, z/mm);
        analysisManager->FillNtupleDColumn(0, 4, vtxx/mm);
        analysisManager->FillNtupleDColumn(0, 5, vtxy/mm);
        analysisManager->FillNtupleDColumn(0, 6, vtxz/mm);
        analysisManager->FillNtupleDColumn(0, 7, edep/MeV);
        analysisManager->FillNtupleDColumn(0, 8, energy/MeV);
        analysisManager->FillNtupleIColumn(0, 9, pdg);
        analysisManager->FillNtupleIColumn(0, 10, procid);
        analysisManager->FillNtupleIColumn(0, 11, detid);
        analysisManager->AddNtupleRow(0);
    }

    nhits = fBDXCollection->entries();
    for (G4int ii = 0; ii < nhits; ++ii) {
        auto bdx = (*fBDXCollection)[ii];

        G4int pdg          = bdx->GetPDG();
        G4int detid        = bdx->GetDetID();
        G4int procid       = bdx->GetProcessID();
        G4double x         = bdx->GetPosition().x();
        G4double y         = bdx->GetPosition().y();
        G4double z         = bdx->GetPosition().z();
        G4double vtxx      = bdx->GetVertex().x();
        G4double vtxy      = bdx->GetVertex().y();
        G4double vtxz      = bdx->GetVertex().z();
        G4double px        = bdx->GetMomentum().x();
        G4double py        = bdx->GetMomentum().y();
        G4double pz        = bdx->GetMomentum().z();
        G4double energy    = bdx->GetEnergy();
        G4double theta     = bdx->GetAngle();
        G4double fluence   = bdx->GetFluence();


        analysisManager->FillNtupleIColumn(4, 0, runManager->GetCurrentEvent()->GetEventID());
        analysisManager->FillNtupleIColumn(4, 1, pdg);
        analysisManager->FillNtupleIColumn(4, 2, detid);
        analysisManager->FillNtupleIColumn(4, 3, procid);
        analysisManager->FillNtupleDColumn(4, 4, x/mm);
        analysisManager->FillNtupleDColumn(4, 5, y/mm);
        analysisManager->FillNtupleDColumn(4, 6, z/mm);
        analysisManager->FillNtupleDColumn(4, 7, vtxx/mm);
        analysisManager->FillNtupleDColumn(4, 8, vtxy/mm);
        analysisManager->FillNtupleDColumn(4, 9, vtxz/mm);
        analysisManager->FillNtupleDColumn(4, 10, px/MeV);
        analysisManager->FillNtupleDColumn(4, 11, py/MeV);
        analysisManager->FillNtupleDColumn(4, 12, pz/MeV);
        analysisManager->FillNtupleDColumn(4, 13, energy/MeV);
        analysisManager->FillNtupleDColumn(4, 14, theta/rad);
        analysisManager->FillNtupleDColumn(4, 15, fluence/(1/mm2));
        analysisManager->AddNtupleRow(4);
    }

}
