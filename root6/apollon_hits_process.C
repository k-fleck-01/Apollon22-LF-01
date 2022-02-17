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


int ProcessList(const std::string&, std::vector<std::string>&);

int apollon_hits_process(std::string fnamelist) {
    // *
    // Open root file to write to
    //
    std::string suffix("_hits");
    std::string foutname = fnamelist.substr(fnamelist.find_last_of("/")+1);
    foutname = foutname.substr(0, foutname.find_last_of("."));
    foutname += suffix + std::string(".root");

    TFile* fout = new TFile(foutname.c_str(), "RECREATE");
    // *
    // Defining histograms
    // *
    TH1I* target_pdg_all = new TH1I("target_pdg_all", "", 50, -25, 25);

    int nbins = 200;
    TH1D* target_kenergy_all = new TH1D("target_kenergy_all", "",  nbins, 0., 2000.);
    TH1D* target_kenergy_electron = new TH1D("target_kenergy_electron", "", nbins, 0., 2000.);
    TH1D* target_kenergy_positron = new TH1D("target_kenergy_positron", "", nbins, 0., 2000.);
    TH1D* target_kenergy_gamma = new TH1D("target_kenergy_gamma", "", nbins, 0., 2000.);
    TH1D* target_kenergy_muminus = new TH1D("target_kenergy_muminus", "", nbins, 0., 2000.);
    TH1D* target_kenergy_muplus = new TH1D("target_kenergy_muplus", "", nbins, 0., 2000.);

    const double targetz0 = 149.99;
    const double targetz1 = 150.0;
    TH1D* target_edep_z_all = new TH1D("target_edep_z_all", "", nbins, targetz0, targetz1);
    TH1D* target_edep_z_electron = new TH1D("target_edep_z_electron", "", nbins, targetz0, targetz1);
    TH1D* target_edep_z_positron = new TH1D("target_edep_z_positron", "", nbins, targetz0, targetz1);
    TH1D* target_edep_z_gamma = new TH1D("target_edep_z_gamma", "", nbins, targetz0, targetz1);
    TH1D* target_edep_z_muminus = new TH1D("target_edep_z_muminus", "", nbins, targetz0, targetz1);
    TH1D* target_edep_z_muplus = new TH1D("target_edep_z_muplus", "", nbins, targetz0, targetz1);

    const double targetx0 = -1.0;
    const double targetx1 = 1.0;
    const double targety0 = -1.0;
    const double targety1 = 1.0;
    nbins = 400;
    TH2D* target_edep_xy_all = new TH2D("target_edep_xy_all", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    TH2D* target_edep_xy_electron = new TH2D("target_edep_xy_electron", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    TH2D* target_edep_xy_positron = new TH2D("target_edep_xy_positron", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    TH2D* target_edep_xy_gamma = new TH2D("target_edep_xy_gamma", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    TH2D* target_edep_xy_muminus = new TH2D("target_edep_xy_muminus", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    TH2D* target_edep_xy_muplus = new TH2D("target_edep_xy_muplus", "", nbins, targetx0, targetx1, nbins, targety0, targety1);
    // *
    // *
    TH1I* yag_pdg_all = new TH1I("yag_pdg_all", "", 50, -25, 25);

    nbins = 200;
    TH1D* yag_kenergy_all = new TH1D("yag_kenergy_all", "",  nbins, 0., 2000.);
    TH1D* yag_kenergy_electron = new TH1D("yag_kenergy_electron", "", nbins, 0., 2000.);
    TH1D* yag_kenergy_positron = new TH1D("yag_kenergy_positron", "", nbins, 0., 2000.);
    TH1D* yag_kenergy_gamma = new TH1D("yag_kenergy_gamma", "", nbins, 0., 2000.);
    TH1D* yag_kenergy_muminus = new TH1D("yag_kenergy_muminus", "", nbins, 0., 2000.);
    TH1D* yag_kenergy_muplus = new TH1D("yag_kenergy_muplus", "", nbins, 0., 2000.);

    const double yagz0 = 101.8;
    const double yagz1 = 101.81;
    TH1D* yag_edep_z_all = new TH1D("yag_edep_z_all", "", nbins, yagz0, yagz1);
    TH1D* yag_edep_z_electron = new TH1D("yag_edep_z_electron", "", nbins, yagz0, yagz1);
    TH1D* yag_edep_z_positron = new TH1D("yag_edep_z_positron", "", nbins, yagz0, yagz1);
    TH1D* yag_edep_z_gamma = new TH1D("yag_edep_z_gamma", "", nbins, yagz0, yagz1);
    TH1D* yag_edep_z_muminus = new TH1D("yag_edep_z_muminus", "", nbins, yagz0, yagz1);
    TH1D* yag_edep_z_muplus = new TH1D("yag_edep_z_muplus", "", nbins, yagz0, yagz1);

    const double yagx0 = 4.0;
    const double yagx1 = 24.0;
    const double yagy0 = -4.0;
    const double yagy1 = 4.0;
    nbins = 400;
    TH2D* yag_edep_xy_all = new TH2D("yag_edep_xy_all", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    TH2D* yag_edep_xy_electron = new TH2D("yag_edep_xy_electron", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    TH2D* yag_edep_xy_positron = new TH2D("yag_edep_xy_positron", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    TH2D* yag_edep_xy_gamma = new TH2D("yag_edep_xy_gamma", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    TH2D* yag_edep_xy_muminus = new TH2D("yag_edep_xy_muminus", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    TH2D* yag_edep_xy_muplus = new TH2D("yag_edep_xy_muplus", "", nbins, yagx0, yagx1, nbins, yagy0, yagy1);
    // *
    // *
    TH1I* kapton_pdg_all = new TH1I("kapton_pdg_all", "", 50, -25, 25);

    nbins = 200;
    TH1D* kapton_kenergy_all = new TH1D("kapton_kenergy_all", "",  nbins, 0., 2000.);
    TH1D* kapton_kenergy_electron = new TH1D("kapton_kenergy_electron", "", nbins, 0., 2000.);
    TH1D* kapton_kenergy_positron = new TH1D("kapton_kenergy_positron", "", nbins, 0., 2000.);
    TH1D* kapton_kenergy_gamma = new TH1D("kapton_kenergy_gamma", "", nbins, 0., 2000.);
    TH1D* kapton_kenergy_muminus = new TH1D("kapton_kenergy_muminus", "", nbins, 0., 2000.);
    TH1D* kapton_kenergy_muplus = new TH1D("kapton_kenergy_muplus", "", nbins, 0., 2000.);

    const double kaptonz0 = 103.98;
    const double kaptonz1 = 104.0;
    TH1D* kapton_edep_z_all = new TH1D("kapton_edep_z_all", "", nbins, kaptonz0, kaptonz1);
    TH1D* kapton_edep_z_electron = new TH1D("kapton_edep_z_electron", "", nbins, kaptonz0, kaptonz1);
    TH1D* kapton_edep_z_positron = new TH1D("kapton_edep_z_positron", "", nbins, kaptonz0, kaptonz1);
    TH1D* kapton_edep_z_gamma = new TH1D("kapton_edep_z_gamma", "", nbins, kaptonz0, kaptonz1);
    TH1D* kapton_edep_z_muminus = new TH1D("kapton_edep_z_muminus", "", nbins, kaptonz0, kaptonz1);
    TH1D* kapton_edep_z_muplus = new TH1D("kapton_edep_z_muplus", "", nbins, kaptonz0, kaptonz1);

    const double kaptonx0 = -25.0;
    const double kaptonx1 = 25.0;
    const double kaptony0 = -4.0;
    const double kaptony1 = 4.0;
    nbins = 400;
    TH2D* kapton_edep_xy_all = new TH2D("kapton_edep_xy_all", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    TH2D* kapton_edep_xy_electron = new TH2D("kapton_edep_xy_electron", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    TH2D *kapton_edep_xy_positron = new TH2D("kapton_edep_xy_positron", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    TH2D* kapton_edep_xy_gamma = new TH2D("kapton_edep_xy_gamma", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    TH2D* kapton_edep_xy_muminus = new TH2D("kapton_edep_xy_muminus", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    TH2D* kapton_edep_xy_muplus = new TH2D("kapton_edep_xy_muplus", "", nbins, kaptonx0, kaptonx1, nbins, kaptony0, kaptony1);
    // *
    // *
    TH1I* lanex_pdg_all = new TH1I("lanex_pdg_all", "", 50, -25, 25);

    nbins = 200;
    TH1D* lanex_kenergy_all = new TH1D("lanex_kenergy_all", "",  nbins, 0., 2000.);
    TH1D* lanex_kenergy_electron = new TH1D("lanex_kenergy_electron", "", nbins, 0., 2000.);
    TH1D* lanex_kenergy_positron = new TH1D("lanex_kenergy_positron", "", nbins, 0., 2000.);
    TH1D* lanex_kenergy_gamma = new TH1D("lanex_kenergy_gamma", "", nbins, 0., 2000.);
    TH1D* lanex_kenergy_muminus = new TH1D("lanex_kenergy_muminus", "", nbins, 0., 2000.);
    TH1D* lanex_kenergy_muplus = new TH1D("lanex_kenergy_muplus", "", nbins, 0., 2000.);

    const double lanexz0 = 225.0;
    const double lanexz1 = 225.05;
    TH1D* lanex_edep_z_all = new TH1D("lanex_edep_z_all", "", nbins, lanexz0, lanexz1);
    TH1D* lanex_edep_z_electron = new TH1D("lanex_edep_z_electron", "", nbins, lanexz0, lanexz1);
    TH1D* lanex_edep_z_positron = new TH1D("lanex_edep_z_positron", "", nbins, lanexz0, lanexz1);
    TH1D* lanex_edep_z_gamma = new TH1D("lanex_edep_z_gamma", "", nbins, lanexz0, lanexz1);
    TH1D* lanex_edep_z_muminus = new TH1D("lanex_edep_z_muminus", "", nbins, lanexz0, lanexz1);
    TH1D* lanex_edep_z_muplus = new TH1D("lanex_edep_z_muplus", "", nbins, lanexz0, lanexz1);

    const double lanexx0 = -20.0;
    const double lanexx1 = 20.0;
    const double lanexy0 = -5.0;
    const double lanexy1 = 5.0;
    nbins = 400;
    TH2D* lanex_edep_xy_all = new TH2D("lanex_edep_xy_all", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    TH2D* lanex_edep_xy_electron = new TH2D("lanex_edep_xy_electron", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    TH2D* lanex_edep_xy_positron = new TH2D("lanex_edep_xy_positron", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    TH2D* lanex_edep_xy_gamma = new TH2D("lanex_edep_xy_gamma", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    TH2D* lanex_edep_xy_muminus = new TH2D("lanex_edep_xy_muminus", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    TH2D* lanex_edep_xy_muplus = new TH2D("lanex_edep_xy_muplus", "", nbins, lanexx0, lanexx1, nbins, lanexy0, lanexy1);
    // *
    // Processing TChain
    // *
    std::vector<std::string> flist;
    ProcessList(fnamelist, flist);

    TChain* hitstree = new TChain("Hits");
    std::for_each(flist.begin(), flist.end(), [hitstree](const std::string ss) { hitstree->Add(ss.c_str(), -1); });
    
    int evid, pdg, procid, detid;
    double xx, yy, zz;
    double ekin, edep;

    hitstree->SetBranchAddress("evid", &evid);
    hitstree->SetBranchAddress("x", &xx);
    hitstree->SetBranchAddress("y", &yy);
    hitstree->SetBranchAddress("z", &zz);
    hitstree->SetBranchAddress("edep", &edep);
    hitstree->SetBranchAddress("eKin", &ekin);
    hitstree->SetBranchAddress("pdg", &pdg);
    hitstree->SetBranchAddress("procid", &procid);
    hitstree->SetBranchAddress("detid", &detid);

    Long64_t nevproc = hitstree->GetEntries();
    std::cout << "Entries: " << nevproc << std::endl;

    for(Long64_t ii = 0; ii < nevproc; ++ii) {

        hitstree->GetEntry(ii);
        if (!(ii%10000)) std::cout << ii << " entries processed" << std::endl;
        

        if (detid == 0) { // target
            target_pdg_all->Fill(pdg);
            if(ekin > -1.0) target_kenergy_all->Fill(ekin);
            target_edep_z_all->Fill(zz, edep);
            target_edep_xy_all->Fill(xx, yy, edep);

            if (pdg==11) { // electrons
                if(ekin > -1.0) target_kenergy_electron->Fill(ekin);
                target_edep_z_electron->Fill(zz, edep);
                target_edep_xy_electron->Fill(xx, yy, edep);
            }
            else if (pdg == -11) { // positrons
                if(ekin > -1.0) target_kenergy_positron->Fill(ekin);
                target_edep_z_positron->Fill(zz, edep);
                target_edep_xy_positron->Fill(xx, yy, edep);
            }
            else if (pdg == 22) { // gammas
                if(ekin > -1.0) target_kenergy_gamma->Fill(ekin);
                target_edep_z_gamma->Fill(zz, edep);
                target_edep_xy_gamma->Fill(xx, yy, edep);
            }
            else if (pdg == 13) { // mu-
                if(ekin > -1.0) target_kenergy_muminus->Fill(ekin);
                target_edep_z_muminus->Fill(zz, edep);
                target_edep_xy_muminus->Fill(xx, yy, edep);
            }
            else if (pdg == -13) { // mu+
                if(ekin > -1.0) target_kenergy_muplus->Fill(ekin);
                target_edep_z_muplus->Fill(zz, edep);
                target_edep_xy_muplus->Fill(xx, yy, edep);
            }
        }
        else if (detid == 1) { // yag screen
            yag_pdg_all->Fill(pdg);
            if(ekin > -1.0) yag_kenergy_all->Fill(ekin);
            yag_edep_z_all->Fill(zz, edep);
            yag_edep_xy_all->Fill(xx, yy, edep);

            if (pdg==11) { 
                if(ekin > -1.0) yag_kenergy_electron->Fill(ekin);
                yag_edep_z_electron->Fill(zz, edep);
                yag_edep_xy_electron->Fill(xx, yy, edep);
            }
            else if (pdg == -11) {
                if(ekin > -1.0) yag_kenergy_positron->Fill(ekin);
                yag_edep_z_positron->Fill(zz, edep);
                yag_edep_xy_positron->Fill(xx, yy, edep);
            }
            else if (pdg == 22) {
                if(ekin > -1.0) yag_kenergy_gamma->Fill(ekin);
                yag_edep_z_gamma->Fill(zz, edep);
                yag_edep_xy_gamma->Fill(xx, yy, edep);
            }
            else if (pdg == 13) {
                if(ekin > -1.0) yag_kenergy_muminus->Fill(ekin);
                yag_edep_z_muminus->Fill(zz, edep);
                yag_edep_xy_muminus->Fill(xx, yy, edep);
            }
            else if (pdg == -13) {
                if(ekin > -1.0) yag_kenergy_muplus->Fill(ekin);
                yag_edep_z_muplus->Fill(zz, edep);
                yag_edep_xy_muplus->Fill(xx, yy, edep);
            }
        }
        else if (detid == 2) { // kapton window
            kapton_pdg_all->Fill(pdg);
            if(ekin > -1.0) kapton_kenergy_all->Fill(ekin);
            kapton_edep_z_all->Fill(zz, edep);
            kapton_edep_xy_all->Fill(xx, yy, edep);

            if (pdg==11) { 
                if(ekin > -1.0) kapton_kenergy_electron->Fill(ekin);
                kapton_edep_z_electron->Fill(zz, edep);
                kapton_edep_xy_electron->Fill(xx, yy, edep);
            }
            else if (pdg == -11) {
                if(ekin > -1.0) kapton_kenergy_positron->Fill(ekin);
                kapton_edep_z_positron->Fill(zz, edep);
                kapton_edep_xy_positron->Fill(xx, yy, edep);
            }
            else if (pdg == 22) {
                if(ekin > -1.0) kapton_kenergy_gamma->Fill(ekin);
                kapton_edep_z_gamma->Fill(zz, edep);
                kapton_edep_xy_gamma->Fill(xx, yy, edep);
            }
            else if (pdg == 13) {
                if(ekin > -1.0) kapton_kenergy_muminus->Fill(ekin);
                kapton_edep_z_muminus->Fill(zz, edep);
                kapton_edep_xy_muminus->Fill(xx, yy, edep);
            }
            else if (pdg == -13) {
                if(ekin > -1.0) kapton_kenergy_muplus->Fill(ekin);
                kapton_edep_z_muplus->Fill(zz, edep);
                kapton_edep_xy_muplus->Fill(xx, yy, edep);
            }
        }
        else if (detid == 3) {
            lanex_pdg_all->Fill(pdg);
            if(ekin > -1.0) lanex_kenergy_all->Fill(ekin);
            lanex_edep_z_all->Fill(zz, edep);
            lanex_edep_xy_all->Fill(xx, yy, edep);

            if (pdg==11) { 
                if(ekin > -1.0) lanex_kenergy_electron->Fill(ekin);
                lanex_edep_z_electron->Fill(zz, edep);
                lanex_edep_xy_electron->Fill(xx, yy, edep);
            }
            else if (pdg == -11) {
                if(ekin > -1.0) lanex_kenergy_positron->Fill(ekin);
                lanex_edep_z_positron->Fill(zz, edep);
                lanex_edep_xy_positron->Fill(xx, yy, edep);
            }
            else if (pdg == 22) {
                if(ekin > -1.0) lanex_kenergy_gamma->Fill(ekin);
                lanex_edep_z_gamma->Fill(zz, edep);
                lanex_edep_xy_gamma->Fill(xx, yy, edep);
            }
            else if (pdg == 13) {
                if(ekin > -1.0) lanex_kenergy_muminus->Fill(ekin);
                lanex_edep_z_muminus->Fill(zz, edep);
                lanex_edep_xy_muminus->Fill(xx, yy, edep);
            }
            else if (pdg == -13) {
                if(ekin > -1.0) lanex_kenergy_muplus->Fill(ekin);
                lanex_edep_z_muplus->Fill(zz, edep);
                lanex_edep_xy_muplus->Fill(xx, yy, edep);
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
