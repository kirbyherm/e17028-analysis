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
#include "TCanvas.h"
#include "TImage.h"

#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring> // std::memcpy
#include "TCutG.h"

// For thresholds (for sure the DSSD class, but potentially more, I should mention whether the threshold is applied to the raw values or the calibrated values

// specifics to e12001: converting clock ticks to nanoseconds in the calibrate function for DSSD, and possibly the requirement that the I2 scintillator be present for an implant

//#include "./gain_matching/SuN_gain_matching_factors.h"
#include "./gain_matching/SuN_DSSD_gain_matching_factors.h"
#include "./cuts.h"

class TAC {

public:

  // Constructors
  TAC () {;} // Need default constuctor to create TAC object in Implantation class
  TAC (double e_i2s_i2n_raw,
       double e_pin1_i2n_raw,
       double e_pin2_i2n_raw,
       double e_pin1_i2s_raw,
       double e_pin2_i2s_raw) // double, even though energy from ddas2root is int

    : 

    e_i2s_i2n_raw(e_i2s_i2n_raw),
    e_pin1_i2n_raw(e_pin1_i2n_raw),
    e_pin2_i2n_raw(e_pin2_i2n_raw),
    e_pin1_i2s_raw(e_pin1_i2s_raw),
    e_pin2_i2s_raw(e_pin2_i2s_raw)

  {

    e_i2s_i2n_cal = 0;
    e_pin1_i2n_cal = 0;
    e_pin2_i2n_cal = 0;
    e_pin1_i2s_cal = 0;
    e_pin2_i2s_cal = 0;

  }

  // Methods
  // Get Methods
  double  Get_I2S_I2N_Raw_Energy() const;
  double  Get_I2S_I2N_Cal_Energy() const;
  double Get_PIN1_I2N_Raw_Energy() const;
  double Get_PIN1_I2N_Cal_Energy() const;
  double Get_PIN2_I2N_Raw_Energy() const;
  double Get_PIN2_I2N_Cal_Energy() const;
  double Get_PIN1_I2S_Raw_Energy() const;
  double Get_PIN1_I2S_Cal_Energy() const;
  double Get_PIN2_I2S_Raw_Energy() const;
  double Get_PIN2_I2S_Cal_Energy() const;
  // Calibration
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const TAC& event);
  
private:

  // Start: I2S, Stop: I2N
  double e_i2s_i2n_raw;
  double e_i2s_i2n_cal;
  // Start: PIN1, stop: I2N
  double e_pin1_i2n_raw;
  double e_pin1_i2n_cal;
  // Start: PIN2, stop: I2N
  double e_pin2_i2n_raw;
  double e_pin2_i2n_cal;

  // Start: PIN1, stop: I2S
  double e_pin1_i2s_raw;
  double e_pin1_i2s_cal;
  // Start: PIN2, stop: I2S
  double e_pin2_i2s_raw;
  double e_pin2_i2s_cal;

};

double TAC::Get_I2S_I2N_Raw_Energy() const{
  return e_i2s_i2n_raw;
}

double TAC::Get_I2S_I2N_Cal_Energy() const{
  return e_i2s_i2n_cal;
}

double TAC::Get_PIN1_I2N_Raw_Energy() const{
  return e_pin1_i2n_raw;
}

double TAC::Get_PIN1_I2N_Cal_Energy() const{
  return e_pin1_i2n_cal;
}

double TAC::Get_PIN2_I2N_Raw_Energy() const{
  return e_pin2_i2n_raw;
}

double TAC::Get_PIN2_I2N_Cal_Energy() const{
  return e_pin2_i2n_cal;
}

double TAC::Get_PIN1_I2S_Raw_Energy() const{
  return e_pin1_i2s_raw;
}

double TAC::Get_PIN1_I2S_Cal_Energy() const{
  return e_pin1_i2s_cal;
}

double TAC::Get_PIN2_I2S_Raw_Energy() const{
  return e_pin2_i2s_raw;
}

double TAC::Get_PIN2_I2S_Cal_Energy() const{
  return e_pin2_i2s_cal;
}

void TAC::Calibrate(){

  // Apply thresholds
  if (e_i2s_i2n_raw > 0){
    //e_i2s_i2n_cal =  0.011823534 * e_i2s_i2n_raw  + 5.166887374;
    e_i2s_i2n_cal =  e_i2s_i2n_raw; // e14041 I do not calibrate TACS
  }
  if (e_pin1_i2n_raw > 0){
    //e_pin1_i2n_cal = 0.011388672 * e_pin1_i2n_raw + 5.152563665;
    e_pin1_i2n_cal = e_pin1_i2n_raw; // e14041 I do not calibrate TACS
  }
  if (e_pin2_i2n_raw > 0){
    //e_pin2_i2n_cal = 0.011270742 * e_pin2_i2n_raw + 4.391973840;
    e_pin2_i2n_cal = e_pin2_i2n_raw; // e14041 I do not calibrate TACS
  }
  if (e_pin1_i2s_raw > 0){
    //e_pin1_i2n_cal = 0.011388672 * e_pin1_i2n_raw + 5.152563665;
    e_pin1_i2s_cal = e_pin1_i2s_raw; // e14041 I do not calibrate TACS
  }
  if (e_pin2_i2s_raw > 0){
    //e_pin2_i2n_cal = 0.011270742 * e_pin2_i2n_raw + 4.391973840;
    e_pin2_i2s_cal = e_pin2_i2s_raw; // e14041 I do not calibrate TACS
  }

}
 
std::ostream& operator<< (std::ostream& os, const TAC& event){

  os << std::setw(20) << event.e_i2s_i2n_raw  << std::setw(20) << event.e_i2s_i2n_cal << std::endl;
  os << std::setw(20) << event.e_pin1_i2n_raw << std::setw(20) << event.e_pin1_i2n_cal << std::endl;
  os << std::setw(20) << event.e_pin2_i2n_raw << std::setw(20) << event.e_pin2_i2n_cal << std::endl;
  os << std::setw(20) << event.e_pin1_i2s_raw << std::setw(20) << event.e_pin1_i2s_cal << std::endl;
  os << std::setw(20) << event.e_pin2_i2s_raw << std::setw(20) << event.e_pin2_i2s_cal << std::endl;

  os << std::setw(20) << event.Get_I2S_I2N_Raw_Energy()  << std::setw(20) << event.Get_I2S_I2N_Cal_Energy() << std::endl;
  os << std::setw(20) << event.Get_PIN1_I2N_Raw_Energy() << std::setw(20) << event.Get_PIN1_I2N_Cal_Energy() << std::endl;
  os << std::setw(20) << event.Get_PIN2_I2N_Raw_Energy() << std::setw(20) << event.Get_PIN2_I2N_Cal_Energy() << std::endl;
  os << std::setw(20) << event.Get_PIN1_I2S_Raw_Energy() << std::setw(20) << event.Get_PIN1_I2S_Cal_Energy() << std::endl;
  os << std::setw(20) << event.Get_PIN2_I2S_Raw_Energy() << std::setw(20) << event.Get_PIN2_I2S_Cal_Energy() << std::endl;

  return os;
}

class PIN {

public:

  // Constructors
  PIN () {;} // Need default constuctor to create PIN object in Implantation and Decay classes
  PIN (double e_PIN1_raw,
       double e_PIN2_raw) // double, even though energy from ddas2root is int

    :

    e_PIN1_raw(e_PIN1_raw),
    e_PIN2_raw(e_PIN2_raw)

  {
    
    e_PIN1_cal = 0;
    e_PIN2_cal = 0;
    
  }

  // Methods
  // Get Methods
  double GetPIN1RawEnergy() const;
  double GetPIN1CalEnergy() const;
  double GetPIN2RawEnergy() const;
  double GetPIN2CalEnergy() const;
  // Calibration
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const PIN& event);

private:

  double e_PIN1_raw;
  double e_PIN1_cal;

  double e_PIN2_raw;
  double e_PIN2_cal;

};

double PIN::GetPIN1RawEnergy() const{
  return e_PIN1_raw;
}

double PIN::GetPIN1CalEnergy() const{
  return e_PIN1_cal;
}

double PIN::GetPIN2RawEnergy() const{
  return e_PIN2_raw;
}

double PIN::GetPIN2CalEnergy() const{
  return e_PIN2_cal;
}


void PIN::Calibrate(){
  // Apply thresholds
    
  if (e_PIN1_raw > 0){
    e_PIN1_cal = e_PIN1_raw;
  }
  if (e_PIN2_raw > 0){
    e_PIN2_cal = e_PIN2_raw;
  }

}

std::ostream& operator<< (std::ostream& os, const PIN& event){

  os << std::setw(20) << event.e_PIN1_raw << std::setw(20) << event.e_PIN1_cal << std::endl;
  os << std::setw(20) << event.e_PIN2_raw << std::setw(20) << event.e_PIN2_cal << std::endl;

  os << std::setw(20) << event.GetPIN1RawEnergy() << std::setw(20) << event.GetPIN1CalEnergy() << std::endl;
  os << std::setw(20) << event.GetPIN2RawEnergy() << std::setw(20) << event.GetPIN2CalEnergy() << std::endl;

  return os;

}

class DSSD {

public:

  // Constructors
  DSSD () {;} // Need default constuctor to create DSSD object in Implantation and Decay classes
  DSSD (int    input_e_low_raw[][16],
	double input_t_low_raw[][16],
	int    input_m_low_raw[2],

	int    input_e_high_raw[][16],
	double input_t_high_raw[][16],
	int    input_m_high_raw[2])

    :

    e_low_cal{{0}},
    t_low_cal{{0}},
    m_low_cal{0},
    
    e_high_cal{{0}},
    t_high_cal{{0}},
    m_high_cal{0}
  
  {

    std::memcpy(e_low_raw,  input_e_low_raw,  2*16*sizeof(int));
    std::memcpy(t_low_raw,  input_t_low_raw,  2*16*sizeof(double));
    std::memcpy(m_low_raw,  input_m_low_raw,  2*sizeof(int));

    std::memcpy(e_high_raw, input_e_high_raw, 2*16*sizeof(int));
    std::memcpy(t_high_raw, input_t_high_raw, 2*16*sizeof(double));
    std::memcpy(m_high_raw, input_m_high_raw, 2*sizeof(int));

    c_strip_low_back = -1; // Notice the -1, this should match the convention/value in ddas2root and the output ROOT file created in main
    e_strip_low_back = 0;
    t_strip_low_back = 0;

    c_strip_low_front = -1; // Notice the -1, this should match the convention/value in ddas2root and the output ROOT file created in main
    e_strip_low_front = 0;
    t_strip_low_front = 0;

    c_strip_high_back = -1; // Notice the -1, this should match the convention/value in ddas2root and the output ROOT file created in main
    e_strip_high_back = 0;
    t_strip_high_back = 0;
    
    c_strip_high_front = -1; // Notice the -1, this should match the convention/value in ddas2root and the output ROOT file created in main
    e_strip_high_front = 0;
    t_strip_high_front = 0;

    e_low_cal_sum_back = 0;
    e_low_cal_sum_front = 0;
    e_low_cal_sum = 0;

    e_high_cal_sum_back = 0;
    e_high_cal_sum_front = 0;
    e_high_cal_sum = 0;
    
  }

  // Methods
  // Get Methods
  double GetLowRawEnergy(int side, int strip) const;
  double GetLowCalEnergy(int side, int strip) const;
  double GetLowRawTime(int side, int strip) const;
  double GetLowCalTime(int side, int strip) const;
  int    GetLowRawMultiplicity(int side) const;
  int    GetLowCalMultiplicity(int side) const;

  double GetHighRawEnergy(int side, int strip) const;
  double GetHighCalEnergy(int side, int strip) const;
  double GetHighRawTime(int side, int strip) const;
  double GetHighCalTime(int side, int strip) const;
  int    GetHighRawMultiplicity(int side) const;
  int    GetHighCalMultiplicity(int side) const;

  int    GetCStripLowBack() const;
  double GetEStripLowBack() const;
  double GetTStripLowBack() const;

  int    GetCStripLowFront() const;
  double GetEStripLowFront() const;
  double GetTStripLowFront() const;

  double GetELowCalSumBack() const;
  double GetELowCalSumFront() const;
  double GetELowCalSum() const;

  int    GetCStripHighBack() const;
  double GetEStripHighBack() const;
  double GetTStripHighBack() const;
  
  int    GetCStripHighFront() const;
  double GetEStripHighFront() const;
  double GetTStripHighFront() const;

  double GetEHighCalSumBack() const;
  double GetEHighCalSumFront() const;
  double GetEHighCalSum() const;

  double GetTStripLowAvg() const;
  double GetTStripHighAvg() const;

  // Calibration
  void Calibrate(double qvalue);
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const DSSD& event);

private:

  // Low gain
  int    e_low_raw[2][16];
  double e_low_cal[2][16];
  double t_low_raw[2][16];
  double t_low_cal[2][16];
  int    m_low_raw[2];
  int    m_low_cal[2];

  // Low gain, back side
  int    c_strip_low_back;
  double e_strip_low_back;
  double t_strip_low_back;
  // Low gain, front side
  int    c_strip_low_front;
  double e_strip_low_front;
  double t_strip_low_front;
  // Low gain, total kinetic energy
  double e_low_cal_sum_back;
  double e_low_cal_sum_front;
  double e_low_cal_sum;

  // High gain
  int    e_high_raw[2][16];
  double e_high_cal[2][16];
  double t_high_raw[2][16];
  double t_high_cal[2][16];
  int    m_high_raw[2];
  int    m_high_cal[2];

  // High gain, back side
  int    c_strip_high_back;
  double e_strip_high_back;
  double t_strip_high_back;
  // High gain, front side
  int    c_strip_high_front;
  double e_strip_high_front;
  double t_strip_high_front;
  // High gain, total kinetic energy
  double e_high_cal_sum_back;
  double e_high_cal_sum_front;
  double e_high_cal_sum;

};

double DSSD::GetLowRawEnergy(int side, int strip) const{
  return e_low_raw[side][strip];
}

double DSSD::GetLowCalEnergy(int side, int strip) const{
  return e_low_cal[side][strip];
}

double DSSD::GetLowRawTime(int side, int strip) const{
  return t_low_raw[side][strip];
}

double DSSD::GetLowCalTime(int side, int strip) const{
  return t_low_cal[side][strip];
}

int DSSD::GetLowRawMultiplicity(int side) const{
  return m_low_raw[side];
}

int DSSD::GetLowCalMultiplicity(int side) const{
  return m_low_cal[side];
}

double DSSD::GetHighRawEnergy(int side, int strip) const{
  return e_high_raw[side][strip];
}

double DSSD::GetHighCalEnergy(int side, int strip) const{
  return e_high_cal[side][strip];
}

double DSSD::GetHighRawTime(int side, int strip) const{
  return t_high_raw[side][strip];
}

double DSSD::GetHighCalTime(int side, int strip) const{
  return t_high_cal[side][strip];
}

int DSSD::GetHighRawMultiplicity(int side) const{
  return m_high_raw[side];
}

int DSSD::GetHighCalMultiplicity(int side) const{
  return m_high_cal[side];
}

int DSSD::GetCStripLowBack() const{
  return c_strip_low_back;
}

double DSSD::GetEStripLowBack() const{
  return e_strip_low_back;
}

double DSSD::GetTStripLowBack() const{
  return t_strip_low_back;
}

int DSSD::GetCStripLowFront() const{
  return c_strip_low_front;
}

double DSSD::GetEStripLowFront() const{
  return e_strip_low_front;
}

double DSSD::GetTStripLowFront() const{
  return t_strip_low_front;
}

double DSSD::GetELowCalSumBack() const{
  return e_low_cal_sum_back;
}

double DSSD::GetELowCalSumFront() const{
  return e_low_cal_sum_front;
}

double DSSD::GetELowCalSum() const{
  return e_low_cal_sum;
}

int DSSD::GetCStripHighBack() const{
  return c_strip_high_back;
}

double DSSD::GetEStripHighBack() const{
  return e_strip_high_back;
}

double DSSD::GetTStripHighBack() const{
  return t_strip_high_back;
}

int DSSD::GetCStripHighFront() const{
  return c_strip_high_front;
}

double DSSD::GetEStripHighFront() const{
  return e_strip_high_front;
}

double DSSD::GetTStripHighFront() const{
  return t_strip_high_front;
}

double DSSD::GetEHighCalSumBack() const{
  return e_high_cal_sum_back;
}

double DSSD::GetEHighCalSumFront() const{
  return e_high_cal_sum_front;
}

double DSSD::GetEHighCalSum() const{
  return e_high_cal_sum;
}

double DSSD::GetTStripLowAvg() const{
  return (t_strip_low_back + t_strip_low_front) / 2.0;
}

double DSSD::GetTStripHighAvg() const{
  return (t_strip_high_back + t_strip_high_front) / 2.0;
}

void DSSD::Calibrate( double qvalue ){

  // Calibration values

  // Low gain


  double e_low_slopes[2][16] = {{0}};
  double e_low_intercepts[2][16] = {{0}};
  
  // Low gain, back side

  e_low_slopes[0][0]  = 0.7762799982499908;
  e_low_slopes[0][1]  = 0.6370921477882443;
  e_low_slopes[0][2]  = 0.6461536816203965;
  e_low_slopes[0][3]  = 1.3350349484575699;
  e_low_slopes[0][4]  = 0.6338242226870399;
  e_low_slopes[0][5]  = 0.6014412522891418;
  e_low_slopes[0][6]  = 0.6369115675478324;
  e_low_slopes[0][7]  = 0.5956368299820562;
  e_low_slopes[0][8]  = 0.6472280077176726;
  e_low_slopes[0][9]  = 0.6236400566251719;
  e_low_slopes[0][10] = 0.5782851165029259;
  e_low_slopes[0][11] = 0.6435731987153466;
  e_low_slopes[0][12] = 0.5929312200376942;
  e_low_slopes[0][13] = 0.6237345108829302;
  e_low_slopes[0][14] = 0.6410321171123639;
  e_low_slopes[0][15] = 0.629001328493527 ;

  e_low_intercepts[0][0]  = 0;
  e_low_intercepts[0][1]  = 0;
  e_low_intercepts[0][2]  = 0;
  e_low_intercepts[0][3]  = 0;
  e_low_intercepts[0][4]  = 0;
  e_low_intercepts[0][5]  = 0;
  e_low_intercepts[0][6]  = 0;
  e_low_intercepts[0][7]  = 0;
  e_low_intercepts[0][8]  = 0;
  e_low_intercepts[0][9]  = 0;
  e_low_intercepts[0][10] = 0;
  e_low_intercepts[0][11] = 0;
  e_low_intercepts[0][12] = 0;
  e_low_intercepts[0][13] = 0;
  e_low_intercepts[0][14] = 0;
  e_low_intercepts[0][15] = 0;

  // Low gain, front side

  e_low_slopes[1][0]  = 1.02324917192895   ;
  e_low_slopes[1][1]  = 0.9796971718885432 ;
  e_low_slopes[1][2]  = 1.0275328870598877 ;
  e_low_slopes[1][3]  = 0.9831303860944447 ;
  e_low_slopes[1][4]  = 1.0277613198174405 ;
  e_low_slopes[1][5]  = 0.9867982636862141 ;
  e_low_slopes[1][6]  = 1.0363994426315222 ;
  e_low_slopes[1][7]  = 0.9999999999993223 ;
  e_low_slopes[1][8]  = 0.9657199493694308 ;
  e_low_slopes[1][9]  = 0.9857486557466126 ;
  e_low_slopes[1][10] = 0.9220548536154274 ;
  e_low_slopes[1][11] = 1.0107866023060632 ;
  e_low_slopes[1][12] = 0.9760405338950183 ;
  e_low_slopes[1][13] = 1.0562689690809908 ;
  e_low_slopes[1][14] = 1.0328272687270714 ;
  e_low_slopes[1][15] = 0.9395092518097922 ;

  e_low_intercepts[1][0]  = 0;
  e_low_intercepts[1][1]  = 0;
  e_low_intercepts[1][2]  = 0;
  e_low_intercepts[1][3]  = 0;
  e_low_intercepts[1][4]  = 0;
  e_low_intercepts[1][5]  = 0;
  e_low_intercepts[1][6]  = 0;
  e_low_intercepts[1][7]  = 0;
  e_low_intercepts[1][8]  = 0;
  e_low_intercepts[1][9]  = 0;
  e_low_intercepts[1][10] = 0;
  e_low_intercepts[1][11] = 0;
  e_low_intercepts[1][12] = 0;
  e_low_intercepts[1][13] = 0;
  e_low_intercepts[1][14] = 0;
  e_low_intercepts[1][15] = 0;

  // High gain

  double e_high_slopes[2][16] = {{0}};
  double e_high_intercepts[2][16] = {{0}};
  
  // High gain, back side
  // Updated Sept 2015 DR
  e_high_slopes[0][0]  = 0.312898; //0.33655 ; 
  e_high_slopes[0][1]  = 0.306559;
  e_high_slopes[0][2]  = 0.317101;
  e_high_slopes[0][3]  = 0.219189;
  e_high_slopes[0][4]  = 0.309117;
  e_high_slopes[0][5]  = 0.314029;
  e_high_slopes[0][6]  = 0.324056;
  e_high_slopes[0][7]  = 0.300898;
  e_high_slopes[0][8]  = 0.327296;
  e_high_slopes[0][9]  = 0.317865;
  e_high_slopes[0][10] = 0.317131;
  e_high_slopes[0][11] = 0.321488;
  e_high_slopes[0][12] = 0.297168;
  e_high_slopes[0][13] = 0.314896;
  e_high_slopes[0][14] = 0.31459 ; //0.309828;
  e_high_slopes[0][15] = 0.30981 ;

  e_high_intercepts[0][0]  =  283.678; //-290.427;
  e_high_intercepts[0][1]  =  249.793;
  e_high_intercepts[0][2]  =  245.306;
  e_high_intercepts[0][3]  =  212.632;
  e_high_intercepts[0][4]  =  188.97 ;
  e_high_intercepts[0][5]  =  167.108;
  e_high_intercepts[0][6]  =  177.131;
  e_high_intercepts[0][7]  =  170.975;
  e_high_intercepts[0][8]  =  164.836;
  e_high_intercepts[0][9]  =  160.853;
  e_high_intercepts[0][10] =  159.962;
  e_high_intercepts[0][11] =  175.508;
  e_high_intercepts[0][12] =  194.928;
  e_high_intercepts[0][13] =  201.776;
  e_high_intercepts[0][14] =  211.493; //356.172;
  e_high_intercepts[0][15] =  222.685;

  // High gain, front side
  // Updated Sept 2015 DR
  e_high_slopes[1][0]  = 0.475071;
  e_high_slopes[1][1]  = 0.472779;
  e_high_slopes[1][2]  = 0.479658;
  e_high_slopes[1][3]  = 0.479327;
  e_high_slopes[1][4]  = 0.483436;
  e_high_slopes[1][5]  = 0.491651;
  e_high_slopes[1][6]  = 0.481077;
  e_high_slopes[1][7]  = 0.490598;
  e_high_slopes[1][8]  = 0.485973;
  e_high_slopes[1][9]  = 0.486615;
  e_high_slopes[1][10] = 0.486875;
  e_high_slopes[1][11] = 0.483699;
  e_high_slopes[1][12] = 0.471633;
  e_high_slopes[1][13] = 0.479327;
  e_high_slopes[1][14] = 0.495206;
  e_high_slopes[1][15] = 0.469878;

  e_high_intercepts[1][0]  = 257.828; 
  e_high_intercepts[1][1]  = 236.947;
  e_high_intercepts[1][2]  = 227.373;
  e_high_intercepts[1][3]  = 199.55 ;
  e_high_intercepts[1][4]  = 189.611;
  e_high_intercepts[1][5]  = 164.205;
  e_high_intercepts[1][6]  = 140.805;
  e_high_intercepts[1][7]  = 142.651;
  e_high_intercepts[1][8]  = 135.799;
  e_high_intercepts[1][9]  = 139.734;
  e_high_intercepts[1][10] = 142.508;
  e_high_intercepts[1][11] = 148.98 ;
  e_high_intercepts[1][12] = 167.078;
  e_high_intercepts[1][13] = 184.211;
  e_high_intercepts[1][14] = 199.338;
  e_high_intercepts[1][15] = 229.329;
 
  // Threshold values

  // Low gain

  double e_low_raw_thresholds_lower_bounds[2][16] = {{0}};
  double e_low_raw_thresholds_upper_bounds[2][16] = {{0}};

  // Low gain, back side, lower bound

  e_low_raw_thresholds_lower_bounds[0][0]  = 10;
  e_low_raw_thresholds_lower_bounds[0][1]  = 10;
  e_low_raw_thresholds_lower_bounds[0][2]  = 10;
  e_low_raw_thresholds_lower_bounds[0][3]  = -1;
  e_low_raw_thresholds_lower_bounds[0][4]  = 10;
  e_low_raw_thresholds_lower_bounds[0][5]  = 10;
  e_low_raw_thresholds_lower_bounds[0][6]  = 10;
  e_low_raw_thresholds_lower_bounds[0][7]  = 10;
  e_low_raw_thresholds_lower_bounds[0][8]  = 10;
  e_low_raw_thresholds_lower_bounds[0][9]  = 10;
  e_low_raw_thresholds_lower_bounds[0][10] = 10;
  e_low_raw_thresholds_lower_bounds[0][11] = 10;
  e_low_raw_thresholds_lower_bounds[0][12] = 10;
  e_low_raw_thresholds_lower_bounds[0][13] = 10;
  e_low_raw_thresholds_lower_bounds[0][14] = 10;
  e_low_raw_thresholds_lower_bounds[0][15] = 10;

  // Low gain, front side, lower bound

  e_low_raw_thresholds_lower_bounds[1][0]  = 10;
  e_low_raw_thresholds_lower_bounds[1][1]  = 10;
  e_low_raw_thresholds_lower_bounds[1][2]  = 10;
  e_low_raw_thresholds_lower_bounds[1][3]  = 10;
  e_low_raw_thresholds_lower_bounds[1][4]  = 10;
  e_low_raw_thresholds_lower_bounds[1][5]  = 10;
  e_low_raw_thresholds_lower_bounds[1][6]  = 10;
  e_low_raw_thresholds_lower_bounds[1][7]  = 10;
  e_low_raw_thresholds_lower_bounds[1][8]  = 10;
  e_low_raw_thresholds_lower_bounds[1][9]  = 10;
  e_low_raw_thresholds_lower_bounds[1][10] = 10;
  e_low_raw_thresholds_lower_bounds[1][11] = 10;
  e_low_raw_thresholds_lower_bounds[1][12] = 10;
  e_low_raw_thresholds_lower_bounds[1][13] = 10;
  e_low_raw_thresholds_lower_bounds[1][14] = 10;
  e_low_raw_thresholds_lower_bounds[1][15] = 10;

  // Low gain, back side, upper bound

  e_low_raw_thresholds_upper_bounds[0][0]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][1]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][2]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][3]  = -1;
  e_low_raw_thresholds_upper_bounds[0][4]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][5]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][6]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][7]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][8]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][9]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][10] = 32000;
  e_low_raw_thresholds_upper_bounds[0][11] = 32000;
  e_low_raw_thresholds_upper_bounds[0][12] = 32000;
  e_low_raw_thresholds_upper_bounds[0][13] = 32000;
  e_low_raw_thresholds_upper_bounds[0][14] = 32000;
  e_low_raw_thresholds_upper_bounds[0][15] = 32000;
  
  // Low gain, front side, upper bound

  e_low_raw_thresholds_upper_bounds[1][0]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][1]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][2]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][3]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][4]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][5]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][6]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][7]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][8]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][9]  = 32000;
  e_low_raw_thresholds_upper_bounds[1][10] = 32000;
  e_low_raw_thresholds_upper_bounds[1][11] = 32000;
  e_low_raw_thresholds_upper_bounds[1][12] = 32000;
  e_low_raw_thresholds_upper_bounds[1][13] = 32000;
  e_low_raw_thresholds_upper_bounds[1][14] = 32000;
  e_low_raw_thresholds_upper_bounds[1][15] = 32000;

  // High gain

  double e_high_raw_thresholds_lower_bounds[2][16] = {{0}};
  double e_high_raw_thresholds_upper_bounds[2][16] = {{0}};
  double e_high_cal_thresholds_lower_bounds[2][16] = {{0}};
  double e_high_cal_thresholds_upper_bounds[2][16] = {{0}};

  // High gain, back side, lower bound

  e_high_raw_thresholds_lower_bounds[0][0]  = 1;// 400; 
  e_high_raw_thresholds_lower_bounds[0][1]  = 1;// 280; 
  e_high_raw_thresholds_lower_bounds[0][2]  = 1;// 300; 
  e_high_raw_thresholds_lower_bounds[0][3]  = 1;// 300; 
  e_high_raw_thresholds_lower_bounds[0][4]  = 1;// 240; 
  e_high_raw_thresholds_lower_bounds[0][5]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][6]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][7]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][8]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][9]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][10] = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[0][11] = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[0][12] = 1;// 230; 
  e_high_raw_thresholds_lower_bounds[0][13] = 1;// 260; 
  e_high_raw_thresholds_lower_bounds[0][14] = 1;// 250; 
  e_high_raw_thresholds_lower_bounds[0][15] = 1;// 400; 
                                                //      
  // High gain, front side, lower bound         //      
                                                //      
  e_high_raw_thresholds_lower_bounds[1][0]  = 1;// 300; 
  e_high_raw_thresholds_lower_bounds[1][1]  = 1;// 240; 
  e_high_raw_thresholds_lower_bounds[1][2]  = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][3]  = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][4]  = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][5]  = 1;// 180; 
  e_high_raw_thresholds_lower_bounds[1][6]  = 1;// 190; 
  e_high_raw_thresholds_lower_bounds[1][7]  = 1;// 180; 
  e_high_raw_thresholds_lower_bounds[1][8]  = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][9]  = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[1][10] = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][11] = 1;// 200; 
  e_high_raw_thresholds_lower_bounds[1][12] = 1;// 210; 
  e_high_raw_thresholds_lower_bounds[1][13] = 1;// 210; 
  e_high_raw_thresholds_lower_bounds[1][14] = 1;// 220; 
  e_high_raw_thresholds_lower_bounds[1][15] = 1;// 300; 

  // High gain, back side, upper bound

  e_high_raw_thresholds_upper_bounds[0][0]  = 34500;
  e_high_raw_thresholds_upper_bounds[0][1]  = 34800;
  e_high_raw_thresholds_upper_bounds[0][2]  = 34700;
  e_high_raw_thresholds_upper_bounds[0][3]  = 34600;
  e_high_raw_thresholds_upper_bounds[0][4]  = 34800;
  e_high_raw_thresholds_upper_bounds[0][5]  = 34900;
  e_high_raw_thresholds_upper_bounds[0][6]  = 34600;
  e_high_raw_thresholds_upper_bounds[0][7]  = 35000;
  e_high_raw_thresholds_upper_bounds[0][8]  = 34700;
  e_high_raw_thresholds_upper_bounds[0][9]  = 34900;
  e_high_raw_thresholds_upper_bounds[0][10] = 34900;
  e_high_raw_thresholds_upper_bounds[0][11] = 34800;
  e_high_raw_thresholds_upper_bounds[0][12] = 35000;
  e_high_raw_thresholds_upper_bounds[0][13] = 34700;
  e_high_raw_thresholds_upper_bounds[0][14] = 34700;
  e_high_raw_thresholds_upper_bounds[0][15] = 34600;

  // High gain, front side, upper bound

  e_high_raw_thresholds_upper_bounds[1][0]  = 33400;
  e_high_raw_thresholds_upper_bounds[1][1]  = 33400;
  e_high_raw_thresholds_upper_bounds[1][2]  = 33300;
  e_high_raw_thresholds_upper_bounds[1][3]  = 33400;
  e_high_raw_thresholds_upper_bounds[1][4]  = 33400;
  e_high_raw_thresholds_upper_bounds[1][5]  = 33300;
  e_high_raw_thresholds_upper_bounds[1][6]  = 33300;
  e_high_raw_thresholds_upper_bounds[1][7]  = 33300;
  e_high_raw_thresholds_upper_bounds[1][8]  = 33300;
  e_high_raw_thresholds_upper_bounds[1][9]  = 33400;
  e_high_raw_thresholds_upper_bounds[1][10] = 33400;
  e_high_raw_thresholds_upper_bounds[1][11] = 33400;
  e_high_raw_thresholds_upper_bounds[1][12] = 33300;
  e_high_raw_thresholds_upper_bounds[1][13] = 33300;
  e_high_raw_thresholds_upper_bounds[1][14] = 33300;
  e_high_raw_thresholds_upper_bounds[1][15] = 33500;

  int reduce = 30;
  // High gain, back side, lower bound

  e_high_cal_thresholds_lower_bounds[0][0]  = 500-reduce-0;// 1;
  e_high_cal_thresholds_lower_bounds[0][1]  = 350-reduce+20;// 1; 
  e_high_cal_thresholds_lower_bounds[0][2]  = 375-reduce+15;// 1; 
  e_high_cal_thresholds_lower_bounds[0][3]  = 280-reduce+10;// 1; 
  e_high_cal_thresholds_lower_bounds[0][4]  = 270-reduce+25;// 1; 
  e_high_cal_thresholds_lower_bounds[0][5]  = 275-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[0][6]  = 260-reduce+25;// 1; 
  e_high_cal_thresholds_lower_bounds[0][7]  = 260-reduce+10;// 1; 
  e_high_cal_thresholds_lower_bounds[0][8]  = 270-reduce+5;// 1; 
  e_high_cal_thresholds_lower_bounds[0][9]  = 260-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[0][10] = 250-reduce+20;// 1; 
  e_high_cal_thresholds_lower_bounds[0][11] = 270-reduce+15;// 1; 
  e_high_cal_thresholds_lower_bounds[0][12] = 290-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[0][13] = 350-reduce-20;// 1; 
  e_high_cal_thresholds_lower_bounds[0][14] = 310-reduce+25;// 1; 
  e_high_cal_thresholds_lower_bounds[0][15] = 350-reduce+55;// 1; 
                                                        
  // High gain, front side, lower bound                 
                                                        
  e_high_cal_thresholds_lower_bounds[1][0]  = 380-reduce+30;// 1;
  e_high_cal_thresholds_lower_bounds[1][1]  = 320-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][2]  = 320-reduce-5;// 1; 
  e_high_cal_thresholds_lower_bounds[1][3]  = 290-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][4]  = 280-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][5]  = 250-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][6]  = 230-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][7]  = 240-reduce-5;// 1; 
  e_high_cal_thresholds_lower_bounds[1][8]  = 225-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][9]  = 230-reduce-5;// 1; 
  e_high_cal_thresholds_lower_bounds[1][10] = 230-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][11] = 230-reduce+5;// 1; 
  e_high_cal_thresholds_lower_bounds[1][12] = 250-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][13] = 270-reduce;// 1; 
  e_high_cal_thresholds_lower_bounds[1][14] = 290-reduce+5;// 1; 
  e_high_cal_thresholds_lower_bounds[1][15] = 340-reduce+10;// 1; 

  // High gain, back side, lower bound

  e_high_cal_thresholds_upper_bounds[0][0]  = 900;// 1;
  e_high_cal_thresholds_upper_bounds[0][1]  = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[0][2]  = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[0][3]  = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[0][4]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][5]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][6]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][7]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][8]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][9]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][10] = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[0][11] = 880;// 1; 
  e_high_cal_thresholds_upper_bounds[0][12] = 880;// 1; 
  e_high_cal_thresholds_upper_bounds[0][13] = 880;// 1; 
  e_high_cal_thresholds_upper_bounds[0][14] = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[0][15] = 880;// 1; 
                                                        
  // High gain, front side, lower bound                 
                                                        
  e_high_cal_thresholds_upper_bounds[1][0]  = 930;// 1;
  e_high_cal_thresholds_upper_bounds[1][1]  = 930;// 1; 
  e_high_cal_thresholds_upper_bounds[1][2]  = 940;// 1; 
  e_high_cal_thresholds_upper_bounds[1][3]  = 930;// 1; 
  e_high_cal_thresholds_upper_bounds[1][4]  = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[1][5]  = 860;// 1; 
  e_high_cal_thresholds_upper_bounds[1][6]  = 860;// 1; 
  e_high_cal_thresholds_upper_bounds[1][7]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[1][8]  = 860;// 1; 
  e_high_cal_thresholds_upper_bounds[1][9]  = 850;// 1; 
  e_high_cal_thresholds_upper_bounds[1][10] = 860;// 1; 
  e_high_cal_thresholds_upper_bounds[1][11] = 860;// 1; 
  e_high_cal_thresholds_upper_bounds[1][12] = 880;// 1; 
  e_high_cal_thresholds_upper_bounds[1][13] = 900;// 1; 
  e_high_cal_thresholds_upper_bounds[1][14] = 920;// 1; 
  e_high_cal_thresholds_upper_bounds[1][15] = 940;// 1; 

  // Calibrate the DSSD while applying the thresholds
  // Determine the channel, energy, and time of the strip that recieved the maximum energy deposition
  for (int i=0; i<2; i++){ // Sides
    for (int j=0; j<16; j++){ // Strips
      // Low gain
      if ((e_low_raw[i][j] > e_low_raw_thresholds_lower_bounds[i][j]) && (e_low_raw[i][j] < e_low_raw_thresholds_upper_bounds[i][j])){

	e_low_cal[i][j] = e_low_slopes[i][j] * e_low_raw[i][j] + e_low_intercepts[i][j];

	t_low_cal[i][j] = t_low_raw[i][j];// * 10; // SPECIFIC to e12001. Convert clock ticks to nanoseconds.
	
	m_low_cal[i] ++;

	if (i==0){ // Low gain, back side

	  e_low_cal_sum_back += e_low_cal[i][j];

	  if (e_low_cal[i][j] > e_strip_low_back){
	    c_strip_low_back = j;
	    e_strip_low_back = e_low_cal[i][j];
	    t_strip_low_back = t_low_cal[i][j];
	  }
	}

	if (i==1){ // Low gain, front side
	  
	  e_low_cal_sum_front += e_low_cal[i][j];

	  if (e_low_cal[i][j] > e_strip_low_front){
	    c_strip_low_front = j;
	    e_strip_low_front = e_low_cal[i][j];
	    t_strip_low_front = t_low_cal[i][j];
	  }
	}

      }
      // High gain
      if ((e_high_raw[i][j] > e_high_raw_thresholds_lower_bounds[i][j]) && (e_high_raw[i][j] < e_high_raw_thresholds_upper_bounds[i][j])){

    double e_high_cal_temp = e_high_slopes[i][j] * e_high_raw[i][j] + e_high_intercepts[i][j];
//	e_high_cal[i][j] = e_high_slopes[i][j] * e_high_raw[i][j] + e_high_intercepts[i][j];

//      if ((e_high_cal[i][j] > e_high_cal_thresholds_lower_bounds[i][j]) && (e_high_cal[i][j] < e_high_cal_thresholds_upper_bounds[i][j])){
      if ((e_high_cal_temp > e_high_cal_thresholds_lower_bounds[i][j]) && (e_high_cal_temp < e_high_cal_thresholds_upper_bounds[i][j])){

	e_high_cal[i][j] = e_high_slopes[i][j] * e_high_raw[i][j] + e_high_intercepts[i][j];
	t_high_cal[i][j] = t_high_raw[i][j];// * 10; // SPECIFIC to e12001. Convert clock ticks to nanoseconds.
	
	m_high_cal[i] ++;


	if (i==0){ // High gain, back side

	  e_high_cal_sum_back += e_high_cal[i][j];
      
	  if (e_high_cal[i][j] > e_strip_high_back){
	    c_strip_high_back = j;
	    e_strip_high_back = e_high_cal[i][j];
	    t_strip_high_back = t_high_cal[i][j];
	  }
	}

	if (i==1){ // High gain, front side

	  e_high_cal_sum_front += e_high_cal[i][j];
	  
	  if (e_high_cal[i][j] > e_strip_high_front){
	    c_strip_high_front = j;
	    e_strip_high_front = e_high_cal[i][j];
	    t_strip_high_front = t_high_cal[i][j];
	  }
	}

      }
      else {
    e_high_cal[i][j] = 0;
      }
      }
    }

  }

  e_low_cal_sum  =  e_low_cal_sum_back +  e_low_cal_sum_front;
  e_high_cal_sum = e_high_cal_sum_back + e_high_cal_sum_front;
//  std::cout<<e_high_cal_sum<<std::endl;
}

std::ostream& operator<< (std::ostream& os, const DSSD& event){

  // Low gain

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<16; j++){
      os << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.e_low_raw[i][j] << std::setw(20) << event.e_low_cal[i][j] << std::setw(20) << event.t_low_raw[i][j] << std::setw(20) << event.t_low_cal[i][j] << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.m_low_raw[i] << std::setw(20) << event.m_low_cal[i] << std::endl;
  }

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<16; j++){
      std::cout << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.GetLowRawEnergy(i,j) << std::setw(20) << event.GetLowCalEnergy(i,j) << std::setw(20) << event.GetLowRawTime(i,j) << std::setw(20) << event.GetLowCalTime(i,j) << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.GetLowRawMultiplicity(i) << std::setw(20) << event.GetLowCalMultiplicity(i) << std::endl;
  }

  std::cout << std::endl;

  std::cout << event.c_strip_low_back << std::endl;
  std::cout << event.e_strip_low_back << std::endl;
  std::cout << event.t_strip_low_back << std::endl;

  std::cout << event.c_strip_low_front << std::endl;
  std::cout << event.e_strip_low_front << std::endl;
  std::cout << event.t_strip_low_front << std::endl;

  std::cout << std::endl;

  std::cout << event.e_low_cal_sum_back << std::endl;
  std::cout << event.e_low_cal_sum_front << std::endl;
  std::cout << event.e_low_cal_sum << std::endl;

  std::cout << std::endl;

  std::cout << event.GetCStripLowBack() << std::endl;
  std::cout << event.GetEStripLowBack() << std::endl;
  std::cout << event.GetTStripLowBack() << std::endl;

  std::cout << event.GetCStripLowFront() << std::endl;
  std::cout << event.GetEStripLowFront() << std::endl;
  std::cout << event.GetTStripLowFront() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetELowCalSumBack() << std::endl;
  std::cout << event.GetELowCalSumFront() << std::endl;
  std::cout << event.GetELowCalSum() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetTStripLowAvg() << std::endl;

  // High gain

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<16; j++){
      os << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.e_high_raw[i][j] << std::setw(20) << event.e_high_cal[i][j] << std::setw(20) << event.t_high_raw[i][j] << std::setw(20) << event.t_high_cal[i][j] << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.m_high_raw[i] << std::setw(20) << event.m_high_cal[i] << std::endl;
  }

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<16; j++){
      std::cout << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.GetHighRawEnergy(i,j) << std::setw(20) << event.GetHighCalEnergy(i,j) << std::setw(20) << event.GetHighRawTime(i,j) << std::setw(20) << event.GetHighCalTime(i,j) << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.GetHighRawMultiplicity(i) << std::setw(20) << event.GetHighCalMultiplicity(i) << std::endl;
  }

  std::cout << std::endl;

  std::cout << event.c_strip_high_back << std::endl;
  std::cout << event.e_strip_high_back << std::endl;
  std::cout << event.t_strip_high_back << std::endl;

  std::cout << event.c_strip_high_front << std::endl;
  std::cout << event.e_strip_high_front << std::endl;
  std::cout << event.t_strip_high_front << std::endl;

  std::cout << std::endl;

  std::cout << event.e_high_cal_sum_back << std::endl;
  std::cout << event.e_high_cal_sum_front << std::endl;
  std::cout << event.e_high_cal_sum << std::endl;

  std::cout << std::endl;

  std::cout << event.GetCStripHighBack() << std::endl;
  std::cout << event.GetEStripHighBack() << std::endl;
  std::cout << event.GetTStripHighBack() << std::endl;

  std::cout << event.GetCStripHighFront() << std::endl;
  std::cout << event.GetEStripHighFront() << std::endl;
  std::cout << event.GetTStripHighFront() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetEHighCalSumBack() << std::endl;
  std::cout << event.GetEHighCalSumFront() << std::endl;
  std::cout << event.GetEHighCalSum() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetTStripHighAvg() << std::endl;

  return os;

}

class SuN {

public:

  // Constructors
  SuN () {;} // Need default constuctor to create SuN object in Implantation and Decay classes
  SuN(int input_e_PMT_raw[][5][4], double input_t_PMT_raw[][5][4])

  :
  
  egmPMT{{{0}}},
    erSEG{0},
    ecSEG{0},
    tSEG{0}
  
  {
  
    std::memcpy(erPMT, input_e_PMT_raw, 2*5*4*sizeof(int));
    std::memcpy(tPMT, input_t_PMT_raw, 2*5*4*sizeof(double));
    multiplicity = 0;
    total_energy = 0;

  }

  // Methods
  // Get Methods
  double GetRSegEnergy(int i) const;
  double GetCSegEnergy(int i) const;
  double GetTSeg(int i) const;
  double GetTotalEnergy() const;
  int GetMultiplicity() const;
  // Calibration
  void GainMatchAndCalibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const SuN& event);

private:

  int erPMT[2][5][4];
  double tPMT[2][5][4];
  double egmPMT[2][5][4];
  double erSEG[8];
  double ecSEG[8];
  double tSEG[8];
  double total_energy;
  int multiplicity;

};

double SuN::GetRSegEnergy(int i) const{
  return erSEG[i];
}

double SuN::GetCSegEnergy(int i) const{
  return ecSEG[i];
}

double SuN::GetTSeg(int i) const{
  return tSEG[i];
}
double SuN::GetTotalEnergy() const{
  return total_energy;
}

int SuN::GetMultiplicity() const{
  return multiplicity;
}

void SuN::GainMatchAndCalibrate(){

  // Gain match the PMTs
//  std::vector<std::vector<std::vector<double>>>  gmPMTFactors = SuN_gm_factors();

  // Bottom PMTs

  egmPMT[0][1][1] = erPMT[0][1][1] * 1.1129725170174785;
  egmPMT[0][1][2] = erPMT[0][1][2] * 1.0627604873982635;
  egmPMT[0][1][3] = erPMT[0][1][3] * 1.0733765312396537;
  egmPMT[0][2][1] = erPMT[0][2][1] * 1.0339134396355354;
  egmPMT[0][2][2] = erPMT[0][2][2] * 1.1034804679522714;
  egmPMT[0][2][3] = erPMT[0][2][3] * 0.9792367835136955;
  egmPMT[0][3][1] = erPMT[0][3][1] * 1.0745556302616501;
  egmPMT[0][3][2] = erPMT[0][3][2] * 1.0029166887627936;
  egmPMT[0][3][3] = erPMT[0][3][3] * 0.9455606016415983;
  egmPMT[0][4][1] = erPMT[0][4][1] * 1.0191485539788037;
  egmPMT[0][4][2] = erPMT[0][4][2] * 1.0267937128430655;
  egmPMT[0][4][3] = erPMT[0][4][3] * 0.9837788162263856;
  // Top PMTs                        
  egmPMT[1][1][1] = erPMT[1][1][1] * 1.033772149592311 ;
  egmPMT[1][1][2] = erPMT[1][1][2] * 1.0042303131139707;
  egmPMT[1][1][3] = erPMT[1][1][3] * 1.1738830875674244;
  egmPMT[1][2][1] = erPMT[1][2][1] * 1.060932167734094 ;
  egmPMT[1][2][2] = erPMT[1][2][2] * 1.0               ;
  egmPMT[1][2][3] = erPMT[1][2][3] * 1.0332261912349872;
  egmPMT[1][3][1] = erPMT[1][3][1] * 1.0056454114414854;
  egmPMT[1][3][2] = erPMT[1][3][2] * 1.0737726636133087;
  egmPMT[1][3][3] = erPMT[1][3][3] * 1.052694077482559 ;
  egmPMT[1][4][1] = erPMT[1][4][1] * 1.0861474844934529;
  egmPMT[1][4][2] = erPMT[1][4][2] * 0.9832247331207543;
  egmPMT[1][4][3] = erPMT[1][4][3] * 1.1646073175913467;
                                     
/*                                   
  egmPMT[0][1][1] = erPMT[0][1][1] * 1.1038838171036205;
  egmPMT[0][1][2] = erPMT[0][1][2] * 1.0540339178781304;
  egmPMT[0][1][3] = erPMT[0][1][3] * 1.062504107171356 ;
  egmPMT[0][2][1] = erPMT[0][2][1] * 1.0346848288156512;
  egmPMT[0][2][2] = erPMT[0][2][2] * 1.1009094888380915;
  egmPMT[0][2][3] = erPMT[0][2][3] * 0.9815534317381576;
  egmPMT[0][3][1] = erPMT[0][3][1] * 1.0808289165831064;
  egmPMT[0][3][2] = erPMT[0][3][2] * 1.0095893099264968;
  egmPMT[0][3][3] = erPMT[0][3][3] * 0.9540584511375801;
  egmPMT[0][4][1] = erPMT[0][4][1] * 1.0276108155223445;
  egmPMT[0][4][2] = erPMT[0][4][2] * 1.0330132710246254;
  egmPMT[0][4][3] = erPMT[0][4][3] * 0.978498435149482 ;
  // Top PMTs                                      
  egmPMT[1][1][1] = erPMT[1][1][1] * 1.0247265681587714;
  egmPMT[1][1][2] = erPMT[1][1][2] * 1.0036446509648127;
  egmPMT[1][1][3] = erPMT[1][1][3] * 1.1641798106931034;
  egmPMT[1][2][1] = erPMT[1][2][1] * 1.0599368789743302;
  egmPMT[1][2][2] = erPMT[1][2][2] * 1.0               ;
  egmPMT[1][2][3] = erPMT[1][2][3] * 1.0270140287835066;
  egmPMT[1][3][1] = erPMT[1][3][1] * 1.003965156299897 ;
  egmPMT[1][3][2] = erPMT[1][3][2] * 1.0755086332234185;
  egmPMT[1][3][3] = erPMT[1][3][3] * 1.0532394051629475;
  egmPMT[1][4][1] = erPMT[1][4][1] * 1.0783779406019838;
  egmPMT[1][4][2] = erPMT[1][4][2] * 0.9814257594020169;
  egmPMT[1][4][3] = erPMT[1][4][3] * 1.1613200461330717;
*/
  // Bottom segments
  tSEG[0] = (tPMT[0][1][1] + tPMT[0][1][2] + tPMT[0][1][3])/3.;
  tSEG[1] = (tPMT[0][2][1] + tPMT[0][2][2] + tPMT[0][2][3])/3.;
  tSEG[2] = (tPMT[0][3][1] + tPMT[0][3][2] + tPMT[0][3][3])/3.;
  tSEG[3] = (tPMT[0][4][1] + tPMT[0][4][2] + tPMT[0][4][3])/3.;
  // Top Segments
  tSEG[4] = (tPMT[1][1][1] + tPMT[1][1][2] + tPMT[1][1][3])/3.;
  tSEG[5] = (tPMT[1][2][1] + tPMT[1][2][2] + tPMT[1][2][3])/3.;
  tSEG[6] = (tPMT[1][3][1] + tPMT[1][3][2] + tPMT[1][3][3])/3.;
  tSEG[7] = (tPMT[1][4][1] + tPMT[1][4][2] + tPMT[1][4][3])/3.;

   // Sum the PMTs to create a SuN segment

  // Bottom segments
  erSEG[0] = egmPMT[0][1][1] + egmPMT[0][1][2] + egmPMT[0][1][3];
  erSEG[1] = egmPMT[0][2][1] + egmPMT[0][2][2] + egmPMT[0][2][3];
  erSEG[2] = egmPMT[0][3][1] + egmPMT[0][3][2] + egmPMT[0][3][3];
  erSEG[3] = egmPMT[0][4][1] + egmPMT[0][4][2] + egmPMT[0][4][3];
  // Top Segments
  erSEG[4] = egmPMT[1][1][1] + egmPMT[1][1][2] + egmPMT[1][1][3];
  erSEG[5] = egmPMT[1][2][1] + egmPMT[1][2][2] + egmPMT[1][2][3];
  erSEG[6] = egmPMT[1][3][1] + egmPMT[1][3][2] + egmPMT[1][3][3];
  erSEG[7] = egmPMT[1][4][1] + egmPMT[1][4][2] + egmPMT[1][4][3];

  // Calibrate the SuN segments *** Updated Apr 2020 e17028 KH
  // Apply an individualized linear calibration to each segment
  // Bottom segments
  ecSEG[0] = 0.394434399161347 * erSEG[0] - 0.976701536441965;
  ecSEG[1] = 0.396142107376216 * erSEG[1] - 2.86334502846591 ;
  ecSEG[2] = 0.389850522267614 * erSEG[2] - 2.1779072855627  ;
  ecSEG[3] = 0.393461081030975 * erSEG[3] - 3.29978713455932 ;
  // Top segments                           
  ecSEG[4] = 0.393607510599563 * erSEG[4] - 10.1717008553894 ;
  ecSEG[5] = 0.399934409725449 * erSEG[5] - 11.0494726009379 ;
  ecSEG[6] = 0.394182545145287 * erSEG[6] - 11.6370862645651 ;
  ecSEG[7] = 0.397683557118171 * erSEG[7] - 10.7100698622565 ;

/*  
  ecSEG[0] = 0.398038739 * erSEG[0] - 1.075134438;
  ecSEG[1] = 0.396071351 * erSEG[1] - 2.962439627;
  ecSEG[2] = 0.387390053 * erSEG[2] - 3.077778230;
  ecSEG[3] = 0.392289740 * erSEG[3] - 3.328342820;
  // Top segments
  ecSEG[4] = 0.395963962 * erSEG[4] - 10.18160872;
  ecSEG[5] = 0.400815366 * erSEG[5] - 10.89715573;
  ecSEG[6] = 0.394026656 * erSEG[6] - 11.36696923;
  ecSEG[7] = 0.399259002 * erSEG[7] - 10.71263228;
*/
  // Apply threshold (include only the positive, non-zero values of energy to avoid any offsets)

  double thresholds[8];
  thresholds[0] = 0; //(10 + 1.075134438)/0.398038739;
  thresholds[1] = 0; //(10 + 2.962439627)/0.396071351;
  thresholds[2] = 0; //(10 + 3.077778230)/0.387390053;
  thresholds[3] = 0; //(10 + 3.328342820)/0.392289740;
  thresholds[4] = 0; //(10 + 10.18160872)/0.395963962;
  thresholds[5] = 0; //(10 + 10.89715573)/0.400815366;
  thresholds[6] = 0; //(10 + 11.36696923)/0.394026656;
  thresholds[7] = 0; //(10 + 10.71263228)/0.399259002;
                  
  for (int i=0; i<8; i++){
    if (ecSEG[i] > thresholds[i]){ // Threshold applies to the calibrated values instead of raw values
      multiplicity ++;
      total_energy += ecSEG[i];
    }
    else {
      ecSEG[i] = 0;
    }
  }
}

std::ostream& operator<< (std::ostream& os, const SuN& event){

  for (int i=0; i<2; i++){ // Halves
    for (int j=1; j<5; j++){ // Segments
      for (int k=1; k<4; k++){ // PMTs
	os << std::setw(20) << i << std::setw(2) << j << std::setw(2) << k << std::setw(20) << event.egmPMT[i][j][k] << std::endl;
      }
    }
  }
 
  for (int i=0; i<8; i++){ // Segments
    os << std::setw(24) << i << std::setw(20) << event.erSEG[i] << std::setw(20) << event.ecSEG[i] << std::endl;
  }

  os << event.total_energy << std::endl;
  os << event.multiplicity << std::endl;

  for (int i=0; i<8; i++){
    os << std::setw(24) << i << std::setw(20) << event.GetRSegEnergy(i) << std::setw(20) << event.GetCSegEnergy(i) << std::endl;
  }

  os << event.GetTotalEnergy() << std::endl;
  os << event.GetMultiplicity() << std::endl;

  return os;

}

class Veto {

public:

  // Constructors
  Veto () {;} // Need default constuctor to create DSSD object in Implantation class
  Veto (double e_raw) : e_raw(e_raw) {;} // double, even though energy from ddas2root is int

  // Methods
  // Get Methods
  double GetRawEnergy() const;
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const Veto& event);

private:

  double e_raw;

};

double Veto::GetRawEnergy() const{
  return e_raw;
}

std::ostream& operator<< (std::ostream& os, const Veto& event){

  os << std::setw(20) << event.e_raw << std::endl;

  os << std::setw(20) << event.GetRawEnergy() << std::endl;

  return os;

}

class Implantation {

public:

  // Constructors
  Implantation () : is_implanted(false), time_between_back_to_back_implantations(0) {;} // Need default constructor to create SuN DSSD pixels in main. Note that is_implanted must be initialized to false so that when dssd_implantation_pixels is created in main, all pixels are initialized to false. You can test this by removing this here and couting "dssd_implantation_pixels[j][k].IsImplanted()" in the event loop and seeing that some pixels have nonsensical Boolean values
  Implantation (TAC tac_sub_event,
		PIN pin_sub_event,
		DSSD dssd_sub_event,
		SuN sun_sub_event,
		Veto veto_sub_event)

    :
    
    tac_sub_event(tac_sub_event),
    pin_sub_event(pin_sub_event),
    dssd_sub_event(dssd_sub_event),
    sun_sub_event(sun_sub_event),
    veto_sub_event(veto_sub_event)

  {
    
    time_between_back_to_back_implantations = 0;
    is_implanted = false;

  }

  // The objects which make up an implantation and/or needed to identify one
  TAC tac_sub_event;
  PIN pin_sub_event;
  DSSD dssd_sub_event;
  SuN sun_sub_event;
  Veto veto_sub_event;

  // Methods
  // Get Methods
  double GetTimeBetweenBackToBackImplantations();
  bool IsValidImplantation();
  bool IsImplanted();
  // Set Methods
  void SetTimeBetweenBackToBackImplantations(double time_difference);
  void SetIsImplanted(bool condition);

private:

  double time_between_back_to_back_implantations;
  bool is_implanted;

};

double Implantation::GetTimeBetweenBackToBackImplantations(){
  return time_between_back_to_back_implantations;
}

bool Implantation::IsValidImplantation(){

  // Conditions for a valid implantation
  // Note whether the thresholds apply to raw or calibrated values
//  if (pin_sub_event.GetPIN1RawEnergy() > 0 &&
//      pin_sub_event.GetPIN2RawEnergy() > 0 &&
//      dssd_sub_event.GetEStripLowBack() > 0 &&
//      dssd_sub_event.GetEStripLowFront() > 0 &&
//      veto_sub_event.GetRawEnergy() == 0){
//    return true;
  if (
      /*tac_sub_event.Get_I2S_I2N_Raw_Energy()  > 0 && tac_sub_event.Get_I2S_I2N_Raw_Energy()  < 32000 &&
      tac_sub_event.Get_PIN2_I2S_Raw_Energy() > 0 && tac_sub_event.Get_PIN2_I2S_Raw_Energy() < 32000 &&
      pin_sub_event.GetPIN1RawEnergy() > 0 && pin_sub_event.GetPIN1RawEnergy() < 32000 &&
      pin_sub_event.GetPIN2RawEnergy() > 0 && pin_sub_event.GetPIN2RawEnergy() < 32000 &&
      */
      dssd_sub_event.GetEStripLowBack() > 0 &&  // This is a calibrated value. Thresholds should have already been applied.
      dssd_sub_event.GetEStripLowFront() > 0 && // This is a calibrated value. Thresholds should have already been applied.
      (veto_sub_event.GetRawEnergy() < 25)){
    return true;
  }

  else {
//    printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d \n",
//      tac_sub_event.Get_I2S_I2N_Raw_Energy()  > 0 , tac_sub_event.Get_I2S_I2N_Raw_Energy()  < 32000 ,
//      tac_sub_event.Get_PIN2_I2S_Raw_Energy() > 0 , tac_sub_event.Get_PIN2_I2S_Raw_Energy() < 32000 ,
//      pin_sub_event.GetPIN1RawEnergy() > 0 , pin_sub_event.GetPIN1RawEnergy() < 32000 ,
//      pin_sub_event.GetPIN2RawEnergy() > 0 , pin_sub_event.GetPIN2RawEnergy() < 32000 ,
//      dssd_sub_event.GetEStripLowBack() > 0 ,  // This is a calibrated value. Thresholds should have already been applied.
//      dssd_sub_event.GetEStripLowFront() > 0 , // This is a calibrated value. Thresholds should have already been applied.
//      (veto_sub_event.GetRawEnergy() < 100));
    return false;
  }

}

bool Implantation::IsImplanted(){
  
  return is_implanted;

}

void Implantation::SetTimeBetweenBackToBackImplantations(double time_difference){
  time_between_back_to_back_implantations = time_difference;
}

void Implantation::SetIsImplanted(bool condition){

  is_implanted = condition;

}

class Decay {

public:

  // Constructors
  Decay (PIN pin_sub_event,
	 DSSD dssd_sub_event,
	 Veto veto_sub_event,
	 SuN sun_sub_event)
    
    :

    pin_sub_event(pin_sub_event),
    dssd_sub_event(dssd_sub_event),
    veto_sub_event(veto_sub_event),
    sun_sub_event(sun_sub_event)

  {;}

  // The objects which make up a decay and/or needed to identify one
  PIN pin_sub_event;
  DSSD dssd_sub_event;
  Veto veto_sub_event;
  SuN sun_sub_event;
  
  // Methods
  // Get Methods
  bool IsValidDecay();

private:

};

bool Decay::IsValidDecay(){

  // Conditions for a valid decay
  // Note whether the thresholds apply to raw or calibrated values
  if (pin_sub_event.GetPIN1RawEnergy() == 0 &&
      pin_sub_event.GetPIN2RawEnergy() == 0 &&
      veto_sub_event.GetRawEnergy() < 25 &&
      dssd_sub_event.GetEStripHighBack() > 2 &&
      dssd_sub_event.GetEStripHighFront() > 2) {
    return true;
  }
  else {
    return false;
  }

}
// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int momentum_correction(int i2pos, int raw_tof){
    return i2pos + 1500 + raw_tof;
}
// copy in binary mode
bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

int main(int argc, char* argv[]){

//  std::cout << argv[1] << ", 2001, " << std::stoi(argv[1]) << std::endl;
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
  bool calib_run = false;
  if (argc > 8 && std::string(argv[8]) == "y") { 
      calib_run = true; 
  }
  bool test = argv[8] == "y" ;
//  std::cout << "calib_run: " << calib_run << ", argc: " << argc << ", argv8: " << argv[8] << ", test: " << test << std::endl;
  
  int random_number_generator_seed = 47093625;
  TRandom3* rand_num_gen = new TRandom3(random_number_generator_seed);
  
  // ===== Output ROOT file
  /*TFile * fOut = new TFile(Form("Output_%s_%s_%i_%s_%i_%i.root",
			       argv[1],
			       argv[2],
			       std::stoi(argv[3]),
			       argv[4],
			       std::stoi(argv[5]),
			       std::stoi(argv[6]),
			  "NEW"));*/
  /*double numev = 0;
  std::cout << "def numev" << std::endl;
  if (std::string(argv[7]) =="all") {
    numev = 9999;
    std::cout << numev << std::endl;
  }
  else {
    std::cout << "entering else" << std::endl;
    numev = std::stod(argv[7]);}
  std::cout << "set numev " << numev << std::endl;*/
  TFile* fOut = new TFile(Form("/mnt/analysis/e17028/correlated_rootfiles/%.0fms/%s/Run%i_SuN_%.0fms_%s_%ipx_bound.root",std::stod(argv[5])*1000,argv[2],std::stoi(argv[1]),std::stod(argv[5])*1000,argv[7],(std::stoi(argv[3])*2-1)*(std::stoi(argv[3])*2-1)),"RECREATE");
  //TFile* fOut = new TFile("highthres.root","RECREATE");
  TTree* tOut = new TTree("t","output after thresholds, calibrations, and correlations");

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

  double output_E_veto = 0;
  tOut->Branch("E_veto_raw", &output_E_veto, "E_veto_raw/D");

  // DSSD

  // Low gain

  double output_E_DSSD_low_raw[2][16] = {{0}};
  double output_E_DSSD_low_cal[2][16] = {{0}};

  tOut->Branch("E_DSSD_low_raw", &output_E_DSSD_low_raw, "E_DSSD_low_raw[2][16]/D");
  tOut->Branch("E_DSSD_low_cal", &output_E_DSSD_low_cal, "E_DSSD_low_cal[2][16]/D");

  double output_t_DSSD_low_raw[2][16] = {{0}};
  double output_t_DSSD_low_cal[2][16] = {{0}};

  tOut->Branch("t_DSSD_low_raw", &output_t_DSSD_low_raw, "t_DSSD_low_raw[2][16]/D");
  tOut->Branch("t_DSSD_low_cal", &output_t_DSSD_low_cal, "t_DSSD_low_cal[2][16]/D");

  int output_m_DSSD_low_raw[2] = {0};
  int output_m_DSSD_low_cal[2] = {0};

  tOut->Branch("m_DSSD_low_raw", &output_m_DSSD_low_raw, "m_DSSD_low_raw[2]/I");
  tOut->Branch("m_DSSD_low_cal", &output_m_DSSD_low_cal, "m_DSSD_low_cal[2]/I");

  int cStripLowFront = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD
  int cStripLowBack  = -1; // Make sure this matches the convention in ddas2root, and in the constructor for DSSD

  tOut->Branch("cStripLowFront", &cStripLowFront, "cStripLowFront/I");
  tOut->Branch("cStripLowBack",  &cStripLowBack,  "cStripLowBack/I");

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

  double output_E_DSSD_high_raw[2][16] = {{0}};
  double output_E_DSSD_high_cal[2][16] = {{0}};

  tOut->Branch("E_DSSD_high_raw", &output_E_DSSD_high_raw, "E_DSSD_high_raw[2][16]/D");
  tOut->Branch("E_DSSD_high_cal", &output_E_DSSD_high_cal, "E_DSSD_high_cal[2][16]/D");

  double output_t_DSSD_high_raw[2][16] = {{0}};
  double output_t_DSSD_high_cal[2][16] = {{0}};

  tOut->Branch("t_DSSD_high_raw", &output_t_DSSD_high_raw, "t_DSSD_high_raw[2][16]/D");
  tOut->Branch("t_DSSD_high_cal", &output_t_DSSD_high_cal, "t_DSSD_high_cal[2][16]/D");

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

  // SuN
  
  // Low gain (isomers)

  double SuN_Seg_low_gain_cal[8] = {0};
  double SuN_TAS_low_gain_cal = 0;
  int SuN_m_low_gain_cal = 0;

  tOut->Branch("SuN_Seg_low_gain_cal", &SuN_Seg_low_gain_cal, "SuN_Seg_low_gain_cal[8]/D");
  tOut->Branch("SuN_TAS_low_gain_cal", &SuN_TAS_low_gain_cal, "SuN_TAS_low_gain_cal/D");
  tOut->Branch("SuN_TAS_low_gain_cal", &SuN_TAS_low_gain_cal, "SuN_TAS_low_gain_cal/D");
  tOut->Branch("SuN_m_low_gain_cal",   &SuN_m_low_gain_cal,   "SuN_m_low_gain_cal/I");

  // High gain (decays)

  double SuN_Seg_high_gain_cal[8] = {0};
  double SuN_TAS_high_gain_cal = 0;
  int SuN_m_high_gain_cal = 0;
  double SuN_dt[8] = {0};
  tOut->Branch("SuN_Seg_high_gain_cal", &SuN_Seg_high_gain_cal, "SuN_Seg_high_gain_cal[8]/D");
  tOut->Branch("SuN_TAS_high_gain_cal", &SuN_TAS_high_gain_cal, "SuN_TAS_high_gain_cal/D");
  tOut->Branch("SuN_m_high_gain_cal",   &SuN_m_high_gain_cal,   "SuN_m_high_gain_cal/I");
  tOut->Branch("SuN_dt", &SuN_dt, "SuN_dt[8]/D");

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

  Chain->Add(Form("/mnt/analysis/e17028/rootfiles/run%04d*", std::stoi(argv[1])));
//  Chain->Add(Form("/mnt/analysis/e14041/Ong/rootfiles/Run%s.root",argv[1]));
  //Chain->Add(Form("%s.root",argv[1]));
  //gFile = fOut;
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

  // PINs
  int E_PIN1_raw = 0;
  int E_PIN2_raw = 0;

  Chain->SetBranchAddress("energy_PIN1", &E_PIN1_raw);
  Chain->SetBranchAddress("energy_PIN2", &E_PIN2_raw);

  // DSSD
  int E_DSSD_low_raw[2][16] = {{0}};
  int E_DSSD_high_raw[2][16] = {{0}};

  Chain->SetBranchAddress("energy_DSSD_low",  &E_DSSD_low_raw);
  Chain->SetBranchAddress("energy_DSSD_high", &E_DSSD_high_raw);

  double t_DSSD_low_raw[2][16] = {{0}};
  double t_DSSD_high_raw[2][16] = {{0}};

  Chain->SetBranchAddress("time_DSSD_low",  &t_DSSD_low_raw);
  Chain->SetBranchAddress("time_DSSD_high", &t_DSSD_high_raw);

  int m_DSSD_low_raw[2] = {0};
  int m_DSSD_high_raw[2] = {0};

  Chain->SetBranchAddress("multiplicity_DSSD_low",  &m_DSSD_low_raw);
  Chain->SetBranchAddress("multiplicity_DSSD_high", &m_DSSD_high_raw);

  // SuN
  int E_SuN_raw[2][5][4] = {{{0}}};
  double T_SuN_raw[2][5][4] = {{{0}}};
  Chain->SetBranchAddress("energy_SuN", &E_SuN_raw);
  Chain->SetBranchAddress("time_SuN", &T_SuN_raw);
  // Veto
  int E_Veto_raw = 0;
  Chain->SetBranchAddress("energy_Veto", &E_Veto_raw);

  // The SuN DSSD
  Implantation dssd_implantation_pixels[16][16]; // 16 by 16 strips
  
  
  //cuts
  std::cout << "Initialising PID cut" << std::endl;
  fOut->cd();
  std::string isotope = argv[7];
  std::map<std::string, double> Qvalues = qvalDict();
  double qvalue = Qvalues[isotope];
  std::map<std::string, TCutG*> cutDict = cuts();
  TCutG* isotope_cut = cutDict[isotope];

  // Loop over events and correlate
  int counter = 10;
  int num = Chain->GetEntries();
  int max = 0;
  /*if (std::string(argv[7]) == "all"){
    max=num;
  }
  else {max = std::stoi(argv[7]);}*/
  int implants = 0, implant_vetos = 0, transmission_vetos = 0;
  for (int i=0; i<num; i++){
    
    Chain->GetEntry(i);

    // Counter
    if (i % (int)(0.1*num) == 0){
      std::cerr << counter << " ";
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

    TAC tac_sub_event(E_TAC_I2S_I2N_raw, E_TAC_PIN1_I2N_raw, E_TAC_PIN2_I2N_raw, E_TAC_PIN1_I2S_raw, E_TAC_PIN2_I2S_raw);
    PIN pin_sub_event(E_PIN1_raw, E_PIN2_raw);
    int e_PIN1_cal = pin_sub_event.GetPIN1RawEnergy();
    DSSD dssd_sub_event(E_DSSD_low_raw,
           		t_DSSD_low_raw,
           		m_DSSD_low_raw,
           		E_DSSD_high_raw,
           		t_DSSD_high_raw,
           		m_DSSD_high_raw);
    SuN sun_sub_event(E_SuN_raw,T_SuN_raw);
    Veto veto_sub_event(E_Veto_raw);

    //if (E_PIN1_cal>0) {std::cout << E_PIN1_cal << std::endl;}
    int e_PIN2_cal = pin_sub_event.GetPIN2RawEnergy();
    int e_TAC_PIN2_I2S_cal = tac_sub_event.Get_PIN2_I2S_Raw_Energy();
    int corr_tof = momentum_correction( tac_sub_event.Get_I2S_I2N_Raw_Energy(), e_TAC_PIN2_I2S_cal );
    if ((isotope_cut->IsInside((corr_tof),(e_PIN2_cal)) && (e_PIN1_cal > 0) && cutDict["pin12"]->IsInside((e_PIN2_cal),(e_PIN1_cal)) && cutDict["i2pos"]->IsInside((E_TAC_I2S_I2N_raw),(E_PIN2_raw))) || calib_run)
    {
//       std::cout << "IsInside" << std::endl;
       //PIN pin_sub_event(E_PIN1_raw, E_PIN2_raw);
       //TAC tac_sub_event(E_TAC_I2S_I2N_raw, E_TAC_PIN1_I2N_raw, E_TAC_PIN2_I2N_raw);
       /*DSSD dssd_sub_event(E_DSSD_low_raw,
           		t_DSSD_low_raw,
           		m_DSSD_low_raw,
           		E_DSSD_high_raw,
           		t_DSSD_high_raw,
           		m_DSSD_high_raw);*/
       //SuN sun_sub_event(E_SuN_raw);
       //Veto veto_sub_event(E_Veto_raw);

       // Implantation event
       Implantation implantation_event(tac_sub_event,
           			    pin_sub_event,
           			    dssd_sub_event,
           			    sun_sub_event,
           			    veto_sub_event);
       if (implantation_event.IsValidImplantation() || implantation_event.IsImplanted()){
         std::cout << "Error for implantations" << std::endl; // EStripLow(Back/Front) is not calculated until after calibrations, and EStripLow(Back/Front) is used to determine valid implant conditions. Also, is_implanted is intialized to false
       }
       
       // Calibrate the detectors. <======================== This is not necessary if you should make the condition of implant condition refer to raw maximum values instead of calibrated values. This would change your comment directly above. Maybe change to channel instead of energy (CANT DO THIS AS CHANNEL REFERS TO THE MAX ___CALIBRATED___ VALUE)           ********* HOWEVER, channels and max energy are only calculated with raw threshold values are satisfied!
       implantation_event.tac_sub_event.Calibrate();
       implantation_event.pin_sub_event.Calibrate();
       implantation_event.dssd_sub_event.Calibrate(qvalue);
       implantation_event.sun_sub_event.GainMatchAndCalibrate();

       if (implantation_event.IsValidImplantation() || calib_run) {
         implants++;
         correlation_flags->Fill(4); // Implantation event
         //std::cout << "flag 4" << std::endl;
         /*
         std::cout << "i: " << i << std::endl;
         std::cout << implantation_event.tac_sub_event << std::endl;
         std::cout << implantation_event.pin_sub_event << std::endl;
         std::cout << implantation_event.dssd_sub_event << std::endl;
         std::cout << implantation_event.sun_sub_event << std::endl;
         std::cout << implantation_event.veto_sub_event << std::endl;
         */

         /*
         for (int j=0; j<16; j++){
           for (int k=0; k<16; k++){
             std::cout << i << " " << j << " " << k << " " << dssd_implantation_pixels[j][k].IsImplanted() << std::endl; // Use this to make sure all pixels are initialized to false. Have to remove correlator logic
           }
           }*/

         // First check for back to back implantations (if there is an existing implant in this pixel)
         /*if (dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].IsImplanted() == true){

           correlation_flags->Fill(8); // Back to back implantation events
           
           // Calculate time between back to back implantations
           double time_between_back_to_back_implantations = implantation_event.dssd_sub_event.GetTStripLowAvg() -
             dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].dssd_sub_event.GetTStripLowAvg();

           implantation_event.SetTimeBetweenBackToBackImplantations(time_between_back_to_back_implantations);

           // Accept the latest implant
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()] = implantation_event;
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].SetIsImplanted(true); // In line above, implantation_event is initialized to false

         }
         */  
         //else {
            
//           correlation_flags->Fill(12); // Implantation without back to back implantations
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()] = implantation_event;
           dssd_implantation_pixels[implantation_event.dssd_sub_event.GetCStripLowFront()][implantation_event.dssd_sub_event.GetCStripLowBack()].SetIsImplanted(true);

       //  }
         }
         else {
           if (implantation_event.dssd_sub_event.GetEStripLowFront() == 0 || 
                implantation_event.dssd_sub_event.GetEStripLowBack() == 0) {
               transmission_vetos++;
               if (implantation_event.veto_sub_event.GetRawEnergy() >= 25 )
                   correlation_flags->Fill(7);
               else 
                   correlation_flags->Fill(5);
           }
           else if (implantation_event.veto_sub_event.GetRawEnergy() >= 25 )
           {    correlation_flags->Fill(6); 
           implant_vetos++;}
//           printf("I2pos: %f, corrTOF: %f, E_PIN1: %f, E_PIN2: %f, DSSDbac: %f, DSSDfro: %f, veto: %f\n",
//           printf("I2pos: %f, corrTOF: %f, E_PIN1: %f, E_PIN2: %f,\n DSSDbac: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, \n DSSDfro: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,\n veto: %f\n",
//      tac_sub_event.Get_I2S_I2N_Raw_Energy(),
//      tac_sub_event.Get_PIN2_I2S_Raw_Energy(),
//      pin_sub_event.GetPIN1RawEnergy(),
//      pin_sub_event.GetPIN2RawEnergy(),
//      (E_DSSD_low_raw[0]),(E_DSSD_low_raw[0][1]),(E_DSSD_low_raw[0][2]),(E_DSSD_low_raw[0][3]),(E_DSSD_low_raw[0][4]),(E_DSSD_low_raw[0][5]),(E_DSSD_low_raw[0][6]),(E_DSSD_low_raw[0][7]),(E_DSSD_low_raw[0][8]),(E_DSSD_low_raw[0][9]),(E_DSSD_low_raw[0][10]),(E_DSSD_low_raw[0][11]),(E_DSSD_low_raw[0][12]),(E_DSSD_low_raw[0][13]),(E_DSSD_low_raw[0][14]),(E_DSSD_low_raw[0][15]),
//      (E_DSSD_low_raw[1]),(E_DSSD_low_raw[1][1]),(E_DSSD_low_raw[1][2]),(E_DSSD_low_raw[1][3]),(E_DSSD_low_raw[1][4]),(E_DSSD_low_raw[1][5]),(E_DSSD_low_raw[1][6]),(E_DSSD_low_raw[1][7]),(E_DSSD_low_raw[1][8]),(E_DSSD_low_raw[1][9]),(E_DSSD_low_raw[1][10]),(E_DSSD_low_raw[1][11]),(E_DSSD_low_raw[1][12]),(E_DSSD_low_raw[1][13]),(E_DSSD_low_raw[1][14]),(E_DSSD_low_raw[1][15]),
//      
////      std::max_element((E_DSSD_low_raw[1]).begin(),E_DSSD_low_raw[1].end()),
////      dssd_sub_event.GetEStripLowBack(),  // This is a calibrated value. Thresholds should have already been applied.
////      dssd_sub_event.GetEStripLowFront(), // This is a calibrated value. Thresholds should have already been applied.
//      (veto_sub_event.GetRawEnergy()));
        }
     }
//     else if (e_PIN1_cal > 0 && e_PIN2_cal > 0 && corr_tof > 14000) 
//    { std::cout << (corr_tof)<< "," <<(e_PIN2_cal)<<","<<e_PIN1_cal<<std::endl;}
//     {}
       // Decay event
       Decay decay_event(pin_sub_event,
           	      dssd_sub_event,
                  veto_sub_event, 
          	      sun_sub_event);
       if (decay_event.IsValidDecay()){
         std::cout << "Error for decays" << std::endl; // EStripHigh(Back/Front) is not calculated until after calibrations, and EStripHigh(Back/Front) is used to determine valid decay conditions
       }
//	std::cout << "Decay!" << std::endl;
       // Calibrate the detectors
       decay_event.pin_sub_event.Calibrate();
       decay_event.dssd_sub_event.Calibrate(qvalue);
       decay_event.sun_sub_event.GainMatchAndCalibrate();
       
       if (decay_event.IsValidDecay() || calib_run){

         correlation_flags->Fill(16); // Decay event
	 //std::cout << "Decay!" << std::endl;

         /*
         std::cout << "i: " << i << std::endl;
         std::cout << decay_event.pin_sub_event << std::endl;
         std::cout << decay_event.dssd_sub_event << std::endl;
         std::cout << decay_event.sun_sub_event << std::endl;
         */

         int front = 500;
         int back = 500;
         double arbitrary_long_time = 1E11; // SPECIFIC to e12001. Since the DSSD time is converted to nanoseconds in the calibrate function, make this in units of nanoseconds

         int correlation_field = std::stoi(argv[3]);
         int correlation_limit = (correlation_field-1)/2;

         int front_lower_limit = decay_event.dssd_sub_event.GetCStripHighFront() - correlation_limit;
         int front_upper_limit = decay_event.dssd_sub_event.GetCStripHighFront() + correlation_limit;
         int back_lower_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  - correlation_limit;
         int back_upper_limit  = decay_event.dssd_sub_event.GetCStripHighBack()  + correlation_limit;

         for (int j = front_lower_limit; j <= front_upper_limit; j++){ // event loop is over "i" 
           for (int k = back_lower_limit; k <= back_upper_limit; k++){
             if (((j >= 0) && (j <= 15) && (k >= 0) && (k<=15)) || calib_run){
               // Choose the implantation closest in time to the decay
               if (dssd_implantation_pixels[j][k].IsImplanted() == true || calib_run){
                 double time_between_implantation_and_decay = decay_event.dssd_sub_event.GetTStripHighAvg() -
           	dssd_implantation_pixels[j][k].dssd_sub_event.GetTStripLowAvg();
                 if (time_between_implantation_and_decay < arbitrary_long_time || calib_run){
           	arbitrary_long_time = time_between_implantation_and_decay;
           	front = j; // j should be the loop over the front side
           	back = k; // k should be the loop over the back side
                 }
               }
             }
           }
         }

         // Random correlations
         // Modify the implantation pixel that was in the correlation field to be a random pixel
         if (((front >= 0) && (front <= 15) && (back >= 0) && (back <= 15)) || calib_run){
           if (std::string(argv[2]) == "rand"){
             
             int temp_front = front;
             int temp_back  = back;
             
             while (temp_front == front){ // Remove possibility of choosing the same pixel as the potential true correlation
               front = rand_num_gen->Integer(16); // Returns random integer from 0 to 15
             }
             while (temp_back == back){ // Remove possibility of choosing the same pixel as the potential true correlation
               back = rand_num_gen->Integer(16); // Returns random integer from 0 to 15
             }
           }
         }
         
         if (((front >= 0) && (front <= 15) && (back >= 0) && (front <= 15)) || calib_run){
         
             //std::cout << "passed1" <<std::endl;
           if (dssd_implantation_pixels[front][back].IsImplanted() == true || calib_run){
             //std::cout << "passed2" <<std::endl;
             
             // Check for back to back implantations
             // Make sure a sufficienct time has passed between back to back implantations
             // Otherwise, do not allow for correlations
             double back_to_back_implantation_acceptance_time = std::stod(argv[6]) * 1E9; // SPECIFIC to e12001. Since the DSSD time is converted to nanoseconds in the calibrate function, make this in units of nanoseconds

             if (dssd_implantation_pixels[front][back].GetTimeBetweenBackToBackImplantations() >= back_to_back_implantation_acceptance_time || calib_run){
               //std::cout << "passed" <<std::endl;
               // Correlations must occur within the correlation time window
               double correlation_time_window = std::stod(argv[5]) * 1E9; // SPECIFIC to e12001. Since the DSSD time is converted to nanoseconds in the calibrate function, make this in units of nanoseconds
              
               double time_between_implantation_and_decay = decay_event.dssd_sub_event.GetTStripHighAvg() -
                 dssd_implantation_pixels[front][back].dssd_sub_event.GetTStripLowAvg();
               //std::cout << "corr time " << time_between_implantation_and_decay << std::endl;  
               if (time_between_implantation_and_decay < correlation_time_window || calib_run){
               
                 correlation_flags->Fill(28); // Decay event successfully correlated
               
                 // If only allowing a single decay event to be correlated to an implantation event, remove the implantation information
                 if (std::string(argv[4]) == "sd"){ // sd = single decay
           	dssd_implantation_pixels[front][back].SetIsImplanted(false);
                 }

                 // Fill output ROOT tree
                 
                 // TACs
                 output_E_TAC_I2S_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_I2S_I2N_Raw_Energy();
                 output_E_TAC_I2S_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_I2S_I2N_Cal_Energy();
                 
                 output_E_TAC_PIN1_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2N_Raw_Energy();
                 output_E_TAC_PIN1_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2N_Cal_Energy();;
                 
                 output_E_TAC_PIN2_I2N_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2N_Raw_Energy();
                 output_E_TAC_PIN2_I2N_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2N_Cal_Energy();
                 
                 output_E_TAC_PIN1_I2S_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2S_Raw_Energy();
                 output_E_TAC_PIN1_I2S_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN1_I2S_Cal_Energy();;
                 
                 output_E_TAC_PIN2_I2S_raw = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2S_Raw_Energy();
                 output_E_TAC_PIN2_I2S_cal = dssd_implantation_pixels[front][back].tac_sub_event.Get_PIN2_I2S_Cal_Energy();
                 
                 // PINs
                 output_E_PIN1_raw = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN1RawEnergy();
                 output_E_PIN1_cal = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN1CalEnergy();
                 
                 output_E_PIN2_raw = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN2RawEnergy();
                 output_E_PIN2_cal = dssd_implantation_pixels[front][back].pin_sub_event.GetPIN2CalEnergy();
                 
                 output_E_veto = decay_event.veto_sub_event.GetRawEnergy();
                 // DSSD
                 
                 // Low gain
                 
                 for (int a=0; a<2; a++){ // Event loop is over "i"
           	for (int b=0; b<16; b++){
           	  
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

                 e_DSSD_high_cal_sum_back = decay_event.dssd_sub_event.GetEHighCalSumBack();
                 e_DSSD_high_cal_sum_front = decay_event.dssd_sub_event.GetEHighCalSumFront();
                 e_DSSD_high_cal_sum = decay_event.dssd_sub_event.GetEHighCalSum();

                 // SuN
                 
                 for (int a=0; a<8; a++){ // Event loop is over "i"
           	
           	// Low gain (isomers)
           	
           	SuN_Seg_low_gain_cal[a] = dssd_implantation_pixels[front][back].sun_sub_event.GetCSegEnergy(a);
           	
           	// High gain (decays)
           	
           	SuN_Seg_high_gain_cal[a] = decay_event.sun_sub_event.GetCSegEnergy(a);
           	SuN_dt[a] = decay_event.dssd_sub_event.GetTStripHighFront() - decay_event.sun_sub_event.GetTSeg(a);
                }
                 
                 // Low gain (isomers)
                 
                 SuN_TAS_low_gain_cal = dssd_implantation_pixels[front][back].sun_sub_event.GetTotalEnergy();
                 SuN_m_low_gain_cal   = dssd_implantation_pixels[front][back].sun_sub_event.GetMultiplicity();
                 
                 // High gain (decays)
                 
                 SuN_TAS_high_gain_cal = decay_event.sun_sub_event.GetTotalEnergy();
                 SuN_m_high_gain_cal   = decay_event.sun_sub_event.GetMultiplicity();
                 
                 // Correlations
                 
                 back_to_back_implant_time = dssd_implantation_pixels[front][back].GetTimeBetweenBackToBackImplantations();
                 correlation_implant_decay_time = time_between_implantation_and_decay;
                 
                 // Fill output tree
                 
                 tOut->Fill();
                 //std::cout << "num entries " << tOut->GetEntries() << std::endl;
                 
               }
               
               else {
                 correlation_flags->Fill(24); // Decay event correlated spatially, but not temporally (exceeded correlation time window)
               }
               
             }

             else {
               correlation_flags->Fill(20); // Time between implantations is too short
             }

           }
           
         }
         
        }
           
  }
  std::cout<<argv[7] << std::endl;
  std::cout<<"implants: "<<implants<<", implant_vetos: "<<implant_vetos<<std::endl;
  std::cout<<"efficiency: "<< (double) implants / (double)(implant_vetos + implants)<<std::endl;
  
  std::ofstream runSummary;
  runSummary.open(Form("/mnt/analysis/e17028/correlated_rootfiles/%0.fms/run%s.csv",std::stod(argv[5])*1000,argv[1]), std::ios_base::app);
  runSummary << argv[7] << "," << implants << "," << implant_vetos << "," << transmission_vetos << std::endl;
  runSummary.close();
  fOut->cd();
  correlation_flags->Write();
  tOut->Write();
  tOut->ResetBranchAddresses();
  fOut->Close();
  if (std::string(argv[1]) == "2090" && std::string(argv[7]) == "ti57")
    bool copy = copyFile("/mnt/projects/e17028/hermansen/correlators/SuN-correlator/correlator_newsuncalib.cpp", Form("/mnt/analysis/e17028/correlated_rootfiles/%0.fms/correlator_%s.cpp",std::stod(argv[5])*1000,currentDateTime().c_str()));
  return 0;
  
}
  
