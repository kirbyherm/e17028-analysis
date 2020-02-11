// Alex Dombos
// 2016-10-03
// Purpose: Fit peaks in SuN spectra

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TSpectrum.h"

std::vector<double> gaussian_and_linear_background(TH1F* hist, std::vector<double> x_peaks_ordered){

  std::vector<double> output;

  // Fitting a histogram without "+",
  // resets all previous functions that were used to fit the histogram with "+".
  // Therefore, use "+" always, and delete functions that are unwanted.

  for (unsigned int i=0; i<x_peaks_ordered.size(); i++){
    
    // Fit with Gaussian to get initial parameters
    TF1* temp_gaus = new TF1("temp_gaus","gaus(0)",0,30000);
    temp_gaus->SetParameter(0, hist->GetBinContent(x_peaks_ordered[i]));
    temp_gaus->SetParameter(1, x_peaks_ordered[i]);
    temp_gaus->SetParameter(2, 40);
    
    hist->Fit("temp_gaus","Q + 0 B","",
	      x_peaks_ordered[i] - 40,
	      x_peaks_ordered[i] + 40);
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << temp_gaus->GetParameter(2) << std::endl;
    //if (temp_gaus->GetParameter(2) < 50 || temp_gaus->GetParameter(2) > 300){
    if (temp_gaus->GetParameter(2) > 60){
      temp_gaus->SetParameter(2, 40);
    }
    if (temp_gaus->GetParameter(1) > (x_peaks_ordered[i] + 20) ||
	temp_gaus->GetParameter(1) < (x_peaks_ordered[i] - 20)){
      temp_gaus->SetParameter(1, x_peaks_ordered[i]);
    }
    
    // Total fitting function
    TF1* fitting_function = new TF1(Form("fitting_function_%s_%d",hist->GetName(),i),"gaus(0)+pol1(3)",0,30000);
    fitting_function->SetParameter(0, temp_gaus->GetParameter(0));
    fitting_function->SetParameter(1, temp_gaus->GetParameter(1));
    fitting_function->SetParameter(2, temp_gaus->GetParameter(2));
    fitting_function->SetParameter(3, 0);
    fitting_function->SetParameter(4, 0);
    
    hist->Fit(fitting_function,"Q + 0 B","",
	      temp_gaus->GetParameter(1) - 3.0 * temp_gaus->GetParameter(2),
	      temp_gaus->GetParameter(1) + 3.0 * temp_gaus->GetParameter(2));
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << fitting_function->GetParameter(2) << std::endl;
    //if (fitting_function->GetParameter(2) < 50 || fitting_function->GetParameter(2) > 300){
    if (fitting_function->GetParameter(2) > 60){
      fitting_function->SetParameter(2, 40);
    }
    if (fitting_function->GetParameter(1) > (x_peaks_ordered[i] + 20) ||
	fitting_function->GetParameter(1) < (x_peaks_ordered[i] - 20)){
      fitting_function->SetParameter(2, x_peaks_ordered[i]);
    }
    
    hist->GetFunction("temp_gaus")->Delete();
    
    //hist->GetFunction(Form("fitting_function_%s_%d",hist->GetName(),i))->Delete();
    
    fitting_function->SetParameter(0, fitting_function->GetParameter(0));
    fitting_function->SetParameter(1, fitting_function->GetParameter(1));
    fitting_function->SetParameter(2, fitting_function->GetParameter(2));
    fitting_function->SetParameter(3, fitting_function->GetParameter(3));
    fitting_function->SetParameter(4, fitting_function->GetParameter(4));
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << fitting_function->GetParameter(2) << std::endl;
    //if (fitting_function->GetParameter(2) < 50 || fitting_function->GetParameter(2) > 300){
    if (fitting_function->GetParameter(2) > 60){
      fitting_function->SetParameter(2, 40);
    }
    if (fitting_function->GetParameter(1) > (x_peaks_ordered[i] + 20) ||
	fitting_function->GetParameter(1) < (x_peaks_ordered[i] - 20)){
      fitting_function->SetParameter(1, x_peaks_ordered[i]);
    }

    hist->Fit(fitting_function,"Q + B","",
	      fitting_function->GetParameter(1) - 3.0 * fitting_function->GetParameter(2),
	      fitting_function->GetParameter(1) + 3.0 * fitting_function->GetParameter(2));
    
    // Components of fitting function
    TF1* component_gaus = new TF1(Form("component_gaus_%s_%d",hist->GetName(),i),"gaus(0)",0,30000);
    component_gaus->FixParameter(0, fitting_function->GetParameter(0));
    component_gaus->FixParameter(1, fitting_function->GetParameter(1));
    component_gaus->FixParameter(2, fitting_function->GetParameter(2));
    component_gaus->SetLineColor(3);
    hist->Fit(Form("component_gaus_%s_%d",hist->GetName(),i),"Q + B");
    
    TF1* component_line = new TF1(Form("component_line_%s_%d",hist->GetName(),i),"pol1(3)",0,30000);
    component_line->FixParameter(3, fitting_function->GetParameter(3));
    component_line->FixParameter(4, fitting_function->GetParameter(4));
    component_line->SetLineColor(5); // Histogram color is blue, so do not choose 4
    hist->Fit(Form("component_line_%s_%d",hist->GetName(),i),"Q + B");
    
    output.push_back(fitting_function->GetParameter(1));
    
  }
  
  return output;

}

void fit_histograms(){

  // Input file
  TFile* fIn = new TFile("output_created_histograms.root","READ");

  // Output ROOT file that contains the fitted histograms
  TFile* fOut = new TFile("output_fit_histograms.root","RECREATE");
  
  // Output Text file that contains the centroids
  std::string file_name = "centroids.txt";
  std::ofstream output_file;
  output_file.open(file_name.c_str(), ios::app);
  output_file << "Check picture of fits. Some fits may need to be performed manually." << std::endl;
  output_file << "Keep in the order of the fits compared to any Excel sheets or filtering scripts." << std::endl;

  // Draw the spectra and fit the peaks
  for (int i=0; i<2; i++){ // Halves
    for (int j=0; j<4; j++){ // Segments
      for (int k=0; k<3; k++){ // PMTs

	//std::cout << "i: " << i << " j: " << j << " k: " << k << std::endl;
      
	fIn->cd();
	TH1F* hist = (TH1F*) fIn->Get(Form("E_SuN_%d_%d_%d",i,j+1,k+1));

	hist->GetXaxis()->SetRangeUser(600,1400);
	
	TSpectrum* spc = new TSpectrum(1);
	int num_found = spc->Search(hist,40,"nodraw");
	double* x_peaks = spc->GetPositionX();
	double* y_peaks = spc->GetPositionY();

	/*for (int m=0; m<num_found; m++){
	  std::cout << "i: " << i << " j: " << j <<
	  " m: " << m << " " <<
	  x_peaks[m] <<
	  " " <<
	  y_peaks[m] << std::endl;
	  }*/
	
	std::vector<double> x_peaks_ordered;
	for (int m=0; m<num_found; m++){
	  x_peaks_ordered.push_back(x_peaks[m]);
	}
	std::sort(x_peaks_ordered.begin(), x_peaks_ordered.end());

	////////////////////
	// Method 1
	std::vector<double> output = gaussian_and_linear_background(hist, x_peaks_ordered);
	/*
	output_file << hist->GetName() << std::endl;
	
	for (unsigned int i=0; i<x_peaks_ordered.size(); i++){
	  output_file << std::setw(10) << x_peaks_ordered[i] << std::setw(10) << output[i] << std::endl;
	}
	*/

	for (unsigned int i=0; i<x_peaks_ordered.size(); i++){
	  output_file << hist->GetName() << std::setw(10) << x_peaks_ordered[i] << std::setw(10) << output[i] << std::endl;
	}
	
	fOut->cd();
	hist->Write();
	////////////////////
	
      }
    }
  }
}



