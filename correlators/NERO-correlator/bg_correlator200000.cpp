// Notes:
// 1) Initialize ALL private variables either in the constructor initializer list, or in the body of the constructor
// 2) When using diagnostics in the event loop, print the object before any calibrations are performed to make sure that all values are initialized to zero (see note above)
// 3) When using diagnostics in the event loop, see that changes have been made after applying any calibrations
// 4) In the diagnostic function, the private variables added to the output stream with two different ways (the private variable and the Get method).
//    This is an extra precaution to make sure they are the same because they should be (and to also make sure I did not make a mistake with the Get method).
// 5) For memcpy, the following must be input correctly: (1) "sizeof" (2) the array dimensions
// 6) Pay attention to whether thresholds are applied to raw or calibrated values
// 7) Without applying any gates, always make sure the branch that has the correlation times is positive and the times are never larger than the correlation time window
// 8) The DSSD is initialised as a 2-D vector of size 40x40. Each 'pixel' contains a vector of Implantation objects.
//    When the event loop encounters an implant, it adds it to the pixel at the end of the vector. 
//    When the event loop encounters a decay, it looks in the correlation field for implants. For each pixel containing at least one implant, the time difference between the decay and implant is calculated, starting from the front of the vector. If the difference is larger than the correlation time window, the implant is popped from the front of the vector and the pointer advances to the next implant in the vector. It then loops through all the implants in each vector, and all the vectors in the correlation field.


// To run:
// g++ correlator.cpp $(root-config --cflags) $(root-config --glibs) -std=c++11 -O3 -o correlator
// ./correlator command_line_arguments

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TRandom3.h"
#include "bg_correlator.h"
#include <iostream>
#include <iomanip>
#include <cstring> // memcpy
#include <deque>
#include <vector>
#include "assert.h"
#include "./cuts.h"


using namespace std;

void save_to_png( TCanvas *c1, std::string name ){

    TImage *img = TImage::Create();
    img->FromPad(c1);
    img->WriteImage(Form("/user/hermanse/analysis/pngs/%s.png",name.c_str()));
    return;
}

void pop_front(vector<vector<vector<Implantation>>>& vec)
{
   assert(!vec.empty());
   vec.erase(vec.begin());
}

int momentum_correction(int i2pos, int raw_tof){
    return i2pos*0.8 + 1500 + raw_tof;
}

int check_inputs(char* argv[]){
    std::cout << ":" << argv[2] << ":" << std::endl;

  if ((string(argv[2]) != "true") &&
      (string(argv[2]) != "rand")){
    std::cout << "Input error" << std::endl;
    std::cout << "Use true for true correlations or "
	      << "rand for random correlations" << std::endl;
    return -1;
  }
  
  if ((string(argv[4]) != "sd") &&
      (string(argv[4]) != "md")){
    std::cout << "Input error" << std::endl;
    std::cout << "Use sd for single decay mode or "
	      << "md for multiple decay mode" << std::endl;
    return -1;
  }

return 0;  
} 

int main(int argc, char* argv[]){
  cout << "Does this even work?" << endl;

  if (check_inputs(argv ) <0)
    return -1;

  int random_number_generator_seed = 47093625;
  TRandom3* rand_num_gen = new TRandom3(random_number_generator_seed);
  double maxtime = 1.E13;
  
    // input to define whether to accept implants and decays with > 1 max strip (overflows)
  bool implants2 = false;  
  bool decays2 = false;  
  if ((std::string(argv[9]) == "imp2" )){
    cout << "implants 2 true " << endl;
    implants2 = true;
  }
  if ((std::string(argv[10]) == "dec2" )){
    cout << "decays 2 true " << endl;
    decays2 = true;
  }
  int scint_thresh = std::stoi(argv[8]);  

  cout << "Making outputfile" << endl;
  // ===== Output ROOT file
  TFile* fOut = new TFile(Form("/mnt/analysis/e17028/correlated_rootfiles_NERO/%.0fms/%s/Run%s_%s_%d_%s_%.0f_%s_%d_200000.root",
			       stod(argv[5])*1000,     //time window
			       argv[2],                     //true/bg
			       argv[1],                     //run number
			       argv[2],                     //true/bg
			       std::stoi(argv[3]),          //pixel size []x[]
			       argv[4],                     //sd or md
			       std::stod(argv[6])*1000,          //back to back acceptance time 
			       argv[7],                     //isotope
                   scint_thresh),                //scintillator threshold
//			       argv[9],                     //imp2 or imp1 (acceptance of any or only one overflow in implants)
//			       argv[10]),                   //dec2 or dec1 (acceptance of any or only one overflow in decays)
			  "RECREATE");
  cout << "Made outputfile" << endl;

  // Main output tree
  TTree* tOut = new TTree("t","output after thresholds, calibrations, and correlations");

  // implant event output tree
  TTree* tImplants = new TTree("implants","output of just the implanted events");

// Declare all variables and branch addresses

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

  // SCINT

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

  double ioutput_E_DSSD_low_raw[2][40] = {{0}};
  double ioutput_E_DSSD_low_cal[2][40] = {{0}};

  tOut->Branch("E_DSSD_low_raw", &output_E_DSSD_low_raw, "E_DSSD_low_raw[2][40]/D");
  tOut->Branch("E_DSSD_low_cal", &output_E_DSSD_low_cal, "E_DSSD_low_cal[2][40]/D");
  tImplants->Branch("iE_DSSD_low_raw", &ioutput_E_DSSD_low_raw, "iE_DSSD_low_raw[2][40]/D");
  tImplants->Branch("iE_DSSD_low_cal", &ioutput_E_DSSD_low_cal, "iE_DSSD_low_cal[2][40]/D");

  double output_t_DSSD_low_raw[2][40] = {{0}};
  double output_t_DSSD_low_cal[2][40] = {{0}};

  double ioutput_t_DSSD_low_raw[2][40] = {{0}};
  double ioutput_t_DSSD_low_cal[2][40] = {{0}};

  tOut->Branch("t_DSSD_low_raw", &output_t_DSSD_low_raw, "t_DSSD_low_raw[2][40]/D");
  tOut->Branch("t_DSSD_low_cal", &output_t_DSSD_low_cal, "t_DSSD_low_cal[2][40]/D");
  tImplants->Branch("it_DSSD_low_raw", &ioutput_t_DSSD_low_raw, "it_DSSD_low_raw[2][40]/D");
  tImplants->Branch("it_DSSD_low_cal", &ioutput_t_DSSD_low_cal, "it_DSSD_low_cal[2][40]/D");

  int output_m_DSSD_low_raw[2] = {0};
  int output_m_DSSD_low_cal[2] = {0};

  int ioutput_m_DSSD_low_raw[2] = {0};
  int ioutput_m_DSSD_low_cal[2] = {0};

  tOut->Branch("m_DSSD_low_raw", &output_m_DSSD_low_raw, "m_DSSD_low_raw[2]/I");
  tOut->Branch("m_DSSD_low_cal", &output_m_DSSD_low_cal, "m_DSSD_low_cal[2]/I");
  tImplants->Branch("im_DSSD_low_raw", &ioutput_m_DSSD_low_raw, "im_DSSD_low_raw[2]/I");
  tImplants->Branch("im_DSSD_low_cal", &ioutput_m_DSSD_low_cal, "im_DSSD_low_cal[2]/I");

  int cStripLowFront = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD
  int cStripLowBack  = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD

  int icStripLowFront = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD
  int icStripLowBack  = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD

  tOut->Branch("cStripLowFront", &cStripLowFront, "cStripLowFront/I");
  tOut->Branch("cStripLowBack",  &cStripLowBack,  "cStripLowBack/I");
  tImplants->Branch("icStripLowFront", &icStripLowFront, "icStripLowFront/I");
  tImplants->Branch("icStripLowBack",  &icStripLowBack,  "icStripLowBack/I");

  double eStripLowFront = 0;
  double eStripLowBack  = 0;

  double ieStripLowFront = 0;
  double ieStripLowBack  = 0;

  tOut->Branch("eStripLowFront", &eStripLowFront, "eStripLowFront/D");
  tOut->Branch("eStripLowBack",  &eStripLowBack,  "eStripLowBack/D");

  tOut->Branch("ieStripLowFront", &ieStripLowFront, "ieStripLowFront/D");
  tOut->Branch("ieStripLowBack",  &ieStripLowBack,  "ieStripLowBack/D");

  double tStripLowFront = 0;
  double tStripLowBack  = 0;

  double itStripLowFront = 0;
  double itStripLowBack  = 0;

  tOut->Branch("tStripLowFront", &tStripLowFront, "tStripLowFront/D");
  tOut->Branch("tStripLowBack",  &tStripLowBack,  "tStripLowBack/D");

  tOut->Branch("itStripLowFront", &itStripLowFront, "itStripLowFront/D");
  tOut->Branch("itStripLowBack",  &itStripLowBack,  "itStripLowBack/D");

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

  // BCS SSSD

  // High gain

  double output_E_SSSD_high_raw[2][16] = {{0}};
  double output_E_SSSD_high_cal[2][16] = {{0}};

  tOut->Branch("E_SSSD_high_raw", &output_E_SSSD_high_raw, "E_SSSD_high_raw[2][16]/D");
  tOut->Branch("E_SSSD_high_cal", &output_E_SSSD_high_cal, "E_SSSD_high_cal[2][16]/D");

  double ioutput_E_SSSD_high_raw[2][16] = {{0}};
  double ioutput_E_SSSD_high_cal[2][16] = {{0}};

  tImplants->Branch("iE_SSSD_high_raw", &ioutput_E_SSSD_high_raw, "iE_SSSD_high_raw[2][16]/D");
  tImplants->Branch("iE_SSSD_high_cal", &ioutput_E_SSSD_high_cal, "iE_SSSD_high_cal[2][16]/D");

  // NERO
  
  int output_E_NERO_raw[4][15] = {{0}};
  double output_t_NERO_raw[4][15] = {{0}};
  double output_tdiff_NERO_raw[4][15] = {{0}};
	
  tOut->Branch("E_NERO_raw", &output_E_NERO_raw, "E_NERO_raw[4][15]/I");
  tOut->Branch("t_NERO_raw", &output_t_NERO_raw, "t_NERO_raw[4][15]/D");
  tOut->Branch("td_NERO_raw", &output_tdiff_NERO_raw, "td_NERO_raw[4][15]/D");

  // Correlations

  double correlation_implant_decay_time = 0;
  double correlation_implant_decay_time_s = 0;
  double back_to_back_implant_time = 0;

  tOut->Branch("correlation_implant_decay_time", &correlation_implant_decay_time, "correlation_implant_decay_time/D");
  tOut->Branch("correlation_implant_decay_time_s", &correlation_implant_decay_time_s, "correlation_implant_decay_time_s/D");
  tOut->Branch("back_to_back_implant_time",      &back_to_back_implant_time,      "back_to_back_implant_time/D");

  TH1F* correlation_flags = new TH1F("correlation_flags","",50,0,50);
  // 2  - PID event  
  // 4  - Implantation event
  // 8  - Back to back implantation events (if running multiple decay mode, this should equal the number of counts in flag 4 minus 256)
  // 12 - Implantation without back to back implantations
  // 16 - Decay event
  // 20 - Time between implantations is too short
  // 24 - Decay event correlated spatially, but not temporally (exceeded correlation time window)
  // 28 - Decay event successfully correlated

  // ===== Input ROOT file(s)

  TChain* Chain = new TChain("t");
  cout << "Made chain" << endl;
  Chain->Add(Form("/mnt/analysis/e17028/rootfiles_200000/run%s*.root", argv[1]));
  cout << "Added stuff" << endl;
  
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
  cout << "Added TACS" << endl;
  
  // PINs
  int E_PIN1_raw = 0;
  int E_PIN2_raw = 0;
  int E_PIN3_raw = 0;

  Chain->SetBranchAddress("energy_PIN1", &E_PIN1_raw);
  Chain->SetBranchAddress("energy_PIN2", &E_PIN2_raw);
  Chain->SetBranchAddress("energy_PIN3", &E_PIN3_raw);
  cout << "Added PINS" << endl;

  // SCINT
  int E_SCINT_raw = 0;
  Chain->SetBranchAddress("energy_Veto", &E_SCINT_raw);
  int t_SCINT_raw = 0;
  Chain->SetBranchAddress("time_Veto", &t_SCINT_raw);

  //cout << E_PIN1_raw << endl;

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

  cout << "Added DSSD" << endl;

  // SSSD
  int E_SSSD_high_raw[2][16] = {{0}};
  double t_SSSD_high_raw[2][16] = {{0}};
  Chain->SetBranchAddress("energy_BCSSSSD_high", &E_SSSD_high_raw);
  Chain->SetBranchAddress("time_BCSSSSD_high", &t_SSSD_high_raw);
  cout << "Added SSSD" << endl;

  // NERO
  int E_NERO_raw[4][15] = {{0}};
  double t_NERO_raw[4][15] = {{0}};
  Chain->SetBranchAddress("energy_NERO", &E_NERO_raw);
  Chain->SetBranchAddress("time_NERO", &t_NERO_raw);

  // The DSSD
  vector<vector<vector<Implantation> > > dssd_implantation_pixels(40,vector<vector<Implantation> > (40, vector<Implantation>(0)));
//  vector<vector<Implantation> > dssd_implantation_pixels(40,vector<Implantation> (40));
//  Implantation dssd_implantation_pixels[40][40];

  cout << "Added DSSD pixel" << endl;
  
  // Make cuts
  cout << "I made the cuts" << endl;
  std::string isotope = argv[7];
  std::map<std::string, TCutG*> cutDict = cuts();
  TCutG* isotope_cut = cutDict[isotope];

  // Loop over events and correlate
  int counter = 10;
  cout << "Added counter" << endl;
  int num = Chain->GetEntries();
  clock_t startTime = clock();
  cout << num << endl;
  int implants = 0;
  int overflow_events = 0;
  int overflows = 0;
  bool first = true;
  for (int i=num; i>0; i--){
    Chain->GetEntry(i);
    //cout << " I am at entry " << i << endl;
    if (i % (int)(0.1*num) == 0){
      cerr << counter << " ";
      counter --;
    }
    if (i==num){
        std::cout<<Chain->GetMaximum("tmax")<<std::endl;
        maxtime = Chain->GetMaximum("tmax");
    }
   
    // TAC diagnostics
    /*cout << "i: " << i << endl;
    cout << endl;
    TAC tac_sub_event(E_TAC_I2S_I2N_raw, E_TAC_PIN1_I2N_raw, E_TAC_PIN2_I2N_raw);
    cout << tac_sub_event << endl;
    tac_sub_event.Calibrate();
    cout << tac_sub_event << endl;*/

    // PIN diagnostics
    /*cout << "i: " << i << endl;
    cout << endl;
    PIN pin_sub_event(E_PIN1_raw, E_PIN2_raw);
    cout << pin_sub_event << endl;
    pin_sub_event.Calibrate();
    cout << pin_sub_event << endl;*/

    // DSSD diagnostics
    /*cout << "i: " << i << endl;
    DSSD dssd_sub_event(E_DSSD_low_raw,
			t_DSSD_low_raw,
			m_DSSD_low_raw,
			E_DSSD_high_raw,
			t_DSSD_high_raw,
			m_DSSD_high_raw);
    cout << dssd_sub_event << endl;
    dssd_sub_event.Calibrate();
    cout << dssd_sub_event << endl;*/

    // SuN diagnostics
    /*cout << "i: " << i << endl;
    SuN sun_sub_event(E_SuN_raw);
    cout << sun_sub_event << endl;
    sun_sub_event.GainMatchAndCalibrate();
    cout << sun_sub_event << endl;*/
    
    // Veto diagnostics
    /*cout << "i: " << i << endl;
    Veto veto_sub_event(E_Veto_raw);
    cout << veto_sub_event << endl;*/

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
    SSSD sssd_sub_event(E_SSSD_high_raw, t_SSSD_high_raw);
    NERO nero_sub_event(E_NERO_raw, t_NERO_raw);
    SCINT scint_sub_event(E_SCINT_raw, t_SCINT_raw);
    //Punchthrough, set to true or false
    Punchthrough punchthrough_event(pin_sub_event,
				   sssd_sub_event,
				   dssd_sub_event,
				   scint_sub_event);

    punchthrough_event.pin_sub_event.Calibrate(); 
    punchthrough_event.sssd_sub_event.Calibrate(maxtime); 
    punchthrough_event.dssd_sub_event.Calibrate(maxtime); 
    punchthrough_event.scint_sub_event.Calibrate(scint_thresh,maxtime); 

    //Implement cuts
    int e_PIN1_raw = pin_sub_event.GetPIN1RawEnergy();
    int e_PIN2_raw = pin_sub_event.GetPIN2RawEnergy();
    int e_TAC_PIN2_I2S_raw = tac_sub_event.Get_PIN2_I2S_Raw_Energy();
    int corr_tof = momentum_correction( tac_sub_event.Get_I2S_I2N_Raw_Energy(), e_TAC_PIN2_I2S_raw );
    if ((isotope_cut->IsInside((corr_tof),(e_PIN2_raw)) && (e_PIN1_raw > 0) && cutDict["pin12"]->IsInside((e_PIN2_raw),(e_PIN1_raw)) && cutDict["i2pos"]->IsInside((E_TAC_I2S_I2N_raw),(E_PIN2_raw)))) {
            correlation_flags->Fill(2);

       // Implantation event
       Implantation implantation_event(tac_sub_event,
           			    pin_sub_event,
           			    dssd_sub_event,
           			    sssd_sub_event,
                        scint_sub_event);
       if (implantation_event.IsValidImplantation(implants2) || implantation_event.IsImplanted()){
         cout << "Error for implantations" << endl;}

       // Calibrate the detectors
       implantation_event.tac_sub_event.Calibrate();
       implantation_event.pin_sub_event.Calibrate();
       implantation_event.sssd_sub_event.Calibrate(maxtime);

       int temp_overflows = implantation_event.dssd_sub_event.Calibrate(maxtime); // temp_overflows is the number of non sequential overflow strips in the low gain
       if (temp_overflows > 2){
//         std::cout << temp_overflows << std::endl;
         overflow_events++;
         overflows+=temp_overflows;
       }
       implantation_event.scint_sub_event.Calibrate(scint_thresh,maxtime);
       
       if (implantation_event.IsValidImplantation(implants2) && temp_overflows == 0){ // temp_overflows==0 removes non sequential overflow events in the low gain strips 
              Double_t x[40];
              Double_t y[16];
               for (int a=0; a<2; a++){ // Event loop is over "i"
         	for (int b=0; b<40; b++){

              x[b] = b;
         	  ioutput_E_DSSD_low_raw[a][b] = implantation_event.dssd_sub_event.GetLowRawEnergy(a,b);
         	  ioutput_E_DSSD_low_cal[a][b] = implantation_event.dssd_sub_event.GetLowCalEnergy(a,b);
         	  
         	  ioutput_t_DSSD_low_raw[a][b] = implantation_event.dssd_sub_event.GetLowRawTime(a,b);
         	  ioutput_t_DSSD_low_cal[a][b] = implantation_event.dssd_sub_event.GetLowCalTime(a,b);
         	  
         	  ioutput_m_DSSD_low_raw[a] = implantation_event.dssd_sub_event.GetLowRawMultiplicity(a);
         	  ioutput_m_DSSD_low_cal[a] = implantation_event.dssd_sub_event.GetLowCalMultiplicity(a);
              
            } // end for strip b
                } // end for side a 
            if (ioutput_m_DSSD_low_cal[0] > 1 || ioutput_m_DSSD_low_cal[1] > 1)
            {
//              TCanvas *c1 = new TCanvas();              
//              TGraph* front_lo = new TGraph(40,x,ioutput_E_DSSD_low_cal[1]);
//              front_lo->Draw();
//              TLine l(implantation_event.dssd_sub_event.GetCStripLowFront(),1,implantation_event.dssd_sub_event.GetCStripLowFront(),32767);
//              l.SetLineColor(kRed);
//              l.Draw("same");
//              save_to_png(c1, Form("lo/%d_front",i));
//              TGraph* back_lo = new TGraph(40,x,ioutput_E_DSSD_low_cal[0]);
//              c1->Clear();
//              back_lo->Draw();
//              TLine l2(implantation_event.dssd_sub_event.GetCStripLowBack(),1,implantation_event.dssd_sub_event.GetCStripLowBack(),32767);
//              l2.SetLineColor(kRed);
//              l2.Draw("same");
//              save_to_png(c1, Form("lo/%d_back",i));
//              first = false;
            } //end if m_low_cal > 1
         	
              icStripLowFront = implantation_event.dssd_sub_event.GetCStripLowFront();
              icStripLowBack  = implantation_event.dssd_sub_event.GetCStripLowBack(); 
               
              ieStripLowFront = implantation_event.dssd_sub_event.GetEStripLowFront();
              ieStripLowBack  = implantation_event.dssd_sub_event.GetEStripLowBack();
              
              itStripLowFront = implantation_event.dssd_sub_event.GetTStripLowFront();
              itStripLowBack  = implantation_event.dssd_sub_event.GetTStripLowBack();

               // BCS SSSD
               
         	for (int b = 0; b<16;b++){
             y[b] = b;
         	 ioutput_E_SSSD_high_cal[1][b] = implantation_event.sssd_sub_event.GetCalEnergy(b);
         	 //Fill what needs to be done for SSSD here
         	} // end SSSD
/*  checks for sssd signals
            {
              TCanvas *c2 = new TCanvas();              
              c2->Clear();
              TGraph* sssd = new TGraph(16,y,ioutput_E_SSSD_high_cal[1]);
              sssd->Draw();
              save_to_png(c2, Form("sssd/%d",i));
            }
*/
              tImplants->Fill();
         implants++;
         correlation_flags->Fill(4); // Implantation event
         
         
         // First check for back to back implantations (if there is an existing implant in this pixel)
         
         if (dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].empty() == false){

           correlation_flags->Fill(8); // Back to back implantation events

           // Calculate time between back to back implantations
           double time_between_back_to_back_implantations = implantation_event.dssd_sub_event.GetTStripLowAvg() -
             dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].back().dssd_sub_event.GetTStripLowAvg();
           (cout << "current evt time: " << implantation_event.dssd_sub_event.GetTStripLowAvg() << "\nold evt time: " << 
             dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].back().dssd_sub_event.GetTStripLowAvg()
             << "\ndifference: " << time_between_back_to_back_implantations << endl);
           implantation_event.SetTimeBetweenBackToBackImplantations(time_between_back_to_back_implantations);

           // Accept the latest implant
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].push_back(implantation_event);

           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].back().SetIsImplanted(true); // In line above, implantation_event is initialized to false

         } // end if back to back
         else {
           correlation_flags->Fill(12); // Implantation without back to back implantations
           implantation_event.SetTimeBetweenBackToBackImplantations(1000000000);
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].push_back(implantation_event);

           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].back().SetIsImplanted(true);

         } // end if (no back to back)
       } // end if valid implantation
    } // end if in pid
    // Decay event
    Decay decay_event(pin_sub_event,
		      dssd_sub_event,
		      nero_sub_event,
		      scint_sub_event);
    if (decay_event.IsValidDecay(decays2)){cout << "Error for decays" << endl;}

    int dummy_overflow = 0;
    // Calibrate the detectors
    decay_event.pin_sub_event.Calibrate();
    dummy_overflow = decay_event.dssd_sub_event.Calibrate(maxtime); // overflows are only relevent for low gain
    decay_event.nero_sub_event.Calibrate(maxtime);
    decay_event.scint_sub_event.Calibrate(scint_thresh,maxtime);
    
    if (decay_event.IsValidDecay(decays2)){
      correlation_flags->Fill(16); // Decay event

      int front = 500;
      int back = 500;

      int correlation_field = stoi(argv[3]);
      int correlation_limit = (correlation_field-1)/2; //3 for 3x3 pixel, 1 or 2 for 1x1 pixel

      int front_lower_limit = decay_event.dssd_sub_event.GetCStripHighFront() - correlation_limit;
      int front_upper_limit = decay_event.dssd_sub_event.GetCStripHighFront() + correlation_limit;
      int back_lower_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  - correlation_limit;
      int back_upper_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  + correlation_limit;
     
      //create deque of all possible implanted sub-pixels
      deque<PossImplants> implantlist;
      for (int j = front_lower_limit; j <= front_upper_limit; j++){ // event loop is over "i" 
	for (int k = back_lower_limit; k <= back_upper_limit; k++){
	  if ((j >= 0) && (j <= 39) && (k >= 0) && (k<=39)){
	    if (dssd_implantation_pixels[j][k].empty()!= true){
            if (dssd_implantation_pixels[j][k].back().IsImplanted() == true ){ 
                PossImplants implantinfo;
	            implantinfo.data[0] = j;
	            implantinfo.data[1] = k;
	            implantlist.push_back(implantinfo);
	    //if (time_between_implantation_and_decay < arbitrary_long_time){
		//    arbitrary_long_time = time_between_implantation_and_decay;
	    //    double arbitrary_long_time = 1000000000000;
		        front = j; // j should be the loop over the front side
		        back = k; // k should be the loop over the back side
            } // end if is implanted

	      //}
	    } // end if pixel is not empty
	  } // end if pixel is real
	} // end for k within back window
      } // end for j within front window

      for (int l = 0; l < implantlist.size(); l++){
         PossImplants coords;
         coords = implantlist.at(l);
         front = coords.data[0];
         back = coords.data[1];
         //cout << "I am looking at a new implanted sub-pixel at " <<front <<" " << back << endl;
        if ((front >= 0) && (front <= 39) && (back >= 0) && (front <= 39)){
      ///////////////////////////////////////////////////////////////////
      // Random correlations
      // Modify the implantation pixel that was in the correlation field to be a random pixel
    /*
	if (string(argv[2]) == "rand"){
	  int temp_front = front;
	  int temp_back  = back;
	  
	  while (temp_front == front){ // Remove possibility of choosing the same pixel as the potential true correlation
	    front = rand_num_gen->Integer(39); // Returns random integer from 0 to 39 
	  }
	  while (temp_back == back){ // Remove possibility of choosing the same pixel as the potential true correlation
	    back = rand_num_gen->Integer(39); // Returns random integer from 0 to 39 
	  }
	} // end if rand 
    */
      ///////////////////////////////////////////////////////////////////
       
         if (dssd_implantation_pixels[front][back].empty() == false && dssd_implantation_pixels[front][back].back().IsImplanted() == true){
           //cout << "Here is a implanted pixel" << endl;
           // Check for back to back implantations
           double back_to_back_implantation_acceptance_time = stod(argv[6]) * 1E9; // in ns 

	   if (dssd_implantation_pixels[front][back].back().GetTimeBetweenBackToBackImplantations() >= back_to_back_implantation_acceptance_time){
          
             // Correlations must occur within the correlation time window
             double correlation_time_window = stod(argv[5]) * 1E9; //ns

             // Loop through vector of implants in sub-pixels here
             double dtime = decay_event.dssd_sub_event.GetTStripHighAvg();
             vector<Implantation>::iterator iter;
             int implant_count = 0;
//             std::cout << std::endl;
             for (iter = dssd_implantation_pixels[front][back].begin(); iter!=dssd_implantation_pixels[front][back].end();){
                if ((*iter).IsImplanted()==true){
               implant_count++;
//               std::cout << " " << implant_count;
               double itime = (*iter).dssd_sub_event.GetTStripLowAvg();
    	       double time_between_implantation_and_decay = dtime - itime;
               //cout << dtime << " " << itime << endl; 
               if (time_between_implantation_and_decay >= correlation_time_window){
    
                 correlation_flags->Fill(24);
                 //cout << "I want to remove the first element" << endl;
                 dssd_implantation_pixels[front][back].erase(dssd_implantation_pixels[front][back].begin()); 
	         //cout << "I managed to remove the first element" << endl;
                 
               } // end if outside correlation time window
               else {
                 correlation_flags->Fill(28); // Decay event successfully correlated
               //cout << "Flag = " << 28 << endl;
             
               // If only allowing a single decay event to be correlated to an implantation event, remove the implantation information
                 if (string(argv[4]) == "sd"){ // sd = single decay
         	  dssd_implantation_pixels[front][back].back().SetIsImplanted(false);
                 } // end if sd

                 // Fill output ROOT tree
                 //cout << "Starting to fill ROOT tree" << endl;
                 // TACs
               output_E_TAC_I2S_I2N_raw = (*iter).tac_sub_event.Get_I2S_I2N_Raw_Energy();
               output_E_TAC_I2S_I2N_cal = (*iter).tac_sub_event.Get_I2S_I2N_Cal_Energy();
               
               output_E_TAC_PIN1_I2N_raw = (*iter).tac_sub_event.Get_PIN1_I2N_Raw_Energy();
               output_E_TAC_PIN1_I2N_cal = (*iter).tac_sub_event.Get_PIN1_I2N_Cal_Energy();;
               
               output_E_TAC_PIN2_I2N_raw = (*iter).tac_sub_event.Get_PIN2_I2N_Raw_Energy();
               output_E_TAC_PIN2_I2N_cal = (*iter).tac_sub_event.Get_PIN2_I2N_Cal_Energy();
               
               // PINs
               output_E_PIN1_raw = (*iter).pin_sub_event.GetPIN1RawEnergy();
               output_E_PIN1_cal = (*iter).pin_sub_event.GetPIN1CalEnergy();

               output_E_PIN2_raw = (*iter).pin_sub_event.GetPIN2RawEnergy();
               output_E_PIN2_cal = (*iter).pin_sub_event.GetPIN2CalEnergy();

               output_E_PIN3_raw = (*iter).pin_sub_event.GetPIN3RawEnergy();
               output_E_PIN3_cal = (*iter).pin_sub_event.GetPIN3CalEnergy();

               output_E_SCINT_iraw = (*iter).scint_sub_event.GetScintRawEnergy();
               output_E_SCINT_ical = (*iter).scint_sub_event.GetScintCalEnergy();
               
               output_E_SCINT_draw = decay_event.scint_sub_event.GetScintRawEnergy();
               output_E_SCINT_dcal = decay_event.scint_sub_event.GetScintCalEnergy();
               
               
               // DSSD
               
               // Low gain
               
              Double_t x[40];
               for (int a=0; a<2; a++){ // Event loop is over "i"
         	for (int b=0; b<40; b++){
                x[b] = b;         	  
         	  // Low gain (that came with an implantation)
         	  
         	  output_E_DSSD_low_raw[a][b] = (*iter).dssd_sub_event.GetLowRawEnergy(a,b);
         	  output_E_DSSD_low_cal[a][b] = (*iter).dssd_sub_event.GetLowCalEnergy(a,b);
         	  
         	  output_t_DSSD_low_raw[a][b] = (*iter).dssd_sub_event.GetLowRawTime(a,b);
         	  output_t_DSSD_low_cal[a][b] = (*iter).dssd_sub_event.GetLowCalTime(a,b);
         	  
         	  // High gain (that came with a decay)
         
         	  output_E_DSSD_high_raw[a][b] = decay_event.dssd_sub_event.GetHighRawEnergy(a,b);
         	  output_E_DSSD_high_cal[a][b] = decay_event.dssd_sub_event.GetHighCalEnergy(a,b);
         	  
         	  output_t_DSSD_high_raw[a][b] = decay_event.dssd_sub_event.GetHighRawTime(a,b);
         	  output_t_DSSD_high_cal[a][b] = decay_event.dssd_sub_event.GetHighCalTime(a,b);
         	  
         	} // end for strip b
         	
         	  // Low gain (that came with an implantation)
         	
         	output_m_DSSD_low_raw[a] = (*iter).dssd_sub_event.GetLowRawMultiplicity(a);
         	output_m_DSSD_low_cal[a] = (*iter).dssd_sub_event.GetLowCalMultiplicity(a);
         	
         	// High gain (that came with a decay)
         	
         	output_m_DSSD_high_raw[a] = decay_event.dssd_sub_event.GetHighRawMultiplicity(a);
         	output_m_DSSD_high_cal[a] = decay_event.dssd_sub_event.GetHighCalMultiplicity(a);
         	
               } // end for side a
               
               // Low gain (that came with an implantation)
               
               cStripLowFront = (*iter).dssd_sub_event.GetCStripLowFront();
               cStripLowBack  = (*iter).dssd_sub_event.GetCStripLowBack(); 
               
               eStripLowFront = (*iter).dssd_sub_event.GetEStripLowFront();
               eStripLowBack  = (*iter).dssd_sub_event.GetEStripLowBack();
               
               tStripLowFront = (*iter).dssd_sub_event.GetTStripLowFront();
               tStripLowBack  = (*iter).dssd_sub_event.GetTStripLowBack();
               
               // High gain (that came with an decay)
               
               cStripHighFront = decay_event.dssd_sub_event.GetCStripHighFront();
               cStripHighBack  = decay_event.dssd_sub_event.GetCStripHighBack();
               
               eStripHighFront = decay_event.dssd_sub_event.GetEStripHighFront();
               eStripHighBack  = decay_event.dssd_sub_event.GetEStripHighBack();
               
               tStripHighFront = decay_event.dssd_sub_event.GetTStripHighFront();
               tStripHighBack  = decay_event.dssd_sub_event.GetTStripHighBack();
               
//            if (ioutput_m_DSSD_low_cal[0] > 1 || ioutput_m_DSSD_low_cal[1] > 1)
            {
//              TCanvas *c3 = new TCanvas();              
//              TGraph* front_hi = new TGraph(40,x,output_E_DSSD_high_cal[1]);
//              front_hi->Draw();
//              save_to_png(c3, Form("hi/%d_front",i));
//              TGraph* back_hi = new TGraph(40,x,output_E_DSSD_high_cal[0]);
//              c3->Clear();
//              back_hi->Draw();
//              save_to_png(c3, Form("hi/%d_back",i));
//              first = false;
            }
               // BCS SSSD
               
               /*for (int a = 0; a <2; a++){
         	for (int b = 0; b<16;b++){
         	 //output_E_SSSD_raw = dssd_implantation_pixels[front][back].sssd_sub_event.GetCalEnergy(b)
         	 //Fill what needs to be done for SSSD here
         	}
               }*/
	       
               //NERO
               for (int q = 0; q<4; q++){
         	for (int d = 0; d<15; d++){
                   output_E_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalEnergy(q,d);
         	  //if (output_E_NERO_raw[q][d] > 0){cout << "A neutron! " << decay_event.nero_sub_event.GetCalEnergy(q,d) << endl;}
                   output_t_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalTime(q,d);
                   if (output_t_NERO_raw[q][d] > 0){
                     output_tdiff_NERO_raw[q][d] = decay_event.nero_sub_event.GetCalTime(q,d) - decay_event.dssd_sub_event.GetTStripHighFront();  
                   }
                   else{
                     output_tdiff_NERO_raw[q][d] = - 3000;
                   }
         	} // end for d 
               } // end for q

               // Correlations
               
               back_to_back_implant_time = (*iter).GetTimeBetweenBackToBackImplantations();
               correlation_implant_decay_time = time_between_implantation_and_decay;
               correlation_implant_decay_time_s = time_between_implantation_and_decay/1e6;
               
               // Fill output tree
               
               tOut->Fill();
               iter++;
             } // end decay else (successful decay)
             
            // else {
              // correlation_flags->Fill(24); // Decay event correlated spatially, but not temporally (exceeded correlation time window)
            // remove the implant from the list
               //cout << "I want to remove the first element" << endl;
               //dssd_implantation_pixels[front][back].erase(dssd_implantation_pixels[front][back].begin()); 
	       //cout << "I managed to remove the first element" << endl;
               
	     //}
                } // end if iter is implanted 
            } // end for iter in implantlist 
           } // end if back to back > check

           else {
               double correlation_time_window = stod(argv[5]) * 1E9; //ns
               double dtime = decay_event.dssd_sub_event.GetTStripHighAvg();
               vector<Implantation>::iterator iter;
               cout << "implant vector size = " << dssd_implantation_pixels[front][back].size() << endl;
               for (iter = dssd_implantation_pixels[front][back].begin(); iter!=dssd_implantation_pixels[front][back].end();){
                   double itime = (*iter).dssd_sub_event.GetTStripLowAvg();
    	           double time_between_implantation_and_decay = dtime - itime;
                   if (time_between_implantation_and_decay >= correlation_time_window){
    
                     correlation_flags->Fill(30); // 
                     //cout << "I want to remove the first element" << endl;
                     dssd_implantation_pixels[front][back].erase(dssd_implantation_pixels[front][back].begin()); 
	             //cout << "I managed to remove the first element" << endl;
                     
                   } // end if outside correlation time window
                   break;  // I think the erase line is causing this for loop to never end
               } // end for iter
             correlation_flags->Fill(20); // Time between implantations is too short
             cout << "Flag = " << 20 << "; front = " << front << "; back = " << back << "; b2btime = " << dssd_implantation_pixels[front][back].back().GetTimeBetweenBackToBackImplantations() << endl;
           } // end else 
         } // end if pixel implanted
       } // end if real pixel
      } // end for implant l in implantlist
    } // end if valid decay
    
  
  } // end for event i in GetEntries
  fOut->cd();
  correlation_flags->Write();
  tOut->Write();
  tOut->ResetBranchAddresses();
  tImplants->Write();
  tImplants->ResetBranchAddresses();
  fOut->Close();
  
  std::cout << overflows << std::endl;
  std::cout << overflow_events << std::endl;
  std::cout << implants << std::endl;
  std::cout << double (clock()-startTime) << std::endl;
  return 0;
  
}
 
