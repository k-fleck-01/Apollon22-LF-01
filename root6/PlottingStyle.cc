// *
// ROOT macro file to apply common plotting styles
// *
// Last modified: 17/02/2022
// *

void SetPlottingStyle() {

    std::cout << "Applying style settings ..." << std::endl;
    gROOT->ForceStyle();

    // Remove stat and fit boxes
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    // Setting histogram styles
    gStyle->SetHistLineWidth(3);
    gStyle->SetHistFillColor(kGreen-9);
    gStyle->SetPalette(55); // RGB rainbow

    // Setting title sizes
    gStyle->SetTitleSize(0.05, "xyz");
    gStyle->SetLabelSize(0.04, "xyz");

}