//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for FileReader class
// Last edited: 08/08/2022
//
#include "G4RootAnalysisReader.hh"
#include "G4SystemOfUnits.hh"

#include "FileReader.hh"

FileReader::FileReader(G4String& fname, G4int& spid) : fFileName(fname), fSpectrumId(spid) {
    G4cout << "Instantiating FileReader..." << G4endl;
    G4cout << "Reading file " << fname << "..." << G4endl;
    ImportBeamFromFile();
}

FileReader::~FileReader()
{}

void FileReader::ImportBeamFromFile() {
    // Create or get analysis reader
    G4RootAnalysisReader* analysisReader = G4RootAnalysisReader::Instance();
    analysisReader->SetFileName(fFileName);
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
        for(int ii = 0; ii < nvars; ++ii) check |= analysisReader->SetNtupleDColumn(vName[ii], vars[ii]);
        
        if(!check) {
            G4Exception("FileReader:ImportBeamFromFile", "Unable to set branch addresses", FatalException, "");
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
    }
 
    fTrackEntries = trackEntry;
}