# Apollon 22-LF-01 Experiment
### A description of the geometry for the 2022 22-LF-01 experiment at Apollon.

## Simulation Information
### Geometry Overview
### Physics List
### Hit Information

## Compiling and Running
### Requirements
Installation requirements are:
    - CMake (version >= 3.15),
    - Geant4 (tested on versions 10.07.p02 and 11.0)
Geant4 can be built with or without multithreading enabled. Additionally, if GDML is enabled (GEANT4_USE_GDML:BOOL = TRUE), an output of the geometry in GDML format can be used in the src/DetectorConstruction.cc file.

### Instructions
The project can be compiled and run as follows.
1. Clone the directory using the command:
    git clone https://github.com/k-fleck-01/Apollon22-LF-01

2. Inside the Apollon22-LF-02 directory, create a build directory, e.g.
    mkdir build && cd build

3. Run CMake. This will invoke the default C++ compiler for the system, and sets the C++11 standards.
    cmake .. && make

4. The project can then be run interactively by running the 'sim' executable
    ./sim
which, by default, uses the macro run.mac as input. 

## Post-Processing