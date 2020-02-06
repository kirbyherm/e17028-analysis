#!/usr/bin/env python
#SBATCH --output=root_plot.txt
#SBATCH -n 1

from ROOT import TFile, TChain, TH1F, TCanvas


def chain_runs():

    chain = TChain("t")
    for i in range(2045,2171):
#    for i in range(2026,2169):
        run_str = str(i)
        filepath = "/mnt/analysis/e17028/rootfiles/run" + run_str + "-*.root"
#        c = filepath.c_str()
#        std::cout << filepath << std::endl
        c = filepath
        chain.Add(c)

    
    correction_factor = -1.3
    c1 = TCanvas("c1") 
    c1.SetLogz()
    chain.Draw("energy_PIN1:(energy_TAC_PIN1_I2N+(energy_TAC_I2N_I2S*1.3)+1500)>>pid_corr(1000,14000,21000,1000,4000,8500)","","colz")
#    pid_corr
#    c2 = TCanvas("c2") 
#    c2.SetLogz()
#    chain.Draw("energy_TAC_PIN1_I2N:energy_TAC_I2N_I2S>>pid(500,2000,7000,500,6000,14000)","","colz")
    c1.SetTitle("Particle ID for 5 consecutive runs")
    
    c1.SaveAs("c1.png")
#    c1.SaveAs("c1.png")
    t = chain.GetTree()

  
def main():

    chain_runs()
    print("hello world")

if __name__ == "__main__":
    main()


