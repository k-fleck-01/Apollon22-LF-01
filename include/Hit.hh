#ifndef _HIT_H_
#define _HIT_H_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for Hit class
//

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class Hit : public G4VHit {
    public:
        Hit();
        Hit(G4ThreeVector, G4double, G4int, G4int, G4int);
        ~Hit();

    public:
        G4double GetEdep() const;
        G4double GetKineticEnergy() const;
        G4ThreeVector GetPosition() const;
        G4int GetParticleType() const;
        G4int GetCreatorProcess() const;
        G4int GetDetectorID() const;

        void AddEdep(G4double);
        void AddKineticEnergy(G4double);
        void AddPosition(G4ThreeVector);
        void AddParticleType(G4int);
        void AddCreatorProcess(G4int);
        void AddDetectorID(G4int);

    private:
        G4double fEdep;
        G4double fKinEnergy;
        G4ThreeVector fPosition;
        G4int fParticleType;
        G4int fCreatorProcess;
        G4int fDetid;

};

typedef G4THitsCollection<Hit> HitCollection;

#endif