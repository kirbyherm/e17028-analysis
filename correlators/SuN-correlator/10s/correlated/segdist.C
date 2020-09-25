segdist(){
	TFile::Open("merged_cutV.root");
	double energy_seg[8] = {0};
	vector<int> hitseg;
	ft->SetBranchAddress("SuN_Seg_high_gain_cal", &energy_seg);
        double energy_tas = 0;
	ft->SetBranchAddress("SuN_TAS_high_gain_cal", &energy_tas);
	double estrip =0;
	ft->SetBranchAddress("eStripHighFront", &estrip);
	double dt =0;
	ft->SetBranchAddress("correlation_implant_decay_time", &dt);
	TH1F *gtas = new TH1F("gtas","gtas",1000,1,10000);
        TH2F *distplot = new TH2F("distplot","distplot",8,0,8,8,0,8);
        TH3F *dist3plot = new TH3F("dist3plot","dist3plot",8,0,8,8,0,8,8,0,8);
	int numentries = ft->GetEntries();
	for (int i =0; i<numentries;i++){
	//for (int i =0; i<10000;i++){
	   ft->GetEntry(i);
	   for(int a =0; a<8; a++){
	      if (energy_seg[a] > 1 && estrip <2000 && dt< 100000000){
	         hitseg.push_back(a);
	      }
	   }
           if (hitseg.size() > 1){
	      distplot->Fill(hitseg[0],hitseg[1]);
	      if(TMath::Abs(hitseg[0]%4 - hitseg[1]%4) <= 1){
	        gtas->Fill(energy_tas);
              }
           }   
           /*if (hitseg.size() > 2){
	      dist3plot->Fill(hitseg[0],hitseg[1],hitseg[2]);
           }*/   
	   hitseg.clear();
        }
	TCanvas *c1 = new TCanvas();
	c1->Divide(2,1);
	c1->cd(1);
	distplot->Draw();	
	c1->cd(2);
	//dist3plot->Draw();
	gtas->SetLineColor(2);
	//gtas->Scale(10);
	gtas->Draw();
	ft->Draw("SuN_TAS_high_gain_cal>>h(1000,1,10000)","correlation_implant_decay_time < 100000000 && eStripHighFront <2000","same");

}
