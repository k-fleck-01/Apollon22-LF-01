//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Macro file for processing tracks from Geant4 simulation.
// Last edited: 17/02/2022
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


int ProcessList(const std::string&, std::vector<std::string>&);

int apollon_tracks_process(std::string fnamelist) {
    // *
    // Open root file to write to
    //
    std::string suffix("_tracks");
    std::string foutname = fnamelist.substr(fnamelist.find_last_of("/")+1);
    foutname = foutname.substr(0, foutname.find_last_of("."));
    foutname += suffix + std::string(".root");

    TFile* fout = new TFile(foutname.c_str(), "RECREATE");
    // *
    // Defining histograms
    // *
    int nbins = 50;
    TH1D* track_pdg = new TH1D("track_pdg", "", nbins, -25, 25);

    nbins = 25;
    TH1D* track_procid = new TH1D("track_procid", "", nbins, 2000, 2025);

    nbins = 400;
    const double zmin = -150.0; //cm
    const double zmax = 250.0;  //cm
    TH2D* track_procid_vtxz = new TH2D("track_procid_vtxz", "", nbins, zmin, zmax, 25, 2000, 2025);
    
    const double xmin = -100.0; //cm
    const double xmax = 100.0;  //cm
    TH2D* track_vtxx_vtxz_all = new TH2D("track_vtxx_vtxz_all", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_vtxx_vtxz_electron = new TH2D("track_vtxx_vtxz_electron", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_vtxx_vtxz_positron = new TH2D("track_vtxx_vtxz_positron", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_vtxx_vtxz_gamma = new TH2D("track_vtxx_vtxz_gamma", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_vtxx_vtxz_muminus = new TH2D("track_vtxx_vtxz_muminus", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_vtxx_vtxz_muplus = new TH2D("track_vtxx_vtxz_muplus", "", nbins, zmin, zmax, nbins, xmin, xmax);

    TH2D* track_endx_endz_muplus = new TH2D("track_endx_endz_muplus", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_all = new TH2D("track_endx_endz_all", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_electron = new TH2D("track_endx_endz_electron", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_positron = new TH2D("track_endx_endz_positron", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_gamma = new TH2D("track_endx_endz_gamma", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_muminus = new TH2D("track_endx_endz_muminus", "", nbins, zmin, zmax, nbins, xmin, xmax);
    TH2D* track_endx_endz_muplus = new TH2D("track_endx_endz_muplus", "", nbins, zmin, zmax, nbins, xmin, xmax);
    // *
    // Processing TChain
    // *
    std::vector<std::string> flist;
    ProcessList(fnamelist, flist);

    TChain* trackstree = new TChain("Tracks");
    std::for_each(flist.begin(), flist.end(), [trackstree](const std::string ss) { hitstree->Add(ss.c_str(), -1); });

    int evid, trackid, pdg, procid;
    double vtxx, vtxy, vtxz;
    double endx, endy, endz;
    double ekin;

    trackstree->SetBranchAddress("evid", &evid);
    trackstree->SetBranchAddress("trackid", &trackid);
    trackstree->SetBranchAddress("pdg", &pdg);
    trackstree->SetBranchAddress("procid", &procid);
    trackstree->SetBranchAddress("vtxx", &vtxx);
    trackstree->SetBranchAddress("vtxy", &vtxy);
    trackstree->SetBranchAddress("vtxz", &vtxz);
    trackstree->SetBranchAddress("endx", &endx);
    trackstree->SetBranchAddress("endy", &endy);
    trackstree->SetBranchAddress("endz", &endz);
    trackstree->SetBranchAddress("kEnergy", &ekin);

    Long64_t nevproc = trackstree->GetEntries();
    std::cout << "Events: " << nevproc << std::endl;

    for (Long64_t ii = 0; ii < nevproc; ++ii) {

        trackstree->GetEntry(ii);
        if (!(ii%10000)) std::cout << ii << " entries processed" << std::endl;

        track_pdg->Fill(pdg);
        track_procid->Fill(procid);
        track_procid_vtxz->Fill(procid, vtxz);
        track_vtxx_vtxz_all->Fill(vtxz, vtxx);
        track_endx_endz_all->Fill(endx, endz);

        if (pdg == 11) { // electrons
            track_vtxx_vtxz_electron->Fill(vtxz, vtxx);
            track_endx_endz_electron->Fill(endz, endx);
        }
        else if (pdg == -11) { // positrons
            track_vtxx_vtxz_positron->Fill(vtxz, vtxx);
            track_endx_endz_positron->Fill(endz, endx);
        }
        else if (pdg == 22) { // photons
            track_vtxx_vtxz_gamma->Fill(vtxz, vtxx);
            track_endx_endz_gamma->Fill(endz, endx);
        }
        else if (pdg == 13) { // muon-
            track_vtxx_vtxz_muminus->Fill(vtxz, vtxx);
            track_endx_endz_muminus->Fill(endz, endx);
        }
        else if (pdg == -13) { // muon+
            track_vtxx_vtxz_muplus->Fill(vtxz, vtxx);
            track_endx_endz_muplus->Fill(endz, endx);
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
