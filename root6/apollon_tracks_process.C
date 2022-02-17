//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Macro file for processing tracks from Geant4 simulation.
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
