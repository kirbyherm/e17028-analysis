//GM for e17028
#include "TChain.h"

void apply_SuN_gain_matching(TChain* chain){
  
  // Gain match the PMTs
  // Bottom PMTs
  
  chain->SetAlias("B11", "E_SuN[0][1][1] * 1.0190251353978614");
  chain->SetAlias("B12", "E_SuN[0][1][2] * 1.0329616918285158");
  chain->SetAlias("B13", "E_SuN[0][1][3] * 1.0105342511952704");
  chain->SetAlias("B21", "E_SuN[0][2][1] * 1.0342809530422363");
  chain->SetAlias("B22", "E_SuN[0][2][2] * 1.0327434681691037");
  chain->SetAlias("B23", "E_SuN[0][2][3] * 1.0309877596365586");
  chain->SetAlias("B31", "E_SuN[0][3][1] * 1.0040172991846372");
  chain->SetAlias("B32", "E_SuN[0][3][2] * 1.012498264129982");
  chain->SetAlias("B33", "E_SuN[0][3][3] * 1.0256611184954472");
  chain->SetAlias("B41", "E_SuN[0][4][1] * 1.0105838474815005");
  chain->SetAlias("B42", "E_SuN[0][4][2] * 0.9886622889678022");
  chain->SetAlias("B43", "E_SuN[0][4][3] * 1.010196996448906");
                                                     
  // Top PMTs                                        
                                                     
  chain->SetAlias("T11", "E_SuN[1][1][1] * 1.0009820064673558");
  chain->SetAlias("T12", "E_SuN[1][1][2] * 0.9984426766123753");
  chain->SetAlias("T13", "E_SuN[1][1][3] * 0.9803658222072331");
  chain->SetAlias("T21", "E_SuN[1][2][1] * 0.9954867379530621");
  chain->SetAlias("T22", "E_SuN[1][2][2] * 1.0");
  chain->SetAlias("T23", "E_SuN[1][2][3] * 0.9952189180074197");
  chain->SetAlias("T31", "E_SuN[1][3][1] * 0.9975598627174797");
  chain->SetAlias("T32", "E_SuN[1][3][2] * 0.9980955026087647");
  chain->SetAlias("T33", "E_SuN[1][3][3] * 0.982381415279624 ");
  chain->SetAlias("T41", "E_SuN[1][4][1] * 0.9831045291328585");
  chain->SetAlias("T42", "E_SuN[1][4][2] * 0.9699089412184816");
  chain->SetAlias("T43", "E_SuN[1][4][3] * 0.9642599242168747");

  // Sum the PMTs to create a SuN segment

  // Bottom segments

  chain->SetAlias("E_SuNB1", "B11 + B12 + B13");
  chain->SetAlias("E_SuNB2", "B21 + B22 + B23");
  chain->SetAlias("E_SuNB3", "B31 + B32 + B33");
  chain->SetAlias("E_SuNB4", "B41 + B42 + B43");

  // Top segments

  chain->SetAlias("E_SuNT1", "T11 + T12 + T13");
  chain->SetAlias("E_SuNT2", "T21 + T22 + T23");
  chain->SetAlias("E_SuNT3", "T31 + T32 + T33");
  chain->SetAlias("E_SuNT4", "T41 + T42 + T43");

}
