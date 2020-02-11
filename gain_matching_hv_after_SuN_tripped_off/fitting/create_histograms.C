// A. Dombos

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include <iostream>

void create_histograms(){

  TFile* fIn = new TFile("hv_run.root","READ");
  TTree* tIn = (TTree*) fIn->Get("t");

  std::vector<TH1F*> histograms;
  
  for (int i=0; i<2; i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
      for (int k=0; k<3; k++){ // PMTs

	std::cout << "i: " << i << " j: " << j << " k: " << k << std::endl;
	
	tIn->Project(Form("E_SuN_%d_%d_%d(1000,1,10001)",i,j+1,k+1),
		     Form("E_SuN[%d][%d][%d]",i,j+1,k+1),
		     "",
		     "");
	histograms.push_back((TH1F*) gDirectory->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1)));

	std::cout << histograms.back()->GetName() << "   "
		  << histograms.back()->GetTitle() << std::endl;
	
      }
    }
  }

  TFile* fOut = new TFile("output_created_histograms.root","RECREATE");
  for (const auto& histogram : histograms){
    histogram->Write();
  }

  fOut->Close();

}
