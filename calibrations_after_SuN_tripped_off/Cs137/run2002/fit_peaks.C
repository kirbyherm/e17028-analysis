// Alex Dombos
// 2016-03-19
// Purpose: Fit peaks in SuN segments

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TSpectrum.h"

std::vector<double> double_gaussian_and_linear_background(TH1F* hist, std::vector<double> x_peaks_ordered){

  std::vector<double> output;
  
  // Fitting a histogram without "+",
  // resets all previous functions that were used to fit the histogram with "+".
  // Therefore, use "+" always, and delete functions that are unwanted.

  std::vector<double> heights;
  std::vector<double> means;
  std::vector<double> std_devs;
  
  for (unsigned int i=0; i<x_peaks_ordered.size(); i++){
    
    // Fit with Gaussian to get initial parameters
    TF1* temp_gaus = new TF1("temp_gaus","gaus(0)",0,30000);
    temp_gaus->SetParameter(0, hist->GetBinContent(x_peaks_ordered[i]));
    temp_gaus->SetParameter(1, x_peaks_ordered[i]);
    temp_gaus->SetParameter(2, 40);
    
    hist->Fit("temp_gaus","Q 0","", // Do not need + here because the logic below does not have a loop (the final fitting function fits the entire spectrum, not individual peaks)
	      x_peaks_ordered[i] - 40,
	      x_peaks_ordered[i] + 40);
    
    heights.push_back( temp_gaus->GetParameter(0));
    means.push_back(   temp_gaus->GetParameter(1));
    std_devs.push_back(temp_gaus->GetParameter(2));
    
    if (std_devs.back() > 200){
      std_devs.back() = 150;
    }
  }

  TF1* fitting_function = new TF1(Form("fitting_function_%s",hist->GetName()),"gaus(0)+gaus(3)+pol1(6)",0,30000);
  for (unsigned int i=0; i<means.size(); i++){
    fitting_function->SetParameter(0+3*i,  heights[i]);
    fitting_function->SetParameter(1+3*i,    means[i]);
    fitting_function->SetParameter(2+3*i, std_devs[i]);
  }
  fitting_function->SetParameter(6, 0);
  fitting_function->SetParameter(7, 0);
  
  hist->Fit(Form("fitting_function_%s",hist->GetName()),"Q 0","", // Do not + here because the final fitting function fits the entire spectrum
	    fitting_function->GetParameter(1) - 2.0 * fitting_function->GetParameter(2),
	    fitting_function->GetParameter(4) + 2.0 * fitting_function->GetParameter(5));
  
  for (int i=0; i<8; i++){
    fitting_function->SetParameter(i, fitting_function->GetParameter(i));
  }

  hist->Fit(Form("fitting_function_%s",hist->GetName()),"Q +","",
	    fitting_function->GetParameter(1) - 3.5 * fitting_function->GetParameter(2),
	    fitting_function->GetParameter(4) + 3.5 * fitting_function->GetParameter(5));
  
  output.push_back(fitting_function->GetParameter(1));
  output.push_back(fitting_function->GetParameter(4));

  // Draw components
  // Need to fit, and not draw for components to remain on histogram.
  // Therefore, fix the parameter (do not set).
  TF1* gaus_1_component = new TF1(Form("gaus_1_component_%s",hist->GetName()),"gaus(0)",0,30000);
  gaus_1_component->FixParameter(0, fitting_function->GetParameter(0));
  gaus_1_component->FixParameter(1, fitting_function->GetParameter(1));
  gaus_1_component->FixParameter(2, fitting_function->GetParameter(2));
  gaus_1_component->SetLineColor(1);
  hist->Fit(Form("gaus_1_component_%s",hist->GetName()),"Q +");


  TF1* gaus_2_component = new TF1(Form("gaus_2_component_%s",hist->GetName()),"gaus(3)",0,30000);
  gaus_2_component->FixParameter(3, fitting_function->GetParameter(3));
  gaus_2_component->FixParameter(4, fitting_function->GetParameter(4));
  gaus_2_component->FixParameter(5, fitting_function->GetParameter(5));
  gaus_2_component->SetLineColor(3);
  hist->Fit(Form("gaus_2_component_%s",hist->GetName()),"Q +");

  TF1* line_component = new TF1(Form("line_component_%s",hist->GetName()),"pol1(6)",0,30000);
  line_component->FixParameter(6, fitting_function->GetParameter(6));
  line_component->FixParameter(7, fitting_function->GetParameter(7));
  line_component->SetLineColor(5); // Histogram color is blue, so do not choose 4
  hist->Fit(Form("line_component_%s",hist->GetName()),"Q +");

  return output;
  
}


void single_fit_of_six_gaussians(TH1F* hist, std::vector<double> x_peaks_ordered){
  
  std::vector<double> heights;
  std::vector<double> means;
  std::vector<double> std_devs;

  for (unsigned int i=0; i<x_peaks_ordered.size(); i++){

    // Fit with Gaussian to get initial parameters
    TF1* temp_gaus = new TF1("temp_gaus","gaus(0)",0,30000);
    temp_gaus->SetParameter(0, hist->GetBinContent(x_peaks_ordered[i]));
    temp_gaus->SetParameter(1, x_peaks_ordered[i]);
    temp_gaus->SetParameter(2, 40);

    hist->Fit("temp_gaus","Q 0","",
	      x_peaks_ordered[i] - 40,
	      x_peaks_ordered[i] + 40);

    heights.push_back( temp_gaus->GetParameter(0));
    means.push_back(   temp_gaus->GetParameter(1));
    std_devs.push_back(temp_gaus->GetParameter(2));

    if (std_devs.back() > 200){
      std_devs.back() = 150;
    }
  }

  // Need to add the Gaussian that is not found (on purpose)
  // by TSpectrum!
  // The peak that needs to be added occurs on the left side
  // of the 3rd peak (2nd element of the vector) that is found by TSpectrum.
  // The location of the peak that needs to be added on will
  // be based on the location of the 3rd peak.
  // Cannot do this in the loop above.
  std::vector<double>::iterator it_heights;
  std::vector<double>::iterator it_means;
  std::vector<double>::iterator it_std_devs;
  it_heights = heights.begin();
  it_means = means.begin();
  it_std_devs = std_devs.begin();
  
  heights.insert(it_heights+2, 0.3 * heights[2]);
  means.insert(it_means+2, means[2] - 3 * std_devs[2]);
  std_devs.insert(it_std_devs+2, 40);

  TF1* fitting_function = new TF1(Form("fitting_function_%s",hist->GetName()),"gaus(0)+gaus(3)+gaus(6)+gaus(9)+gaus(12)+gaus(15)",0,30000);
  for (unsigned int i=0; i<means.size(); i++){ // Cannot use x_peaks_ordered.size() since this does not contain the peak that was added manually
    fitting_function->SetParameter(0+3*i,  heights[i]);
    fitting_function->SetParameter(1+3*i,    means[i]);
    fitting_function->SetParameter(2+3*i, std_devs[i]);
  }
 
  hist->Fit(Form("fitting_function_%s",hist->GetName()),"Q+","",
	    means.front() - 1.5 * std_devs.front(),
	    means.back()  + 1.5 * std_devs.back());

  // Components of fitting function
  // Need to fit, and not draw for components to remain on histogram.
  // Therefore, fix the parameter (do not set).


}


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
    
    hist->Fit("temp_gaus","Q + 0","",
	      x_peaks_ordered[i] - 40,
	      x_peaks_ordered[i] + 40);
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << temp_gaus->GetParameter(2) << std::endl;
    if (temp_gaus->GetParameter(2) < 50 || temp_gaus->GetParameter(2) > 300){
      temp_gaus->SetParameter(2, 120);
    }

    // Total fitting function
    TF1* fitting_function = new TF1(Form("fitting_function_%s_%d",hist->GetName(),i),"gaus(0)+pol1(3)",0,30000);
    fitting_function->SetParameter(0, temp_gaus->GetParameter(0));
    fitting_function->SetParameter(1, temp_gaus->GetParameter(1));
    fitting_function->SetParameter(2, temp_gaus->GetParameter(2));
    fitting_function->SetParameter(3, 0);
    fitting_function->SetParameter(4, 0);
    
    hist->Fit(fitting_function,"Q + 0","",
	      temp_gaus->GetParameter(1) - temp_gaus->GetParameter(2),
	      temp_gaus->GetParameter(1) + temp_gaus->GetParameter(2));
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << fitting_function->GetParameter(2) << std::endl;
    if (fitting_function->GetParameter(2) < 50 || fitting_function->GetParameter(2) > 300){
      fitting_function->SetParameter(2, 120);
    }
    
    hist->GetFunction("temp_gaus")->Delete();
    
    hist->GetFunction(Form("fitting_function_%s_%d",hist->GetName(),i))->Delete();
    
    fitting_function->SetParameter(0, fitting_function->GetParameter(0));
    fitting_function->SetParameter(1, fitting_function->GetParameter(1));
    fitting_function->SetParameter(2, fitting_function->GetParameter(2));
    fitting_function->SetParameter(3, fitting_function->GetParameter(3));
    fitting_function->SetParameter(4, fitting_function->GetParameter(4));
    
    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << fitting_function->GetParameter(2) << std::endl;
    if (fitting_function->GetParameter(2) < 50 || fitting_function->GetParameter(2) > 300){
      fitting_function->SetParameter(2, 120);
    }
    
    hist->Fit(fitting_function,"Q +","",
	      fitting_function->GetParameter(1) - 3.0 * fitting_function->GetParameter(2),
	      fitting_function->GetParameter(1) + 3.0 * fitting_function->GetParameter(2));
    
    // Components of fitting function
    // Need to fit, and not draw for components to remain on histogram.
    // Therefore, fix the parameter (do not set).
    TF1* component_gaus = new TF1(Form("component_gaus_%s_%d",hist->GetName(),i),"gaus(0)",0,30000);
    component_gaus->FixParameter(0, fitting_function->GetParameter(0));
    component_gaus->FixParameter(1, fitting_function->GetParameter(1));
    component_gaus->FixParameter(2, fitting_function->GetParameter(2));
    component_gaus->SetLineColor(3);
    hist->Fit(Form("component_gaus_%s_%d",hist->GetName(),i),"Q +");
    
    TF1* component_line = new TF1(Form("component_line_%s_%d",hist->GetName(),i),"pol1(3)",0,30000);
    component_line->FixParameter(3, fitting_function->GetParameter(3));
    component_line->FixParameter(4, fitting_function->GetParameter(4));
    component_line->SetLineColor(5); // Histogram color is blue, so do not choose 4
    hist->Fit(Form("component_line_%s_%d",hist->GetName(),i),"Q +");
    
    output.push_back(fitting_function->GetParameter(1));

  }

  return output;

}

std::vector<double> single_gaussians(TH1F* hist, std::vector<double> x_peaks_ordered){

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
    
    hist->Fit("temp_gaus","Q + 0","",
	      x_peaks_ordered[i] - 40,
	      x_peaks_ordered[i] + 40);

    // Remove unrealistic standard deviations that will mess up some fits
    //std::cout << temp_gaus->GetParameter(2) << std::endl;
    if (temp_gaus->GetParameter(2) < 40 || temp_gaus->GetParameter(2) > 300){
      temp_gaus->SetParameter(2, 45);
    }
    
    // Fitting function
    TF1* fitting_function = new TF1(Form("fitting_function_%s_%d",hist->GetName(),i),"gaus(0)",0,30000);
    fitting_function->SetParameter(0, temp_gaus->GetParameter(0));
    fitting_function->SetParameter(1, temp_gaus->GetParameter(1));
    fitting_function->SetParameter(2, temp_gaus->GetParameter(2));
    
    hist->Fit(fitting_function,"Q +","",
	      temp_gaus->GetParameter(1) - 1.25 * temp_gaus->GetParameter(2),
	      temp_gaus->GetParameter(1) + 2.25 * temp_gaus->GetParameter(2));
    
    hist->GetFunction("temp_gaus")->Delete();
    
    output.push_back(fitting_function->GetParameter(1));

  }
  
  return output;

}


void fit_peaks(){

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
  std::vector<std::string> Halves;
  Halves.push_back("B");
  Halves.push_back("T");
  
  for (unsigned int i=0; i<Halves.size(); i++){ // Halves
    for (int j=0; j<4; j++){ // Segments

      //std::cout << Halves[i] << " " << j << std::endl;

      fIn->cd();
      TH1F* hist = (TH1F*) gDirectory->Get(Form("E_SuN%s%d",Halves[i].c_str(),j+1));

      hist->GetXaxis()->SetRangeUser(1200,2000);

      TSpectrum* spc = new TSpectrum(1);
      int num_found = spc->Search(hist,40,"nodraw");
      double* x_peaks = spc->GetPositionX();
      double* y_peaks = spc->GetPositionY();

      /*for (int k=0; k<num_found; k++){
	std::cout << "i: " << i << " j: " << j <<
	  " k: " << k << " " <<
	  x_peaks[k] <<
	  " " <<
	  y_peaks[k] << std::endl;
	  }*/
	  
      std::vector<double> x_peaks_ordered;
      for (int k=0; k<num_found; k++){
	x_peaks_ordered.push_back(x_peaks[k]);
      }
      std::sort(x_peaks_ordered.begin(), x_peaks_ordered.end());

      ////////////////////
      // Method 1
      /*std::vector<double> output = gaussian_and_linear_background(hist, x_peaks_ordered);

      output_file << hist->GetName() << std::endl;

      for (int i=0; i<x_peaks_ordered.size(); i++){
	output_file << std::setw(10) << x_peaks_ordered[i] << std::setw(10) << output[i] << std::endl;
      }

      fOut->cd();
      hist->Write();*/
      ////////////////////
      
      ////////////////////
      // Method 2
      //single_fit_of_six_gaussians(hist, x_peaks_ordered);
      ////////////////////
      
      ////////////////////
      // Method 3
      std::vector<double> output = single_gaussians(hist, x_peaks_ordered);

      output_file << hist->GetName() << std::endl;

      for (unsigned int i=0; i<x_peaks_ordered.size(); i++){
	output_file << std::setw(10) << x_peaks_ordered[i] << std::setw(10) << output[i] << std::endl;
      }

      fOut->cd();
      hist->Write();
      ////////////////////

      ////////////////////
      // Method 4
      /*std::vector<double> output = double_gaussian_and_linear_background(hist, x_peaks_ordered);

      output_file << hist->GetName() << std::endl;

      for (int i=0; i<x_peaks_ordered.size(); i++){
	output_file << std::setw(10) << x_peaks_ordered[i] << std::setw(10) << output[i] << std::endl;
      }

      fOut->cd();
      hist->Write();*/
      ////////////////////



    }
  }
}




