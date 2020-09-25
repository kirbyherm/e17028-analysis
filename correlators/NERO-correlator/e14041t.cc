////////////	Loops over all e14041 rootfiles and the tree t and creates
////////////    a new root file with the tree of events in a given cut.
////////////    Re
////////////    Compile with 'make e14041t'
////////////    Run with './e14041t /path/to/inputlist inputcuts.root outputfile.root


#include <iostream>
#include <iomanip>
#include <string.h> 
//#include <sys/time.h>
#include <signal.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2S.h"
#include "TH1S.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TMath.h"
#include "TCutG.h"
#include "TEnv.h"
#include "TKey.h"
#include "TDirectory.h"

using namespace TMath;
using namespace std;


bool signal_received = false;
void signalhandler(int sig){
  if (sig == SIGINT){
    signal_received = true;
  }
}

int main(int argc, char* argv[]){
  signal(SIGINT,signalhandler);

  FILE* infile = fopen(argv[1],"r");
  vector<string> chainfiles;
  char buffer[55];
  while(!feof(infile))
   {
	fgets(buffer,55,infile);
 	int len = strlen(buffer);
	if( buffer[len-1] == '\n' )
	 buffer[len-1] = 0;
	chainfiles.push_back(buffer);
   }
  cout << "I found " << chainfiles.size()-1 << " root files in " << argv[1] << endl;
  fclose(infile);

  TChain* tr = new TChain("t");
  for(uint i = 0; i < chainfiles.size()-1 ; i++)
  tr->Add(chainfiles.at(i).c_str());
  tr->LoadTree(0);
  Double_t nentries = tr->GetEntries();
  cout << "I have: " << nentries << " events in the tree." << endl;

  //Put before-loop actions here.
  //////////////// INIT CUTS
  //cout << "Initialise the Incoming cuts" << endl;
  //TFile* thefile = new TFile(argv[2]);
  //TCutG *incoming_cut;
  //thefile->GetObject("V61",incoming_cut);
  //thefile->Close();
  //thefile->Delete();
  //cout << "I found " << incoming_cut->GetName() << " in the file." << endl;
  //cout << "Init the Outgoing cuts" << endl;
  //thefile->Close();
  //thefile->Delete();

  ///////////////

  TFile *newfile = new TFile(argv[2],"recreate");
  TTree *newtree = tr->CloneTree(0); 

  for(int i=0; i<nentries;i++){
    if(signal_received){
      break;
    }
    tr->GetEntry(i);
    
    if (i%100000 == 0) { cout << "Progress tracker: at event " << i << endl; }
    //if(incoming_cut->IsInside((tof+(tofcor-11156.5)/2.3011), (de-(tofcor-13327.7)/12.3372))){
       newtree->Fill();
       //cout << "Filled " << i << endl;
    //}

  }
  cout << endl;  
  newtree->AutoSave();
  TH1F *cflags;
  for (int l = 0; l < chainfiles.size()-1; l++){
    TFile *fIn = TFile::Open(chainfiles.at(l).c_str());
    TH1F *correlation_flags = (TH1F*)fIn->Get("correlation_flags");
    if (l ==0){cflags = (TH1F*)correlation_flags->Clone("cflags");}
    else { 
       cflags->Add(correlation_flags,1);
    }
  }
  newfile->cd();
  cflags->Write();
  newfile->Close();
  delete tr;

  return 0;
}
