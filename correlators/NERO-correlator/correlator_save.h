#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TCutG.h"
#include <iostream>
#include <iomanip>
#include <cstring>

struct PossImplants
{
   int data[2];
};


class TAC {

public:

  // Constructors
  TAC () {;} // Need default constuctor to create TAC object in Implantation class
  TAC (double e_i2s_i2n_raw,
       double e_pin1_i2n_raw,
       double e_pin1_i2s_raw,
       double e_pin2_i2n_raw, // double, even though energy from ddas2root is int
       double e_pin2_i2s_raw) // double, even though energy from ddas2root is int

    : 

    e_i2s_i2n_raw(e_i2s_i2n_raw),
    e_pin1_i2n_raw(e_pin1_i2n_raw),
    e_pin1_i2s_raw(e_pin1_i2s_raw),
    e_pin2_i2n_raw(e_pin2_i2n_raw),
    e_pin2_i2s_raw(e_pin2_i2s_raw)

  {

    e_i2s_i2n_cal = 0;
    e_pin1_i2n_cal = 0;
    e_pin1_i2s_cal = 0;
    e_pin2_i2n_cal = 0;
    e_pin2_i2s_cal = 0;

  }

  // Methods
  // Get Methods
  double Get_I2S_I2N_Raw_Energy() const;
  double Get_I2S_I2N_Cal_Energy() const;
  double Get_PIN1_I2N_Raw_Energy() const;
  double Get_PIN1_I2N_Cal_Energy() const;
  double Get_PIN1_I2S_Raw_Energy() const;
  double Get_PIN1_I2S_Cal_Energy() const;
  double Get_PIN2_I2N_Raw_Energy() const;
  double Get_PIN2_I2N_Cal_Energy() const;
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
  // Start: PIN1, stop: I2S
  double e_pin1_i2s_raw;
  double e_pin1_i2s_cal;
  // Start: PIN2, stop: I2N
  double e_pin2_i2n_raw;
  double e_pin2_i2n_cal;
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

double TAC::Get_PIN1_I2S_Raw_Energy() const{
  return e_pin1_i2s_raw;
}

double TAC::Get_PIN1_I2S_Cal_Energy() const{
  return e_pin1_i2s_cal;
}

double TAC::Get_PIN2_I2N_Raw_Energy() const{
  return e_pin2_i2n_raw;
}

double TAC::Get_PIN2_I2N_Cal_Energy() const{
  return e_pin2_i2n_cal;
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
       double e_PIN2_raw,
       double e_PIN3_raw
       ) // double, even though energy from ddas2root is int

    :

    e_PIN1_raw(e_PIN1_raw),
    e_PIN2_raw(e_PIN2_raw),
    e_PIN3_raw(e_PIN3_raw)


  {
    
    e_PIN1_cal = 0;
    e_PIN2_cal = 0;
    e_PIN3_cal = 0;
    
  }

  // Methods
  // Get Methods
  double GetPIN1RawEnergy() const;
  double GetPIN1CalEnergy() const;
  double GetPIN2RawEnergy() const;
  double GetPIN2CalEnergy() const;
  double GetPIN3RawEnergy() const;
  double GetPIN3CalEnergy() const;
  // Calibration
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const PIN& event);

private:

  double e_PIN1_raw;
  double e_PIN1_cal;

  double e_PIN2_raw;
  double e_PIN2_cal;

  double e_PIN3_raw;
  double e_PIN3_cal;

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

double PIN::GetPIN3RawEnergy() const{
  return e_PIN3_raw;
}

double PIN::GetPIN3CalEnergy() const{
  return e_PIN3_cal;
}


void PIN::Calibrate(){

  if (e_PIN1_raw > 0){
    e_PIN1_cal = e_PIN1_raw;
  }
  if (e_PIN2_raw > 0){
    e_PIN2_cal = e_PIN2_raw;
  }
  if (e_PIN3_raw > 0){
    e_PIN3_cal = e_PIN3_raw;
  }
  /*if (e_PIN4_raw > 0){
    e_PIN4_cal = e_PIN4_raw;
  }
  if (e_PIN5_raw > 400 && e_PIN5_raw < 32700){
    e_PIN5_cal = 0.4852*e_PIN5_raw + 140.15; //PIN5 cal from run 257, low-energy. 
  }
  if (e_PIN5_raw > 32700){
    e_PIN5_cal = 32767; //only overflows, which would be implants and not any betas or light particles. 
  }*/
}

std::ostream& operator<< (std::ostream& os, const PIN& event){

  os << std::setw(20) << event.e_PIN1_raw << std::setw(20) << event.e_PIN1_cal << std::endl;
  os << std::setw(20) << event.e_PIN2_raw << std::setw(20) << event.e_PIN2_cal << std::endl;
  os << std::setw(20) << event.e_PIN3_raw << std::setw(20) << event.e_PIN3_cal << std::endl;

  os << std::setw(20) << event.GetPIN1RawEnergy() << std::setw(20) << event.GetPIN1CalEnergy() << std::endl;
  os << std::setw(20) << event.GetPIN2RawEnergy() << std::setw(20) << event.GetPIN2CalEnergy() << std::endl;
  os << std::setw(20) << event.GetPIN3RawEnergy() << std::setw(20) << event.GetPIN3CalEnergy() << std::endl;

  return os;

}

class DSSD {

public:

  // Constructors
  DSSD () {;} // Need default constuctor to create DSSD object in Implantation and Decay classes
  DSSD (int    input_e_low_raw[][40],
	double input_t_low_raw[][40],
	int    input_m_low_raw[2],

	int    input_e_high_raw[][40],
	double input_t_high_raw[][40],
	int    input_m_high_raw[2])

    :

    e_low_cal{{0}},
    t_low_cal{{0}},
    m_low_cal{0},
    
    e_high_cal{{0}},
    t_high_cal{{0}},
    m_high_cal{0}
  
  {

    std::memcpy(e_low_raw,  input_e_low_raw,  2*40*sizeof(int));
    std::memcpy(t_low_raw,  input_t_low_raw,  2*40*sizeof(double));
    std::memcpy(m_low_raw,  input_m_low_raw,  2*sizeof(int));

    std::memcpy(e_high_raw, input_e_high_raw, 2*40*sizeof(int));
    std::memcpy(t_high_raw, input_t_high_raw, 2*40*sizeof(double));
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
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const DSSD& event);

private:

  // Low gain
  int    e_low_raw[2][40];
  double e_low_cal[2][40];
  double t_low_raw[2][40];
  double t_low_cal[2][40];
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
 
  // High gain
  int    e_high_raw[2][40];
  double e_high_cal[2][40];
  double t_high_raw[2][40];
  double t_high_cal[2][40];
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

double DSSD::GetTStripLowAvg() const{
  return (t_strip_low_back + t_strip_low_front) / 2.0;
}

double DSSD::GetTStripHighAvg() const{
  return (t_strip_high_back + t_strip_high_front) / 2.0;
}

void DSSD::Calibrate(){

  // Calibration values

  // Low gain

  double e_low_slopes[2][40] = {{0}};
  
  // Low gain, back side

  e_low_slopes[0][0]  = 1;
  e_low_slopes[0][1]  = 1;
  e_low_slopes[0][2]  = 1;
  e_low_slopes[0][3]  = 1;
  e_low_slopes[0][4]  = 1;
  e_low_slopes[0][5]  = 1;
  e_low_slopes[0][6]  = 1;
  e_low_slopes[0][7]  = 1;
  e_low_slopes[0][8]  = 1;
  e_low_slopes[0][9]  = 1;
  e_low_slopes[0][10] = 1;
  e_low_slopes[0][11] = 1;
  e_low_slopes[0][12] = 1;
  e_low_slopes[0][13] = 1;
  e_low_slopes[0][14] = 1;
  e_low_slopes[0][15] = 1;
  e_low_slopes[0][16] = 1;
  e_low_slopes[0][17] = 1;
  e_low_slopes[0][18] = 1;
  e_low_slopes[0][19] = 1;
  e_low_slopes[0][20] = 1;
  e_low_slopes[0][21] = 1;
  e_low_slopes[0][22] = 1;
  e_low_slopes[0][23] = 1;
  e_low_slopes[0][24] = 1;
  e_low_slopes[0][25] = 1;
  e_low_slopes[0][26] = 1;
  e_low_slopes[0][27] = 1;
  e_low_slopes[0][28] = 1;
  e_low_slopes[0][29] = 1;
  e_low_slopes[0][30] = 1;
  e_low_slopes[0][31] = 1;
  e_low_slopes[0][32] = 1;
  e_low_slopes[0][33] = 1;
  e_low_slopes[0][34] = 1;
  e_low_slopes[0][35] = 1;
  e_low_slopes[0][36] = 1;
  e_low_slopes[0][37] = 1;
  e_low_slopes[0][38] = 1;
  e_low_slopes[0][39] = 1;


  // Low gain, front side

  e_low_slopes[1][0]  = 0.35;
  e_low_slopes[1][1]  = 0.35;
  e_low_slopes[1][2]  = 0.35;
  e_low_slopes[1][3]  = 0.35;
  e_low_slopes[1][4]  = 0.35;
  e_low_slopes[1][5]  = 0.35;
  e_low_slopes[1][6]  = 0.35;
  e_low_slopes[1][7]  = 0.35;
  e_low_slopes[1][8]  = 0.35;
  e_low_slopes[1][9]  = 0.35;
  e_low_slopes[1][10] = 0.35;
  e_low_slopes[1][11] = 0.35;
  e_low_slopes[1][12] = 0.35;
  e_low_slopes[1][13] = 0.35;
  e_low_slopes[1][14] = 0.35;
  e_low_slopes[1][15] = 0.35;
  e_low_slopes[1][16] = 1;
  e_low_slopes[1][17] = 1;
  e_low_slopes[1][18] = 1;
  e_low_slopes[1][19] = 1;
  e_low_slopes[1][20] = 1;
  e_low_slopes[1][21] = 1;
  e_low_slopes[1][22] = 1;
  e_low_slopes[1][23] = 1;
  e_low_slopes[1][24] = 1;
  e_low_slopes[1][25] = 1;
  e_low_slopes[1][26] = 1;
  e_low_slopes[1][27] = 1;
  e_low_slopes[1][28] = 1;
  e_low_slopes[1][29] = 1;
  e_low_slopes[1][30] = 1;
  e_low_slopes[1][31] = 1;
  e_low_slopes[1][32] = 1;
  e_low_slopes[1][33] = 1;
  e_low_slopes[1][34] = 1;
  e_low_slopes[1][35] = 1;
  e_low_slopes[1][36] = 1;
  e_low_slopes[1][37] = 1;
  e_low_slopes[1][38] = 1;
  e_low_slopes[1][39] = 1;



  // High gain

  double e_high_slopes[2][40] = {{0}};
  double e_high_intercepts[2][40] = {{0}};
  
  // High gain, back side
  // Updated Sept 2015 DR
  e_high_slopes[0][0]  = 0.323733795945225  ; 
  e_high_slopes[0][1]  = 0.324359592380002  ;
  e_high_slopes[0][2]  = 0.326932414775029  ;
  e_high_slopes[0][3]  = 0.326691819048582  ;
  e_high_slopes[0][4]  = 0.327148847539125  ;
  e_high_slopes[0][5]  = 0.327792475914978  ;
  e_high_slopes[0][6]  = 0.328566230049802  ;
  e_high_slopes[0][7]  = 0.326910043658186  ;
  e_high_slopes[0][8]  = 0.328729120130563  ;
  e_high_slopes[0][9]  = 0.32631830693956   ;
  e_high_slopes[0][10] = 0.327298961480787  ;
  e_high_slopes[0][11] = 0.327131331399056  ;
  e_high_slopes[0][12] = 0.329171811557007  ;
  e_high_slopes[0][13] = 0.327994863517023  ;
  e_high_slopes[0][14] = 0.328892400746753  ;
  e_high_slopes[0][15] = 0.330517074002105  ;
  e_high_slopes[0][16] = 0.330183329625891  ;
  e_high_slopes[0][17] = 0.328548250354625  ;
  e_high_slopes[0][18] = 0.326721590303494  ;
  e_high_slopes[0][19] = 0.328845211187942  ;
  e_high_slopes[0][20] = 0.328270165939463  ;
  e_high_slopes[0][21] = 0.330061971312721  ;
  e_high_slopes[0][22] = 0.32821617474534   ;
  e_high_slopes[0][23] = 0.32853725393803   ;
  e_high_slopes[0][24] = 0.329189263435923  ;
  e_high_slopes[0][25] = 0.328777120135847  ;
  e_high_slopes[0][26] = 0.323164928035458  ;
  e_high_slopes[0][27] = 0.32765424373814   ;
  e_high_slopes[0][28] = 0.327660896494148  ;
  e_high_slopes[0][29] = 0.32735755358426   ;
  e_high_slopes[0][30] = 0.326487135449126  ;
  e_high_slopes[0][31] = 0.327444202597588  ;
  e_high_slopes[0][32] = 0.327864758580303  ;
  e_high_slopes[0][33] = 0.329565300666926  ;
  e_high_slopes[0][34] = 0.328140481434774  ;
  e_high_slopes[0][35] = 0.328546798858887  ;
  e_high_slopes[0][36] = 0.32758144537855   ;
  e_high_slopes[0][37] = 0.325859085625138  ;
  e_high_slopes[0][38] = 0.328842880651988  ;
  e_high_slopes[0][39] = 0.327086375363498  ;

  e_high_intercepts[0][0]  = 356.045939038348;
  e_high_intercepts[0][1]  = 338.72630942434 ;
  e_high_intercepts[0][2]  = 299.898444679736;
  e_high_intercepts[0][3]  = 285.544837699936;
  e_high_intercepts[0][4]  = 287.156627653237;
  e_high_intercepts[0][5]  = 271.184771770673;
  e_high_intercepts[0][6]  = 262.158617863166;
  e_high_intercepts[0][7]  = 288.212153362701;
  e_high_intercepts[0][8]  = 253.983371700464;
  e_high_intercepts[0][9]  = 298.564888022109;
  e_high_intercepts[0][10] = 278.623815159156;
  e_high_intercepts[0][11] = 281.772759552876;
  e_high_intercepts[0][12] = 242.252865804106;
  e_high_intercepts[0][13] = 264.234108297172;
  e_high_intercepts[0][14] = 248.895790452792;
  e_high_intercepts[0][15] = 206.504270683067;
  e_high_intercepts[0][16] = 232.919382329595;
  e_high_intercepts[0][17] = 250.023016183961;
  e_high_intercepts[0][18] = 293.02477671039 ;
  e_high_intercepts[0][19] = 252.070256036851;
  e_high_intercepts[0][20] = 256.026531337926;
  e_high_intercepts[0][21] = 233.726634474999;
  e_high_intercepts[0][22] = 256.55176365522 ;
  e_high_intercepts[0][23] = 253.9294007128  ;
  e_high_intercepts[0][24] = 241.352745362559;
  e_high_intercepts[0][25] = 248.033706817275;
  e_high_intercepts[0][26] = 287.866783253261;
  e_high_intercepts[0][27] = 269.588941588074;
  e_high_intercepts[0][28] = 267.841589437344;
  e_high_intercepts[0][29] = 278.582020194561;
  e_high_intercepts[0][30] = 280.02721910015 ;
  e_high_intercepts[0][31] = 279.438187997393;
  e_high_intercepts[0][32] = 276.190458862168;
  e_high_intercepts[0][33] = 233.694727555014;
  e_high_intercepts[0][34] = 259.371238924269;
  e_high_intercepts[0][35] = 250.639996460612;
  e_high_intercepts[0][36] = 265.941740846745;
  e_high_intercepts[0][37] = 298.177602257559;
  e_high_intercepts[0][38] = 240.593579774289;
  e_high_intercepts[0][39] = 285.483841956998;

  // High gain, front side
  // Updated Sept 2015 DR
  e_high_slopes[1][0]  = 0.337578389181782  ;
  e_high_slopes[1][1]  = 0.330179415828127  ;
  e_high_slopes[1][2]  = 0.32919855996993   ;
  e_high_slopes[1][3]  = 0.326798333250502  ;
  e_high_slopes[1][4]  = 0.331242587968144  ;
  e_high_slopes[1][5]  = 0.327460500632693  ;
  e_high_slopes[1][6]  = 0.331592766704564  ;
  e_high_slopes[1][7]  = 0.32954061866559   ;
  e_high_slopes[1][8]  = 0.329134627795873  ;
  e_high_slopes[1][9]  = 0.332850484851119  ;
  e_high_slopes[1][10] = 0.333652253672129  ;
  e_high_slopes[1][11] = 0.328797058460784  ;
  e_high_slopes[1][12] = 0.328129934763746  ;
  e_high_slopes[1][13] = 0.327760493848066  ;
  e_high_slopes[1][14] = 0.330959762745841  ;
  e_high_slopes[1][15] = 0.329234159999472  ;
  e_high_slopes[1][16] = 0.330091646943652  ;
  e_high_slopes[1][17] = 0.329325997523022  ;
  e_high_slopes[1][18] = 0.332082645143637  ;
  e_high_slopes[1][19] = 0.330044126327628  ;
  e_high_slopes[1][20] = 0.329876281234484  ;
  e_high_slopes[1][21] = 0.331673249808065  ;
  e_high_slopes[1][22] = 0.33117703162146   ;
  e_high_slopes[1][23] = 0.333242152791873  ;
  e_high_slopes[1][24] = 0.32520493524762   ;
  e_high_slopes[1][25] = 0.326965880402742  ;
  e_high_slopes[1][26] = 0.326956453984368  ;
  e_high_slopes[1][27] = 0.32500991782034   ;
  e_high_slopes[1][28] = 0.327746028007485  ;
  e_high_slopes[1][29] = 0.32984532202897   ;
  e_high_slopes[1][30] = 0.330564240518469  ;
  e_high_slopes[1][31] = 0.32276082544274   ;
  e_high_slopes[1][32] = 0.330287890663481  ;
  e_high_slopes[1][33] = 0.328452550232151  ;
  e_high_slopes[1][34] = 0.326848872119308  ;
  e_high_slopes[1][35] = 0.326622100247018  ;
  e_high_slopes[1][36] = 0.325822889873377  ;
  e_high_slopes[1][37] = 0.328179339591684  ;
  e_high_slopes[1][38] = 0.328482850878518  ;
  e_high_slopes[1][39] = 0.325742321552237  ;

  e_high_intercepts[1][0]  =  67.4122094185241 ; 
  e_high_intercepts[1][1]  =  227.74651453642  ;
  e_high_intercepts[1][2]  =  244.680404680306 ;
  e_high_intercepts[1][3]  =  295.240563049113 ;
  e_high_intercepts[1][4]  =  204.904857654253 ;
  e_high_intercepts[1][5]  =  293.338206160359 ;
  e_high_intercepts[1][6]  =  267.685846887035 ;
  e_high_intercepts[1][7]  =  234.21375217237  ;
  e_high_intercepts[1][8]  =  262.131142274891 ;
  e_high_intercepts[1][9]  =  150.403078831437 ;
  e_high_intercepts[1][10] =  158.102227214969 ;
  e_high_intercepts[1][11] =  270.637848797511 ;
  e_high_intercepts[1][12] =  282.53836238879  ;
  e_high_intercepts[1][13] =  274.401257127903 ;
  e_high_intercepts[1][14] =  220.963328752045 ;
  e_high_intercepts[1][15] =  236.45528346939  ;
  e_high_intercepts[1][16] =  216.080757577433 ;
  e_high_intercepts[1][17] =  237.18897522303  ;
  e_high_intercepts[1][18] =  182.790093793815 ;
  e_high_intercepts[1][19] =  230.822825054657 ;
  e_high_intercepts[1][20] =  210.935064282164 ;
  e_high_intercepts[1][21] =  209.401823757497 ;
  e_high_intercepts[1][22] =  200.67755741889  ;
  e_high_intercepts[1][23] =  164.033780355473 ;
  e_high_intercepts[1][24] =  299.223722552225 ;
  e_high_intercepts[1][25] =  276.815239514583 ;
  e_high_intercepts[1][26] =  274.676456382026 ;
  e_high_intercepts[1][27] =  302.262296744786 ;
  e_high_intercepts[1][28] =  258.507792975174 ;
  e_high_intercepts[1][29] =  223.546569867465 ;
  e_high_intercepts[1][30] =  248.330583972292 ;
  e_high_intercepts[1][31] =  364.594964942908 ;
  e_high_intercepts[1][32] =  212.186774037374 ;
  e_high_intercepts[1][33] =  259.813819839425 ;
  e_high_intercepts[1][34] =  271.58384214474  ;
  e_high_intercepts[1][35] =  290.06600318201  ;
  e_high_intercepts[1][36] =  299.321494959878 ;
  e_high_intercepts[1][37] =  244.018415798814 ;
  e_high_intercepts[1][38] =  231.492570840137 ;
  e_high_intercepts[1][39] =  287.21588120289  ;
 
  // Threshold values

  // Low gain

  double e_low_raw_thresholds_lower_bounds[2][40] = {{0}};
  double e_low_raw_thresholds_upper_bounds[2][40] = {{0}};

  // Low gain, back side, lower bound

  e_low_raw_thresholds_lower_bounds[0][0]  = 10;
  e_low_raw_thresholds_lower_bounds[0][1]  = 10;
  e_low_raw_thresholds_lower_bounds[0][2]  = 10;
  e_low_raw_thresholds_lower_bounds[0][3]  = 10;
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
  e_low_raw_thresholds_lower_bounds[0][16] = 10;
  e_low_raw_thresholds_lower_bounds[0][17] = 10;
  e_low_raw_thresholds_lower_bounds[0][18] = 10;
  e_low_raw_thresholds_lower_bounds[0][19] = 10;
  e_low_raw_thresholds_lower_bounds[0][20] = 10;
  e_low_raw_thresholds_lower_bounds[0][21] = 10;
  e_low_raw_thresholds_lower_bounds[0][22] = 10;
  e_low_raw_thresholds_lower_bounds[0][23] = 10;
  e_low_raw_thresholds_lower_bounds[0][24] = 10;
  e_low_raw_thresholds_lower_bounds[0][25] = 10;
  e_low_raw_thresholds_lower_bounds[0][26] = 10;
  e_low_raw_thresholds_lower_bounds[0][27] = 10;
  e_low_raw_thresholds_lower_bounds[0][28] = 10;
  e_low_raw_thresholds_lower_bounds[0][29] = 10;
  e_low_raw_thresholds_lower_bounds[0][30] = 10;
  e_low_raw_thresholds_lower_bounds[0][31] = 10;
  e_low_raw_thresholds_lower_bounds[0][32] = 10;
  e_low_raw_thresholds_lower_bounds[0][33] = 10;
  e_low_raw_thresholds_lower_bounds[0][34] = 10;
  e_low_raw_thresholds_lower_bounds[0][35] = 10;
  e_low_raw_thresholds_lower_bounds[0][36] = 10;
  e_low_raw_thresholds_lower_bounds[0][37] = 10;
  e_low_raw_thresholds_lower_bounds[0][38] = 10;
  e_low_raw_thresholds_lower_bounds[0][39] = 10;

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
  e_low_raw_thresholds_lower_bounds[1][16] = 10;
  e_low_raw_thresholds_lower_bounds[1][17] = 10;
  e_low_raw_thresholds_lower_bounds[1][18] = 10;
  e_low_raw_thresholds_lower_bounds[1][19] = 10;
  e_low_raw_thresholds_lower_bounds[1][20] = 10;
  e_low_raw_thresholds_lower_bounds[1][21] = 10;
  e_low_raw_thresholds_lower_bounds[1][22] = 10;
  e_low_raw_thresholds_lower_bounds[1][23] = 10;
  e_low_raw_thresholds_lower_bounds[1][24] = 10;
  e_low_raw_thresholds_lower_bounds[1][25] = 10;
  e_low_raw_thresholds_lower_bounds[1][26] = 10;
  e_low_raw_thresholds_lower_bounds[1][27] = 10;
  e_low_raw_thresholds_lower_bounds[1][28] = 10;
  e_low_raw_thresholds_lower_bounds[1][29] = 10;
  e_low_raw_thresholds_lower_bounds[1][30] = 10;
  e_low_raw_thresholds_lower_bounds[1][31] = 10;
  e_low_raw_thresholds_lower_bounds[1][32] = 10;
  e_low_raw_thresholds_lower_bounds[1][33] = 10;
  e_low_raw_thresholds_lower_bounds[1][34] = 10;
  e_low_raw_thresholds_lower_bounds[1][35] = 10;
  e_low_raw_thresholds_lower_bounds[1][36] = 10;
  e_low_raw_thresholds_lower_bounds[1][37] = 10;
  e_low_raw_thresholds_lower_bounds[1][38] = 10;
  e_low_raw_thresholds_lower_bounds[1][39] = 10;
  // Low gain, back side, upper bound

  e_low_raw_thresholds_upper_bounds[0][0]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][1]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][2]  = 32000;
  e_low_raw_thresholds_upper_bounds[0][3]  = 32000;
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
  e_low_raw_thresholds_upper_bounds[0][16] = 32000;
  e_low_raw_thresholds_upper_bounds[0][17] = 32000;
  e_low_raw_thresholds_upper_bounds[0][18] = 32000;
  e_low_raw_thresholds_upper_bounds[0][19] = 32000;
  e_low_raw_thresholds_upper_bounds[0][20] = 32000;
  e_low_raw_thresholds_upper_bounds[0][21] = 32000;
  e_low_raw_thresholds_upper_bounds[0][22] = 32000;
  e_low_raw_thresholds_upper_bounds[0][23] = 32000;
  e_low_raw_thresholds_upper_bounds[0][24] = 32000;
  e_low_raw_thresholds_upper_bounds[0][25] = 32000;
  e_low_raw_thresholds_upper_bounds[0][26] = 32000;
  e_low_raw_thresholds_upper_bounds[0][27] = 32000;
  e_low_raw_thresholds_upper_bounds[0][28] = 32000;
  e_low_raw_thresholds_upper_bounds[0][29] = 32000;
  e_low_raw_thresholds_upper_bounds[0][30] = 32000;
  e_low_raw_thresholds_upper_bounds[0][31] = 32000;
  e_low_raw_thresholds_upper_bounds[0][32] = 32000;
  e_low_raw_thresholds_upper_bounds[0][33] = 32000;
  e_low_raw_thresholds_upper_bounds[0][34] = 32000;
  e_low_raw_thresholds_upper_bounds[0][35] = 32000;
  e_low_raw_thresholds_upper_bounds[0][36] = 32000;
  e_low_raw_thresholds_upper_bounds[0][37] = 32000;
  e_low_raw_thresholds_upper_bounds[0][38] = 32000;
  e_low_raw_thresholds_upper_bounds[0][39] = 32000;
 
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
  e_low_raw_thresholds_upper_bounds[1][16] = 32000;
  e_low_raw_thresholds_upper_bounds[1][17] = 32000;
  e_low_raw_thresholds_upper_bounds[1][18] = 32000;
  e_low_raw_thresholds_upper_bounds[1][19] = 32000;
  e_low_raw_thresholds_upper_bounds[1][20] = 32000;
  e_low_raw_thresholds_upper_bounds[1][21] = 32000;
  e_low_raw_thresholds_upper_bounds[1][22] = 32000;
  e_low_raw_thresholds_upper_bounds[1][23] = 32000;
  e_low_raw_thresholds_upper_bounds[1][24] = 32000;
  e_low_raw_thresholds_upper_bounds[1][25] = 32000;
  e_low_raw_thresholds_upper_bounds[1][26] = 32000;
  e_low_raw_thresholds_upper_bounds[1][27] = 32000;
  e_low_raw_thresholds_upper_bounds[1][28] = 32000;
  e_low_raw_thresholds_upper_bounds[1][29] = 32000;
  e_low_raw_thresholds_upper_bounds[1][30] = 32000;
  e_low_raw_thresholds_upper_bounds[1][31] = 32000;
  e_low_raw_thresholds_upper_bounds[1][32] = 32000;
  e_low_raw_thresholds_upper_bounds[1][33] = 32000;
  e_low_raw_thresholds_upper_bounds[1][34] = 32000;
  e_low_raw_thresholds_upper_bounds[1][35] = 32000;
  e_low_raw_thresholds_upper_bounds[1][36] = 32000;
  e_low_raw_thresholds_upper_bounds[1][37] = 32000;
  e_low_raw_thresholds_upper_bounds[1][38] = 32000;
  e_low_raw_thresholds_upper_bounds[1][39] = 32000;



  // High gain

  double e_high_raw_thresholds_lower_bounds[2][40] = {{0}};
  double e_high_raw_thresholds_upper_bounds[2][40] = {{0}};

  // High gain, back side, lower bound

  e_high_raw_thresholds_lower_bounds[0][0]  = 1000; 
  e_high_raw_thresholds_lower_bounds[0][1]  = 1000; 
  e_high_raw_thresholds_lower_bounds[0][2]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][3]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][4]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][5]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][6]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][7]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][8]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][9]  = 1000;
  e_high_raw_thresholds_lower_bounds[0][10] = 1000;
  e_high_raw_thresholds_lower_bounds[0][11] = 1000;
  e_high_raw_thresholds_lower_bounds[0][12] = 1000;
  e_high_raw_thresholds_lower_bounds[0][13] = 1000;
  e_high_raw_thresholds_lower_bounds[0][14] = 1000;
  e_high_raw_thresholds_lower_bounds[0][15] = 1000;
  e_high_raw_thresholds_lower_bounds[0][16] = 1000;
  e_high_raw_thresholds_lower_bounds[0][17] = 1000;
  e_high_raw_thresholds_lower_bounds[0][18] = 1000;
  e_high_raw_thresholds_lower_bounds[0][19] = 1000;
  e_high_raw_thresholds_lower_bounds[0][20] = 1000;
  e_high_raw_thresholds_lower_bounds[0][21] = 1000;
  e_high_raw_thresholds_lower_bounds[0][22] = 1000;
  e_high_raw_thresholds_lower_bounds[0][23] = 1000;
  e_high_raw_thresholds_lower_bounds[0][24] = 1000;
  e_high_raw_thresholds_lower_bounds[0][25] = 1000;
  e_high_raw_thresholds_lower_bounds[0][26] = 1000;
  e_high_raw_thresholds_lower_bounds[0][27] = 1000;
  e_high_raw_thresholds_lower_bounds[0][28] = 1000;
  e_high_raw_thresholds_lower_bounds[0][29] = 1000;
  e_high_raw_thresholds_lower_bounds[0][30] = 1000;
  e_high_raw_thresholds_lower_bounds[0][31] = 1000;
  e_high_raw_thresholds_lower_bounds[0][32] = 1000;
  e_high_raw_thresholds_lower_bounds[0][33] = 1000;
  e_high_raw_thresholds_lower_bounds[0][34] = 1000;
  e_high_raw_thresholds_lower_bounds[0][35] = 1000;
  e_high_raw_thresholds_lower_bounds[0][36] = 1000;
  e_high_raw_thresholds_lower_bounds[0][37] = 1000;
  e_high_raw_thresholds_lower_bounds[0][38] = 1000;
  e_high_raw_thresholds_lower_bounds[0][39] = 1000;

  // High gain, front side, lower bound

  e_high_raw_thresholds_lower_bounds[1][0]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][1]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][2]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][3]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][4]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][5]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][6]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][7]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][8]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][9]  = 1000;
  e_high_raw_thresholds_lower_bounds[1][10] = 1000;
  e_high_raw_thresholds_lower_bounds[1][11] = 1000;
  e_high_raw_thresholds_lower_bounds[1][12] = 1000;
  e_high_raw_thresholds_lower_bounds[1][13] = 1000;
  e_high_raw_thresholds_lower_bounds[1][14] = 1000;
  e_high_raw_thresholds_lower_bounds[1][15] = 1000;
  e_high_raw_thresholds_lower_bounds[1][16] = 1000;
  e_high_raw_thresholds_lower_bounds[1][17] = 1000;
  e_high_raw_thresholds_lower_bounds[1][18] = 1000;
  e_high_raw_thresholds_lower_bounds[1][19] = 1000;
  e_high_raw_thresholds_lower_bounds[1][20] = 1000;
  e_high_raw_thresholds_lower_bounds[1][21] = 1000;
  e_high_raw_thresholds_lower_bounds[1][22] = 1000;
  e_high_raw_thresholds_lower_bounds[1][23] = 1000;
  e_high_raw_thresholds_lower_bounds[1][24] = 1000;
  e_high_raw_thresholds_lower_bounds[1][25] = 1000;
  e_high_raw_thresholds_lower_bounds[1][26] = 1000;
  e_high_raw_thresholds_lower_bounds[1][27] = 1000;
  e_high_raw_thresholds_lower_bounds[1][28] = 1000;
  e_high_raw_thresholds_lower_bounds[1][29] = 1000;
  e_high_raw_thresholds_lower_bounds[1][30] = 1000;
  e_high_raw_thresholds_lower_bounds[1][31] = 1000;
  e_high_raw_thresholds_lower_bounds[1][32] = 1000;
  e_high_raw_thresholds_lower_bounds[1][33] = 1000;
  e_high_raw_thresholds_lower_bounds[1][34] = 1000;
  e_high_raw_thresholds_lower_bounds[1][35] = 1000;
  e_high_raw_thresholds_lower_bounds[1][36] = 1000;
  e_high_raw_thresholds_lower_bounds[1][37] = 1000;
  e_high_raw_thresholds_lower_bounds[1][38] = 1000;
  e_high_raw_thresholds_lower_bounds[1][39] = 1000;

  // High gain, back side, upper bound

  e_high_raw_thresholds_upper_bounds[0][0]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][1]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][2]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][3]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][4]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][5]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][6]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][7]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][8]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][9]  = 27000;
  e_high_raw_thresholds_upper_bounds[0][10] = 27000;
  e_high_raw_thresholds_upper_bounds[0][11] = 27000;
  e_high_raw_thresholds_upper_bounds[0][12] = 27000;
  e_high_raw_thresholds_upper_bounds[0][13] = 27000;
  e_high_raw_thresholds_upper_bounds[0][14] = 27000;
  e_high_raw_thresholds_upper_bounds[0][15] = 27000;
  e_high_raw_thresholds_upper_bounds[0][16] = 27000;
  e_high_raw_thresholds_upper_bounds[0][17] = 27000;
  e_high_raw_thresholds_upper_bounds[0][18] = 27000;
  e_high_raw_thresholds_upper_bounds[0][19] = 27000;
  e_high_raw_thresholds_upper_bounds[0][20] = 27000;
  e_high_raw_thresholds_upper_bounds[0][21] = 27000;
  e_high_raw_thresholds_upper_bounds[0][22] = 27000;
  e_high_raw_thresholds_upper_bounds[0][23] = 27000;
  e_high_raw_thresholds_upper_bounds[0][24] = 27000;
  e_high_raw_thresholds_upper_bounds[0][25] = 27000;
  e_high_raw_thresholds_upper_bounds[0][26] = 27000;
  e_high_raw_thresholds_upper_bounds[0][27] = 27000;
  e_high_raw_thresholds_upper_bounds[0][28] = 27000;
  e_high_raw_thresholds_upper_bounds[0][29] = 27000;
  e_high_raw_thresholds_upper_bounds[0][30] = 27000;
  e_high_raw_thresholds_upper_bounds[0][31] = 27000;
  e_high_raw_thresholds_upper_bounds[0][32] = 27000;
  e_high_raw_thresholds_upper_bounds[0][33] = 27000;
  e_high_raw_thresholds_upper_bounds[0][34] = 27000;
  e_high_raw_thresholds_upper_bounds[0][35] = 27000;
  e_high_raw_thresholds_upper_bounds[0][36] = 27000;
  e_high_raw_thresholds_upper_bounds[0][37] = 27000;
  e_high_raw_thresholds_upper_bounds[0][38] = 27000;
  e_high_raw_thresholds_upper_bounds[0][39] = 27000;

  // High gain, front side, upper bound

  e_high_raw_thresholds_upper_bounds[1][0]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][1]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][2]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][3]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][4]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][5]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][6]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][7]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][8]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][9]  = 27000;
  e_high_raw_thresholds_upper_bounds[1][10] = 27000;
  e_high_raw_thresholds_upper_bounds[1][11] = 27000;
  e_high_raw_thresholds_upper_bounds[1][12] = 27000;
  e_high_raw_thresholds_upper_bounds[1][13] = 27000;
  e_high_raw_thresholds_upper_bounds[1][14] = 27000;
  e_high_raw_thresholds_upper_bounds[1][15] = 27000;
  e_high_raw_thresholds_upper_bounds[1][16] = 27000;
  e_high_raw_thresholds_upper_bounds[1][17] = 27000;
  e_high_raw_thresholds_upper_bounds[1][18] = 27000;
  e_high_raw_thresholds_upper_bounds[1][19] = 27000;
  e_high_raw_thresholds_upper_bounds[1][20] = 27000;
  e_high_raw_thresholds_upper_bounds[1][21] = 27000;
  e_high_raw_thresholds_upper_bounds[1][22] = 27000;
  e_high_raw_thresholds_upper_bounds[1][23] = 27000;
  e_high_raw_thresholds_upper_bounds[1][24] = 27000;
  e_high_raw_thresholds_upper_bounds[1][25] = 27000;
  e_high_raw_thresholds_upper_bounds[1][26] = 27000;
  e_high_raw_thresholds_upper_bounds[1][27] = 27000;
  e_high_raw_thresholds_upper_bounds[1][28] = 27000;
  e_high_raw_thresholds_upper_bounds[1][29] = 27000;
  e_high_raw_thresholds_upper_bounds[1][30] = 27000;
  e_high_raw_thresholds_upper_bounds[1][31] = 27000;
  e_high_raw_thresholds_upper_bounds[1][32] = 27000;
  e_high_raw_thresholds_upper_bounds[1][33] = 27000;
  e_high_raw_thresholds_upper_bounds[1][34] = 27000;
  e_high_raw_thresholds_upper_bounds[1][35] = 27000;
  e_high_raw_thresholds_upper_bounds[1][36] = 27000;
  e_high_raw_thresholds_upper_bounds[1][37] = 27000;
  e_high_raw_thresholds_upper_bounds[1][38] = 27000;
  e_high_raw_thresholds_upper_bounds[1][39] = 27000;

  // Calibrate the DSSD while applying the thresholds
  // Determine the channel, energy, and time of the strip that recieved the maximum energy deposition
  for (int i=0; i<2; i++){ // Sides
    for (int j=0; j<40; j++){ // Strips
      // Low gain
      //std::cout << "into dssd" << std::endl;
      if ((e_low_raw[i][j] > e_low_raw_thresholds_lower_bounds[i][j])){
        if ((e_low_raw[i][j] > e_low_raw_thresholds_upper_bounds[i][j])){
	    e_low_cal[i][j] = 32768;
            //std::cout << e_low_cal[i][j] << std::endl;
        }
        else {
	  e_low_cal[i][j] = e_low_slopes[i][j] * e_low_raw[i][j];
//          std::cout << e_low_cal[i][j] << std::endl;
        }
	//std::cout << "elow for side " << i << " strip " << j << " is " << e_low_cal[i][j] << std::endl;
	t_low_cal[i][j] = t_low_raw[i][j];// * 10; // SPECIFIC to e12001. Convert clock ticks to nanoseconds.
	
	m_low_cal[i] ++;
        double totenergy = 0;
        double stripno = 0;
        int mult = 0;
        int overflow = 0;
        int multover = 0;

	if (i==0){ // Low gain, back side

          mult+=1;
          totenergy += e_low_cal[i][j];
          stripno += j*e_low_cal[i][j];

          if (e_low_cal[0][j] == 32768) {
            overflow += j;
            multover += 1;
          }
	  
//        std::cout << e_low_cal[i][j] << ", " << e_strip_low_back << std::endl;
          if (e_low_cal[i][j] > e_strip_low_back){
	    c_strip_low_back = j;
	    e_strip_low_back = e_low_cal[i][j];
	    t_strip_low_back = t_low_cal[i][j];
//        std::cout << e_low_cal[i][j] << ", " << e_strip_low_back << std::endl;
	  }
	}

	if (i==1){ // Low gain, front side
          //std::cout << "dssd front" << std::endl;
	  
	  mult+=1;
          totenergy += e_low_cal[i][j];
          stripno += j*e_low_cal[i][j];
         
          if (e_low_cal[1][j] == 32768) {
            overflow += j;
            multover += i;
          }
//        std::cout << e_low_cal[i][j] << ", " << e_strip_low_front << std::endl;
          if (e_low_cal[i][j] > e_strip_low_front){
	    c_strip_low_front = j;
	    e_strip_low_front = e_low_cal[i][j];
	    t_strip_low_front = t_low_cal[i][j];
//        std::cout << e_low_cal[i][j] << ", " << e_strip_low_front << std::endl;
	  }
}
        //std::cout << "calculating mid" << std::endl;
        int mid = 0; 
        if (overflow == 0){ mid = int(stripno/totenergy); }
        else {
           std::cout << int(overflow/multover) << " " << int(stripno/totenergy) << " " << multover << std::endl;
           if (TMath::Abs(int(overflow/multover) - int(stripno/totenergy)) <= 1){
             mid = int(stripno/totenergy);
             std::cout << "calculating mid" << std::endl;
           }
           else {
             mid = int(overflow/multover);
             std::cout << "using overflow" << std::endl;
           }
           std::cout << "mid is " << mid << std::endl;
           if (i == 0){
             c_strip_low_back = mid;
             t_strip_low_back = t_low_cal[i][mid];
             e_strip_low_back = int(totenergy/mult);
           }
           if (i == 1){
             c_strip_low_front = mid;
             t_strip_low_front = t_low_cal[i][mid];
             e_strip_low_front = int(totenergy/mult);
           }
        }
        std::cout << e_strip_low_back << ", " << e_strip_low_front << std::endl;
      }
  
      
      // High gain
      if ((e_high_raw[i][j] > e_high_raw_thresholds_lower_bounds[i][j]) && (e_high_raw[i][j] < e_high_raw_thresholds_upper_bounds[i][j])){

	e_high_cal[i][j] = e_high_slopes[i][j] * e_high_raw[i][j] + e_high_intercepts[i][j];
	
	t_high_cal[i][j] = t_high_raw[i][j];// * 10; // SPECIFIC to e12001. Convert clock ticks to nanoseconds.
	
	m_high_cal[i] ++;

	if (i==0){ // High gain, back side

	  if (e_high_cal[i][j] > e_strip_high_back){
	    c_strip_high_back = j;
	    e_strip_high_back = e_high_cal[i][j];
	    t_strip_high_back = t_high_cal[i][j];
	  }
	}

	if (i==1){ // High gain, front side

	  if (e_high_cal[i][j] > e_strip_high_front){
	    c_strip_high_front = j;
	    e_strip_high_front = e_high_cal[i][j];
	    t_strip_high_front = t_high_cal[i][j];
	  }
	}

      }
    }
  }

}

std::ostream& operator<< (std::ostream& os, const DSSD& event){

  // Low gain

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<40; j++){
      os << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.e_low_raw[i][j] << std::setw(20) << event.e_low_cal[i][j] << std::setw(20) << event.t_low_raw[i][j] << std::setw(20) << event.t_low_cal[i][j] << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.m_low_raw[i] << std::setw(20) << event.m_low_cal[i] << std::endl;
  }

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<40; j++){
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

  std::cout << event.GetCStripLowBack() << std::endl;
  std::cout << event.GetEStripLowBack() << std::endl;
  std::cout << event.GetTStripLowBack() << std::endl;

  std::cout << event.GetCStripLowFront() << std::endl;
  std::cout << event.GetEStripLowFront() << std::endl;
  std::cout << event.GetTStripLowFront() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetTStripLowAvg() << std::endl;

  // High gain

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<40; j++){
      os << std::setw(20) << i << std::setw(3) << j << std::setw(20) << event.e_high_raw[i][j] << std::setw(20) << event.e_high_cal[i][j] << std::setw(20) << event.t_high_raw[i][j] << std::setw(20) << event.t_high_cal[i][j] << std::endl;
    }
    std::cout << std::endl;
    os << std::setw(43) << event.m_high_raw[i] << std::setw(20) << event.m_high_cal[i] << std::endl;
  }

  for (int i=0; i<2; i++){
    std::cout << std::endl;
    for (int j=0; j<40; j++){
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

  std::cout << event.GetCStripHighBack() << std::endl;
  std::cout << event.GetEStripHighBack() << std::endl;
  std::cout << event.GetTStripHighBack() << std::endl;

  std::cout << event.GetCStripHighFront() << std::endl;
  std::cout << event.GetEStripHighFront() << std::endl;
  std::cout << event.GetTStripHighFront() << std::endl;

  std::cout << std::endl;

  std::cout << event.GetTStripHighAvg() << std::endl;

  return os;

}

class SuN {

public:

  // Constructors
  SuN () {;} // Need default constuctor to create SuN object in Implantation and Decay classes
  SuN(int input_e_PMT_raw[][5][4])

  :
  
  egmPMT{{{0}}},
    erSEG{0},
    ecSEG{0}
  
  {
  
    std::memcpy(erPMT, input_e_PMT_raw, 2*5*4*sizeof(int));
    multiplicity = 0;
    total_energy = 0;

  }

  // Methods
  // Get Methods
  double GetRSegEnergy(int i) const;
  double GetCSegEnergy(int i) const;
  double GetTotalEnergy() const;
  int GetMultiplicity() const;
  // Calibration
  void GainMatchAndCalibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const SuN& event);

private:

  int erPMT[2][5][4];
  double egmPMT[2][5][4];
  double erSEG[8];
  double ecSEG[8];
  double total_energy;
  int multiplicity;

};

double SuN::GetRSegEnergy(int i) const{
  return erSEG[i];
}

double SuN::GetCSegEnergy(int i) const{
  return ecSEG[i];
}

double SuN::GetTotalEnergy() const{
  return total_energy;
}

int SuN::GetMultiplicity() const{
  return multiplicity;
}

void SuN::GainMatchAndCalibrate(){

  // Gain match the PMTs

  // Bottom PMTs
  egmPMT[0][1][1] = erPMT[0][1][1] * 1.016375;
  egmPMT[0][1][2] = erPMT[0][1][2] * 1.000000;
  egmPMT[0][1][3] = erPMT[0][1][3] * 1.008881;
  egmPMT[0][2][1] = erPMT[0][2][1] * 1.000287;
  egmPMT[0][2][2] = erPMT[0][2][2] * 1.000000;
  egmPMT[0][2][3] = erPMT[0][2][3] * 1.023810;
  egmPMT[0][3][1] = erPMT[0][3][1] * 1.021925;
  egmPMT[0][3][2] = erPMT[0][3][2] * 1.000000;
  egmPMT[0][3][3] = erPMT[0][3][3] * 0.974332;
  egmPMT[0][4][1] = erPMT[0][4][1] * 1.001665;
  egmPMT[0][4][2] = erPMT[0][4][2] * 1.000000;
  egmPMT[0][4][3] = erPMT[0][4][3] * 0.993995;
  // Top PMTs
  egmPMT[1][1][1] = erPMT[1][1][1] * 1.001665;
  egmPMT[1][1][2] = erPMT[1][1][2] * 1.000000;
  egmPMT[1][1][3] = erPMT[1][1][3] * 1.005526;
  egmPMT[1][2][1] = erPMT[1][2][1] * 1.005124;
  egmPMT[1][2][2] = erPMT[1][2][2] * 1.000000;
  egmPMT[1][2][3] = erPMT[1][2][3] * 0.996750;
  egmPMT[1][3][1] = erPMT[1][3][1] * 1.002984;
  egmPMT[1][3][2] = erPMT[1][3][2] * 1.000000;
  egmPMT[1][3][3] = erPMT[1][3][3] * 1.000164;
  egmPMT[1][4][1] = erPMT[1][4][1] * 1.013340;
  egmPMT[1][4][2] = erPMT[1][4][2] * 1.000000;
  egmPMT[1][4][3] = erPMT[1][4][3] * 1.016710;

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

  // Calibrate the SuN segments *** Updated Sept 2015 e14041 DR
  // Apply an individualized linear calibration to each segment

  // Bottom segments
  ecSEG[0] = 0.4320 * erSEG[0] - 10.158;
  ecSEG[1] = 0.4354 * erSEG[1] - 11.652;
  ecSEG[2] = 0.4293 * erSEG[2] - 11.826;
  ecSEG[3] = 0.4331 * erSEG[3] - 9.5567;
  // Top segments
  ecSEG[4] = 0.4332 * erSEG[4] - 8.6053;
  ecSEG[5] = 0.4383 * erSEG[5] - 8.0076;
  ecSEG[6] = 0.4341 * erSEG[6] - 8.3615;
  ecSEG[7] = 0.4353 * erSEG[7] - 9.7179;

  // Apply threshold (include only the positive, non-zero values of energy to avoid any offsets)

  double thresholds[8];
  thresholds[0] = 0;
  thresholds[1] = 0;
  thresholds[2] = 0;
  thresholds[3] = 0;
  thresholds[4] = 0;
  thresholds[5] = 0;
  thresholds[6] = 0;
  thresholds[7] = 0;

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


class SSSD {

public:

  // Constructors
  SSSD () {;} // Need default constuctor to create DSSD object in Implantation class
  SSSD (int input_e_SSSD_raw[][16], // double, even though energy from ddas2root is int
	double input_t_SSSD_raw[][16])

     :  
     e_SSSD_cal{0},
     t_SSSD_cal{0}

  {

    std::memcpy(e_SSSD_raw, input_e_SSSD_raw, 16*sizeof(double));
    std::memcpy(t_SSSD_raw, input_t_SSSD_raw, 16*sizeof(double));
    e_SSSD_max = 0;
    t_SSSD_max = 0;

  }

  // Methods
  // Get Methods
  double GetRawEnergy(int strip) const;
  double GetCalEnergy(int strip) const;
  double GetRawTime(int strip) const;
  double GetCalTime(int strip) const;
  double GetMaxEnergy() const;
  double GetMaxTime() const;


  // Calibration
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const SSSD& event);

private:

  double e_SSSD_raw[16];
  double e_SSSD_cal[16];
  double t_SSSD_raw[16];
  double t_SSSD_cal[16];
  double e_SSSD_max;
  double t_SSSD_max;
};

double SSSD::GetRawEnergy(int strip) const{
  return e_SSSD_raw[strip];
}

double SSSD::GetCalEnergy(int strip) const{
  return e_SSSD_cal[strip];
}

double SSSD::GetRawTime(int strip) const{
  return t_SSSD_raw[strip];
}

double SSSD::GetCalTime(int strip) const{
  return t_SSSD_cal[strip];
}

double SSSD::GetMaxEnergy() const{
  return e_SSSD_max;
}

double SSSD::GetMaxTime() const{
  return t_SSSD_max;
}

void SSSD::Calibrate(){

   //Calibration Values

   double e_SSSD_slopes[16] = {0};
   double e_SSSD_intercepts[16] = {0};

   // Slopes
   e_SSSD_slopes[0]  = 1;
   e_SSSD_slopes[1]  = 1;
   e_SSSD_slopes[2]  = 1;
   e_SSSD_slopes[3]  = 1;
   e_SSSD_slopes[4]  = 1;
   e_SSSD_slopes[5]  = 1;
   e_SSSD_slopes[6]  = 1;
   e_SSSD_slopes[7]  = 1;
   e_SSSD_slopes[8]  = 1;
   e_SSSD_slopes[9]  = 1;
   e_SSSD_slopes[10] = 1;
   e_SSSD_slopes[11] = 1;
   e_SSSD_slopes[12] = 1;
   e_SSSD_slopes[13] = 1;
   e_SSSD_slopes[14] = 1;
   e_SSSD_slopes[15] = 1;

   // Intercepts
   e_SSSD_intercepts[0]  = 1;
   e_SSSD_intercepts[1]  = 1;
   e_SSSD_intercepts[2]  = 1;
   e_SSSD_intercepts[3]  = 1;
   e_SSSD_intercepts[4]  = 1;
   e_SSSD_intercepts[5]  = 1;
   e_SSSD_intercepts[6]  = 1;
   e_SSSD_intercepts[7]  = 1;
   e_SSSD_intercepts[8]  = 1;
   e_SSSD_intercepts[9]  = 1;
   e_SSSD_intercepts[10] = 1;
   e_SSSD_intercepts[11] = 1;
   e_SSSD_intercepts[12] = 1;
   e_SSSD_intercepts[13] = 1;
   e_SSSD_intercepts[14] = 1;
   e_SSSD_intercepts[15] = 1;

   // Thresholds for SSSD
   
   double e_SSSD_raw_thresholds_lower_bounds[16] = {0};
   double e_SSSD_raw_thresholds_upper_bounds[16] = {0};
   
   e_SSSD_raw_thresholds_lower_bounds[0]  = 800;
   e_SSSD_raw_thresholds_lower_bounds[1]  = 1600;
   e_SSSD_raw_thresholds_lower_bounds[2]  = 1200;
   e_SSSD_raw_thresholds_lower_bounds[3]  = 900;
   e_SSSD_raw_thresholds_lower_bounds[4]  = 800;
   e_SSSD_raw_thresholds_lower_bounds[5]  = 750;
   e_SSSD_raw_thresholds_lower_bounds[6]  = 750;
   e_SSSD_raw_thresholds_lower_bounds[7]  = 950;
   e_SSSD_raw_thresholds_lower_bounds[8]  = 750;
   e_SSSD_raw_thresholds_lower_bounds[9]  = 750;
   e_SSSD_raw_thresholds_lower_bounds[10] = 750;
   e_SSSD_raw_thresholds_lower_bounds[11] = 750;
   e_SSSD_raw_thresholds_lower_bounds[12] = 775;
   e_SSSD_raw_thresholds_lower_bounds[13] = 800;
   e_SSSD_raw_thresholds_lower_bounds[14] = 1025;
   e_SSSD_raw_thresholds_lower_bounds[15] = 1250;
   
   e_SSSD_raw_thresholds_upper_bounds[0]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[1]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[2]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[3]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[4]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[5]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[6]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[7]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[8]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[9]  = 32000;
   e_SSSD_raw_thresholds_upper_bounds[10] = 32000;
   e_SSSD_raw_thresholds_upper_bounds[11] = 32000;
   e_SSSD_raw_thresholds_upper_bounds[12] = 32000;
   e_SSSD_raw_thresholds_upper_bounds[13] = 32000;
   e_SSSD_raw_thresholds_upper_bounds[14] = 32000;
   e_SSSD_raw_thresholds_upper_bounds[15] = 32000;

   // Calibrate the SSSD while applying the thresholds
   for (int i=0; i<16; i++){
      if ((e_SSSD_raw[i] > e_SSSD_raw_thresholds_lower_bounds[i]) && (e_SSSD_raw[i] < e_SSSD_raw_thresholds_upper_bounds[i])){
	 e_SSSD_cal[i] = e_SSSD_slopes[i] * e_SSSD_raw[i] + e_SSSD_intercepts[i]; //cal SSSD energy
	 t_SSSD_cal[i] = t_SSSD_raw[i];
	 //Find max energy
         if (e_SSSD_cal[i] > e_SSSD_max){
	    e_SSSD_max = e_SSSD_cal[i];
	 }
      }
   }
}

std::ostream& operator<< (std::ostream& os, const SSSD& event){

  for (int i=0; i<16; i++){
    std::cout << std::endl;
    
    os << std::setw(20) << i << std::setw(20) << event.e_SSSD_raw[i] << std::setw(20) << event.e_SSSD_cal[i] << std::setw(20) << event.t_SSSD_raw[i] << std::endl;
  }

  for (int i=0; i<16; i++){
    std::cout << std::endl;
    
    std::cout << std::setw(20) << i << std::setw(20) << event.GetRawEnergy(i) << std::setw(20) << event.GetCalEnergy(i) << std::setw(20) << event.GetRawTime(i) << std::endl;
    std::cout << std::endl;
  }

  std::cout << std::endl;

  std::cout << event.e_SSSD_max << std::endl;
  std::cout << event.t_SSSD_max << std::endl; 

  std::cout << std::endl;
  
  return os;

}

class NERO {

public:

  // Constructors
  NERO () {;} // Need default constuctor to create DSSD object in Implantation class
  NERO (int input_e_NERO_raw[][15],
	double input_t_NERO_raw[][15])

     :  

     e_NERO_cal{{0}},
     t_NERO_cal{{0}}

  {
    std::memcpy(e_NERO_raw, input_e_NERO_raw, 4*15*sizeof(int));
    std::memcpy(t_NERO_raw, input_t_NERO_raw, 4*15*sizeof(double));
    //e_SSSD_max = 0;
    //t_SSSD_max = 0;
    //std::cout << "Constructer" << std::endl;
  }

  // Methods
  // Get Methods
  int GetRawEnergy(int quadno, int detno) const;
  int GetCalEnergy(int quadno, int detno) const;
  double GetRawTime(int quadno, int detno) const;
  double GetCalTime(int quadno, int detno) const;
  double GetMult() const;
  //double GetMaxEnergy() const;
  //double GetMaxTime() const;


  // Calibration
  void Calibrate();
  // Diagnostics
  friend std::ostream& operator<< (std::ostream& os, const NERO& event);

private:

  int e_NERO_raw[4][15];
  int e_NERO_cal[4][15];
  double t_NERO_raw[4][15];
  double t_NERO_cal[4][15];
  double multN;

  //double e_SSSD_max;
  //double t_SSSD_max;
};

int NERO::GetRawEnergy(int quadno, int detno) const{
   return e_NERO_raw[quadno][detno];
}

int NERO::GetCalEnergy(int quadno, int detno) const{
  //std::cout << "I'm trying to get Cal E " << e_NERO_raw[quadno][detno] << std::endl;
  return e_NERO_cal[quadno][detno];
}

double NERO::GetRawTime(int quadno, int detno) const{
  return t_NERO_raw[quadno][detno];
}

double NERO::GetCalTime(int quadno, int detno) const{
  return t_NERO_cal[quadno][detno];
}

double NERO::GetMult() const{
  return multN;
}

/*double SSSD::GetMaxEnergy() const{
  return e_SSSD_max;
}

double SSSD::GetMaxTime() const{
  return t_SSSD_max;
}*/

void NERO::Calibrate(){
   //std::cout << "NERO is being calibrated" << std::endl;
   //Calibration Values
   double e_NERO_slopes[4][15] = {{0}};
   double e_NERO_intercepts[4][15] = {{0}};

   
   // Thresholds for SSSD
   
   double e_NERO_raw_thresholds_lower_bounds[4][15] = {{0}};
   double e_NERO_raw_thresholds_upper_bounds[4][15] = {{0}};
   
   e_NERO_raw_thresholds_lower_bounds[0][0]  = 1100 ;
   e_NERO_raw_thresholds_lower_bounds[0][1]  = 1100;
   e_NERO_raw_thresholds_lower_bounds[0][2]  = 1200;
   e_NERO_raw_thresholds_lower_bounds[0][3]  = 1050;
   e_NERO_raw_thresholds_lower_bounds[0][4]  = 500;
   e_NERO_raw_thresholds_lower_bounds[0][5]  = 300;
   e_NERO_raw_thresholds_lower_bounds[0][6]  = 350;
   e_NERO_raw_thresholds_lower_bounds[0][7]  = 250;
   e_NERO_raw_thresholds_lower_bounds[0][8]  = 300;
   e_NERO_raw_thresholds_lower_bounds[0][9]  = 450;
   e_NERO_raw_thresholds_lower_bounds[0][10] = 250;
   e_NERO_raw_thresholds_lower_bounds[0][11] = 250;
   e_NERO_raw_thresholds_lower_bounds[0][12] = 250;
   e_NERO_raw_thresholds_lower_bounds[0][13] = 900;
   e_NERO_raw_thresholds_lower_bounds[0][14] = 980;
   e_NERO_raw_thresholds_lower_bounds[1][0]  = 800;
   e_NERO_raw_thresholds_lower_bounds[1][1]  = 800;
   e_NERO_raw_thresholds_lower_bounds[1][2]  = 750;
   e_NERO_raw_thresholds_lower_bounds[1][3]  = 750;
   e_NERO_raw_thresholds_lower_bounds[1][4]  = 600;
   e_NERO_raw_thresholds_lower_bounds[1][5]  = 600;
   e_NERO_raw_thresholds_lower_bounds[1][6]  = 550;
   e_NERO_raw_thresholds_lower_bounds[1][7]  = 600;
   e_NERO_raw_thresholds_lower_bounds[1][8]  = 650;
   e_NERO_raw_thresholds_lower_bounds[1][9]  = 600;
   e_NERO_raw_thresholds_lower_bounds[1][10] = 600;
   e_NERO_raw_thresholds_lower_bounds[1][11] = 700;
   e_NERO_raw_thresholds_lower_bounds[1][12] = 700;
   e_NERO_raw_thresholds_lower_bounds[1][13] = 600;
   e_NERO_raw_thresholds_lower_bounds[1][14] = 200;
   e_NERO_raw_thresholds_lower_bounds[2][0]  = 550;
   e_NERO_raw_thresholds_lower_bounds[2][1]  = 530;
   e_NERO_raw_thresholds_lower_bounds[2][2]  = 550;
   e_NERO_raw_thresholds_lower_bounds[2][3]  = 550;
   e_NERO_raw_thresholds_lower_bounds[2][4]  = 500;
   e_NERO_raw_thresholds_lower_bounds[2][5]  = 500;
   e_NERO_raw_thresholds_lower_bounds[2][6]  = 600;
   e_NERO_raw_thresholds_lower_bounds[2][7]  = 550;
   e_NERO_raw_thresholds_lower_bounds[2][8]  = 500;
   e_NERO_raw_thresholds_lower_bounds[2][9]  = 600;
   e_NERO_raw_thresholds_lower_bounds[2][10] = 550;
   e_NERO_raw_thresholds_lower_bounds[2][11] = 550;
   e_NERO_raw_thresholds_lower_bounds[2][12] = 550;
   e_NERO_raw_thresholds_lower_bounds[2][13] = 550;
   e_NERO_raw_thresholds_lower_bounds[2][14] = 550;
   e_NERO_raw_thresholds_lower_bounds[3][0]  = 100;
   e_NERO_raw_thresholds_lower_bounds[3][1]  = 150;
   e_NERO_raw_thresholds_lower_bounds[3][2]  = 145;
   e_NERO_raw_thresholds_lower_bounds[3][3]  = 160;
   e_NERO_raw_thresholds_lower_bounds[3][4]  = 100;
   e_NERO_raw_thresholds_lower_bounds[3][5]  = 120;
   e_NERO_raw_thresholds_lower_bounds[3][6]  = 150;
   e_NERO_raw_thresholds_lower_bounds[3][7]  = 120;
   e_NERO_raw_thresholds_lower_bounds[3][8]  = 140;
   e_NERO_raw_thresholds_lower_bounds[3][9]  = 80;
   e_NERO_raw_thresholds_lower_bounds[3][10] = 80;
   e_NERO_raw_thresholds_lower_bounds[3][11] = 140;
   e_NERO_raw_thresholds_lower_bounds[3][12] = 750;
   e_NERO_raw_thresholds_lower_bounds[3][13] = 90;
   e_NERO_raw_thresholds_lower_bounds[3][14] = 160;
 
   e_NERO_raw_thresholds_upper_bounds[0][0]  = 6000;
   e_NERO_raw_thresholds_upper_bounds[0][1]  = 5300;
   e_NERO_raw_thresholds_upper_bounds[0][2]  = 5500;
   e_NERO_raw_thresholds_upper_bounds[0][3]  = 5200;
   e_NERO_raw_thresholds_upper_bounds[0][4]  = 2700;
   e_NERO_raw_thresholds_upper_bounds[0][5]  = 2000;
   e_NERO_raw_thresholds_upper_bounds[0][6]  = 2000;
   e_NERO_raw_thresholds_upper_bounds[0][7]  = 2000;
   e_NERO_raw_thresholds_upper_bounds[0][8]  = 1850;
   e_NERO_raw_thresholds_upper_bounds[0][9]  = 2050;
   e_NERO_raw_thresholds_upper_bounds[0][10] = 1900;
   e_NERO_raw_thresholds_upper_bounds[0][11] = 2100;
   e_NERO_raw_thresholds_upper_bounds[0][12] = 1900;
   e_NERO_raw_thresholds_upper_bounds[0][13] = 1650;
   e_NERO_raw_thresholds_upper_bounds[0][14] = 1850;
   e_NERO_raw_thresholds_upper_bounds[1][0]  = 4200;
   e_NERO_raw_thresholds_upper_bounds[1][1]  = 4200;
   e_NERO_raw_thresholds_upper_bounds[1][2]  = 4300;
   e_NERO_raw_thresholds_upper_bounds[1][3]  = 3900;
   e_NERO_raw_thresholds_upper_bounds[1][4]  = 2700;
   e_NERO_raw_thresholds_upper_bounds[1][5]  = 2100;
   e_NERO_raw_thresholds_upper_bounds[1][6]  = 1950;
   e_NERO_raw_thresholds_upper_bounds[1][7]  = 1950;
   e_NERO_raw_thresholds_upper_bounds[1][8]  = 2100;
   e_NERO_raw_thresholds_upper_bounds[1][9]  = 1900;
   e_NERO_raw_thresholds_upper_bounds[1][10] = 2150;
   e_NERO_raw_thresholds_upper_bounds[1][11] = 2500;
   e_NERO_raw_thresholds_upper_bounds[1][12] = 2600;
   e_NERO_raw_thresholds_upper_bounds[1][13] = 2400;
   e_NERO_raw_thresholds_upper_bounds[1][14] = 1700;
   e_NERO_raw_thresholds_upper_bounds[2][0]  = 2750;
   e_NERO_raw_thresholds_upper_bounds[2][1]  = 2750;
   e_NERO_raw_thresholds_upper_bounds[2][2]  = 2750;
   e_NERO_raw_thresholds_upper_bounds[2][3]  = 2850;
   e_NERO_raw_thresholds_upper_bounds[2][4]  = 2700;
   e_NERO_raw_thresholds_upper_bounds[2][5]  = 2950;
   e_NERO_raw_thresholds_upper_bounds[2][6]  = 2850;
   e_NERO_raw_thresholds_upper_bounds[2][7]  = 2700;
   e_NERO_raw_thresholds_upper_bounds[2][8]  = 2650;
   e_NERO_raw_thresholds_upper_bounds[2][9]  = 2350;
   e_NERO_raw_thresholds_upper_bounds[2][10] = 2900;
   e_NERO_raw_thresholds_upper_bounds[2][11] = 2700;
   e_NERO_raw_thresholds_upper_bounds[2][12] = 2700;
   e_NERO_raw_thresholds_upper_bounds[2][13] = 2900;
   e_NERO_raw_thresholds_upper_bounds[2][14] = 2600;
   e_NERO_raw_thresholds_upper_bounds[3][0]  = 760 ;
   e_NERO_raw_thresholds_upper_bounds[3][1]  = 1070;
   e_NERO_raw_thresholds_upper_bounds[3][2]  = 1000;
   e_NERO_raw_thresholds_upper_bounds[3][3]  = 1120;
   e_NERO_raw_thresholds_upper_bounds[3][4]  = 380 ;
   e_NERO_raw_thresholds_upper_bounds[3][5]  = 580 ;
   e_NERO_raw_thresholds_upper_bounds[3][6]  = 1050;
   e_NERO_raw_thresholds_upper_bounds[3][7]  = 640 ;
   e_NERO_raw_thresholds_upper_bounds[3][8]  = 980 ;
   e_NERO_raw_thresholds_upper_bounds[3][9]  = 320 ;
   e_NERO_raw_thresholds_upper_bounds[3][10] = 370 ;
   e_NERO_raw_thresholds_upper_bounds[3][11] = 1050;
   e_NERO_raw_thresholds_upper_bounds[3][12] = 2500;
   e_NERO_raw_thresholds_upper_bounds[3][13] = 340 ;
   e_NERO_raw_thresholds_upper_bounds[3][14] = 950 ;


   // Calibrate the SSSD while applying the thresholds
   for (int i=0; i<4; i++){
    for (int j=0; j<15; j++){
      //if(e_NERO_raw[i][j] > 1){std::cout << "e_NERO_raw is " << e_NERO_raw[i][j] << std::endl;}
      if ((e_NERO_raw[i][j] > e_NERO_raw_thresholds_lower_bounds[i][j]) && (e_NERO_raw[i][j] < e_NERO_raw_thresholds_upper_bounds[i][j])){
	 e_NERO_cal[i][j] = e_NERO_raw[i][j]; //cal SSSD energy
	 t_NERO_cal[i][j] = t_NERO_raw[i][j];
         multN ++;
         //std::cout << "NERO tube "<< j << " in quad " << i << " is being calibrated" << std::endl;
         //std::cout << "NERO time = " << t_NERO_cal[i][j] << std::endl;
         //std::cout << "NERO energy = " << e_NERO_cal[i][j] << std::endl;
	 //Find max energy
         //if (e_SSSD_cal[i] > e_SSSD_max){
	 //   e_SSSD_max = e_SSSD_cal[i];
	 //}
      }
    }
   }
}

std::ostream& operator<< (std::ostream& os, const NERO& event){
 for (int i=0; i<4; i++){
  for (int j=0; j<15; j++){
    std::cout << std::endl;
    
    os << std::setw(20) << i << std::setw(20) << j << std::setw(20) << event.e_NERO_raw[i][j] << std::setw(20) << event.e_NERO_cal[i][j] << std::setw(20) << event.t_NERO_raw[i] << std::endl;
  }
 }

 for (int i=0; i<4; i++){
  for (int j=0; j<15; j++){
    std::cout << std::endl;
    
    std::cout << std::setw(20) << i << std::setw(20) << event.GetRawEnergy(i,j) << std::setw(20) << event.GetCalEnergy(i,j) << std::setw(20) << event.GetRawTime(i,j) << std::endl;
    std::cout << std::endl;
  }
 }
  //std::cout << std::endl;

  //std::cout << event.e_NERO_max << std::endl;
  //std::cout << event.t_NERO_max << std::endl; 

  //std::cout << std::endl;
  
  return os;

}

class SCINT {

public:

   // Constructors
   SCINT () {;} //Default constructor
   SCINT (double e_Scint_raw, double t_Scint_raw)

     :
 
     e_Scint_raw(e_Scint_raw),
     t_Scint_raw(t_Scint_raw)

   {e_Scint_raw = 0;
    t_Scint_raw = 0;}

   //Get Methods
   double GetScintRawEnergy() const;
   double GetScintCalEnergy() const;
   double GetScintRawTime() const;
   double GetScintCalTime() const;

   //Calibrate
   void Calibrate();

private:

   double e_Scint_raw;
   double e_Scint_cal;
   double t_Scint_raw;
   double t_Scint_cal;
};

double SCINT::GetScintRawEnergy() const{
   return e_Scint_raw;
}

double SCINT::GetScintCalEnergy() const{
   return e_Scint_cal;
}

double SCINT::GetScintRawTime() const{
   return t_Scint_raw;
}

double SCINT::GetScintCalTime() const{
   return t_Scint_cal;
}

void SCINT::Calibrate(){

   double e_scint_lowthres = 0;
   double e_scint_hithres = 0;

   e_scint_lowthres = 225;
   e_scint_hithres = 32000;

   //Apply threshold conditions
   if (e_Scint_raw > e_scint_lowthres && e_Scint_raw < e_scint_hithres) {
     e_Scint_cal = e_Scint_raw;
     t_Scint_cal = t_Scint_raw;

   }
}

class Punchthrough {

public:

  //Constructors
  Punchthrough (PIN pin_sub_event,
	 	SSSD sssd_sub_event,
		DSSD dssd_sub_event,
		SCINT scint_sub_event)
    :
    
    pin_sub_event(pin_sub_event),
    sssd_sub_event(sssd_sub_event),
    dssd_sub_event(dssd_sub_event),
    scint_sub_event(scint_sub_event)

  {;}

  PIN pin_sub_event;
  SSSD sssd_sub_event;
  DSSD dssd_sub_event;
  SCINT scint_sub_event;

  //Methods
  bool IsPunchthrough();

private:

};

bool Punchthrough::IsPunchthrough(){

   if(/*pin_sub_event.GetPIN3RawEnergy() == 32767 &&*/
      sssd_sub_event.GetMaxEnergy() != 0 &&
      dssd_sub_event.GetEStripLowBack()  > 0 &&
      dssd_sub_event.GetEStripLowFront() > 0){// &&
//      scint_sub_event.GetScintCalEnergy()!=0) {
    return true;
   }
   else {
    return false;
   }
}

class Implantation {

	public:

	  // Constructors
	  Implantation () : is_implanted(false), time_between_back_to_back_implantations(0) {;} // Need default constructor to create SuN DSSD pixels in main. Note that is_implanted must be initialized to false so that when dssd_implantation_pixels is created in main, all pixels are initialized to false. You can test this by removing this here and couting "dssd_implantation_pixels[j][k].IsImplanted()" in the event loop and seeing that some pixels have nonsensical Boolean values
	  Implantation (TAC tac_sub_event,
			PIN pin_sub_event,
			DSSD dssd_sub_event,
			SSSD sssd_sub_event)

	    :
	    
	    tac_sub_event(tac_sub_event),
	    pin_sub_event(pin_sub_event),
	    dssd_sub_event(dssd_sub_event),
	    sssd_sub_event(sssd_sub_event)

	  {
	    
	    time_between_back_to_back_implantations = 0;
	    is_implanted = false;

	  }

	  // The objects which make up an implantation and/or needed to identify one
	  TAC tac_sub_event;
	  PIN pin_sub_event;
	  DSSD dssd_sub_event;
	  SSSD sssd_sub_event;

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

	  /*std::cout << "Pin1Raw = " << pin_sub_event.GetPIN1RawEnergy() << " Pin2Raw = " << pin_sub_event.GetPIN2RawEnergy() << 
		       " StripLowBack = " << dssd_sub_event.GetEStripLowBack() << " StripLowFront = " << dssd_sub_event.GetEStripLowFront() <<
		       " SSSDEnergy = " <<  sssd_sub_event.GetRawEnergy() << std::endl;
	  */
	  if (pin_sub_event.GetPIN1RawEnergy() > 0 &&
	      pin_sub_event.GetPIN2RawEnergy() > 0 &&
	      dssd_sub_event.GetEStripLowBack() > 0 &&
	      dssd_sub_event.GetEStripLowFront() > 0){
//	      sssd_sub_event.GetMaxEnergy() <32000){
	    return true;
	  }

	  else {
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
	 NERO nero_sub_event)
    
    :

    pin_sub_event(pin_sub_event),
    dssd_sub_event(dssd_sub_event),
    nero_sub_event(nero_sub_event)

  {;}

  // The objects which make up a decay and/or needed to identify one
  PIN pin_sub_event;
  DSSD dssd_sub_event;
  NERO nero_sub_event;
  
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
      dssd_sub_event.GetEStripHighBack() > 0 &&
      dssd_sub_event.GetEStripHighFront() > 0) {
/*  if (dssd_sub_event.GetEStripHighBack() > 0 &&
      dssd_sub_event.GetEStripHighFront() > 0) {*/
    return true;
  }
  else {
    return false;
  }

}

