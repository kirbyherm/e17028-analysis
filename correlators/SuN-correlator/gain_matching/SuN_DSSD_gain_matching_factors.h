//GM for e17028
#include "TChain.h"

void apply_SuN_DSSD_gain_matching(TChain* chain){
  
  // Gain match the PMTs
  // Bottom PMTs
  
  chain->SetAlias("Bhi0", "energy_DSSD_high[0][0]   * 0.6687374047713248");
  chain->SetAlias("Bhi1", "energy_DSSD_high[0][1]   * 0.6489024695472295");
  chain->SetAlias("Bhi2", "energy_DSSD_high[0][2]   * 0.6710619650451675");
  chain->SetAlias("Bhi3", "energy_DSSD_high[0][3]   * 0.4610404961634933");
  chain->SetAlias("Bhi4", "energy_DSSD_high[0][4]   * 0.6482510475286352");
  chain->SetAlias("Bhi5", "energy_DSSD_high[0][5]   * 0.6569297266121172");
  chain->SetAlias("Bhi6", "energy_DSSD_high[0][6]   * 0.6780774267775947");
  chain->SetAlias("Bhi7", "energy_DSSD_high[0][7]   * 0.6291257980513585");
  chain->SetAlias("Bhi8", "energy_DSSD_high[0][8]   * 0.6839820793042485");
  chain->SetAlias("Bhi9", "energy_DSSD_high[0][9]   * 0.6638067658030719");
  chain->SetAlias("Bhi10", "energy_DSSD_high[0][10] * 0.661996233236323 ");
  chain->SetAlias("Bhi11", "energy_DSSD_high[0][11] * 0.6727887262621948");
  chain->SetAlias("Bhi12", "energy_DSSD_high[0][12] * 0.6238726239718508");
  chain->SetAlias("Bhi13", "energy_DSSD_high[0][13] * 0.6619876526570083");
  chain->SetAlias("Bhi14", "energy_DSSD_high[0][14] * 0.6627720516233344");
  chain->SetAlias("Bhi15", "energy_DSSD_high[0][15] * 0.6543432312208095");
                                                     
  // Top PMTs                                        
                                                     
  chain->SetAlias("Fhi0", "energy_DSSD_high[1][0]   * 1.0074361880880645");
  chain->SetAlias("Fhi1", "energy_DSSD_high[1][1]   * 0.9976171264235845");
  chain->SetAlias("Fhi2", "energy_DSSD_high[1][2]   * 1.010210722168577 ");
  chain->SetAlias("Fhi3", "energy_DSSD_high[1][3]   * 1.0051475244360755");
  chain->SetAlias("Fhi4", "energy_DSSD_high[1][4]   * 1.0119555479439477");
  chain->SetAlias("Fhi5", "energy_DSSD_high[1][5]   * 1.0252867281502107");
  chain->SetAlias("Fhi6", "energy_DSSD_high[1][6]   * 1.0               ");
  chain->SetAlias("Fhi7", "energy_DSSD_high[1][7]   * 1.020019925521865 ");
  chain->SetAlias("Fhi8", "energy_DSSD_high[1][8]   * 1.0099623359809387");
  chain->SetAlias("Fhi9", "energy_DSSD_high[1][9]   * 1.0119754963536152");
  chain->SetAlias("Fhi10", "energy_DSSD_high[1][10] * 1.0122986933411844");
  chain->SetAlias("Fhi11", "energy_DSSD_high[1][11] * 1.0063178177544727");
  chain->SetAlias("Fhi12", "energy_DSSD_high[1][12] * 0.9846241306629423");
  chain->SetAlias("Fhi13", "energy_DSSD_high[1][13] * 1.0036198732685877");
  chain->SetAlias("Fhi14", "energy_DSSD_high[1][14] * 1.0395193050456772");
  chain->SetAlias("Fhi15", "energy_DSSD_high[1][15] * 0.9918296732691634");


}
