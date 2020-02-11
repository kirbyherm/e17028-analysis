// A. Dombos

void draw_output_fit_histograms(){
  
  TFile* fIn = new TFile("output_fit_histograms.root","READ");

  TCanvas* c1 = (TCanvas*) fIn->Get("c1");
  TCanvas* c2 = (TCanvas*) fIn->Get("c2");

  c1->Draw();
  c2->Draw();
  
}
