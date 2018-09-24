#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TLorentzVector.h"
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFile.h"
#include "TROOT.h"
#include <vector>

void Rebinning(TFile *file, TString histogram){

	TH1F *histo = (TH1F*) file->Get(histogram)->Clone();

//     TCanvas *c1 = new TCanvas("c1", "c1", 500, 500);
//     histo->Draw();

	int nbins = histo->GetNbinsX();	

	std::vector<float> new_binning;
	new_binning.push_back(histo->GetBinLowEdge(nbins+1));
	bool empty = true;

	
	float tot = 0;
	for(int i = nbins; i >= 0; i--){	
// 		std::cout<<histo->GetBinContent(i);
		if(empty && histo->GetBinContent(i) != 0){ 
			new_binning.pop_back();
			new_binning.push_back(histo->GetBinLowEdge(i+1));
			empty = false;
		}
		tot += histo->GetBinContent(i);
// 		std::cout<<"\t"<<tot<<"\t"<<histo->GetBinLowEdge(i)<<std::endl;
		float bin_Width = fabs(new_binning.back() - histo->GetBinLowEdge(i));
		if(tot > 10 && bin_Width > 0.1){
			new_binning.push_back(histo->GetBinLowEdge(i));
// 			std::cout<<tot<<"\t"<<histo->GetBinLowEdge(i)<<std::endl;
			tot = 0;
		}
		if(i == 0){// && tot > 500){
			new_binning.push_back(histo->GetBinLowEdge(i));
// 			std::cout<<tot<<"\t"<<histo->GetBinLowEdge(i)<<std::endl;
			tot = 0;
		}
		
	}	
	
// 	new_binning.push_back(0);

// 	std::cout<<""<<std::endl;	
// 	std::cout<<new_binning.size()<<std::endl;
	
	Int_t nBINS = new_binning.size();
	Double_t BINS[nBINS];
	for(int i = 0; i < nBINS; i++)	
		BINS[i] = new_binning.at(nBINS-1-i);
	

// 	std::cout<<""<<std::endl;
// 	for(int i = 0; i < nBINS; i++)	
// 		std::cout<<i<<"\t"<<BINS[i]<<"\t"<<new_binning.at(i)<<std::endl;


	std::cout<<" -------------------------- "<<histogram<<std::endl;
	std::cout<<"{";
	for(int i = 0; i < nBINS; i++){
		if(i != nBINS-1)
			std::cout<<BINS[i]<<", ";
		else
			std::cout<<BINS[i]<<"};"<<std::endl;
	}
	std::cout<<nBINS-1<<"\n"<<std::endl;

	TH1F* hnew_BDT = new TH1F("hnew_BDT", "hnew_BDT", nBINS, -1, 1);

	hnew_BDT = (TH1F*)histo->Rebin(nBINS-1, "hnew_BDT", BINS);

//     TCanvas *c2 = new TCanvas(histogram, histogram, 500, 500);
//     hnew_BDT->Draw("TEXT");

}


void Binning(){

// 	TFile * file_BDT = new TFile ("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/BDT_17_07_10var_Final/DYJetstoLL_amc_Filter105_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTzero_v25_reskim.root");
	TString histogram = "hBDT_VBF";
// 	Rebinning(file_BDT, histogram);
// 	histogram = "hBDT_VBF_atanh";
// 	Rebinning(file_BDT, histogram);

	TFile * file_BDTG = new TFile ("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/DYJetstoLL_amc_Filter105_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTzero_v25_reskim.root");
// 	histogram = "hBDTG_VBF";
// 	Rebinning(file_BDTG, histogram);
	histogram = "hBDTG_VBF_atanh";
	Rebinning(file_BDTG, histogram);

// 	TFile * file_MLP = new TFile ("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/MLP_17_07_16var_Final/DYJetstoLL_amc_Filter105_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTzero_v25_reskim.root");
// 	histogram = "hMLP_VBF";
// 	Rebinning(file_MLP, histogram);
// 	histogram = "hMLP_VBF_atanh";
// 	Rebinning(file_MLP, histogram);
// 
// 	TFile * file_Hll_mass = new TFile ("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/MLP_17_07_16var_Final_CUT/DYJetstoLL_amc_Filter105_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTzero_v25_reskim.root");
// 	histogram = "hHll_mass";
// 	Rebinning(file_Hll_mass, histogram);

}