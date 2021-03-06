#ifndef HIT_H 
#define HIT_H 1
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
        ~Hit();

    public:
        G4double GetEdep() const;
        G4double GetEnergy() const;
        G4ThreeVector GetPosition() const;
        G4ThreeVector GetVertexPosition() const;
        G4int GetParticleType() const;
        G4int GetProcess() const;
        G4int GetDetectorID() const;
        G4int GetTrackID() const;

        void AddEdep(G4double);
        void AddEnergy(G4double);
        void AddPosition(G4ThreeVector);
        void AddVertexPosition(G4ThreeVector);
        void AddParticleType(G4int);
        void AddProcess(G4int);
        void AddDetectorID(G4int);
        void AddTrackID(G4int);

    private:
        G4double fEdep;
        G4double fEnergy;
        G4ThreeVector fPosition;
        G4ThreeVector fVertexPosition;
        G4int fParticleType;
        G4int fProcess;
        G4int fDetid;
        G4int fTrackid;

};

typedef G4THitsCollection<Hit> HitCollection;

#endif