#include "TH1F.h";

using namespace std;

Double_t binno[250];
Double_t stuffs[250];
Double_t errorc[250];

Double_t fitfunc(double x, Double_t *par){
  //Flat bg
  Double_t bg = par[0];
  
  //Exponential decay of parent
  Double_t dec = par[1]*TMath::Exp(-par[2]*x);

  return (dec+bg);
}

Double_t background(Double_t *x, Double_t *par){
  Double_t bg = par[0];
  return bg;
} 

Double_t parent(Double_t *x, Double_t *par){
  Double_t parentV = par[0]*TMath::Exp(-par[1]*x[0]/2.);
  return parentV;
} 

Double_t totalfit(Double_t *x, Double_t *par){
  Double_t totfitV = par[0] + par[1]*TMath::Exp(-par[2]*x[0]/2.);
  return totfitV;
}
static Double_t fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)   /* This calculates Xi square */
{

   Int_t i;

   //calculate chisquare
   Double_t chisq = 0;
   Double_t delta;
   for (i=0;i<250;i++) {
     binno[i] = i+0.5;
     if(errorc[i] >0) 
       {
         delta = (stuffs[i]-fitfunc(binno[i],par))/errorc[i] /*(TMath::Sqrt(func(i+(bin/2),par)))*/ ;    
       }
     else delta = 0;
     chisq += delta*delta;
     }
   f = chisq;
   return f;
}

void main(){
  TFile::Open("merged_cutV.root");
  ft->Draw("correlation_implant_decay_time>>h(250,0,500000000)","SuN_TAS_high_gain_cal > 1000 && SuN_TAS_high_gain_cal < 1050");
  TH1F *dat = new TH1F("dat","dat",250,0,500);
  for (int i = 0; i<250; i++){
    dat->SetBinContent(i+1, h->GetBinContent(i+1));
    stuffs[i] = h->GetBinContent(i+1);
    if(h->GetBinContent(i+1) > 0){
      errorc[i] = h->GetBinError(i+1);
    }
    else{errorc[i] = 0;}
  }
  dat->Draw();
  
  TMinuit *gMinuit = new TMinuit(3);
  gMinuit->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg=0;
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR",arglist,1,ierflg);

  gMinuit->mnparm(0,"Background",0,0.01,0,200,ierflg);
  gMinuit->mnparm(1,"A0",0,0.01,0,500,ierflg);
  gMinuit->mnparm(2,"Decay constant",0,0.001,0,1,ierflg);

  arglist[0] = 10000;
  arglist[1] = 0.01;
  
  gMinuit->mnexcm("MIGRAD",arglist,2,ierflg);
  Double_t amin, edm, errdef;
  Int_t nvpar, nparx, icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(3,amin);

  Double_t BG[2];
  gMinuit->GetParameter(0,BG[0],BG[1]);
  
  Double_t A0[2];
  gMinuit->GetParameter(1,A0[0],A0[1]);
  
  Double_t ParentT[2];
  gMinuit->GetParameter(2,ParentT[0],ParentT[1]);
  
  TF1 *dparent = new TF1("V-61",parent,0,500,2);
  dparent->SetParameter(0,A0[0]);
  dparent->SetParameter(1,ParentT[0]);
  
  TF1 *backg = new TF1("Background",background,0,500,1);
  backg->SetParameter(0,BG[0]);

  TF1 *totalrate = new TF1("Total Fit",totalfit,0,500,3);
  totalrate->SetParameter(0,BG[0]);
  totalrate->SetParameter(1,A0[0]);
  totalrate->SetParameter(2,ParentT[0]);

  dat->Draw();
  TH1F *datclone = (TH1F*)dat->Clone("datclone");
  datclone->Draw("Same");
  backg->Draw("same");
  dparent->Draw("same");
  totalrate->Draw("same");
  
}
