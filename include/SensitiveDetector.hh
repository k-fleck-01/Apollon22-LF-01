#ifndef _SENSITIVE_DETECTOR_H_
#define _SENSITIVE_DETECTOR_H_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for SensitiveDetector class
// Last edited: 07/02/2022
//

#include "Hit.hh"

#include "G4VSensitiveDetector.hh"

class G4HCofThisEvent;
class G4Step;
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
        G4int fHCID;
};

#endif