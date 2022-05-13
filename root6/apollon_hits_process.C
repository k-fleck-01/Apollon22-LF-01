//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Macro file for processing detector hits from Geant4 simulation.
// Last edited: 16/02/2022
//

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"


int ProcessList(const std::string&, std::vector<std::string>&);

int apollon_hits_process(std::string fnamelist) {
    // ************************************************************************
    // Open root file to write to
    // ************************************************************************
    std::string suffix("_hits");
    std::string foutname = fnamelist.substr(fnamelist.find_last_of("/")+1);
    foutname = foutname.substr(0, foutname.find_last_of("."));
    foutname += suffix + std::string(".root");

    TFile* fout = new TFile(foutname.c_str(), "RECREATE");
    // ************************************************************************
    // Defining histograms
    // ************************************************************************
    constexpr int& nSpaceBins  = 200;
    constexpr int& nEnergyBins = 100; 

    // Hits histograms
    TH2D* hits_xy_all = new TH2D("hits_xy_all", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_electron = new TH2D("hits_xy_electron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_positron = new TH2D("hits_xy_positron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_gamma = new TH2D("hits_xy_gamma", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_muminus = new TH2D("hits_xy_muminus", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_muplus = new TH2D("hits_xy_muplus", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);

    TH2D* hits_xy_edep_all = new TH2D("hits_xy_edep_all", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_edep_electron = new TH2D("hits_xy_edep_electron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_edep_positron = new TH2D("hits_xy_edep_positron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_edep_gamma = new TH2D("hits_xy_edep_gamma", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_edep_muminus = new TH2D("hits_xy_edep_muminus", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    TH2D* hits_xy_edep_muplus = new TH2D("hits_xy_edep_muplus", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80.);

    TH1D* hits_edep_all = new TH1D("hits_edep_all", "", nEnergyBins, 0., 2.);
    TH1D* hits_edep_electron = new TH1D("hits_edep_electron", "", nEnergyBins, 0., 2.);
    TH1D* hits_edep_positron = new TH1D("hits_edep_positron", "", nEnergyBins, 0., 2.);
    TH1D* hits_edep_gamma = new TH1D("hits_edep_gamma", "", nEnergyBins, 0., 2.);
    TH1D* hits_edep_muminus = new TH1D("hits_edep_muminus", "", nEnergyBins, 0., 2.);
    TH1D* hits_edep_muplus = new TH1D("hits_edep_muplus", "", nEnergyBins, 0., 2.);

    // Primaries histograms
    TH2D* primaries_xy = new TH2D("primaries_xy", "", nSpaceBins, -0.1, 0.1, nSpaceBins, -0.1, 0.1);
    TH1D* primaries_energy = new TH1D("primaries_energy", "", nEnergyBins, 0., 2000.);

    // Boundary crossing histograms
    TH3D* bdx_xyz_all = new TH3D("bdx_xyz_all", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    TH3D* bdx_xyz_electron = new TH3D("bdx_xyz_electron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    TH3D* bdx_xyz_positron = new TH3D("bdx_xyz_positron", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    TH3D* bdx_xyz_gamma = new TH3D("bdx_xyz_gamma", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    TH3D* bdx_xyz_muminus = new TH3D("bdx_xyz_muminus", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    TH3D* bdx_xyz_muplus = new TH3D("bdx_xyz_all", "", nSpaceBins, -150., 150., nSpaceBins, -80., 80., nSpaceBins, 49., 51.);
    
    TH2D* bdx_vtxx_vtxz_all = new TH2D("bdx_vtxx_vtxz_all", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxx_vtxz_electron = new TH2D("bdx_vtxx_vtxz_electron", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxx_vtxz_positron = new TH2D("bdx_vtxx_vtxz_positron", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxx_vtxz_gamma = new TH2D("bdx_vtxx_vtxz_gamma", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxx_vtxz_muminus = new TH2D("bdx_vtxx_vtxz_muminus", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxx_vtxz_muplus = new TH2D("bdx_vtxx_vtxz_all", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);

    TH2D* bdx_vtxy_vtxz_all = new TH2D("bdx_vtxy_vtxz_all", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxy_vtxz_electron = new TH2D("bdx_vtxy_vtxz_electron", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxy_vtxz_positron = new TH2D("bdx_vtxy_vtxz_positron", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxy_vtxz_gamma = new TH2D("bdx_vtxy_vtxz_gamma", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxy_vtxz_muminus = new TH2D("bdx_vtxy_vtxz_muminus", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    TH2D* bdx_vtxy_vtxz_muplus = new TH2D("bdx_vtxy_vtxz_all", nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);

    TH1D* bdx_vtxz_all = new TH1D("bdx_vtxz_all", "", nSpaceBins, -2500., 50.);
    TH1D* bdx_vtxz_electron = new TH1D("bdx_vtxz_electron", "", nSpaceBins, -2500., 50.);
    TH1D* bdx_vtxz_positron = new TH1D("bdx_vtxz_positron", "", nSpaceBins, -2500., 50.);
    TH1D* bdx_vtxz_gamma = new TH1D("bdx_vtxz_gamma", "", nSpaceBins, -2500., 50.);
    TH1D* bdx_vtxz_muminus = new TH1D("bdx_vtxz_muminus", "", nSpaceBins, -2500., 50.);
    TH1D* bdx_vtxz_muplus = new TH1D("bdx_vtxz_muplus", "", nSpaceBins, -2500., 50.); 

    TH1D* bdx_polar_all = new TH1D("bdx_polar_all", "", nEnergyBins, 0., CLHEP::pi);
    TH1D* bdx_polar_electron = new TH1D("bdx_polar_electron", "", nEnergyBins, 0., CLHEP::pi);
    TH1D* bdx_polar_positron = new TH1D("bdx_polar_positron", "", nEnergyBins, 0., CLHEP::pi);
    TH1D* bdx_polar_gamma = new TH1D("bdx_polar_gamma", "", nEnergyBins, 0., CLHEP::pi);
    TH1D* bdx_polar_muminus = new TH1D("bdx_polar_muminus", "", nEnergyBins, 0., CLHEP::pi);
    TH1D* bdx_polar_muplus = new TH1D("bdx_polar_muplus", "", nEnergyBins, 0., CLHEP::pi);
    
    TH1D* bdx_energy_all = new TH1D("bdx_energy_all", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_electron = new TH1D("bdx_energy_electron", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_positron = new TH1D("bdx_energy_positron", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_gamma = new TH1D("bdx_energy_gamma", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_muminus = new TH1D("bdx_energy_muminus", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_muplus = new TH1D("bdx_energy_muplus", nEnergyBins, 0., 2000.);

    TH1D* bdx_fluence_all = new TH1D("bdx_fluence_all", nEnergyBins, 0., 2000.);
    TH1D* bdx_fluence_electron = new TH1D("bdx_fluence_electron", nEnergyBins, 0., 2000.);
    TH1D* bdx_fluence_positron = new TH1D("bdx_fluence_positron", nEnergyBins, 0., 2000.);
    TH1D* bdx_fluence_gamma = new TH1D("bdx_fluence_gamma", nEnergyBins, 0., 2000.);
    TH1D* bdx_fluence_muminus = new TH1D("bdx_fluence_muminus", nEnergyBins, 0., 2000.);
    TH1D* bdx_fluence_muplus = new TH1D("bdx_fluence_muplus", nEnergyBins, 0., 2000.);

    TH1D* bdx_energy_fluence_all = new TH1D("bdx_energy_fluence_all", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_fluence_electron = new TH1D("bdx_energy_fluence_electron", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_fluence_positron = new TH1D("bdx_energy_fluence_positron", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_fluence_gamma = new TH1D("bdx_energy_fluence_gamma", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_fluence_muminus = new TH1D("bdx_energy_fluence_muminus", nEnergyBins, 0., 2000.);
    TH1D* bdx_energy_fluence_muplus = new TH1D("bdx_energy_fluence_muplus", nEnergyBins, 0., 2000.);

    TH1I* bdx_creation_process = new TH1I("bdx_creation_process", "", 25, 2000, 2025);
    TH1I* bdx_pdg = new TH1I("bdx_pdg", "", 40, -15, 25);

    // ************************************************************************
    // Processing hits TChain
    // ************************************************************************
    std::vector<std::string> flist;
    ProcessList(fnamelist, flist);

    TChain* hitstree = new TChain("Hits");
    std::for_each(flist.begin(), flist.end(), [hitstree](const std::string ss) { hitstree->Add(ss.c_str(), -1); });
    
    int evid, pdg, procid, detid;
    double xx, yy, zz;
    double vtxx, vtxy, vtxz;
    double eneg, edep;

    hitstree->SetBranchAddress("evid", &evid);
    hitstree->SetBranchAddress("x", &xx);
    hitstree->SetBranchAddress("y", &yy);
    hitstree->SetBranchAddress("z", &zz);
    hitstree->SetBranchAddress("vtxx", &vtxx);
    hitstree->SetBranchAddress("vtxy", &vtxy);
    hitstree->SetBranchAddress("vtxz", &vtxz);
    hitstree->SetBranchAddress("edep", &edep);
    hitstree->SetBranchAddress("energy", &eneg);
    hitstree->SetBranchAddress("pdg", &pdg);
    hitstree->SetBranchAddress("detid", &detid);

    Long64_t nevproc = hitstree->GetEntries();
    std::cout << "Entries: " << nevproc << std::endl;

    for(Long64_t ii = 0; ii < nevproc; ++ii) {

        hitstree->GetEntry(ii);
        if (!(ii%1000000)) std::cout << ii << " entries processed" << std::endl;
        
        if (detid != 3000) continue; // Only gamma spectrometer LANEX are processed
        hits_xy_all->Fill(xx, yy);
        hits_xy_edep_all->Fill(xx, yy, edep);
        hits_edep_all->Fill(edep);

        if (pdg == 11) { // electrons 
            hits_xy_electron->Fill(xx, yy);
            hits_xy_edep_electron->Fill(xx, yy, edep);
            hits_edep_electron->Fill(edep);
        }
        else if (pdg == -11) { // positrons
            hits_xy_positron->Fill(xx, yy);
            hits_xy_edep_positron->Fill(xx, yy, edep);
            hits_edep_positron->Fill(edep);
        }
        else if (pdg == 22) { // gammas
            hits_xy_gamma->Fill(xx, yy);
            hits_xy_edep_gamma->Fill(xx, yy, edep);
            hits_edep_gamma->Fill(edep);
        }
        else if (pdg == 13) { // mu- 
            hits_xy_muminus->Fill(xx, yy);
            hits_xy_edep_muminus->Fill(xx, yy, edep);
            hits_edep_muminus->Fill(edep);
        }
        else if (pdg == -13) { // mu+ 
            hits_xy_muplus->Fill(xx, yy);
            hits_xy_edep_muplus->Fill(xx, yy, edep);
            hits_edep_muplus->Fill(edep);
        }
    }
    // ************************************************************************
    // Process primary TChain
    // ************************************************************************
    TChain* primarytree = new TChain("Primaries");
    std::for_each(flist.begin(), flist.end(), [primarytree](const std::string ss) { primarytree->Add(ss.c_str(), -1); });

    primarytree->SetBranchAddress("x", &xx);
    primarytree->SetBranchAddress("y", &yy);
    primarytree->SetBranchAddress("z", &zz);
    primarytree->SetBranchAddress("E", &eneg);

    nevproc = primarytree->GetEntries();
    std::cout << "Entries: " << nevproc << std::endl;
    for (Long64_t ii = 0; ii < nevproc; ++ii) {

        primarytree->GetEntry(ii);
        if (!(ii%1000000)) std::cout << ii << " entries processed" << std::endl;

        primaries_xy->Fill(xx, yy);
        primaries_energy->Fill(eneg);
    }

    // ************************************************************************
    // Processing bdx TChain
    // ************************************************************************
    TChain* bdxtree = new TChain("Bdx");
    std::for_each(flist.begin(), flist.end(), [bdxtree](const std::string ss) { bdxtree->Add(ss.c_str(), -1); });
    
    int procid;
    double px, py, pz;
    double theta, fluence;

    bdxtree->SetBranchAddress("evid", &evid);
    bdxtree->SetBranchAddress("pdg", &pdg);
    bdxtree->SetBranchAddress("detid", &detid);
    bdxtree->SetBranchAddress("procid", &procid);
    bdxtree->SetBranchAddress("x", &xx);
    bdxtree->SetBranchAddress("y", &yy);
    bdxtree->SetBranchAddress("z", &zz);
    bdxtree->SetBranchAddress("vtxx", &vtxx);
    bdxtree->SetBranchAddress("vtxy", &vtxy);
    bdxtree->SetBranchAddress("vtxz", &vtxz);
    bdxtree->SetBranchAddress("px", &px);
    bdxtree->SetBranchAddress("py", &py);
    bdxtree->SetBranchAddress("pz", &pz);
    bdxtree->SetBranchAddress("energy", &eneg);
    bdxtree->SetBranchAddress("theta", &theta);
    bdxtree->SetBranchAddress("fluence", &fluence);

    nevproc = bdxtree->GetEntries();
    std::cout << "Entries: " << nevproc << std::endl;

    for(Long64_t ii = 0; ii < nevproc; ++ii) {

        bdxtree->GetEntry(ii);
        if (!(ii%1000000)) std::cout << ii << " entries processed" << std::endl;

        if (detid != 3000) continue;
        bdx_xyz_all->Fill(xx, yy, zz);
        bdx_vtxx_vtxz_all->Fill(vtxz, vtxx);
        bdx_vtxy_vtxz_all->Fill(vtxz, vtxy);
        bdx_vtxz_all->Fill(vtxz);
        bdx_polar_all->Fill(theta);
        bdx_energy_all->Fill(eneg);
        if (fluence >= 0.) {
            bdx_fluence_all->Fill(eneg, fluence);
            bdx_energy_fluence_all->Fill(eneg, eneg*fluence);
        }
        bdx_pdg->Fill(pdg);
        bdx_creation_process->Fill(procid);

        if (pdg == 11) { // electron 
            bdx_xyz_electron->Fill(xx, yy, zz);
            bdx_vtxx_vtxz_electron->Fill(vtxz, vtxx);
            bdx_vtxy_vtxx_electron->Fill(vtxz, vtxy);
            bdx_vtxz_electron->Fill(vtxz);
            bdx_polar_electron->Fill(theta); 
            bdx_energy_electron->Fill(eneg);
            if (fluence > 0.) {
                bdx_fluence_electron->Fill(eneg, fluence);
                bdx_energy_fluence_electron->Fill(eneg, eneg*fluence);
            }
        }
        else if (pdg == -11) { // positrons
            bdx_xyz_positron->Fill(xx, yy, zz);
            bdx_vtxx_vtxz_positron->Fill(vtxz, vtxx);
            bdx_vtxy_vtxx_positron->Fill(vtxz, vtxy);
            bdx_vtxz_positron->Fill(vtxz);
            bdx_polar_positron->Fill(theta);
            bdx_energy_positron->Fill(eneg);
            if (fluence > 0.) {
                bdx_fluence_positron->Fill(eneg, fluence);
                bdx_energy_fluence_positron->Fill(eneg, eneg*fluence);
            }
        }
        else if (pdg == 22) { // gamma
            bdx_xyz_gamma->Fill(xx, yy, zz);
            bdx_vtxx_vtxz_gamma->Fill(vtxz, vtxx);
            bdx_vtxy_vtxx_gamma->Fill(vtxz, vtxy);
            bdx_vtxz_gamma->Fill(vtxz);
            bdx_polar_gamma->Fill(theta); 
            bdx_energy_gamma->Fill(eneg);
            if (fluence > 0.) {
                bdx_fluence_gamma->Fill(eneg, fluence);
                bdx_energy_fluence_gamma->Fill(eneg, eneg*fluence);
            }
        }
        else if (pdg == 13) { // mu-
            bdx_xyz_muminus->Fill(xx, yy, zz);
            bdx_vtxx_vtxz_muminus->Fill(vtxz, vtxx);
            bdx_vtxy_vtxx_muminus->Fill(vtxz, vtxy);
            bdx_vtxz_muminus->Fill(vtxz);
            bdx_polar_muminus->Fill(theta);
            bdx_energy_muminus->Fill(eneg);
            if (fluence > 0.) {
                bdx_fluence_muminus->Fill(eneg, fluence);
                bdx_energy_fluence_muminus->Fill(eneg, eneg*fluence);
            }
        }
        else if (pdg == -13) { // mu+
            bdx_xyz_muplus->Fill(xx, yy, zz);
            bdx_vtxx_vtxz_muplus->Fill(vtxz, vtxx);
            bdx_vtxy_vtxx_muplus->Fill(vtxz, vtxy);
            bdx_vtxz_muplus->Fill(vtxz);
            bdx_polar_muplus->Fill(theta);
            bdx_energy_muplus->Fill(eneg);
            if (fluence > 0.) {
                bdx_fluence_muplus->Fill(eneg, fluence);
                bdx_energy_fluence_muplus->Fill(eneg, eneg*fluence);
            }
        }
    }

    fout->Write();
    fout->Close();

    return 0;
}

int ProcessList(const std::string& fnamelist, std::vector<std::string>& flist) {

    std::fstream fdata;
    fdata.open(fnamelist, std::ios::in);
    if(!fdata.is_open()) {
        throw std::runtime_error(std::string("Error opening file ") + fnamelist);
        return -1;
    }

    unsigned long lid = 0;
    while(!fdata.eof()) {
        std::string fname;
        fdata >> fname;
        if(!fdata.fail()) {
            flist.push_back(fname);
        }
        else if (fdata.eof()) {
            break;
        }
        else {
            std::cout << "ProcessList(..)  :  Error reading data from the file " << fnamelist 
                << ",  line: " << lid << ". Exit." << std::endl;
            fdata.close();
            return -2;
        }
        ++lid;
    }

    fdata.close();
    return 0;
}
