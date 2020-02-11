#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

void draw_fit_histograms(){

  TFile* fIn = new TFile("output_fit_histograms.root","READ");

  TCanvas* c1 = new TCanvas("c1","c1");
  TCanvas* c2 = new TCanvas("c2","c2");

  c1->Divide(2,2);
  c2->Divide(2,2);
  
  std::vector<std::string> Halves{"B","T"};

  for (unsigned int i=0; i<Halves.size(); i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
    
      TH1F* hist = (TH1F*) fIn->Get(Form("E_SuN%s%d",Halves[i].c_str(),j+1));

      if (i==0){
	c1->cd(j+1);
	hist->Draw();
      }
      if (i==1){
	c2->cd(j+1);
	hist->Draw();
      }

    }
  }
}
