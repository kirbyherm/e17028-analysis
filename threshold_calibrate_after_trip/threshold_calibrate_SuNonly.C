///////////////////////////////////////////////////////////////////
// Alex Dombos
// 
// This applies an energy threshold for each segment. The energy 
// in the segment needs to be above this threshold for it to be
// added to the other segments in the total absorption spectrum.
//
// To run:
// root[0] .L threshold_calibrate.C+
// root[1] threshold_calibrate(arguments)
//
// Edited by Caley Harris for e17028 21 March 2019
///////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "TRandom3.h"

#include <iostream>

void threshold_calibrate(int run_number,
			 int segment_threshold){
  
  // For calibrations
  int seed = 47093625;
  TRandom3* ran = new TRandom3(seed);
  
  // Output ROOT file
  TFile* fOut = new TFile(Form("run_%d_segment_threshold_%03d_keV.root",
			       run_number,
			       segment_threshold),
			  "RECREATE");

  // Create a chain of input ROOT files
  TChain* chain = new TChain("t");  
  chain->Add(Form("/mnt/analysis/e17028/rootfiles/run%d-*.root",
		  run_number));
  
  // Variables for output ROOT file
  double sumAll = 0.0;
  double mult = 0.0;
  double energyT1 = 0.0;
  double energyT2 = 0.0;
  double energyT3 = 0.0;
  double energyT4 = 0.0;
  double energyB1 = 0.0;
  double energyB2 = 0.0;
  double energyB3 = 0.0;
  double energyB4 = 0.0;
  
  // Create tree and branches for the output ROOT file
  TTree* ttc  = new TTree("ttc","output after applying calibrations and thresholds");
  ttc->Branch("eneAll", &sumAll,  "eneAll/D");
  ttc->Branch("eneT1", &energyT1, "eneT1/D");
  ttc->Branch("eneT2", &energyT2, "eneT2/D");
  ttc->Branch("eneT3", &energyT3, "eneT3/D");
  ttc->Branch("eneT4", &energyT4, "eneT4/D");
  ttc->Branch("eneB1", &energyB1, "eneB1/D");
  ttc->Branch("eneB2", &energyB2, "eneB2/D");
  ttc->Branch("eneB3", &energyB3, "eneB3/D");
  ttc->Branch("eneB4", &energyB4, "eneB4/D");
  ttc->Branch("multi", &mult,     "multi/D");
  
  // Read in energies from input file
  int ene[2][5][4] = {{{0}}};
  chain->SetBranchAddress("energy_SuN", &ene);  
  
  int counter = 10;
  int num = chain->GetEntries();
  for (int i=0; i<num; i++){                                                                

    chain->GetEntry(i);

    // Counter
    if (i % (int)(0.1*num) == 0){
      std::cerr << counter << " ";
      counter --;
    }
          
    // Apply gain matching
    // Gain match the PMTs

    // Bottom PMTs
    double b11 = (ene[0][1][1] + ran->Uniform()) * 1.04729440;
    double b12 = (ene[0][1][2] + ran->Uniform()) * 1.00000000;
    double b13 = (ene[0][1][3] + ran->Uniform()) * 1.00803597;
    double b21 = (ene[0][2][1] + ran->Uniform()) * 0.93985409;
    double b22 = (ene[0][2][2] + ran->Uniform()) * 1.00000000;
    double b23 = (ene[0][2][3] + ran->Uniform()) * 0.90554753;
    double b31 = (ene[0][3][1] + ran->Uniform()) * 1.07039106;
    double b32 = (ene[0][3][2] + ran->Uniform()) * 1.00000000;
    double b33 = (ene[0][3][3] + ran->Uniform()) * 0.94512370;
    double b41 = (ene[0][4][1] + ran->Uniform()) * 1.01024426;
    double b42 = (ene[0][4][2] + ran->Uniform()) * 1.00000000;
    double b43 = (ene[0][4][3] + ran->Uniform()) * 0.94730107;

    // Top PMTs				     
    double t11 = (ene[1][1][1] + ran->Uniform()) * 1.02100555;
    double t12 = (ene[1][1][2] + ran->Uniform()) * 1.00000000;
    double t13 = (ene[1][1][3] + ran->Uniform()) * 1.15993832;
    double t21 = (ene[1][2][1] + ran->Uniform()) * 1.05993688;
    double t22 = (ene[1][2][2] + ran->Uniform()) * 1.00000000;
    double t23 = (ene[1][2][3] + ran->Uniform()) * 1.02701403;
    double t31 = (ene[1][3][1] + ran->Uniform()) * 0.93347940;
    double t32 = (ene[1][3][2] + ran->Uniform()) * 1.00000000;
    double t33 = (ene[1][3][3] + ran->Uniform()) * 0.99418978;
    double t41 = (ene[1][4][1] + ran->Uniform()) * 1.09887283;
    double t42 = (ene[1][4][2] + ran->Uniform()) * 1.00000000;
    double t43 = (ene[1][4][3] + ran->Uniform()) * 1.18339127;
    
    // Sum the PMTs to create a SuN segment
    
    // Bottom Segments
    double eB1 = b11 + b12 + b13;
    double eB2 = b21 + b22 + b23;
    double eB3 = b31 + b32 + b33;
    double eB4 = b41 + b42 + b43;
    
    // Top Segments
    double eT1 = t11 + t12 + t13;
    double eT2 = t21 + t22 + t23;
    double eT3 = t31 + t32 + t33;
    double eT4 = t41 + t42 + t43;
    
    // Calibrate the SuN segments
    // Apply an individualized quadratic calibration to each segment
	
    // Bottom Segments
    double eB1cal = 0.4114 * eB1 + 17.889;
    double eB2cal = 0.4517 * eB2 + 50.022;
    double eB3cal = 0.3842 * eB3 - 40.333;
    double eB4cal = 0.3932 * eB4 + 40.348;
    		            
    // Top Segments         
    double eT1cal = 0.3602 * eT1 + 103.940;
    double eT2cal = 0.3670 * eT2 +  98.461;
    double eT3cal = 0.4153 * eT3 +  16.761;
    double eT4cal = 0.3462 * eT4 + 138.400;
    
    // Apply threshold
    // (include only the positive, non-zero values of energy to avoid any offsets)
    
    // Energy (keV) cutoff for each segment
    double eB1cutOff = segment_threshold;
    double eB2cutOff = segment_threshold;
    double eB3cutOff = segment_threshold;
    double eB4cutOff = segment_threshold;
    
    double eT1cutOff = segment_threshold;
    double eT2cutOff = segment_threshold;
    double eT3cutOff = segment_threshold;
    double eT4cutOff = segment_threshold;
    
    // Initialize output rootfile variables to zero
    sumAll = 0.0;
    mult = 0.0;
    energyT1 = 0.0;
    energyT2 = 0.0;
    energyT3 = 0.0;
    energyT4 = 0.0;
    energyB1 = 0.0;
    energyB2 = 0.0;
    energyB3 = 0.0;
    energyB4 = 0.0;
    
    // If calibrated segments are above the threshold,
    // put the event in the new tree and add it to the total absorption spectrum
    if (eB1cal > eB1cutOff){
    	energyB1 = eB1cal;
    	mult ++;
    	sumAll += eB1cal;
    }
    if (eB2cal > eB2cutOff){
    	energyB2 = eB2cal;
    	mult ++;
    	sumAll += eB2cal;
    }
    if (eB3cal > eB3cutOff){
    	energyB3 = eB3cal;
    	mult ++;
    	sumAll += eB3cal;
    }
    if (eB4cal > eB4cutOff){
    	energyB4 = eB4cal;
    	mult ++;
    	sumAll += eB4cal;
    }
    if (eT1cal > eT1cutOff){
    	energyT1 = eT1cal;
    	mult ++;
    	sumAll += eT1cal;
    }
    if (eT2cal > eT2cutOff){
    	energyT2 = eT2cal;
    	mult ++;
    	sumAll += eT2cal;
    }
    if (eT3cal > eT3cutOff){
    	energyT3 = eT3cal;
    	mult ++;
    	sumAll += eT3cal;
    }
    if (eT4cal > eT4cutOff){
    	energyT4 = eT4cal;
    	mult ++;
    	sumAll += eT4cal;
    }
    
    // Fill Tree
    ttc->Fill();
  
  }

  // Write to file
  chain->Delete();
  fOut->cd();
  ttc->Write();
  fOut->Close();
  
}
