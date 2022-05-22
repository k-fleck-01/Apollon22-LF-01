#ifndef QGSP_BERT_EXT_H
#define QGSP_BERT_EXT_H 1
//
// GEANT4 simulation of the Apollon 2022 experiment.
// Geometry has been derived from the FLUKA simulation of the same experiment.
// 
// Header file for QGSP_BERT_EXT class - an extension of the QGSP_BERT physics
// list to include gamma conversion and e+e- annihilation to muons.
//
// Created: 22/05/2022
// Last edited: 22/05/2022
//
#include "globals.hh"
#include "G4VModularPhysicsList.hh"
 
class QGSP_BERT_EXT: public G4VModularPhysicsList {
    public:
        QGSP_BERT_EXT(G4int ver = 1);
        virtual ~QGSP_BERT_EXT()=default;
 
        QGSP_BERT_EXT(const QGSP_BERT_EXT &) = delete;
        QGSP_BERT_EXT & operator=(const QGSP_BERT_EXT &)=delete;
   
};
 
#endif