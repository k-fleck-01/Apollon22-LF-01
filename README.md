# Apollon 22-LF-01 Experiment
### A description of the geometry for the 2022 22-LF-01 experiment at Apollon.

## Simulation Information
### Geometry Overview
### Physics List
The physics list used is "QGSP_BERT_EXT.cc"; this is an extended version of the QGSP_BERT standard physics list which allows for simulation of both electromagnetic and hadronic processes. This contains all standard electromagnetic processes including:

- Coulomb and multiple scattering
- ionisation
- photoelectric effect
- bremsstrahlung
- pair annihilation
- **pair annihilation to muon pair**
- Compton scattering
- gamma conversion (pair production)
- **gamma conversion to muon pair**
- pair production ($\mu^-\mu^+ \rightarrow e^-e^+$)

The processes in bold are not included by default in QGSP_BERT, but have been added manually.

### Detectors
Two types of detector have been implemented here. The first is a monitor for the primary particles produced at the start of each event.  The second utilises sensitive volumes within the geometry. Volumes labeled as such are:

- vacuum chamber YAG screens (detector ID = 1000)
- Cr-39 stacks for muon detection (detector ID = 2000 - 2019)
- gamma spectrometer LANEX scintillator (detector ID = 3000)

Any time an appropriate interaction event happens within a sensitive volume, the information of this event is entered into the corresponding ROOT TTree. The possible interaction events are listed below.

#### Primary Information
Information of the primary particles produced at the beginning of each event. Tabulated information includes:

- Position (x, y, z) of particle origin (mm)
- Energy (MeV)
- Polar and azimuthal angles with respect to z axis (mrad/rad)

#### Hit Information
A 'hit' is defined to be an energy deposition event within a sensitive volume.
Collection of hit information is implemented in the Hit class. This includes:

- Position (x, y, z) of hit within volume (mm)
- Energy deposited (MeV)
- Energy of particle responsible for hit (MeV)
- Production vertex of said particle (mm)
- Particle type
- ID of particle's creation process
- Detector ID
- Track ID

#### Boundary Crossing (Bdx) Information
A boundary crossing (bdx) event is an event where a particle crosses the boundary **into** a senstive volume.
Collection of bdx information is implemented in the BDCrossing class. This includes:

- Position (x, y, z) of boundary crossing (mm)
- Energy of particle (MeV)
- 3-momentum vector of particle (MeV)
- Angle between boundary normal and 3-momentum (rad)
- Fluence; calculated from angle and surface area of boundary (1/mm^2)
- Production vertex of particle (mm)
- ID of particle' creation process
- Particle type
- Detector ID

#### Track Information
Tracking information is controlled in the TrackingAction class and is performed at the end of a particle's track (termination point). Normally, the TrackingAction is called for every particle termination - it is only recorded if the particle track ends within a sensitive volume. The information tabulated is:

- Track ID
- Particle type
- Detector ID
- Creation process ID
- Production vertex (mm)
- Termination vertex (mm)
- Kinetic energy of particle **at beginning** of track (MeV)

## Compiling and Running
### Requirements
Installation requirements are:

* CMake (version >= 3.15),
* Geant4 (tested on versions 10.07.p02 and 11.0)

Geant4 can be built with or without multithreading enabled. Additionally, if GDML is enabled (GEANT4_USE_GDML:BOOL = TRUE), an output of the geometry in GDML format can be used in the src/DetectorConstruction.cc file.

### Instructions
The project can be compiled and run as follows.
1. Clone the directory using the command: <br>
    `git clone https://github.com/k-fleck-01/Apollon22-LF-01`

2. Inside the Apollon22-LF-02 directory, create a build directory, e.g. <br>
   `mkdir build && cd build`

3. Run CMake. This will invoke the default C++ compiler for the system, and sets the C++11 standards. <br>
   `cmake .. && make`

4. The project can then be run interactively by running the 'sim' executable <br>
   `./sim` <br>
    which, by default, uses the macro run.mac as input. To run in batch mode, the macro can be supplied
    as an argument
    `./sim run.mac` <br>

## Post-Processing
