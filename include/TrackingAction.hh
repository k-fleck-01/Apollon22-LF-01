#ifndef _TRACKING_ACTION_H_
#define _TRACKING_ACTION_H_
// *
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for TrackingAction class
// Last edited: 16/02/2022
// *

#include "G4UserTrackingAction.hh"

#include "G4ThreeVector.hh"

enum G4EmProcessSubType 
{ 
  fCoulombScattering = 1, 
  fIonisation = 2, 
  fBremsstrahlung = 3,
  fMuBremsstrahlung = 4, 
  fPairProdByCharged = 5,
  fAnnihilation = 6, 
  fAnnihilationToMuMu = 7,
  fAnnihilationToHadrons = 8,
  fNuclearStopping = 9,
  fElectronGeneralProcess = 10,

  fMultipleScattering = 11, 
  
  fRayleigh = 12,
  fPhotoElectricEffect = 13,
  fComptonScattering = 14,
  fGammaConversion = 15,
  fGammaConversionToMuMu = 16,
  fGammaGeneralProcess = 17,
 
  fCerenkov = 21,
  fScintillation = 22,
  fSynchrotronRadiation = 23,
  fTransitionRadiation = 24,
  
};

class RunAction;
class G4Track;

class TrackingAction : public G4UserTrackingAction {
    public:
        TrackingAction(RunAction*);
        ~TrackingAction();

    public:
        virtual void PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);

};

#endif