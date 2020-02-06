// Alex Dombos
// 2016-03-18
// Purpose: To create histograms of SuNs PMTs

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>

void create_histograms(){

  // Output ROOT file
  TFile* fOut = new TFile("output_created_histograms.root","RECREATE");
  
  // Input ROOT file
  TChain* chain = new TChain("t");
  chain->Add("/mnt/analysis/e17028/rootfiles/run0072-*.root");
  
  // Draw PMTs
  for (int i=0; i<2; i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
      for (int k=0; k<3; k++){ // PMTs
	
	std::cout << "i: " << i << " j: " << j << " k: " << k << std::endl;
	
	chain->Project(Form("E_SuN_%d_%d_%d(4000,1,4001)",i,j+1,k+1),
		       Form("E_SuN[%d][%d][%d]",i,j+1,k+1),
		       "",
		       "");
	fOut->cd();
	((TH1F*) gDirectory->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1)))->Write();

      }
    }
  }
}
