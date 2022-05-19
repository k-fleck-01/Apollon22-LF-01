#ifndef _MHISTS_HH_
#define _MHISTS_HH_
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for MHists class. Used to create and store multiple histograms
// for analysis of the Geant4 simulation results.
//
// Created: 19/05/2022
// Last edited: 19/05/2022
//
#include <vector>
#include <string>

class TH1;

class MHists {
    public:
        MHists();
        ~MHists();
    public:
        void AddHistograms(std::string, int, int, double, double);
        void AddHistograms(std::string, int, int, double, double, int, double, double);
        void AddHistograms(std::string, int, int, double, double, int, double, double, int, double, double);

        void FillHistW(std::string, int, double);
        void FillHistW(std::string, int, double, double);
        void FillHistW(std::string, int, double, double, double);
        void FillHistW(std::string, int, double, double, double, double);
    private:
        std::vector<std::string> fNameList;
        std::vector<std::vector<TH1*>> fHistList;
};

typedef std::vector<TH1*> HistV;

#endif
