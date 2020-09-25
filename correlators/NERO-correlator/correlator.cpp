// Alex Dombos
// 2016-03-08
// Purpose: Take the SuN ROOT file and correlate implantation events with decay events.
// Notes:
// 1) Initialize ALL private variables either in the constructor initializer list, or in the body of the constructor
// 2) When using diagnostics in the event loop, print the object before any calibrations are performed to make sure that all values are initialized to zero (see note above)
// 3) When using diagnostics in the event loop, see that changes have been made after applying any calibrations
// 4) In the diagnostic function, the private variables added to the output stream with two different ways (the private variable and the Get method).
//    This is an extra precaution to make sure they are the same because they should be (and to also make sure I did not make a mistake with the Get method).
// 5) For std::memcpy, the following must be input correctly: (1) "sizeof" (2) the array dimensions
// 6) Pay attention to whether thresholds are applied to raw or calibrated values
// 7) Without applying any gates, always make sure the branch that has the correlation times is positive and the times are never larger than the correlation time window

// To run:
// g++ correlator.cpp $(root-config --cflags) $(root-config --glibs) -std=c++11 -O3 -o correlator
// ./correlator command_line_arguments

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "correlator.h"
#include <iostream>
#include <iomanip>
#include <cstring> // std::memcpy
#include <deque>
#include <ctime>
#include <sys/time.h>

#include "./cuts.h"
// For thresholds (for sure the DSSD class, but potentially more, I should mention whether the threshold is applied to the raw values or the calibrated values

// specifics to e12001: converting clock ticks to nanoseconds in the calibrate function for DSSD, and possibly the requirement that the I2 scintillator be present for an implant
int momentum_correction(int i2pos, int raw_tof){
    return i2pos*0.8 + 1500 + raw_tof;
}

int main(int argc, char* argv[]){
  std::cout << "Does this even work?" << std::endl;
  if ((std::string(argv[2]) != "true") &&
      (std::string(argv[2]) != "rand")){
    std::cout << "Input error" << std::endl;
    std::cout << "Use true for true correlations or "
	      << "rand for random correlations" << std::endl;
    return -1;
  }
  
  if ((std::string(argv[4]) != "sd") &&
      (std::string(argv[4]) != "md")){
    std::cout << "Input error" << std::endl;
    std::cout << "Use sd for single decay mode or "
	      << "md for multiple decay mode" << std::endl;
    return -1;
  }
  
  int random_number_generator_seed = 47093625;
  TRandom3* rand_num_gen = new TRandom3(random_number_generator_seed);
  
  std::cout << "Making outputfile" << std::endl;
  // ===== Output ROOT file
  TFile* fOut = new TFile(Form("/mnt/analysis/e17028/correlated_rootfiles_NERO/%dms/%s/Run%s_%s_%d_%s_%i_%s.root",
			       std::stoi(argv[5])*1000,
			       argv[2],
			       argv[1],
			       argv[2],
			       std::stoi(argv[3]),
			       argv[4],
			       std::stoi(argv[6]),
			       argv[7]),
			  "RECREATE");
  std::cout << "Made outputfile" << std::endl;
  TTree* tOut = new TTree("t","output after thresholds, calibrations, and correlations");

  TTree* tImplants = new TTree("implants","output of just the implanted events");
  // TACs

  double output_E_TAC_I2S_I2N_raw = 0;
  double output_E_TAC_I2S_I2N_cal = 0;

  tOut->Branch("E_TAC_I2S_I2N_raw", &output_E_TAC_I2S_I2N_raw, "E_TAC_I2S_I2N_raw/D");
  tOut->Branch("E_TAC_I2S_I2N_cal", &output_E_TAC_I2S_I2N_cal, "E_TAC_I2S_I2N_cal/D");

  double output_E_TAC_PIN1_I2N_raw = 0;
  double output_E_TAC_PIN1_I2N_cal = 0;

  tOut->Branch("E_TAC_PIN1_I2N_raw", &output_E_TAC_PIN1_I2N_raw, "E_TAC_PIN1_I2N_raw/D");
  tOut->Branch("E_TAC_PIN1_I2N_cal", &output_E_TAC_PIN1_I2N_cal, "E_TAC_PIN1_I2N_cal/D");

  double output_E_TAC_PIN2_I2N_raw = 0;
  double output_E_TAC_PIN2_I2N_cal = 0;

  tOut->Branch("E_TAC_PIN2_I2N_raw", &output_E_TAC_PIN2_I2N_raw, "E_TAC_PIN2_I2N_raw/D");
  tOut->Branch("E_TAC_PIN2_I2N_cal", &output_E_TAC_PIN2_I2N_cal, "E_TAC_PIN2_I2N_cal/D");

  double output_E_TAC_PIN1_I2S_raw = 0;
  double output_E_TAC_PIN1_I2S_cal = 0;

  tOut->Branch("E_TAC_PIN1_I2S_raw", &output_E_TAC_PIN1_I2S_raw, "E_TAC_PIN1_I2S_raw/D");
  tOut->Branch("E_TAC_PIN1_I2S_cal", &output_E_TAC_PIN1_I2S_cal, "E_TAC_PIN1_I2S_cal/D");

  double output_E_TAC_PIN2_I2S_raw = 0;
  double output_E_TAC_PIN2_I2S_cal = 0;

  tOut->Branch("E_TAC_PIN2_I2S_raw", &output_E_TAC_PIN2_I2S_raw, "E_TAC_PIN2_I2S_raw/D");
  tOut->Branch("E_TAC_PIN2_I2S_cal", &output_E_TAC_PIN2_I2S_cal, "E_TAC_PIN2_I2S_cal/D");

  // PINs

  double output_E_PIN1_raw = 0;
  double output_E_PIN1_cal = 0;
  
  tOut->Branch("E_PIN1_raw", &output_E_PIN1_raw, "E_PIN1_raw/D");
  tOut->Branch("E_PIN1_cal", &output_E_PIN1_cal, "E_PIN1_cal/D"); 

  double output_E_PIN2_raw = 0;
  double output_E_PIN2_cal = 0;
  
  tOut->Branch("E_PIN2_raw", &output_E_PIN2_raw, "E_PIN2_raw/D");
  tOut->Branch("E_PIN2_cal", &output_E_PIN2_cal, "E_PIN2_cal/D");

  double output_E_PIN3_raw = 0;
  double output_E_PIN3_cal = 0;
  
  tOut->Branch("E_PIN3_raw", &output_E_PIN3_raw, "E_PIN3_raw/D");
  tOut->Branch("E_PIN3_cal", &output_E_PIN3_cal, "E_PIN3_cal/D");

  double output_E_SCINT_raw = 0;
  double output_E_SCINT_cal = 0;
  
  tOut->Branch("E_SCINT_raw", &output_E_SCINT_raw, "E_SCINT_raw/D");
  tOut->Branch("E_SCINT_cal", &output_E_SCINT_cal, "E_SCINT_cal/D");

  double output_E_SCINT_iraw = 0;
  double output_E_SCINT_ical = 0;
  
  tOut->Branch("E_SCINT_iraw", &output_E_SCINT_iraw, "E_SCINT_iraw/D");
  tOut->Branch("E_SCINT_ical", &output_E_SCINT_ical, "E_SCINT_ical/D");

  double output_E_SCINT_draw = 0;
  double output_E_SCINT_dcal = 0;
  
  tOut->Branch("E_SCINT_draw", &output_E_SCINT_draw, "E_SCINT_draw/D");
  tOut->Branch("E_SCINT_dcal", &output_E_SCINT_dcal, "E_SCINT_dcal/D");
  // DSSD

  // Low gain

  double output_E_DSSD_low_raw[2][40] = {{0}};
  double output_E_DSSD_low_cal[2][40] = {{0}};

  tOut->Branch("E_DSSD_low_raw", &output_E_DSSD_low_raw, "E_DSSD_low_raw[2][40]/D");
  tOut->Branch("E_DSSD_low_cal", &output_E_DSSD_low_cal, "E_DSSD_low_cal[2][40]/D");
  tImplants->Branch("E_DSSD_low_raw", &output_E_DSSD_low_raw, "E_DSSD_low_raw[2][40]/D");
  tImplants->Branch("E_DSSD_low_cal", &output_E_DSSD_low_cal, "E_DSSD_low_cal[2][40]/D");

  double output_t_DSSD_low_raw[2][40] = {{0}};
  double output_t_DSSD_low_cal[2][40] = {{0}};

  tOut->Branch("t_DSSD_low_raw", &output_t_DSSD_low_raw, "t_DSSD_low_raw[2][40]/D");
  tOut->Branch("t_DSSD_low_cal", &output_t_DSSD_low_cal, "t_DSSD_low_cal[2][40]/D");
  tImplants->Branch("t_DSSD_low_raw", &output_t_DSSD_low_raw, "t_DSSD_low_raw[2][40]/D");
  tImplants->Branch("t_DSSD_low_cal", &output_t_DSSD_low_cal, "t_DSSD_low_cal[2][40]/D");

  int output_m_DSSD_low_raw[2] = {0};
  int output_m_DSSD_low_cal[2] = {0};

  tOut->Branch("m_DSSD_low_raw", &output_m_DSSD_low_raw, "m_DSSD_low_raw[2]/I");
  tOut->Branch("m_DSSD_low_cal", &output_m_DSSD_low_cal, "m_DSSD_low_cal[2]/I");
  tImplants->Branch("m_DSSD_low_raw", &output_m_DSSD_low_raw, "m_DSSD_low_raw[2]/I");
  tImplants->Branch("m_DSSD_low_cal", &output_m_DSSD_low_cal, "m_DSSD_low_cal[2]/I");

  int cStripLowFront = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD
  int cStripLowBack  = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD

  tOut->Branch("cStripLowFront", &cStripLowFront, "cStripLowFront/I");
  tOut->Branch("cStripLowBack",  &cStripLowBack,  "cStripLowBack/I");
  tImplants->Branch("cStripLowFront", &cStripLowFront, "cStripLowFront/I");
  tImplants->Branch("cStripLowBack",  &cStripLowBack,  "cStripLowBack/I");

  double eStripLowFront = 0;
  double eStripLowBack  = 0;

  tOut->Branch("eStripLowFront", &eStripLowFront, "eStripLowFront/D");
  tOut->Branch("eStripLowBack",  &eStripLowBack,  "eStripLowBack/D");

  double tStripLowFront = 0;
  double tStripLowBack  = 0;

  tOut->Branch("tStripLowFront", &tStripLowFront, "tStripLowFront/D");
  tOut->Branch("tStripLowBack",  &tStripLowBack,  "tStripLowBack/D");

  double e_DSSD_low_cal_sum_back = 0;
  double e_DSSD_low_cal_sum_front = 0;
  double e_DSSD_low_cal_sum = 0;

  tOut->Branch("e_DSSD_low_cal_sum_back",  &e_DSSD_low_cal_sum_back,  "e_DSSD_low_cal_sum_back/D");
  tOut->Branch("e_DSSD_low_cal_sum_front", &e_DSSD_low_cal_sum_front, "e_DSSD_low_cal_sum_front/D");
  tOut->Branch("e_DSSD_low_cal_sum",       &e_DSSD_low_cal_sum,       "e_DSSD_low_cal_sum/D");

  // High gain

  double output_E_DSSD_high_raw[2][40] = {{0}};
  double output_E_DSSD_high_cal[2][40] = {{0}};

  tOut->Branch("E_DSSD_high_raw", &output_E_DSSD_high_raw, "E_DSSD_high_raw[2][40]/D");
  tOut->Branch("E_DSSD_high_cal", &output_E_DSSD_high_cal, "E_DSSD_high_cal[2][40]/D");

  double output_t_DSSD_high_raw[2][40] = {{0}};
  double output_t_DSSD_high_cal[2][40] = {{0}};

  tOut->Branch("t_DSSD_high_raw", &output_t_DSSD_high_raw, "t_DSSD_high_raw[2][40]/D");
  tOut->Branch("t_DSSD_high_cal", &output_t_DSSD_high_cal, "t_DSSD_high_cal[2][40]/D");

  int output_m_DSSD_high_raw[2] = {0};
  int output_m_DSSD_high_cal[2] = {0};

  tOut->Branch("m_DSSD_high_raw", &output_m_DSSD_high_raw, "m_DSSD_high_raw[2]/I");
  tOut->Branch("m_DSSD_high_cal", &output_m_DSSD_high_cal, "m_DSSD_high_cal[2]/I");

  int cStripHighFront = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD
  int cStripHighBack  = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD

  tOut->Branch("cStripHighFront", &cStripHighFront, "cStripHighFront/I");
  tOut->Branch("cStripHighBack",  &cStripHighBack,  "cStripHighBack/I");

  double eStripHighFront = 0;
  double eStripHighBack  = 0;

  tOut->Branch("eStripHighFront", &eStripHighFront, "eStripHighFront/D");
  tOut->Branch("eStripHighBack",  &eStripHighBack,  "eStripHighBack/D");

  double tStripHighFront = 0;
  double tStripHighBack  = 0;

  tOut->Branch("tStripHighFront", &tStripHighFront, "tStripHighFront/D");
  tOut->Branch("tStripHighBack",  &tStripHighBack,  "tStripHighBack/D");

  double e_DSSD_high_cal_sum_back = 0;
  double e_DSSD_high_cal_sum_front = 0;
  double e_DSSD_high_cal_sum = 0;

  tOut->Branch("e_DSSD_high_cal_sum_back",  &e_DSSD_high_cal_sum_back,  "e_DSSD_high_cal_sum_back/D");
  tOut->Branch("e_DSSD_high_cal_sum_front", &e_DSSD_high_cal_sum_front, "e_DSSD_high_cal_sum_front/D");
  tOut->Branch("e_DSSD_high_cal_sum",       &e_DSSD_high_cal_sum,       "e_DSSD_high_cal_sum/D");

  // NERO
  
  int output_E_NERO_raw[4][15] = {{0}};
  double output_t_NERO_raw[4][15] = {{0}};
  double output_tdiff_NERO_raw[4][15] = {{0}};
	
  tOut->Branch("E_NERO_raw", &output_E_NERO_raw, "E_NERO_raw[4][15]/I");
  tOut->Branch("t_NERO_raw", &output_t_NERO_raw, "t_NERO_raw[4][15]/D");
  tOut->Branch("td_NERO_raw", &output_tdiff_NERO_raw, "td_NERO_raw[4][15]/D");

  // Correlations

  double correlation_implant_decay_time = 0;
  double back_to_back_implant_time = 0;

  tOut->Branch("correlation_implant_decay_time", &correlation_implant_decay_time, "correlation_implant_decay_time/D");
  tOut->Branch("back_to_back_implant_time",      &back_to_back_implant_time,      "back_to_back_implant_time/D");

  TH1F* correlation_flags = new TH1F("correlation_flags","",50,0,50);
  // 4  - Implantation event
  // 8  - Back to back implantation events (if running multiple decay mode, this should equal the number of counts in flag 4 minus 256)
  // 12 - Implantation without back to back implantations
  // 16 - Decay event
  // 20 - Time between implantations is too short
  // 24 - Decay event correlated spatially, but not temporally (exceeded correlation time window)
  // 28 - Decay event successfully correlated

  // ===== Input ROOT file(s)

  TChain* Chain = new TChain("t");
  std::cout << "Made chain" << std::endl;
  Chain->Add(Form("/mnt/analysis/e17028/rootfiles/run%s*.root", argv[1]));
  std::cout << "Added stuff" << std::endl;
  
  // TACs
  int E_TAC_I2S_I2N_raw = 0;
  int E_TAC_PIN1_I2N_raw = 0;
  int E_TAC_PIN2_I2N_raw = 0;
  int E_TAC_PIN1_I2S_raw = 0;
  int E_TAC_PIN2_I2S_raw = 0;

  Chain->SetBranchAddress("energy_TAC_I2N_I2S",  &E_TAC_I2S_I2N_raw);
  Chain->SetBranchAddress("energy_TAC_PIN1_I2N", &E_TAC_PIN1_I2N_raw);
  Chain->SetBranchAddress("energy_TAC_PIN2_I2N", &E_TAC_PIN2_I2N_raw);
  Chain->SetBranchAddress("energy_TAC_PIN1_I2S", &E_TAC_PIN1_I2S_raw);
  Chain->SetBranchAddress("energy_TAC_PIN2_I2S", &E_TAC_PIN2_I2S_raw);
  std::cout << "Added TACS" << std::endl;
  
  // PINs
  int E_PIN1_raw = 0;
  int E_PIN2_raw = 0;
  int E_PIN3_raw = 0;

  Chain->SetBranchAddress("energy_PIN1", &E_PIN1_raw);
  Chain->SetBranchAddress("energy_PIN2", &E_PIN2_raw);
  Chain->SetBranchAddress("energy_PIN3", &E_PIN3_raw);
  std::cout << "Added PINS" << std::endl;

  // SCINT
  int E_SCINT_raw = 0;
  Chain->SetBranchAddress("energy_Veto", &E_SCINT_raw);
  int t_SCINT_raw = 0;
  Chain->SetBranchAddress("time_Veto", &t_SCINT_raw);

  //std::cout << E_PIN1_raw << std::endl;

  // DSSD
  int E_DSSD_low_raw[2][40] = {{0}};
  int E_DSSD_high_raw[2][40] = {{0}};

  Chain->SetBranchAddress("energy_BCSDSSD_low",  &E_DSSD_low_raw);
  Chain->SetBranchAddress("energy_BCSDSSD_high", &E_DSSD_high_raw);

  double t_DSSD_low_raw[2][40] = {{0}};
  double t_DSSD_high_raw[2][40] = {{0}};

  Chain->SetBranchAddress("time_BCSDSSD_low",  &t_DSSD_low_raw);
  Chain->SetBranchAddress("time_BCSDSSD_high", &t_DSSD_high_raw);

  int m_DSSD_low_raw[2] = {0};
  int m_DSSD_high_raw[2] = {0};

  Chain->SetBranchAddress("multiplicity_BCSDSSD_low",  &m_DSSD_low_raw);
  Chain->SetBranchAddress("multiplicity_BCSDSSD_high", &m_DSSD_high_raw);

  int E_SCINT[6] = {0};
 
  Chain->SetBranchAddress("energy_LaBr", &E_SCINT);

  std::cout << "Added DSSD" << std::endl;

  // SuN
  int E_SuN_raw[2][5][4] = {{{0}}};
  Chain->SetBranchAddress("energy_SuN", &E_SuN_raw);
  std::cout << "Added SuN" << std::endl;

  // SSSD
  int E_SSSD_raw[1][16] = {{0}};
  double t_SSSD_raw[1][16] = {{0}};
  Chain->SetBranchAddress("energy_BCSSSSD_high", &E_SSSD_raw);
  Chain->SetBranchAddress("time_BCSSSSD_high", &t_SSSD_raw);
  std::cout << "Added SSSD" << std::endl;

  // NERO
  int E_NERO_raw[4][15] = {{0}};
  double t_NERO_raw[4][15] = {{0}};
  Chain->SetBranchAddress("energy_NERO", &E_NERO_raw);
  Chain->SetBranchAddress("time_NERO", &t_NERO_raw);

  // The DSSD
  std::vector<std::vector<Implantation> > dssd_implantation_pixels(40, std::vector<Implantation>(40));
//  std::cout << "back pixels = " << dssd_implantation_pixels.size();

//  Implantation dssd_implantation_pixels[40][40]; // 16 by 16 strips
  std::cout << "Added DSSD pixel" << std::endl;
//  std::cout << "total pixels = " << dssd_implantation_pixels.size();
  std::cout << "front pixels = " << dssd_implantation_pixels[39].size() << std::endl;
  // Make cuts
  std::cout << "I made the cuts" << std::endl;
  std::string isotope = argv[7];
  std::map<std::string, TCutG*> cutDict = cuts();
  TCutG* isotope_cut = cutDict[isotope];

  // Loop over events and correlate
  int counter = 10;
  std::cout << "Added counter" << std::endl;
  int num = Chain->GetEntries();
  clock_t startTime = clock();
  int implants = 0;
  int overflow_events = 0;
  int overflows = 0;
  for (int i=0; i<num; i++){
    //std::cout << "Enters, entry = " << i << std::endl; 
    Chain->GetEntry(i);
    /*for(int m = 0; m<4; m++){
      for(int n=0;n<15;n++){
         if(E_NERO_raw[m][n]>0){std::cout << "Quad " << m << " Det " << n << " with " << E_NERO_raw[m][n] << std::endl;}  
      }
    }*/
    //std::cout << e
    //std::cout << "///////////////////////////////////////Entry " << i << std::endl;
    //Chain->Print();
    //std::cout << Chain->Scan() << std::endl;
    /*if (E_PIN1_raw!=0){
	std::cout << E_PIN1_raw << std::endl;
    }*/
    // Counter
    if (i % (int)(0.1*num) == 0){
      std::cerr << counter << " ";
      std::cout << double (clock()-startTime) << std::endl;
      if (counter < -10){
        fOut->cd();
        correlation_flags->Write();
        tOut->Write();
        tOut->ResetBranchAddresses();
        fOut->Close();
        return 0;
      }
      counter --;
    }
    // TAC diagnostics
    /*std::cout << "i: " << i << std::endl;
    std::cout << std::endl;
    TAC tac_sub_event(E_TAC_I2S_I2N_raw, E_TAC_PIN1_I2N_raw, E_TAC_PIN2_I2N_raw);
    std::cout << tac_sub_event << std::endl;
    tac_sub_event.Calibrate();
    std::cout << tac_sub_event << std::endl;*/

    // PIN diagnostics
    /*std::cout << "i: " << i << std::endl;
    std::cout << std::endl;
    PIN pin_sub_event(E_PIN1_raw, E_PIN2_raw);
    std::cout << pin_sub_event << std::endl;
    pin_sub_event.Calibrate();
    std::cout << pin_sub_event << std::endl;*/

    // DSSD diagnostics
    /*std::cout << "i: " << i << std::endl;
    DSSD dssd_sub_event(E_DSSD_low_raw,
			t_DSSD_low_raw,
			m_DSSD_low_raw,
			E_DSSD_high_raw,
			t_DSSD_high_raw,
			m_DSSD_high_raw);
    std::cout << dssd_sub_event << std::endl;
    dssd_sub_event.Calibrate();
    std::cout << dssd_sub_event << std::endl;*/

    // SuN diagnostics
    /*std::cout << "i: " << i << std::endl;
    SuN sun_sub_event(E_SuN_raw);
    std::cout << sun_sub_event << std::endl;
    sun_sub_event.GainMatchAndCalibrate();
    std::cout << sun_sub_event << std::endl;*/
    
    // Veto diagnostics
    /*std::cout << "i: " << i << std::endl;
    Veto veto_sub_event(E_Veto_raw);
    std::cout << veto_sub_event << std::endl;*/

    // First check detectors individual with the above diagnostics.
    // If there are no problems, copy and paste their instantiation here.
    
    TAC tac_sub_event(E_TAC_I2S_I2N_raw, E_TAC_PIN1_I2N_raw, E_TAC_PIN1_I2S_raw, E_TAC_PIN2_I2N_raw, E_TAC_PIN2_I2S_raw);
    PIN pin_sub_event(E_PIN1_raw, E_PIN2_raw, E_PIN3_raw );
    DSSD dssd_sub_event(E_DSSD_low_raw,
			t_DSSD_low_raw,
			m_DSSD_low_raw,
			E_DSSD_high_raw,
			t_DSSD_high_raw,
			m_DSSD_high_raw);
    SSSD sssd_sub_event(E_SSSD_raw, t_SSSD_raw);
    NERO nero_sub_event(E_NERO_raw, t_NERO_raw);
    SCINT scint_sub_event(E_SCINT_raw, t_SCINT_raw);

    //Punchthrough, set to true or false
    Punchthrough punchthrough_event(pin_sub_event,
				   sssd_sub_event,
				   dssd_sub_event,
				   scint_sub_event);
   
    punchthrough_event.pin_sub_event.Calibrate(); 
    punchthrough_event.sssd_sub_event.Calibrate(); 
    punchthrough_event.dssd_sub_event.Calibrate(); 
    punchthrough_event.scint_sub_event.Calibrate(); 
    
    //Implement cuts
    int e_PIN1_raw = pin_sub_event.GetPIN1RawEnergy();
    int e_PIN2_raw = pin_sub_event.GetPIN2RawEnergy();
    int e_TAC_PIN2_I2S_raw = tac_sub_event.Get_PIN2_I2S_Raw_Energy();
    int corr_tof = momentum_correction( tac_sub_event.Get_I2S_I2N_Raw_Energy(), e_TAC_PIN2_I2S_raw );
//    int corr_tof = E_TAC_PIN2_I2S_raw + E_TAC_I2S_I2N_raw * 0.8 + 1500;
    if ((isotope_cut->IsInside((corr_tof),(e_PIN2_raw)) && (e_PIN1_raw > 0) && cutDict["pin12"]->IsInside((e_PIN2_raw),(e_PIN1_raw)) && cutDict["i2pos"]->IsInside((E_TAC_I2S_I2N_raw),(E_PIN2_raw)))) {
            correlation_flags->Fill(2);

       // Implantation event
       Implantation implantation_event(tac_sub_event,
           			    pin_sub_event,
           			    dssd_sub_event,
           			    sssd_sub_event,
           			    scint_sub_event);
       if (implantation_event.IsValidImplantation() || implantation_event.IsImplanted()){
         std::cout << "Error for implantations" << std::endl; // EStripLow(Back/Front) is not calculated until after calibrations, and EStripLow(Back/Front) is used to determine valid implant conditions. Also, is_implanted is intialized to false
       }

       // Calibrate the detectors. <======================== This is not necessary if you should make the condition of implant condition refer to raw maximum values instead of calibrated values. This would change your comment directly above. Maybe change to channel instead of energy (CANT DO THIS AS CHANNEL REFERS TO THE MAX ___CALIBRATED___ VALUE)           ********* HOWEVER, channels and max energy are only calculated with raw threshold values are satisfied!
       implantation_event.tac_sub_event.Calibrate();
       implantation_event.pin_sub_event.Calibrate();
//       std::cout << "calibrate" << std::endl;
       int temp_overflows = implantation_event.dssd_sub_event.Calibrate();
       if (temp_overflows > 2){
         std::cout << temp_overflows << std::endl;
         overflow_events++;
         overflows+=temp_overflows;
       }
       implantation_event.scint_sub_event.Calibrate();
       //implantation_event.sun_sub_event.GainMatchAndCalibrate();
       //std::cout << implantation_event.dssd_sub_event.GetLowCalEnergy(1,15) << std::endl;
       //std::cout << "calibrated" << std::endl;
       /*for (int j=0; j<40; j++){
           for (int k=0; k<40; k++){
             std::cout << i << " " << j << " " << k << " " << dssd_implantation_pixels[j][k].IsImplanted() << std::endl; // Use this to make sure all pixels are initialized to false. Have to remove correlator logic
           }
       }*/

       if (implantation_event.IsValidImplantation()){ // && punchthrough_event.IsPunchthrough( false ) == false) 

         correlation_flags->Fill(4); // Implantation event
         
         //std::cout << "Fill correlation_flag 4" << std::endl;
                 
         /*for (int j=0; j<40; j++){
           for (int k=0; k<40; k++){
             std::cout << i << " " << j << " " << k << " " << dssd_implantation_pixels[j][k].IsImplanted() << std::endl; // Use this to make sure all pixels are initialized to false. Have to remove correlator logic
           }
         }*/
         
         // First check for back to back implantations (if there is an existing implant in this pixel)
         int stripLF = implantation_event.dssd_sub_event.GetCStripLowFront();
         int stripLB = implantation_event.dssd_sub_event.GetCStripLowBack();
         //int stripHF = implantation_event.dssd_sub_event.GetCStripHighFront();
         //int stripHB = implantation_event.dssd_sub_event.GetCStripHighBack();

    
         if (dssd_implantation_pixels[stripLF][stripLB].IsImplanted() == true){

           
           correlation_flags->Fill(8); // Back to back implantation events

           // Calculate time between back to back implantations
           double time_between_back_to_back_implantations = implantation_event.dssd_sub_event.GetTStripLowAvg() -
             dssd_implantation_pixels[stripLF][stripLB].dssd_sub_event.GetTStripLowAvg();

           if (time_between_back_to_back_implantations > 1e10)
             correlation_flags->Fill(10); //Back to back implantation events of 10s or more

           implantation_event.SetTimeBetweenBackToBackImplantations(time_between_back_to_back_implantations);

           // Accept the latest implant
           dssd_implantation_pixels[stripLF][stripLB] = implantation_event;
           dssd_implantation_pixels[stripLF][stripLB].SetIsImplanted(true);

         }
         else {
           //std::cout << "Implant?" << std::endl;
           correlation_flags->Fill(12); // Implantation without back to back implantations
          // std::cout << "Flag = " << 12 << std::endl;
           dssd_implantation_pixels[stripLF][stripLB] = implantation_event;
//           std::cout << "Implant in " << stripLF << ", " << stripLB << " at t = " << dssd_implantation_pixels[stripLF][stripLB].dssd_sub_event.GetTStripLowAvg() << std::endl;
//           std::cout << t_DSSD_high_raw[stripLF][stripLB] << std::endl;
           dssd_implantation_pixels[stripLF][stripLB].SetIsImplanted(true);
         }
           implants++;
       }
    }
    // Decay event
    Decay decay_event(pin_sub_event,
		      dssd_sub_event,
		      nero_sub_event,
		      scint_sub_event);
    if (decay_event.IsValidDecay()){
      std::cout << "Error for decays" << std::endl; // EStripHigh(Back/Front) is not calculated until after calibrations, and EStripHigh(Back/Front) is used to determine valid decay conditions
    }
    int dummy_overflow = 0;
    // Calibrate the detectors
    decay_event.pin_sub_event.Calibrate();
    dummy_overflow = decay_event.dssd_sub_event.Calibrate();
    decay_event.nero_sub_event.Calibrate();
    decay_event.scint_sub_event.Calibrate();
    //std::cout << "Calibrate decay" << std::endl;
    if (decay_event.IsValidDecay()){
      //decay_event.nero_sub_event.Calibrate();
      correlation_flags->Fill(16); // Decay event

      //std::cout << "Flag = " << 16 << std::endl;
      
//      std::cout << "i: " << i << std::endl;
//      std::cout << decay_event.pin_sub_event << std::endl;
//      std::cout << decay_event.dssd_sub_event << std::endl;
//      std::cout << decay_event.sun_sub_event << std::endl;
      
      
      int front = 500;
      int back = 500;
//      double arbitrary_long_time = 1E11;

      int correlation_field = std::stoi(argv[3]);
      int correlation_limit = (correlation_field-1)/2; //3 for 3x3 pixel, 1 or 2 for 1x1 pixel

      int front_lower_limit = decay_event.dssd_sub_event.GetCStripHighFront() - correlation_limit;
      int front_upper_limit = decay_event.dssd_sub_event.GetCStripHighFront() + correlation_limit;
      int back_lower_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  - correlation_limit;
      int back_upper_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  + correlation_limit;
      //std::cout << front_lower_limit << " " << front_upper_limit << " " << back_lower_limit << " " << back_upper_limit << std::endl;
      std::deque<PossImplants> implantlist;
      for (int j = front_lower_limit; j <= front_upper_limit; j++){ // event loop is over "i" 
	for (int k = back_lower_limit; k <= back_upper_limit; k++){
	  if ((j >= 0) && (j <= 39) && (k >= 0) && (k<=39)){
//        std::cout<<j<< ", " << k<< std::endl;
	    // Choose the implantation closest in time to the decay
	    if (dssd_implantation_pixels[j][k].IsImplanted() == true){
//              std::cout << "Implant in (" << j << "," << k << ")" << std::endl;
              PossImplants implantinfo;
	      implantinfo.data[0] = j;
	      implantinfo.data[1] = k;
	      implantlist.push_back(implantinfo);
// /*
              double time_between_implantation_and_decay = decay_event.dssd_sub_event.GetTStripHighAvg() -
		dssd_implantation_pixels[j][k].dssd_sub_event.GetTStripLowAvg();
//          std::cout << "implant t = " << dssd_implantation_pixels[j][k].dssd_sub_event.GetTStripLowAvg() << " decay at t + " << time_between_implantation_and_decay << std::endl;
	      double arbitrary_long_time = 1000000000000;
	      if (time_between_implantation_and_decay < arbitrary_long_time){
		arbitrary_long_time = time_between_implantation_and_decay;
		front = j; // j should be the loop over the front side
		back = k; // k should be the loop over the back side
	      }
//	      */
	    }
	  }
	}
      }

      for (int l = 0; l < implantlist.size(); l++)
      {
         PossImplants coords;
         coords = implantlist.at(l);
         front = coords.data[0];
         back = coords.data[1];

         if (dssd_implantation_pixels[front][back].IsImplanted() == true){
           
           // Check for back to back implantations
           // Make sure a sufficienct time has passed between back to back implantations
           // Otherwise, do not allow for correlations
           double back_to_back_implantation_acceptance_time = std::stod(argv[6]) * 1E9; // SPECIFIC to e12001. Since the DSSD time is converted to nanoseconds in the calibrate function, make this in units of nanoseconds
//           std::cout << "b2b acceptance time: " << back_to_back_implantation_acceptance_time << std::endl;

           if (dssd_implantation_pixels[front][back].GetTimeBetweenBackToBackImplantations() > back_to_back_implantation_acceptance_time || dssd_implantation_pixels[front][back].GetTimeBetweenBackToBackImplantations() < 0){
//           if (true) {         } 
             // Correlations must occur within the correlation time window
             double correlation_time_window = std::stod(argv[5]) * 1E9; // SPECIFIC to e12001. Since the DSSD time is converted to nanoseconds in the calibrate function, make this in units of nanoseconds
             
             double time_between_implantation_and_decay = decay_event.dssd_sub_event.GetTStripHighAvg() -
               dssd_implantation_pixels[front][back].dssd_sub_event.GetTStripLowAvg();
             
             if (time_between_implantation_and_decay < correlation_time_window){
             
               correlation_flags->Fill(28); // Decay event successfully correlated
               //std::cout << "Flag = " << 28 << std::endl;
             
               // If only allowing a single decay event to be correlated to an implantation event, remove the implantation information
          
               // Fill output ROOT tree
               //std::cout << "Starting to fill ROOT tree" << std::endl;
               // TACs
               //std::cout << "Starting to write at " << double (clock()-startTime) << std::endl;
               output_E_TAC_I2S_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_I2S_I2N_Raw_Energy();
               output_E_TAC_I2S_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_I2S_I2N_Cal_Energy();
               
               output_E_TAC_PIN1_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2N_Raw_Energy();
               output_E_TAC_PIN1_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2N_Cal_Energy();;
               
               output_E_TAC_PIN2_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2N_Raw_Energy();
               output_E_TAC_PIN2_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2N_Cal_Energy();
               
               // PINs
               output_E_PIN1_raw = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN1RawEnergy();
               output_E_PIN1_cal = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN1CalEnergy();
               
               output_E_PIN2_raw = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN2RawEnergy();
               output_E_PIN2_cal = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN2CalEnergy();
               
               output_E_PIN3_raw = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN3RawEnergy();
               output_E_PIN3_cal = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN3CalEnergy();
               
               
               output_E_SCINT_iraw = dssd_implantation_pixels[front][back].scint_sub_event.GetScintRawEnergy();
               output_E_SCINT_ical = dssd_implantation_pixels[front][back].scint_sub_event.GetScintCalEnergy();
               
               output_E_SCINT_draw = decay_event.scint_sub_event.GetScintRawEnergy();
               output_E_SCINT_dcal = decay_event.scint_sub_event.GetScintCalEnergy();
               
               // DSSD
               
               // Low gain
               
               for (int a=0; a<2; a++){ // Event loop is over "i"
         	for (int b=0; b<40; b++){
         	  
         	  // Low gain (that came with an implantation)
         	  
         	  output_E_DSSD_low_raw[a][b] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowRawEnergy(a,b);
         	  output_E_DSSD_low_cal[a][b] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowCalEnergy(a,b);
         	  
         	  output_t_DSSD_low_raw[a][b] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowRawTime(a,b);
         	  output_t_DSSD_low_cal[a][b] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowCalTime(a,b);
         	  
         	  // High gain (that came with a decay)
         
         	  output_E_DSSD_high_raw[a][b] = decay_event.dssd_sub_event.GetHighRawEnergy(a,b);
         	  output_E_DSSD_high_cal[a][b] = decay_event.dssd_sub_event.GetHighCalEnergy(a,b);
         	  
         	  output_t_DSSD_high_raw[a][b] = decay_event.dssd_sub_event.GetHighRawTime(a,b);
         	  output_t_DSSD_high_cal[a][b] = decay_event.dssd_sub_event.GetHighCalTime(a,b);
         	  //std::cout << output_t_DSSD_high_cal[a][b] << std::endl;
         	  
         	}
         	
         	// Low gain (that came with an implantation)
         	
         	output_m_DSSD_low_raw[a] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowRawMultiplicity(a);
         	output_m_DSSD_low_cal[a] = dssd_implantation_pixels[front][back].dssd_sub_event.GetLowCalMultiplicity(a);
         	
         	// High gain (that came with a decay)
         	
         	output_m_DSSD_high_raw[a] = decay_event.dssd_sub_event.GetHighRawMultiplicity(a);
         	output_m_DSSD_high_cal[a] = decay_event.dssd_sub_event.GetHighCalMultiplicity(a);
         	
               }
               
               // Low gain (that came with an implantation)
               
               cStripLowFront = dssd_implantation_pixels[front][back].dssd_sub_event.GetCStripLowFront();
               cStripLowBack  = dssd_implantation_pixels[front][back].dssd_sub_event.GetCStripLowBack(); 
               
               eStripLowFront = dssd_implantation_pixels[front][back].dssd_sub_event.GetEStripLowFront();
               eStripLowBack  = dssd_implantation_pixels[front][back].dssd_sub_event.GetEStripLowBack();
               
               tStripLowFront = dssd_implantation_pixels[front][back].dssd_sub_event.GetTStripLowFront();
               tStripLowBack  = dssd_implantation_pixels[front][back].dssd_sub_event.GetTStripLowBack();
               
               // High gain (that came with an decay)
               
               cStripHighFront = decay_event.dssd_sub_event.GetCStripHighFront();
               cStripHighBack  = decay_event.dssd_sub_event.GetCStripHighBack();
               
               eStripHighFront = decay_event.dssd_sub_event.GetEStripHighFront();
               eStripHighBack  = decay_event.dssd_sub_event.GetEStripHighBack();
               
               tStripHighFront = decay_event.dssd_sub_event.GetTStripHighFront();
               tStripHighBack  = decay_event.dssd_sub_event.GetTStripHighBack();
               
               //NERO
               for (int q = 0; q<4; q++){
         	for (int d = 0; d<15; d++){
                   output_E_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalEnergy(q,d);
         	  //if (output_E_NERO_raw[q][d] > 0){std::cout << "A neutron! " << decay_event.nero_sub_event.GetCalEnergy(q,d) << std::endl;}
                   output_t_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalTime(q,d);
                   if (output_t_NERO_raw[q][d] > 0){
                     output_tdiff_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalTime(q,d) - decay_event.dssd_sub_event.GetTStripHighFront();  
                   }
                   else{
                     output_tdiff_NERO_raw[q][d] = -3000;
                   }
                   //if(output_E_NERO_raw[q][d] > 1){std::cout << output_E_NERO_raw[q][d] << std::endl;}
                   //std::cout << "time of decay is " << decay_event.dssd_sub_event.GetTStripHighFront() << " and time of n is " << decay_event.nero_sub_event.GetRawTime(q,d) << " and the tdiff is " << output_tdiff_NERO_raw[q][d] << std::endl;
         	}
               }

               
               // Correlations
               
               back_to_back_implant_time = dssd_implantation_pixels[front][back].GetTimeBetweenBackToBackImplantations();
               correlation_implant_decay_time = time_between_implantation_and_decay;
               
               // Fill output tree
               
               tOut->Fill();
               //std::cout << "Finished writing at " << double (clock()-startTime) << std::endl;
               
             }
             
             else {
               correlation_flags->Fill(24); // Decay event correlated spatially, but not temporally (exceeded correlation time window)
//             std::cout << "Flag = " << 24 << ", decayTime = " << time_between_implantation_and_decay << std::endl;
             }
             
           }

           else {
             correlation_flags->Fill(20); // Time between implantations is too short
           //  std::cout << "Flag = " << 20 << std::endl;
           }

         }
         
       
      }//std::cout << "correlation flag" << std::endl;
    }
    
  
  }
  fOut->cd();
  correlation_flags->Write();
  tOut->Write();
  tOut->ResetBranchAddresses();
  fOut->Close();
  
  std::cout << overflows << std::endl;
  std::cout << overflow_events << std::endl;
  std::cout << implants << std::endl;
  std::cout << double (clock()-startTime) << std::endl;
  return 0;
}
 
