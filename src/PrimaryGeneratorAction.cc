//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for PrimaryGeneratorAction class
// Last edited: 11/02/2022
//

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
//#include "PrimaryGeneratorMessenger.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

#include "G4RootAnalysisManager.hh"
#include "G4RootAnalysisReader.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det) : 
        G4VUserPrimaryGeneratorAction(),  
        fParticleGun(0),
        fDetectorConstruction(det),
        fBeamMode(-1),
        fTrackEntries(0),
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

    //fMessenger = new PrimaryGeneratorMessenger(this);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
    //delete fMessenger;
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
            if(evid > fTrackEntries) {
                // Event ID greater than number of sampled particles
                fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("geantino"));
                fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
            }

            G4ThreeVector pPos = fParticleGun->GetParticlePosition();
            pPos += GetImportBeamPos(evid);
            fParticleGun->SetParticlePosition(pPos);
            fParticleGun->SetParticleEnergy(GetImportBeamEnergy(evid));

            G4double theta = GetImportBeamDiv(evid);
            G4double phi   = 2.*3.14159265*G4UniformRandom();
            G4ThreeVector momentum_dir(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
            fParticleGun->SetParticleMomentumDirection(momentum_dir.unit());

            fParticleGun->GeneratePrimaryVertex(anEvent);
    }


    // Adding primary information to tree
    G4RootAnalysisManager* analysisManager = G4RootAnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(3, 0, x0/mm);
    analysisManager->FillNtupleDColumn(3, 1, y0/mm);
    analysisManager->FillNtupleDColumn(3, 2, z0/mm);
    analysisManager->FillNtupleDColumn(3, 3, energy/MeV);
    analysisManager->FillNtupleDColumn(3, 4, theta/mrad);
    analysisManager->FillNtupleDColumn(3, 5, phi/rad);
    analysisManager->AddNtupleRow(3);

}

void PrimaryGeneratorAction::ImportBeamFromFile(G4String& fname) {
    // Create or get analysis reader
    G4RootAnalysisReader* analysisReader = G4RootAnalysisReader::Instance();
    analysisReader->SetFileName(fname);
    G4String vName[] = {"pos_x", "pos_y", "pos_z", "energy", "div"};
    const int nvars = 5;
    G4double vars[nvars];
    G4int trackEntry = 0;

    G4String tName;
    switch(fSpectrumId) {
        case 1:
            tName = "dNdE_1";
            break;
        case 2:
            tName = "dNdE_2";
            break;
        case 3:
            tName = "dNdE_3";
            break;
        default:
            tName = "dNdE_1";
    }

    G4int ntupleId = analysisReader->GetNtuple(tName);
    if(ntupleId >= 0) {
        bool check = false;
        for(int ii = 0; ii < nvars; ++ii) check |= analysisReader->SetNtupleDcolumn(vName[ii], vars[ii]);
        
        if(!check) {
            G4Exception("PrimaryGeneratorAction:ImportBeamFromFile", "Unable to set branch addresses", FatalException, "");
        }
        else {
            G4cout << "Branch addresses set successfully!" << G4endl;
        }

        // Reading ntuple
        while(analysisReader->GetNtupleRow()) {
            trackEntry++;
            
            fImportPos.push_back(G4ThreeVector(vars[0]*um, vars[1]*um, vars[2]*um));
            fImportEnergy.push_back(vars[3]*GeV);
            fImportDiv.push_back(vars[4]*mrad);
        }

        // Set beam mode to "import from file"
        fBeamMode = 1; 
        fTrackEntries = trackEntry;
    }
}
