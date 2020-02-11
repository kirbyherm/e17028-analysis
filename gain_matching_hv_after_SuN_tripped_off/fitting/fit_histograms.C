// A. Dombos

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <fstream>
#include <sstream>
#include <iostream>

double calculate_high_voltage(double current_channel_number,
			      double current_high_voltage){

  double new_channel_number = 1025; // Peak will be gain matched to this position
  const double e = 2.71828;

  double new_high_voltage = std::pow(e, (std::log(current_high_voltage) + 0.125 * std::log(new_channel_number / current_channel_number)));
  return new_high_voltage;
  
}

double gaussian_centroid(TH1F* hist){

  // The background subtracted histogram range needs to be set.
  // Otherwise, TH1::GetMaximumBin will not correspond to the background peak.
  // In int fit_histograms(), the range for the original histogram has been set,
  // but not the one that is background subtracted.
  hist->GetXaxis()->SetRangeUser(500,2000);
  
  TF1* fitting_function = new TF1(Form("fitting_function_%s",hist->GetName()),
				  "gaus(0)", 0, 10000);
  
  fitting_function->SetParameter(0, hist->GetBinContent(hist->GetMaximumBin()));
  fitting_function->SetParameter(1, hist->GetBinWidth(1) * hist->GetMaximumBin());
  fitting_function->SetParameter(2, 40);

   // Using option 0, otherwise the drawing here messes up the drawing in the pads
  hist->Fit(fitting_function, "Q 0", "",
	    fitting_function->GetParameter(1) - 100,
	    fitting_function->GetParameter(1) + 100);

  return fitting_function->GetParameter(1);

}

int fit_histograms(){

  // Get the current high voltages applied to each PMT
  std::ifstream current_high_voltages_file("../current_high_voltages.txt");
  if (!current_high_voltages_file){
    std::cerr << "Could not open the file that contains the current high voltages" << std::endl;
    return -1;
  }

  std::string line;
  std::string website_channel;
  double website_high_voltage = 0;

  std::vector<std::string> website_channels_top;
  std::vector<std::string> website_channels_bottom;
  std::vector<double> website_high_voltages_top;
  std::vector<double> website_high_voltages_bottom;
  
  while (std::getline(current_high_voltages_file, line)){
    std::istringstream iss(line);
    //std::cout << line << std::endl;
    if (iss >> website_channel >> website_high_voltage){
      std::cout << website_channel << "\t" << website_high_voltage << std::endl;
      // The order on the website is top PMTs first, then bottom PMTs
      if (website_channel.size() < 4){ // Top PMTs
	website_channels_top.push_back(website_channel);
	website_high_voltages_top.push_back(website_high_voltage);
      }
      else { // Bottom PMTs
	website_channels_bottom.push_back(website_channel);
	website_high_voltages_bottom.push_back(website_high_voltage);
      }
    }
  }

  std::cout << std::endl;
  for (unsigned int i=0; i<website_high_voltages_top.size(); i++){
    std::cout << website_high_voltages_top[i] << std::endl;
  }
  std::cout << std::endl;
  for (unsigned int i=0; i<website_high_voltages_bottom.size(); i++){
    std::cout << website_high_voltages_bottom[i] << std::endl;
  }
  std::cout << std::endl;
  
  TFile* fIn = new TFile("output_created_histograms.root","READ");

  std::vector<double> new_high_voltages_top;
  std::vector<double> new_high_voltages_bottom;
  
  TCanvas* c1 = new TCanvas("c1","c1");
  TCanvas* c2 = new TCanvas("c2","c2");
  
  c1->Divide(3,4);
  c2->Divide(3,4);
  
  for (int i=0; i<2; i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
      for (int k=0; k<3; k++){ // PMTs    

	TH1F* hist = (TH1F*) fIn->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1));
	hist->GetXaxis()->SetRangeUser(500,2000);

	// Second parameter in TH1::ShowBackground is "same" by default,
	// so state explicitly to do not draw with empty argument "".
	// The name of the histogram returned from TH1::ShowBackground
	// is the is name of the histogram on which TH1::ShowBackground
	// was called with "_background" appended to it.
	// I decided to change the name.
	TH1F* bkgd = (TH1F*) hist->ShowBackground(20,"");
	bkgd->SetName(Form("E_SuN_%d_%d_%d_bkgd",i,j+1,k+1));

	TH1F* hist_bkgd_subtracted = new TH1F(Form("E_SuN_%d_%d_%d_bkgd_subtracted",i,j+1,k+1),
					      "",
					      hist->GetNbinsX(),
					      hist->GetXaxis()->GetXmin(),
					      hist->GetXaxis()->GetXmax());

	hist_bkgd_subtracted->Add(hist, +1);
	hist_bkgd_subtracted->Add(bkgd, -1);

	double centroid = gaussian_centroid(hist_bkgd_subtracted);
	
	TF1* fitting_function = (TF1*) hist_bkgd_subtracted->GetFunction(Form("fitting_function_%s",
									      hist_bkgd_subtracted->GetName()));
	
	double new_high_voltage = 0;
	TLatex* label_current_high_voltage;
	TLatex* label_new_high_voltage;
	
	if (i==0){ // Bottom Half

	  double new_high_voltage = calculate_high_voltage(centroid, website_high_voltages_bottom[3*j+k]);
	  new_high_voltages_bottom.push_back(new_high_voltage);
	  std::cout << "bottom\t" << website_high_voltages_bottom[3*j+k] << "\t" << new_high_voltage << "\t" << new_high_voltages_bottom.back() << std::endl;

	  label_current_high_voltage = new TLatex(0.50,0.60,Form("current high voltage = %f", website_high_voltages_bottom[3*j+k]));
	  label_current_high_voltage->SetTextSize(0.05);
	  label_current_high_voltage->SetNDC(kTRUE);

	  label_new_high_voltage = new TLatex(0.50,0.40,Form("new high voltage = %f", new_high_voltages_bottom.back()));
	  label_new_high_voltage->SetTextSize(0.05);
	  label_new_high_voltage->SetNDC(kTRUE);
	  
	}
	
	if (i==1){ // Top Half

	  double new_high_voltage = calculate_high_voltage(centroid, website_high_voltages_top[3*j+k]);
	  new_high_voltages_top.push_back(new_high_voltage);
	  std::cout << "top\t" << website_high_voltages_top[3*j+k] << "\t" << new_high_voltage << "\t" << new_high_voltages_top.back() << std::endl;

	  label_current_high_voltage = new TLatex(0.50,0.60,Form("current high voltage = %f", website_high_voltages_top[3*j+k]));
	  label_current_high_voltage->SetTextSize(0.05);
	  label_current_high_voltage->SetNDC(kTRUE);

	  label_new_high_voltage = new TLatex(0.50,0.40,Form("new high voltage = %f", new_high_voltages_top.back()));
	  label_new_high_voltage->SetTextSize(0.05);
	  label_new_high_voltage->SetNDC(kTRUE);
	  
	}

	hist->SetLineColor(4);
	bkgd->SetLineColor(2);
	hist_bkgd_subtracted->SetLineColor(3);
	fitting_function->SetLineColor(1);
	
	if (i==0){ // Bottom Half
	  c1->cd(3*j+k+1);
	  hist->Draw("same");
	  bkgd->Draw("same");
	  hist_bkgd_subtracted->Draw("same");
	  fitting_function->Draw("same");

	  label_current_high_voltage->Draw("same");
	  label_new_high_voltage->Draw("same");
	  
	}

	if (i==1){ // Top Half
	  c2->cd(3*j+k+1);
	  hist->Draw("same");
	  bkgd->Draw("same");
	  hist_bkgd_subtracted->Draw("same");
	  fitting_function->Draw("same");

	  label_current_high_voltage->Draw("same");
	  label_new_high_voltage->Draw("same");
	  
	}
      }
    }
  }

  TFile* fOut = new TFile("output_fit_histograms.root","RECREATE");
  c1->Write();
  c2->Write();
  fOut->Close();
 
  std::ofstream new_high_voltages_file("../new_high_voltages.txt");
  // The order on the website is top PMTs first, then bottom PMTs
  for (unsigned int i=0; i<website_channels_top.size(); i++){
    new_high_voltages_file << website_channels_top[i] << "\t"
			   << website_high_voltages_top[i] << "\t"
			   << new_high_voltages_top[i] << std::endl;
  }
  for (unsigned int i=0; i<website_channels_bottom.size(); i++){
    new_high_voltages_file << website_channels_bottom[i] << "\t"
			   << website_high_voltages_bottom[i] << "\t"
			   << new_high_voltages_bottom[i] << std::endl;
  }
    
  return 0;
  
}
