#ifndef BD_CROSSING_HH
#define BD_CROSSING_HH 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for BDCrossing class
// Last edited: 11/05/2022
//

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"

class G4VProcess;

class BDCrossing: public G4VHit {
    public:
        BDCrossing();
        ~BDCrossing();

    public:
        void SetPDG(G4int);
        void SetDetID(G4int);
        void SetVertex(G4ThreeVector);
        void SetPosition(G4ThreeVector);
        void SetEnergy(G4double);
        void SetMomentum(G4ThreeVector);
        void SetAngle(G4ThreeVector);
        void SetFluence(G4ThreeVector, G4double);
        void SetCreatorProcess(G4int);

        G4int GetPDG() const;
        G4int GetDetID() const;
        G4ThreeVector GetVertex() const;
        G4ThreeVector GetPosition() const;
        G4double GetEnergy() const;
        G4ThreeVector GetMomentum() const;
        G4double GetAngle() const;
        G4double GetFluence() const;
        G4int GetProcessID() const;

    private:
        G4int fPdg;
        G4int fDetid;
        G4ThreeVector fVertex;
        G4ThreeVector fPosition;
        G4double fEnergy;
        G4ThreeVector fMomentum;
        G4double fAngle;
        G4double fFluence;
        G4int fProcid;
        
};

typedef G4THitsCollection<BDCrossing> BDXCollection;

#endif