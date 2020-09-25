//GM for e17028
#include "TChain.h"

void apply_PIN_gain_matching(TChain* chain){
  
  // Gain match the PMTs
  // Bottom PMTs
  
  chain->SetAlias("PIN1", "energy_PIN1 * 1.06637654974");
  chain->SetAlias("PIN2", "energy_PIN2 * 1.0");

  return;
}
