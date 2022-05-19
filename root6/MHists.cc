//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Source file for MHists class. Used to create and store multiple histograms
// for analysis of the Geant4 simulation results.
//
// Created: 19/05/2022
// Last edited: 19/05/2022
//

#include "MHists.hh"

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

MHists::MHists(): fNameList(0), fHistList(0)
{}

MHists::~MHists()
{}

//
//

void MHists::AddHistograms(std::string hprefix, int ndet, int nbins, double xmin, double xmax) {
    // Creates a vector of ndet TH1Ds
    HistV hlist;
    for (int ii = 0; ii < ndet; ++ii) {
        std::string hname(hprefix + "_" + std::to_string(ii));
        TH1D* hh = new TH1D(hname.c_str(), hname.c_str(), nbins, xmin, xmax);
        hlist.push_back(hh);
    }

    fNameList.push_back(hprefix);
    fHistList.push_back(hlist);
}

void MHists::AddHistograms(std::string hprefix, int ndet, int nbinsx, double xmin, double xmax, 
                                                     int nbinsy, double ymin, double ymax) {
    // Creates a vector of ndet TH2Ds
    HistV hlist;
    for (int ii = 0; ii < ndet; ++ii) {
        std::string hname(hprefix + "_" + std::to_string(ii));
        TH2D* hh = new TH2D(hname.c_str(), hname.c_str(), nbinsx, xmin, xmax, nbinsy, ymin, ymax);
        hlist.push_back(hh);
    }

    fNameList.push_back(hprefix);
    fHistList.push_back(hlist);
}

void MHists::AddHistograms(std::string hprefix, int ndet, int nbinsx, double xmin, double xmax,
                                                     int nbinsy, double ymin, double ymax,
                                                     int nbinsz, double zmin, double zmax) {
    // Creates a vector of ndet TH3Ds
    HistV hlist;
    for (int ii = 0; ii < ndet; ++ii) {
        std::string hname(hprefix + "_" + std::to_string(ii));
        TH3D* hh = new TH3D(hname.c_str(), hname.c_str(), nbinsx, xmin, xmax, nbinsy, ymin, ymax, nbinsz, zmin, zmax);
        hlist.push_back(hh);
    }

    fNameList.push_back(hprefix);
    fHistList.push_back(hlist);
}

//
//

void MHists::FillHistW(std::string hprefix, int ndet, double value) {
    int hindex = 0;
    for (size_t ii = 0; ii < fNameList.size(); ++ii) {
        if (fNameList[ii] == hprefix) {
            hindex = ii;
            break;
        }
    }
    fHistList[hindex][ndet - 1]->Fill(value);
}

void MHists::FillHistW(std::string hprefix, int ndet, double value1, double value2) {
    int hindex = 0;
    for (size_t ii = 0; ii < fNameList.size(); ++ii) {
        if (fNameList[ii] == hprefix) {
            hindex = ii;
            break;
        }
    }
    fHistList[hindex][ndet - 1]->Fill(value1, value2);
}

void MHists::FillHistW(std::string hprefix, int ndet, double value1, double value2, double value3) {
    int hindex = 0;
    for (size_t ii = 0; ii < fNameList.size(); ++ii) {
        if (fNameList[ii] == hprefix) {
            hindex = ii;
            break;
        }
    }
    TH1* hh = fHistList[hindex][ndet - 1];
    if (hh->InheritsFrom("TH3")) {
        TH3D* h3 = dynamic_cast<TH3D*>(hh);
        h3->Fill(value1, value2, value3);
    }
    else if (hh->InheritsFrom("TH2")) {
        TH2D* h2 = dynamic_cast<TH2D*>(hh);
        h2->Fill(value1, value2, value3);
    }
}

void MHists::FillHistW(std::string hprefix, int ndet, double value1, double value2, double value3, double value4) {
    int hindex = 0;
    for (size_t ii = 0; ii < fNameList.size(); ++ii) {
        if (fNameList[ii] == hprefix) {
            hindex = ii;
            break;
        }
    }
    TH3D* hh = dynamic_cast<TH3D*>(fHistList[hindex][ndet - 1]);
    hh->Fill(value1, value2, value3, value4);
}
