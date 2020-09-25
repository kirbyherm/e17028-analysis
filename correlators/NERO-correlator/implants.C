// Alex Dombos
// 2016-03-18
// Purpose: To create histograms of SuNs PMTs

#include "TFile.h"
#include "TChain.h"
#include "TH1F.h"

#include <iostream>
#include <string>
//#include "cuts.h"
//#include "../src/PIN_gain_matching.h"
void save_to_png( TCanvas *c1, std::string name ){

    TImage *img = TImage::Create();
    img->FromPad(c1);
    img->WriteImage(Form("%s.png",name.c_str()));
    return;
}

void implants(){

  // Input ROOT file
    TChain* chain = new TChain("t");
    for (int i=1036; i<1069; i++)
//    for (int i=1051; i<1060; i++)
    {
        std::string run_str = std::to_string(i);
//        std::string filepath = "/mnt/analysis/e17028/rootfiles/run" + run_str + "-*.root";
        std::string filepath = "/mnt/analysis/e17028/correlated_rootfiles_NERO/1000ms/true/Run" + run_str + "_true_3_md_0_v61.root";
        const char * c = filepath.c_str();
        std::cout << filepath << std::endl;
        chain->Add(c);
    } 
//    apply_PIN_gain_matching(chain);
//    cuts();
    std::string PIN2TOF = "(";
    for (int i=0; i<16; i++){
        PIN2TOF += "energy_BCSSSSD_high[1]["+std::to_string(i)+"]";
        if (i < 15)
            PIN2TOF += "&&";
        }
    PIN2TOF += ")";
    std::cout<<PIN2TOF<<std::endl;
    std::vector<std::string> I2 = {"N","S"};
    std::string ar_iso = "ar49||ar50||ar51";
    std::string k_iso = "k51||k52||k53";
    std::string ca_iso = "ca52||ca53||ca54||ca55";
    std::string sc_iso = "sc55||sc56||sc57||sc58";
    std::string ti_iso = "ti57||ti58||ti59||ti60";
    std::string v_iso= "v59||v60||v61||v62";
    std::string cr_iso = "cr61||cr62||cr63||cr64";
    std::string mn_iso = "mn63||mn64||mn65";//||mn65";
    std::string iso = ar_iso + "||" + k_iso + "||" + ca_iso + "||" + sc_iso + "||" + ti_iso + "||" + v_iso + "||" + cr_iso + "||" + mn_iso;
    iso = "cr62";
    {{{
    TCanvas *c2 = new TCanvas("c2"); 
    double correction_factor = 0.8;
    for(int i=0; i<1; i++){
    int tof_low = correction_factor * 3300+8000, tof_hi = correction_factor*4700+18000;
    tof_low = 8000, tof_hi = 19000;
//    chain->Draw(Form("PIN2:(energy_TAC_PIN2_I2S+(energy_TAC_I2N_I2S*%f)+1500)>>pid_pin12(1000,%d,%d,1000,5500,15000)",correction_factor,tof_low,tof_hi),Form("pin12",PIN2TOF.c_str()),"colz");
//    TH2F* pid_pin12 = ((TH2F*) gDirectory->Get("pid_pin12"));
//    double pid_pin12_total = pid_pin12->Integral(); 
//    save_to_png(c2, "pid_pin12");    
    c2->Clear();
//    chain->Draw(Form("PIN2:(energy_TAC_PIN2_I2S+(energy_TAC_I2N_I2S*%f)+1500)>>pid_i2pos(1000,%d,%d,1000,5500,15000)",correction_factor,tof_low,tof_hi),Form("i2pos",PIN2TOF.c_str()),"colz");
//    TH2F* pid_i2pos = ((TH2F*) gDirectory->Get("pid_i2pos"));
//    double pid_i2pos_total = pid_i2pos->Integral(); 
//    save_to_png(c2, "pid_i2pos");    
    c2->Clear();
//    chain->Draw(Form("PIN2:(energy_TAC_PIN2_I2S+(energy_TAC_I2N_I2S*%f)+1500)>>pid_pin2tof(1000,%d,%d,1000,5500,15000)",correction_factor,tof_low,tof_hi),Form("%s",PIN2TOF.c_str()),"colz");
//    TH2F* pid_pin2tof = ((TH2F*) gDirectory->Get("pid_pin2tof"));
//    double pid_pin2tof_total = pid_pin2tof->Integral(); 
//    save_to_png(c2, "pid_pin2tof");    
    c2->Clear();
    chain->Draw(("cStripLowBack:cStripLowFront>>implants(40,1,41,40,1,41)"),(""),"colz");
    TH2F* pid_final = ((TH2F*) gDirectory->Get("implants"));
//    c2->SetLogz();
//    c2->SetLogy();
//    chain->Draw(("energy_BCSSSSD_high[1][]>>implants(1000,0,40000)"),("pin12&&i2pos&&ti57 && cBCSStripLowBack>0 && cBCSStripLowFront>0"),"colz");
//    TH1F* pid_final = ((TH1F*) gDirectory->Get("implants"));
    save_to_png(c2, "implants");    
    c2->Clear();
    correction_factor+=0.1;
    }
//    double pid_final_total = pid_final->Integral(); 
    TCanvas *c3 = new TCanvas("c3"); 
//    chain->Draw("energy_PIN2>>pin2(3000,1,17001)","","");
 //   TH1F* pin2 = ((TH1F*) gDirectory->Get("pin2"));
    int lower_bin = (3000.0/17000.0*8000.0), upper_bin = (3000.0/17000.0*16000.0);
    printf("lower_bin: %d, upper_bin: %d\n",lower_bin,upper_bin);
//    double pin2_total = pin2->Integral(lower_bin, upper_bin);
//    printf("pin2: %f\tpid_pin12: %f\t efficiency: %f\n",pin2_total,pid_pin12_total,pid_pin12_total/pin2_total);
//    printf("pin2: %f\tpid_i2pos: %f\t efficiency: %f\n",pin2_total,pid_i2pos_total,pid_i2pos_total/pin2_total);
//    printf("pin2: %f\tpid_pin2tof: %f\t efficiency: %f\n",pin2_total,pid_pin2tof_total,pid_pin2tof_total/pin2_total);
//    printf("pin2: %f\tpid: %f\t efficiency: %f\n",pin2_total,pid_final_total,pid_final_total/pin2_total);

    }}} 

}

