// Alex Dombos
// 2017-01-29
// Purpose: To create histograms of SuN segments

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"

#include "../SuN_gain_matching_factors.h"

#include <iostream>

void create_histograms(){

  // Output ROOT file
  TFile* fOut = new TFile("output_created_histograms.root","RECREATE");
  
  // Input ROOT file
  TChain* chain = new TChain("t");
  chain->Add("/mnt/analysis/e17028/rootfiles/run2002-*.root");

  // Apply SuN gain matching
  apply_SuN_gain_matching(chain);

  // Draw SuN segments
  std::vector<std::string> Halves{"B","T"};
  
  for (unsigned int i=0; i<Halves.size(); i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
	
      std::cout << "i: " << i << " j: " << j << std::endl;
      
      chain->Project(Form("E_SuN%s%d(32000,1,32001)",Halves[i].c_str(),j+1),
		     Form("E_SuN%s%d",Halves[i].c_str(),j+1),
		     "",
		     "");
      fOut->cd();
      ((TH1F*) gDirectory->Get(Form("E_SuN%s%d",Halves[i].c_str(),j+1)))->Write();
      
    }
  }
}

