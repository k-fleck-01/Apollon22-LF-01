//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Macro file for processing detector hits from Geant4 simulation.
// Last edited: 16/02/2022
//
#ifndef __RUN_HITS_PROCESS__

void apollon_hits_process(std::string fnamelist)
{
   gROOT->ProcessLineSync("#define __RUN_HITS_PROCESS__ 1");
   gROOT->ProcessLineSync(".L MHists.cc+");
   gROOT->ProcessLine("#include \"apollon_hits_process.C\"");
   gROOT->ProcessLine(fnamelist.c_str() ? Form("run_apollon_process_hits(\"%s\")", fnamelist.c_str()) : "run_apollon_process_hits(0)");
   gROOT->ProcessLine("#undef __RUN_HITS_PROCESS__");
}

#else

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

#include "MHists.hh"

int ProcessList(const std::string&, std::vector<std::string>&);
void CreateHistograms(MHists*);

int run_apollon_process_hits(const char* fnamelist) {
    // ************************************************************************
    // Open root file to write to
    // ************************************************************************
    std::string suffix("_hits");
    std::string fname(fnamelist);
    std::string foutname = fname.substr(fname.find_last_of("/")+1);
    foutname = foutname.substr(0, foutname.find_last_of("."));
    foutname += suffix + std::string(".root");

    // ************************************************************************
    // Defining histograms
    // ************************************************************************
    MHists* mh = new MHists();
    CreateHistograms(mh);

    // ************************************************************************
    // Processing hits TChain
    // ************************************************************************
    std::vector<std::string> flist;
    ProcessList(fnamelist, flist);

    TChain* hitstree = new TChain("Hits");
    std::for_each(flist.begin(), flist.end(), [hitstree](const std::string ss) { hitstree->Add(ss.c_str(), -1); });
    
    int evid, pdg, detid;
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
    int ndet = 0;

    for(Long64_t ii = 0; ii < nevproc; ++ii) {
        hitstree->GetEntry(ii);
        if (!(ii%1000000)) std::cout << ii << " entries processed" << std::endl;

        if (detid >= 2000 && detid <= 2020) { // Cr39 stacks
            ndet = detid - 1999;

            mh->FillHistW("cr39_hits_xy", ndet, xx+180., yy);
            mh->FillHistW("cr39_hits_edep_xy", ndet, xx+180., yy, edep);
            mh->FillHistW("cr39_hits_edep", ndet, edep);
            mh->FillHistW("cr39_hits_e_edep", ndet, eneg, edep);

            if (pdg == 13 || pdg == -13) mh->FillHistW("cr39_hits_edep_muon", ndet, edep);
            if (pdg == 2112) mh->FillHistW("cr39_hits_edep_neutron", ndet, edep);
        }
        else if (detid == 3000) { // LANEX screens
            ndet = detid - 2999;

            mh->FillHistW("lanex_hits_xy", ndet, xx, yy);
            mh->FillHistW("lanex_hits_edep_xy", ndet, xx, yy, edep);
            mh->FillHistW("lanex_hits_edep", ndet, edep);
            mh->FillHistW("lanex_hits_e_edep", ndet, eneg, edep);

            if (pdg == 11) mh->FillHistW("lanex_hits_edep_electron", ndet, edep);
            if (pdg == -11) mh->FillHistW("lanex_hits_edep_positron", ndet, edep);
            if (pdg == 22) mh->FillHistW("lanex_hits_edep_gamma", ndet, edep);
            if (pdg == 2112) mh->FillHistW("lanex_hits_edep_neutron", ndet, edep);
        }
    }
    // ************************************************************************
    // Process primary TChain
    // ************************************************************************
    TChain* primarytree = new TChain("Primaries");
    std::for_each(flist.begin(), flist.end(), [primarytree](const std::string ss) { primarytree->Add(ss.c_str(), -1); });

    double polar, azimuth;
    primarytree->SetBranchAddress("x", &xx);
    primarytree->SetBranchAddress("y", &yy);
    primarytree->SetBranchAddress("z", &zz);
    primarytree->SetBranchAddress("E", &eneg);
    primarytree->SetBranchAddress("theta", &polar);
    primarytree->SetBranchAddress("phi", &azimuth);

    nevproc = primarytree->GetEntries();
    std::cout << "Entries: " << nevproc << std::endl;
    for (Long64_t ii = 0; ii < nevproc; ++ii) {
        primarytree->GetEntry(ii);
        if (!(ii%1000000)) std::cout << ii << " entries processed" << std::endl;

        mh->FillHistW("primaries_xy", 1, xx, yy);
        mh->FillHistW("primaries_e", 1, eneg);
        mh->FillHistW("primaries_theta", 1, polar);
        mh->FillHistW("primaries_phi", 1, azimuth);     
    }

    // ************************************************************************
    // Processing bdx TChain
    // ************************************************************************
    TChain* bdxtree = new TChain("Bdx");
    std::for_each(flist.begin(), flist.end(), [bdxtree](const std::string ss) { bdxtree->Add(ss.c_str(), -1); });
    
    int procid;
    double px, py, pz;
    double theta, fluence;

    bdxtree->SetBranchAddress("eventid", &evid);
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

        if (detid >= 2000 && detid <= 2020) { // Cr39 stacks
            ndet = detid - 1999;

            mh->FillHistW("cr39_bdx_xy", ndet, xx+180., yy);
            mh->FillHistW("cr39_bdx_vtxx_vtxz", ndet, vtxz, vtxx);
            mh->FillHistW("cr39_bdx_vtxy_vtxz", ndet, vtxz, vtxy);
            mh->FillHistW("cr39_bdx_vtxz", ndet, vtxz);
            mh->FillHistW("cr39_bdx_e", ndet, eneg);
            mh->FillHistW("cr39_bdx_pdg", ndet, pdg);
            mh->FillHistW("cr39_bdx_procid", ndet, procid);

            if (pdg == 13 || pdg == -13) { // muons 
                mh->FillHistW("cr39_bdx_xy_muon", ndet, xx+180., yy);
                mh->FillHistW("cr39_bdx_vtxx_vtxz_muon", ndet, vtxz, vtxx);
                mh->FillHistW("cr39_bdx_vtxy_vtxz_muon", ndet, vtxz, vtxy);
                mh->FillHistW("cr39_bdx_vtxz_muon", ndet, vtxz);
                mh->FillHistW("cr39_bdx_e_muon", ndet, eneg);
            }
            else if (pdg == 2112) { // neutrons
                mh->FillHistW("cr39_bdx_xy_neutron", ndet, xx+180., yy);
                mh->FillHistW("cr39_bdx_vtxx_vtxz_neutron", ndet, vtxz, vtxx);
                mh->FillHistW("cr39_bdx_vtxy_vtxz_neutron", ndet, vtxz, vtxy);
                mh->FillHistW("cr39_bdx_vtxz_neutron", ndet, vtxz);
                mh->FillHistW("cr39_bdx_e_neutron", ndet, eneg);
            }
            else {
                mh->FillHistW("cr39_bdx_xy_other", ndet, xx+180., yy);
                mh->FillHistW("cr39_bdx_vtxx_vtxz_other", ndet, vtxz, vtxx);
                mh->FillHistW("cr39_bdx_vtxy_vtxz_other", ndet, vtxz, vtxy);
                mh->FillHistW("cr39_bdx_vtxz_other", ndet, vtxz);
                mh->FillHistW("cr39_bdx_e_other", ndet, eneg);
            }
        }
        else if (detid == 3000) { // LANEX screen
            ndet = detid - 2999;

            mh->FillHistW("lanex_bdx_xy", ndet, xx+180., yy);
            mh->FillHistW("lanex_bdx_vtxx_vtxz", ndet, vtxz, vtxx);
            mh->FillHistW("lanex_bdx_vtxy_vtxz", ndet, vtxz, vtxy);
            mh->FillHistW("lanex_bdx_vtxz", ndet, vtxz);
            mh->FillHistW("lanex_bdx_e", ndet, eneg);
            mh->FillHistW("lanex_bdx_pdg", ndet, pdg);
            mh->FillHistW("lanex_bdx_procid", ndet, procid);

            if (pdg == 11) { // electrons 
                mh->FillHistW("lanex_bdx_xy_electron", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_electron", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_electron", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_electron", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_electron", ndet, eneg);
            }
            else if (pdg == -11) { // positrons
                mh->FillHistW("lanex_bdx_xy_positron", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_positron", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_positron", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_positron", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_positron", ndet, eneg);
            }
            else if (pdg == 22) { // photons
                mh->FillHistW("lanex_bdx_xy_gamma", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_gamma", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_gamma", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_gamma", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_gamma", ndet, eneg);
            }
            else if (pdg == 2112) { // neutrons
                mh->FillHistW("lanex_bdx_xy_neutron", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_neutron", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_neutron", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_neutron", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_neutron", ndet, eneg);
            }
            else {
                mh->FillHistW("lanex_bdx_xy_other", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_other", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_other", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_other", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_other", ndet, eneg);
            }
        }
    }

    mh->SaveHists(foutname);
    std::cout << "Histograms written to: " << foutname << std::endl;
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
//
//
void CreateHistograms(MHists* mh) {
    constexpr int nSpaceBins  = 200;
    constexpr int nEnergyBins = 100;

    // Hits histograms
    mh->AddHistograms("lanex_hits_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep", 1, nEnergyBins, 0., 200.);
    mh->AddHistograms("lanex_hits_edep_electron", 1, nEnergyBins, 0., 200.);
    mh->AddHistograms("lanex_hits_edep_positron", 1, nEnergyBins, 0., 200.);
    mh->AddHistograms("lanex_hits_edep_gamma", 1, nEnergyBins, 0., 200.);
    mh->AddHistograms("lanex_hits_edep_neutron", 1, nEnergyBins, 0., 200.);
    mh->AddHistograms("lanex_hits_e_edep", 1, nEnergyBins, 0., 2000., nEnergyBins, 0., 200.);

    mh->AddHistograms("cr39_hits_xy", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);
    mh->AddHistograms("cr39_hits_edep_xy", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);
    mh->AddHistograms("cr39_hits_edep", 20, nEnergyBins, 0., 200.);
    mh->AddHistograms("cr39_hits_edep_muon", 20, nEnergyBins, 0., 200.);
    mh->AddHistograms("cr39_hits_edep_neutron", 20, nEnergyBins, 0., 200.);
    mh->AddHistograms("cr39_hits_e_edep", 20, nEnergyBins, 0., 2000., nEnergyBins, 0., 200.);
    
    // Primaries histograms
    mh->AddHistograms("primaries_xy", 1, nSpaceBins, -2., 2., nSpaceBins, -2., 2.);
    mh->AddHistograms("primaries_e", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("primaries_theta", 1, nEnergyBins, 0., 3.14159265);
    mh->AddHistograms("primaries_phi", 1, nEnergyBins, 0., 2.*3.14159265);

    // Boundary crossing histograms
    mh->AddHistograms("lanex_bdx_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_electron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_positron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_gamma", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_neutron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_other", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);

    mh->AddHistograms("lanex_bdx_vtxx_vtxz", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_electron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_positron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_gamma", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_neutron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_other", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxy_vtxz", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_electron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_positron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_gamma", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_neutron", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_other", 1, nSpaceBins, -2500.0, 50., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxz", 1, nSpaceBins, -2500.0, 50.);
    mh->AddHistograms("lanex_bdx_vtxz_electron", 1, nSpaceBins, -2500.0, 50.);
    mh->AddHistograms("lanex_bdx_vtxz_positron", 1, nSpaceBins, -2500.0, 50.);
    mh->AddHistograms("lanex_bdx_vtxz_gamma", 1, nSpaceBins, -2500.0, 50.);
    mh->AddHistograms("lanex_bdx_vtxz_neutron", 1, nSpaceBins, -2500.0, 50.);
    mh->AddHistograms("lanex_bdx_vtxz_other", 1, nSpaceBins, -2500.0, 50.);

    mh->AddHistograms("lanex_bdx_e", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_positron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_gamma", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_neutron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_other", 1, nEnergyBins, 0., 2000.);

    mh->AddHistograms("lanex_bdx_pdg", 1, 2300, -25., 2275.);
    mh->AddHistograms("lanex_bdx_procid", 1, 220, 0., 220.);
    //
    mh->AddHistograms("cr39_bdx_xy", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);
    mh->AddHistograms("cr39_bdx_xy_muon", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);
    mh->AddHistograms("cr39_bdx_xy_neutron", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);
    mh->AddHistograms("cr39_bdx_xy_other", 20, nSpaceBins, -50., 50., nSpaceBins, -50., 50.);

    mh->AddHistograms("cr39_bdx_vtxx_vtxz", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxx_vtxz_muon", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxx_vtxz_neutron", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxx_vtxz_other", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);

    mh->AddHistograms("cr39_bdx_vtxy_vtxz", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxy_vtxz_muon", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxy_vtxz_neutron", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);
    mh->AddHistograms("cr39_bdx_vtxy_vtxz_other", 20, nSpaceBins, -2500., 50., nSpaceBins, -600., 600.);

    mh->AddHistograms("cr39_bdx_vtxz", 20, nSpaceBins, -2500., 50.);
    mh->AddHistograms("cr39_bdx_vtxz_muon", 20, nSpaceBins, -2500., 50.);
    mh->AddHistograms("cr39_bdx_vtxz_neutron", 20, nSpaceBins, -2500., 50.);
    mh->AddHistograms("cr39_bdx_vtxz_other", 20, nSpaceBins, -2500., 50.);

    mh->AddHistograms("cr39_bdx_e", 20, nEnergyBins, 0., 2000.);
    mh->AddHistograms("cr39_bdx_e_muon", 20, nEnergyBins, 0., 2000.);
    mh->AddHistograms("cr39_bdx_e_neutron", 20, nEnergyBins, 0., 2000.);
    mh->AddHistograms("cr39_bdx_e_other", 20, nEnergyBins, 0., 2000.);

    mh->AddHistograms("cr39_bdx_pdg", 20, 2300, -25., 2275.);
    mh->AddHistograms("cr39_bdx_procid", 20, 220, 0., 230.);
}

#endif