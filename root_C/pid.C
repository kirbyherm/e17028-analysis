// Alex Dombos
// 2016-03-18
// Purpose: To create histograms of SuNs PMTs

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>

void chain_runs(){

  // Output ROOT file
//  TFile* fOut = new TFile("output_created_histograms.root","RECREATE");
  
  // Input ROOT file
    TChain* chain = new TChain("t");
    for (int i=2026; i<2169; i++)
//    for (int i=2097; i<2098; i++)
//    for (int i=2027; i<2046; i++)
    {
        std::string run_str = std::to_string(i);
        std::string filepath = "/mnt/analysis/e17028/rootfiles/run" + run_str + "-*.root";
        const char * c = filepath.c_str();
        std::cout << filepath << std::endl;
        chain->Add(c);
    } 

/*
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
*/
    
    double correction_factor = -1.3;
//    chain->Draw("energy_PIN1:energy_TAC_PIN1_I2N_corr>>pid(500,0,36000,1000,0,21000","","colz");
    TCanvas *c1 = new TCanvas("c1"); 
    c1->SetLogz();
//    chain->Draw("energy_PIN1:energy_TAC_PIN1_I2N_corr>>pid(1000,7000,25000,10000,3500,8000","","colz");
//    TCanvas *c4 = new TCanvas("c4"); 
//    c4->SetLogz();
//    chain->Draw("energy_PIN1:(energy_TAC_PIN1_I2N+1500)>>pid_no_corr(1000,7000,25000,10000,3500,8000","","colz");
    chain->Draw("energy_PIN1:(energy_TAC_PIN1_I2N+(energy_TAC_I2N_I2S*1.3)+1500)>>pid_corr(1000,13000,22000,1000,4000,8500","","colz");
//    pid_corr->SetTitle("Corrected PID");
//    pid_corr->SetXTitle("Corrected Time of Flight (arb. units)");
//    pid_corr->SetYTitle("Energy Loss in PIN1 (arb. units)");
//    pid_corr->SetStats(0);
    TCanvas *c2 = new TCanvas("c2"); 
    c2->SetLogz();
//    chain->Draw("energy_PIN1:energy_TAC_I2N_I2S>>pid_corr(1000,1000,7000,1000,4000,8500","","colz");
    chain->Draw("energy_TAC_PIN1_I2N:energy_TAC_I2N_I2S>>pid(500,2000,7000,500,6000,14000","","colz");
//    chain->Draw("energy_PIN1+energy_PIN2:energy_TAC_PIN1_I2N>>pid(500,0,30000,1000,10000,30000","","colz");
  
//    chain->Draw("energy_TAC_I2N_I2S:energy_TAC_PIN1_I2N>>uncorrected(500,0,26000,1000,-1000,10000","","colz");
//    TCanvas *c3 = new TCanvas("c3"); 
//    c3->SetLogz();
//    chain->Draw("(energy_TAC_PIN1_I2N+(energy_TAC_I2N_I2S*1.3)+1500):energy_TAC_I2N_I2S>>pid(500,2000,7000,1000,12000,24000","","colz");
//    chain->Draw("energy_PIN1:(energy_TAC_PIN1_I2N+(energy_TAC_I2N_I2S*1.8)+1500)>>pid_uncorr(1000,7000,25000,10000,3500,8000","","colz");
//    chain->Draw("energy_TAC_I2N_I2S:(energy_TAC_I2N_I2S*1.3+energy_TAC_PIN1_I2N+1500)>>corrected(1000,13000,22000,1000,3000,5000","","colz");
 //   chain->Draw("energy_TAC_I2N_I2S:energy_TAC_PIN1_I2N_corr>>corrected_var(500,0,26000,1000,-1000,10000","","colz");

}

