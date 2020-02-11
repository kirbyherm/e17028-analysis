
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

void draw_fit_histograms(){

  TFile* fIn = new TFile("output_fit_histograms.root","READ");

  TCanvas* c1 = new TCanvas("c1","c1");
  TCanvas* c2 = new TCanvas("c2","c2");

  c1->Divide(3,4);
  c2->Divide(3,4);
  
  for (int i=0; i<2; i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
      for (int k=0; k<3; k++){ // PMTs

	if (i==0){ // Bottom Half
	  
	  c1->cd(3*j+k+1);
	  ((TH1F*) fIn->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1)))->Draw();
	    
	}

	if (i==1){ // Top Half

	  c2->cd(3*j+k+1);
	  ((TH1F*) fIn->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1)))->Draw();
	  
	}
      }
    }
  }
}
