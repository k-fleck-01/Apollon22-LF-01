###############################################################################
###############################################################################
# Geant4 simulation of the Apollon experiment.
#
# Python3 script to export the ROOT TTree simulation data to hdf5 file format.
# Requires the following modules:
#         - sys
#         - h5py
#         - ROOT (PyROOT)
#
# Created: 18/05/2022
# Last modified: 18/05/2022
###############################################################################
###############################################################################
import sys
import h5py
import ROOT 
###############################################################################
def ttree_to_hdf5(hfile: h5py.File , tree: ROOT.TChain) -> None:
    treename = tree.GetName()
    group = hfile.create_group(treename)
    print(f'Group {group.name} created.')

    nentries = tree.GetEntries()
    for branch in tree.GetListOfBranches():
        branchName = branch.GetName()
        dset = group.create_dataset(branchName, (nentries,), dtype = 'f8')
        print(f'Dataset {dset.name} created.')
#
#
def main() -> int:
    flistname = sys.argv[1]
    print(flistname)
    hfilename = flistname.rsplit('.', 1)[0] + ".h5"
    hfile = h5py.File(hfilename, 'w')

    flist = open(flistname, 'r')
    primaryTree = ROOT.TChain("Primaries")
    hitsTree    = ROOT.TChain("Hits")
    bdxTree     = ROOT.TChain("Bdx")
    for fname in flist: 
        primaryTree.Add(fname.strip('\n'), -1)             # Need to strip 
                                                           # newline character.
        hitsTree.Add(fname.strip('\n'), -1)
        bdxTree.Add(fname.strip('\n'), -1)

    print(f'Nentries in primaryTree: {primaryTree.GetEntries()}')
    print(f'Nentries in hitsTree: {hitsTree.GetEntries()}')
    print(f'Nentries in bdxTree: {bdxTree.GetEntries()}')

    ttree_to_hdf5(hfile, primaryTree)
    ttree_to_hdf5(hfile, hitsTree)
    ttree_to_hdf5(hfile, bdxTree)

    groupName = primaryTree.GetName()
    ii = 0
    for entry in primaryTree:
        if (ii%1000 == 0): print(f'Processed {ii} entries of {primaryTree.GetEntries()}...')
        hfile[groupName + '/' + 'E'][ii]     = entry.E
        hfile[groupName + '/' + 'x'][ii]     = entry.x
        hfile[groupName + '/' + 'y'][ii]     = entry.y
        hfile[groupName + '/' + 'z'][ii]     = entry.z
        hfile[groupName + '/' + 'theta'][ii] = entry.theta
        hfile[groupName + '/' + 'phi'][ii]   = entry.phi
        ii += 1

    groupName = hitsTree.GetName()
    ii = 0
    for entry in hitsTree:
        if (ii%1000 == 0): print(f'Processed {ii} entries of {hitsTree.GetEntries()}...')
        hfile[groupName + '/' + 'evid'][ii] = entry.evid
        hfile[groupName + '/' + 'x'][ii] = entry.x
        hfile[groupName + '/' + 'y'][ii] = entry.y
        hfile[groupName + '/' + 'z'][ii] = entry.z
        hfile[groupName + '/' + 'vtxx'][ii] = entry.vtxx
        hfile[groupName + '/' + 'vtxy'][ii] = entry.vtxy
        hfile[groupName + '/' + 'vtxz'][ii] = entry.vtxz
        hfile[groupName + '/' + 'edep'][ii] = entry.edep
        hfile[groupName + '/' + 'energy'][ii] = entry.energy
        hfile[groupName + '/' + 'pdg'][ii] = entry.pdg
        hfile[groupName + '/' + 'detid'][ii] = entry.detid
        ii += 1

    groupName = bdxTree.GetName()
    ii = 0
    for entry in bdxTree:
        if (ii%1000 == 0): print(f'Processed {ii} entries of {bdxTree.GetEntries()}...')
        hfile[groupName + '/' + 'eventid'][ii] = entry.eventid
        hfile[groupName + '/' + 'detid'][ii] = entry.detid
        hfile[groupName + '/' + 'pdg'][ii] = entry.pdg
        hfile[groupName + '/' + 'procid'][ii] = entry.procid
        hfile[groupName + '/' + 'x'][ii] = entry.x
        hfile[groupName + '/' + 'y'][ii] = entry.y
        hfile[groupName + '/' + 'z'][ii] = entry.z
        hfile[groupName + '/' + 'vtxx'][ii] = entry.vtxx
        hfile[groupName + '/' + 'vtxy'][ii] = entry.vtxy
        hfile[groupName + '/' + 'vtxz'][ii] = entry.vtxz
        hfile[groupName + '/' + 'px'][ii] = entry.px
        hfile[groupName + '/' + 'py'][ii] = entry.py
        hfile[groupName + '/' + 'pz'][ii] = entry.pz
        hfile[groupName + '/' + 'energy'][ii] = entry.energy
        hfile[groupName + '/' + 'theta'][ii] = entry.theta
        hfile[groupName + '/' + 'fluence'][ii] = entry.fluence
        ii += 1
    
    print(f'Finished compiling {hfile.filename}. Closing...')
    hfile.close()
    return 0

###############################################################################
if (__name__ == '__main__'):
    main()
###############################################################################
###############################################################################x


