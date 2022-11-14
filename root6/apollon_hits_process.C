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
#include <cmath>

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

        if (detid == 3000) { // LANEX screens
            ndet = detid - 2999;

            mh->FillHistW("lanex_hits_xy", ndet, xx, yy);
            mh->FillHistW("lanex_hits_edep_xy", ndet, xx, yy, edep);
            mh->FillHistW("lanex_hits_edep_x", ndet, xx, edep);
            mh->FillHistW("lanex_hits_edep_y", ndet, yy, edep);
            mh->FillHistW("lanex_hits_edep", ndet, edep);
            mh->FillHistW("lanex_hits_e_edep", ndet, eneg, edep);
            mh->FillHistW("lanex_hits_edep_z", ndet, zz - 49.7425, edep);

            if (std::abs(yy) <= 7.65) {
                mh->FillHistW("lanex_hits_edep_xy_cut", ndet, xx, yy, edep);
            }

            if (pdg == 11) {
                mh->FillHistW("lanex_hits_edep_electron", ndet, edep);
                mh->FillHistW("lanex_hits_edep_z_electron", ndet, zz - 49.7425, edep);
                mh->FillHistW("lanex_hits_edep_xy_electron", ndet, xx, yy, edep);
                mh->FillHistW("lanex_hits_edep_x_electron", ndet, xx, edep);
                mh->FillHistW("lanex_hits_edep_y_electron", ndet, yy, edep);
                if (std::abs(yy) <= 7.65) {
                    mh->FillHistW("lanex_hits_edep_xy_cut_electron", ndet, xx, yy, edep);
                    mh->FillHistW("lanex_hits_xy_cut_electron", ndet, xx, yy);
                }
            }
            else if (pdg == -11) {
                mh->FillHistW("lanex_hits_edep_positron", ndet, edep);
                mh->FillHistW("lanex_hits_edep_z_positron", ndet, zz - 49.7425, edep);
                mh->FillHistW("lanex_hits_edep_xy_positron", ndet, xx, yy, edep);
                mh->FillHistW("lanex_hits_edep_x_positron", ndet, xx, edep);
                mh->FillHistW("lanex_hits_edep_y_positron", ndet, yy, edep);
                if (std::abs(yy) <= 7.65) {
                    mh->FillHistW("lanex_hits_edep_xy_cut_positron", ndet, xx, yy, edep);
                    mh->FillHistW("lanex_hits_xy_cut_positron", ndet, xx, yy);
                }
            }
            else if (pdg == 22) { 
                mh->FillHistW("lanex_hits_edep_gamma", ndet, edep);
                mh->FillHistW("lanex_hits_edep_z_gamma", ndet, zz - 49.7425, edep);
                mh->FillHistW("lanex_hits_edep_xy_gamma", ndet, xx, yy, edep);
                mh->FillHistW("lanex_hits_edep_x_gamma", ndet, xx, edep);
                mh->FillHistW("lanex_hits_edep_y_gamma", ndet, yy, edep);
                if (std::abs(yy) <= 7.65) {
                    mh->FillHistW("lanex_hits_edep_xy_cut_gamma", ndet, xx, yy, edep);
                    mh->FillHistW("lanex_hits_xy_cut_gamma", ndet, xx, yy);
                }
            }
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

        if (detid == 3000) { // LANEX screen
            ndet = detid - 2999;

            mh->FillHistW("lanex_bdx_xy", ndet, xx, yy);
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

                if (std::abs(yy) <= 7.65) {
                    mh->FillHistW("lanex_bdx_e_x_electron", ndet, xx, eneg);
                    mh->FillHistW("lanex_bdx_e_poscut_electron", ndet, eneg);

                    if (std::abs(vtxz + 985.1125) <= 0.1125 && 
                        std::abs(vtxx) <= 10. &&
                        std::abs(vtxy) <= 10.) { // positrons produced in GRS converter
                        mh->FillHistW("lanex_bdx_e_converter_poscut_electron", ndet, eneg);
                    }
                }

                if (std::abs(xx) <= 5. && std::abs(yy) <= 7.) { // on axis central spike
                    mh->FillHistW("lanex_bdx_vtxy_vtxz_cut_electron", ndet, vtxz, vtxy);
                    mh->FillHistW("lanex_bdx_vtxx_vtxz_cut_electron", ndet, vtxz, vtxx);
                }

                if (std::abs(vtxz + 985.1125) <= 0.1125 && 
                    std::abs(vtxx) <= 10. &&
                    std::abs(vtxy) <= 10.) { // positrons produced in GRS converter
                    mh->FillHistW("lanex_bdx_e_converter_electron", ndet, eneg);
                }
            }
            else if (pdg == -11) { // positrons
                mh->FillHistW("lanex_bdx_xy_positron", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_positron", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_positron", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_positron", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_positron", ndet, eneg);

                if (std::abs(yy) <= 7.65) {
                    mh->FillHistW("lanex_bdx_e_x_positron", ndet, xx, eneg);
                    mh->FillHistW("lanex_bdx_e_poscut_positron", ndet, eneg);

                    if (std::abs(vtxz + 985.1125) <= 0.1125 && 
                        std::abs(vtxx) <= 10. &&
                        std::abs(vtxy) <= 10.) { // positrons produced in GRS converter
                        mh->FillHistW("lanex_bdx_e_converter_poscut_positron", ndet, eneg);
                    }
                }

                if (std::abs(xx) <= 5. && std::abs(yy) <= 7.) { // on axis central spike
                    mh->FillHistW("lanex_bdx_vtxy_vtxz_cut_positron", ndet, vtxz, vtxy);
                    mh->FillHistW("lanex_bdx_vtxx_vtxz_cut_positron", ndet, vtxz, vtxx);
                }

                if (std::abs(vtxz + 985.1125) <= 0.1125 && 
                    std::abs(vtxx) <= 10. &&
                    std::abs(vtxy) <= 10.) { // positrons produced in GRS converter
                    mh->FillHistW("lanex_bdx_e_converter_positron", ndet, eneg);
                }
            }
            else if (pdg == 22) { // photons
                mh->FillHistW("lanex_bdx_xy_gamma", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_gamma", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_gamma", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_gamma", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_gamma", ndet, eneg);

                if (std::abs(xx) <= 5. && std::abs(yy) <= 7.) { // on axis central spike
                    mh->FillHistW("lanex_bdx_vtxy_vtxz_cut_gamma", ndet, vtxz, vtxy);
                    mh->FillHistW("lanex_bdx_vtxx_vtxz_cut_gamma", ndet, vtxz, vtxx);
                }
            }
            else {
                mh->FillHistW("lanex_bdx_xy_other", ndet, xx, yy);
                mh->FillHistW("lanex_bdx_vtxx_vtxz_other", ndet, vtxz, vtxx);
                mh->FillHistW("lanex_bdx_vtxy_vtxz_other", ndet, vtxz, vtxy);
                mh->FillHistW("lanex_bdx_vtxz_other", ndet, vtxz);
                mh->FillHistW("lanex_bdx_e_other", ndet, eneg);
            }
        }
        else if (detid == 4000) {
            ndet = detid - 3999;
            if (pdg == 11) {
                mh->FillHistW("converter_bdx_e_electron", ndet, eneg);
            }
            else if (pdg == -11) {
                mh->FillHistW("converter_bdx_e_positron", ndet, eneg);
            }
            else if (pdg == 22) {
                mh->FillHistW("converter_bdx_e_gamma", ndet, eneg);
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
    // Primaries histograms
    mh->AddHistograms("primaries_xy", 1, nSpaceBins, -2., 2., nSpaceBins, -2., 2.);
    mh->AddHistograms("primaries_e", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("primaries_theta", 1, nEnergyBins, 0., 3.14159265);
    mh->AddHistograms("primaries_phi", 1, nEnergyBins, 0., 2.*3.14159265);

    // Hits histograms
    mh->AddHistograms("lanex_hits_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_xy_cut_electron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_xy_cut_positron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_xy_cut_gamma", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_x", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_y", 1, nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy_electron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_x_electron", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_y_electron", 1, nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy_positron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_x_positron", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_y_positron", 1, nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy_gamma", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_x_gamma", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_y_gamma", 1, nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_hits_edep_xy_cut", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_xy_cut_electron", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_xy_cut_positron", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep_xy_cut_gamma", 1, nSpaceBins, -150., 150.);
    mh->AddHistograms("lanex_hits_edep", 1, nEnergyBins, 0., 2.);
    mh->AddHistograms("lanex_hits_edep_electron", 1, nEnergyBins, 0., 2.);
    mh->AddHistograms("lanex_hits_edep_positron", 1, nEnergyBins, 0., 2.);
    mh->AddHistograms("lanex_hits_edep_gamma", 1, nEnergyBins, 0., 2.);
    mh->AddHistograms("lanex_hits_edep_z", 1, nSpaceBins, 0., 0.3);
    mh->AddHistograms("lanex_hits_edep_z_electron", 1, nSpaceBins, 0., 0.3);
    mh->AddHistograms("lanex_hits_edep_z_positron", 1, nSpaceBins, 0., 0.3);
    mh->AddHistograms("lanex_hits_edep_z_gamma", 1, nSpaceBins, 0., 0.3);
    mh->AddHistograms("lanex_hits_e_edep", 1, nEnergyBins, 0., 2000., nEnergyBins, 0., 2.);
    
    // Boundary crossing histograms
    mh->AddHistograms("lanex_bdx_xy", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_electron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_positron", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);
    mh->AddHistograms("lanex_bdx_xy_gamma", 1, nSpaceBins, -150., 150., nSpaceBins, -80., 80.);

    mh->AddHistograms("lanex_bdx_e_x_electron", 1, nSpaceBins, -150., 150., nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_x_positron", 1, nSpaceBins, -150., 150., nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_poscut_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_poscut_positron", 1, nEnergyBins, 0., 2000.);

    mh->AddHistograms("lanex_bdx_vtxx_vtxz", 1, nSpaceBins, -2575, 100., nSpaceBins, -100., 100.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_electron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_positron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_gamma", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxy_vtxz", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_electron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_positron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_gamma", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxx_vtxz_cut", 1, nSpaceBins, -2575, 100., nSpaceBins, -100., 100.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_cut_electron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_cut_positron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxx_vtxz_cut_gamma", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxy_vtxz_cut", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_cut_electron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_cut_positron", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);
    mh->AddHistograms("lanex_bdx_vtxy_vtxz_cut_gamma", 1, nSpaceBins, -2575, 100., nSpaceBins, -600., 600.);

    mh->AddHistograms("lanex_bdx_vtxz", 1, nSpaceBins, -2575, 100.);
    mh->AddHistograms("lanex_bdx_vtxz_electron", 1, nSpaceBins, -2575, 100.);
    mh->AddHistograms("lanex_bdx_vtxz_positron", 1, nSpaceBins, -2575, 100.);
    mh->AddHistograms("lanex_bdx_vtxz_gamma", 1, nSpaceBins, -2575, 100.);

    mh->AddHistograms("lanex_bdx_e", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_positron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_gamma", 1, nEnergyBins, 0., 2000.);

    mh->AddHistograms("lanex_bdx_e_converter_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_converter_positron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_converter_poscut_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("lanex_bdx_e_converter_poscut_positron", 1, nEnergyBins, 0., 2000.);

    mh->AddHistograms("lanex_bdx_pdg", 1, 2300, -25., 2325.);
    mh->AddHistograms("lanex_bdx_procid", 1, 220, 0., 220.);

    mh->AddHistograms("converter_bdx_e_electron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("converter_bdx_e_positron", 1, nEnergyBins, 0., 2000.);
    mh->AddHistograms("converter_bdx_e_gamma", 1, nEnergyBins, 0., 2000.);
    
}

#endif