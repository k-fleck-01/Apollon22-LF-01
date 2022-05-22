#ifndef SENSITIVE_DETECTOR_H
#define SENSITIVE_DETECTOR_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for SensitiveDetector class
// Last edited: 07/02/2022
//

#include "G4VSensitiveDetector.hh"

#include "Hit.hh"
#include "BDCrossing.hh"

class G4HCofThisEvent;
class G4Step;
class G4StepPoint;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector {
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

    public:
        virtual void Initialize(G4HCofThisEvent*);
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        virtual void EndOfEvent(G4HCofThisEvent*);
        
    private:
        HitCollection* fHitCollection;
        BDXCollection* fBDXCollection;
        G4int fHCID;
        G4int fBXCID;
};

#endif