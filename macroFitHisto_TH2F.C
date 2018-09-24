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

void addErrorToHist (TH1F * histo, float sigma, int n);
void writeStatisticalErrors (TH1F* h1, TString BDTversionName, TString fileTag, TFile * file_FitHisto,Int_t nBINS, Double_t BINS[], TFile * file, float lumi);
void Rebinning(TH1F *histo, TString fileTag, TFile * file_FitHisto, Int_t nBINS, Double_t BINS[], float lumi, int integral = 0);
TH1D* Proiezione(TString fileTag, TString file, int bin, int increment);
float DY_scale(TString histo_Dir, TString systematic_name, TString MVA, float lumi);

// Double_t BDT_ATANH_BINS[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};
// Double_t BDTG_ATANH_BINS[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.5}; //2.4, 
// Double_t MLP_ATANH_BINS[] = {0.0, 0.25, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.75, 3, 3.5, 4.25, 5};
Double_t BDT_BINS[] = {-1.002, -0.768, -0.666, -0.564, -0.462, -0.36, -0.26, -0.158, -0.058, 0.042, 0.144, 0.244, 0.346, 0.446, 0.548, 0.65, 0.75};
Double_t BDT_ATANH_BINS[] = {-0.005, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.255, 1.355};
Double_t BDTG_BINS[] = {-1.002, -0.948, -0.846, -0.744, -0.642, -0.542, -0.442, -0.34, -0.238, -0.136, -0.036, 0.064, 0.164, 0.264, 0.366, 0.468, 0.57, 0.672, 0.774, 0.876, 0.978};
Double_t BDTG_ATANH_BINS[] = {-0.005, 0, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.25, 1.35, 1.45, 1.555, 1.66, 1.765, 1.865, 1.965, 2.07, 2.175, 2.28, 2.38, 2.485, 2.585};
Double_t MLP_BINS[] = {-0.001, 0.095, 0.196, 0.297, 0.397, 0.498, 0.599, 0.699, 0.799, 0.9, 1};
Double_t MLP_ATANH_BINS[] = {-0.005, 0.525, 0.625, 0.725, 0.83, 0.93, 1.035, 1.14, 1.24, 1.34, 1.44, 1.545, 1.65, 1.755, 1.855, 1.955, 2.06, 2.165, 2.265, 2.37, 2.475, 2.58, 2.685, 2.79, 2.89, 2.995, 3.1, 3.205, 3.31, 3.415, 3.515, 3.62, 3.725, 3.83, 3.935, 4.765};
Int_t nBDT_BINS = 16;
Int_t nBDT_ATANH_BINS = 13;
Int_t nBDTG_BINS = 20;
Int_t nBDTG_ATANH_BINS = 26;
Int_t nMLP_BINS = 10;
Int_t nMLP_ATANH_BINS = 35;
Int_t nMLL = 42;
Double_t MLL[43] = {0};
// Int_t nMLL = 2;
// Double_t MLL[3] = {108, 129, 150};

TH1F* hnew_BDT_ATANH = new TH1F("hnew_BDT_ATANH", "hnew_BDT_ATANH", nBDT_ATANH_BINS, 0, 5);
// TH1F* hnew_BDTG_ATANH = new TH1F("hnew_BDTG_ATANH", "hnew_BDTG_ATANH", nBDTG_ATANH_BINS, 0, 5);
// TH1F* hnew_MLP_ATANH = new TH1F("hnew_MLP_ATANH", "hnew_MLP_ATANH", nMLP_ATANH_BINS, 0, 5);

const int nfiles  = 9;
// TString file_names[nfiles] = {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
// TString file_names[nfiles] = {"WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names[nfiles] = {"VBF_HToMuMu","DYJetstoLL_amc_Filter105","TT","ST","WJetsToLNu","ZZ","WW","WZ", "GluGlu_HToMuMu"};
TString file_names_2[nfiles] = {""};


void macroFitHisto_2(TString histo_Dir, TString MVA, Int_t nBINS, Double_t BINS[]) {
    std::cout << "I can see here" << std::endl;

for(int i = 0; i < nfiles; i++)
	file_names_2[i] = file_names[i];

const float lumi = 35900.;
const int Nsyst = 4;
int Nsyst_NoConst = Nsyst;
//TString uncertainty_name[Nsyst] = {"","puWeight","LHE_weights_scale","JES","JER","QGL"};
TString uncertainty_name[Nsyst] = {"","puWeight","LHE_weights_scale","QGL"};
//, "JES", "HF", "LF", "HFStats1", "HFStats2", "LFStats1", "LFStats2","cfErr1", "cfErr2"};

TString DY_name = "DYInclusivetoLL";
TString DY_file_name = "DYInclusivetoLL_M";

DY_name = "DYJetstoLL_amc";
DY_file_name = "DYJetstoLL_amc";

//DY_name = "DYJetstoLL_amc_Pt";
//DY_file_name = "DYJetstoLL_amc_Pt";


// DY_name = "DYJetstoLL_Final_amc";
// DY_file_name = "DYJetstoLL_Final_amc";


TString file_names_PUup[nfiles];
TString file_names_PUdown[nfiles];
TString file_names_QCDup[nfiles];
TString file_names_QCDdown[nfiles];
TString file_names_JESup[nfiles];
TString file_names_JESdown[nfiles];
TString file_names_JERup[nfiles];
TString file_names_JERdown[nfiles];
TString file_names_bTagJESup[nfiles]; //1
TString file_names_bTagJESdown[nfiles];
TString file_names_bTagHFup[nfiles]; //2
TString file_names_bTagHFdown[nfiles];
TString file_names_bTagHFStats1up[nfiles]; //3
TString file_names_bTagHFStats1down[nfiles];
TString file_names_bTagHFStats2up[nfiles]; //4
TString file_names_bTagHFStats2down[nfiles];
TString file_names_bTagLFup[nfiles];  //5
TString file_names_bTagLFdown[nfiles];
TString file_names_bTagLFStats1up[nfiles]; //6
TString file_names_bTagLFStats1down[nfiles];
TString file_names_bTagLFStats2up[nfiles]; //7
TString file_names_bTagLFStats2down[nfiles];
TString file_names_bTagcfErr1up[nfiles]; //8
TString file_names_bTagcfErr1down[nfiles];
TString file_names_bTagcfErr2up[nfiles]; //9
TString file_names_bTagcfErr2down[nfiles];


TString dir = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/";
TString file_data_name = dir + histo_Dir + "/SingleMuon";
TString file_tag[nfiles];

//TString file_names_QCDup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_QCDdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_JESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_JESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};


for (int i=0;i<nfiles;i++){
    file_tag[i] = file_names[i];
//    file_names[i].Prepend("/afs/pi.infn.it/user/mandorli/mandorli/Hmumu/CMSSW_8_0_28/src/giulioMandorli/histoFileDir/TH2F/15_07/"); 

    file_names[i].Append("_mu");
    file_names_PUup[i]   = file_names[i];
    file_names_PUdown[i] = file_names[i];
    file_names_QCDup[i]   = file_names[i];
    file_names_QCDdown[i] = file_names[i];
    file_names_JESup[i]   = file_names[i];
    file_names_JESdown[i] = file_names[i];
    file_names_JERup[i]   = file_names[i];
    file_names_JERdown[i] = file_names[i];
    file_names_bTagJESup[i]   = file_names[i];
    file_names_bTagJESdown[i]   = file_names[i];
    file_names_bTagHFup[i]   = file_names[i];
    file_names_bTagHFdown[i]   = file_names[i];
    file_names_bTagHFStats1up[i]   = file_names[i];
    file_names_bTagHFStats1down[i]   = file_names[i];
    file_names_bTagHFStats2up[i]   = file_names[i];
    file_names_bTagHFStats2down[i]   = file_names[i];
    file_names_bTagLFup[i]   = file_names[i];
    file_names_bTagLFdown[i]   = file_names[i];
    file_names_bTagLFStats1up[i]   = file_names[i];
    file_names_bTagLFStats1down[i]   = file_names[i];
    file_names_bTagLFStats2up[i]   = file_names[i];
    file_names_bTagLFStats2down[i]   = file_names[i];
    file_names_bTagcfErr1up[i]   = file_names[i];
    file_names_bTagcfErr1down[i]   = file_names[i];
    file_names_bTagcfErr2up[i]   = file_names[i];
    file_names_bTagcfErr2down[i]   = file_names[i];
    
    TString directory = dir + histo_Dir + "/";
    
    file_names[i].Prepend(directory);
    file_names_PUdown[i].Prepend(directory);
    file_names_PUup[i].Prepend(directory);
    file_names_QCDup[i].Prepend(directory);
    file_names_QCDdown[i].Prepend(directory);
    file_names_JESup[i].Prepend(directory);
    file_names_JESdown[i].Prepend(directory);
    file_names_JERup[i].Prepend(directory);
    file_names_JERdown[i].Prepend(directory);
    file_names_bTagJESup[i].Prepend(directory);
    file_names_bTagJESdown[i].Prepend(directory);
    file_names_bTagHFup[i].Prepend(directory);
    file_names_bTagHFdown[i].Prepend(directory);
    file_names_bTagLFup[i].Prepend(directory);
    file_names_bTagLFdown[i].Prepend(directory);
    file_names_bTagHFStats1up[i].Prepend(directory);
    file_names_bTagHFStats1down[i].Prepend(directory);
    file_names_bTagHFStats2up[i].Prepend(directory);
    file_names_bTagHFStats2down[i].Prepend(directory);
    file_names_bTagLFStats1up[i].Prepend(directory);
    file_names_bTagLFStats1down[i].Prepend(directory);
    file_names_bTagLFStats2up[i].Prepend(directory);
    file_names_bTagLFStats2down[i].Prepend(directory);
    file_names_bTagcfErr1up[i].Prepend(directory);
    file_names_bTagcfErr1down[i].Prepend(directory);
    file_names_bTagcfErr2up[i].Prepend(directory);
    file_names_bTagcfErr2down[i].Prepend(directory);

    file_names[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_PUup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUup_WEIGHTnom_v25_reskim.root");
    file_names_PUdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUdown_WEIGHTnom_v25_reskim.root");
    file_names_QCDup[i].Append("_QCDScaleup_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_QCDdown[i].Append("_QCDScaledown_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_JESup[i].Append("_QCDScalenom_JESup_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_JESdown[i].Append("_QCDScalenom_JESdown_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_JERup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERup_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_JERdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERdown_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagJESup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagJESdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFStats1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFStats1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFStats2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagHFStats2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFStats1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFStats1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFStats2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagLFStats2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagcfErr1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagcfErr1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagcfErr2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
    file_names_bTagcfErr2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");

}
    file_data_name.Append("_mu");
    file_data_name.Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");



    TFile * file_data   = new TFile (file_data_name);
    TH2F* th2f = (TH2F*)file_data->Get(MVA);
	int Xbins = th2f->GetNbinsX();
	std::cout<<Xbins<<std::endl;
    
	for(int k = 1; k < Xbins+1; k++){
	
		int increment = 10;
		TString bhu = Form("_%i_Systematics.root", k);
		TString nomeNewFile = MVA + bhu;
	    TFile * file_FitHisto= new TFile (nomeNewFile, "recreate");
    	file_FitHisto->cd();


    for (int i=0;i<nfiles;i++){
//         if (i==0) Nsyst_NoConst = 1;
//         else Nsyst_NoConst = Nsyst;
        
        TFile * file         = new TFile (file_names[i]);
        TFile * file_PUup   = new TFile (file_names_PUup[i]);
        TFile * file_PUdown = new TFile (file_names_PUdown[i]);
        TFile * file_QCDup   = new TFile (file_names_QCDup[i]);
        TFile * file_QCDdown = new TFile (file_names_QCDdown[i]);
        TFile * file_JESup   = new TFile (file_names_JESup[i]);
        TFile * file_JESdown = new TFile (file_names_JESdown[i]);
        TFile * file_JERup   = new TFile (file_names_JERup[i]);
        TFile * file_JERdown = new TFile (file_names_JERdown[i]);
//         TFile * file_bTagJESup  = new TFile (file_names_bTagJESup[i]);
//         TFile * file_bTagJESdown   = new TFile (file_names_bTagJESdown[i]);
//         TFile * file_bTagHFup   = new TFile (file_names_bTagHFup[i]);
//         TFile * file_bTagHFdown   = new TFile (file_names_bTagHFdown[i]);
//         TFile * file_bTagLFup   = new TFile (file_names_bTagLFup[i]);
//         TFile * file_bTagLFdown   = new TFile (file_names_bTagLFdown[i]);
//         TFile * file_bTagHFStats1up   = new TFile (file_names_bTagHFStats1up[i]);
//         TFile * file_bTagHFStats1down   = new TFile (file_names_bTagHFStats1down[i]);
//         TFile * file_bTagHFStats2up   = new TFile (file_names_bTagHFStats2up[i]);
//         TFile * file_bTagHFStats2down   = new TFile (file_names_bTagHFStats2down[i]);
//         TFile * file_bTagLFStats1up   = new TFile (file_names_bTagLFStats1up[i]);
//         TFile * file_bTagLFStats1down   = new TFile (file_names_bTagLFStats1down[i]);
//         TFile * file_bTagLFStats2up   = new TFile (file_names_bTagLFStats2up[i]);
//         TFile * file_bTagLFStats2down   = new TFile (file_names_bTagLFStats2down[i]);
//         TFile * file_bTagcfErr1up   = new TFile (file_names_bTagcfErr1up[i]);
//         TFile * file_bTagcfErr1down   = new TFile (file_names_bTagcfErr1down[i]);
//         TFile * file_bTagcfErr2up   = new TFile (file_names_bTagcfErr2up[i]);
//         TFile * file_bTagcfErr2down   = new TFile (file_names_bTagcfErr2down[i]);

/*        TString histTitle = "hBDT_VBF_"; 
        for (int j=10;j<Nsyst_NoConst;++j){
			std::cout<<(histTitle+"_mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Up").Data()<<std::endl;
            TH1F * histoSys_atanhBDT_Up = (TH1F*) file->Get((histTitle+"_mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Up").Data())->Clone();
            TH1F * histoSys_atanhBDT_Down = (TH1F*) file->Get((histTitle+"_mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Down").Data())->Clone();

            file_FitHisto->cd();

            histoSys_atanhBDT_Up->Scale(lumi);
            histoSys_atanhBDT_Down->Scale(lumi);


			histoSys_atanhBDT_Up = (TH1F*)histoSys_atanhBDT_Up->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
            histoSys_atanhBDT_Up->Write();
			histoSys_atanhBDT_Down = (TH1F*)histoSys_atanhBDT_Down->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
            histoSys_atanhBDT_Down->Write();

        }*/
             

// 		TH2F * th2f = (TH2F*) file->Get(MVA)->Clone;
// 		TCanvas *c1 = ("c1", "c1", 500 ,500);
// 		TH1D* proiezione = th2f->ProjectionX("prova",1,1);
// 		proiezione->Draw();

//         TH1F * histo_atanhBDT_nom = (TH1F*) file->Get(MVA)->Clone();
        TH1F * histo_atanhBDT_nom      = (TH1F*) Proiezione(MVA, file_names[i], k, increment);//(TH1F*) file_PUup->Get(MVA);//(TH1F*) histoSys_atanhBDT_PUup_get->Clone(histoSys_atanhBDT_PUup_get->GetName());

//         TString histTitleLinear = "hBDT_VBF_atanh";
        writeStatisticalErrors (histo_atanhBDT_nom, MVA, file_tag[i], file_FitHisto, nBINS, BINS, file, lumi);
		
        TH1F * histoSys_atanhBDT_PUup      = (TH1F*) Proiezione(MVA, file_names_PUup[i], k, increment);//file_PUup->Get(MVA);//(TH1F*) histoSys_atanhBDT_PUup_get->Clone(histoSys_atanhBDT_PUup_get->GetName());
        TH1F * histoSys_atanhBDT_PUdown    = (TH1F*) Proiezione(MVA, file_names_PUdown[i], k, increment);//file_PUdown->Get(MVA);//(TH1F*) histoSys_atanhBDT_PUdown_get->Clone(histoSys_atanhBDT_PUdown_get->GetName());                
        TH1F * histoSys_atanhBDT_QCDup      = (TH1F*) Proiezione(MVA, file_names_QCDup[i], k, increment);//file_QCDup->Get(MVA);//(TH1F*) histoSys_atanhBDT_QCDup_get->Clone(histoSys_atanhBDT_QCDup_get->GetName());
        TH1F * histoSys_atanhBDT_QCDdown    = (TH1F*) Proiezione(MVA, file_names_QCDdown[i], k, increment);//file_QCDdown->Get(MVA);//(TH1F*) histoSys_atanhBDT_QCDdown_get->Clone(histoSys_atanhBDT_QCDdown_get->GetName());
        TH1F * histoSys_atanhBDT_JESup      = (TH1F*) Proiezione(MVA, file_names_JESup[i], k, increment);//file_JESup->Get(MVA);//(TH1F*) histoSys_atanhBDT_JESup_get->Clone(histoSys_atanhBDT_JESup_get->GetName());
        TH1F * histoSys_atanhBDT_JESdown    = (TH1F*) Proiezione(MVA, file_names_JESdown[i], k, increment);//file_JESdown->Get(MVA);//(TH1F*) histoSys_atanhBDT_JESdown_get->Clone(histoSys_atanhBDT_JESdown_get->GetName());
        TH1F * histoSys_atanhBDT_JERup      = (TH1F*) Proiezione(MVA, file_names_JERup[i], k, increment);//file_JERup->Get(MVA);//(TH1F*) histoSys_atanhBDT_JERup_get->Clone(histoSys_atanhBDT_JERup_get->GetName());
        TH1F * histoSys_atanhBDT_JERdown    = (TH1F*) Proiezione(MVA, file_names_JERdown[i], k, increment);//file_JERdown->Get(MVA);//(TH1F*) histoSys_atanhBDT_JERdown_get->Clone(histoSys_atanhBDT_JERdown_get->GetName());
//         TH1F * histoSys_atanhBDT_bTagJESup      = (TH1F*) Proiezione(MVA, file_names_bTagJESup[i], k, increment);//file_bTagJESup->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagJESdown    = (TH1F*) Proiezione(MVA, file_names_bTagJESdown[i], k, increment);//file_bTagJESdown->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFup      = (TH1F*) file_bTagHFup->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFdown    = (TH1F*) file_bTagHFdown->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFup      = (TH1F*) file_bTagLFup->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFdown    = (TH1F*) file_bTagLFdown->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFStats1up      = (TH1F*) file_bTagHFStats1up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFStats1down    = (TH1F*) file_bTagHFStats1down->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFStats2up      = (TH1F*) file_bTagHFStats2up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagHFStats2down    = (TH1F*) file_bTagHFStats2down->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFStats1up      = (TH1F*) file_bTagLFStats1up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFStats1down    = (TH1F*) file_bTagLFStats1down->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFStats2up      = (TH1F*) file_bTagLFStats2up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagLFStats2down    = (TH1F*) file_bTagLFStats2down->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagcfErr1up      = (TH1F*) file_bTagcfErr1up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagcfErr1down    = (TH1F*) file_bTagcfErr1down->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagcfErr2up      = (TH1F*) file_bTagcfErr2up->Get(MVA);
//         TH1F * histoSys_atanhBDT_bTagcfErr2down    = (TH1F*) file_bTagcfErr2down->Get(MVA);

		if(file_names_2[i].CompareTo("DYJetstoLL_amc_Filter105")==0){
			histo_atanhBDT_nom->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_PUup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESnom_JERnom_PUup_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_PUdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESnom_JERnom_PUdown_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_QCDup->Scale(DY_scale(histo_Dir, "_QCDScaleup_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_QCDdown->Scale(DY_scale(histo_Dir, "_QCDScaledown_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_JESup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESup_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_JESdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESdown_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_JERup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESnom_JERup_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
			histoSys_atanhBDT_JERdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESnom_JERdown_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagJESup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagJESdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));

// 			histoSys_atanhBDT_bTagHFup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagHFdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFup->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFdown->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagHFStats1up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagHFStats1down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagHFStats2up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagHFStats2down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFStats1up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFStats1down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFStats2up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagLFStats2down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagcfErr1up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagcfErr1down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagcfErr2up->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
// 			histoSys_atanhBDT_bTagcfErr2down->Scale(DY_scale(histo_Dir, "_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim", MVA, lumi));
		}
        
		Rebinning(histo_atanhBDT_nom, MVA+"_mu_"+file_tag[i], file_FitHisto, nBINS, BINS, lumi, 1);
		Rebinning(histoSys_atanhBDT_PUup, MVA+"_mu_"+file_tag[i]+"_PU_Up", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_PUdown, MVA+"_mu_"+file_tag[i]+"_PU_Down", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_QCDup, MVA+"_mu_"+file_tag[i]+"_QCD_Up", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_QCDdown, MVA+"_mu_"+file_tag[i]+"_QCD_Down", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_JESup, MVA+"_mu_"+file_tag[i]+"_JES_Up", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_JESdown, MVA+"_mu_"+file_tag[i]+"_JES_Down", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_JERup, MVA+"_mu_"+file_tag[i]+"_JER_Up", file_FitHisto, nBINS, BINS, lumi);
		Rebinning(histoSys_atanhBDT_JERdown, MVA+"_mu_"+file_tag[i]+"_JER_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagJESup, MVA+"_mu_"+file_tag[i]+"_bTagJES_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagJESdown, MVA+"_mu_"+file_tag[i]+"_bTagJES_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFup, MVA+"_mu_"+file_tag[i]+"_bTagHF_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFdown, MVA+"_mu_"+file_tag[i]+"_bTagHF_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFup, MVA+"_mu_"+file_tag[i]+"_bTagLF_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFdown, MVA+"_mu_"+file_tag[i]+"_bTagLF_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFStats1up, MVA+"_mu_"+file_tag[i]+"_bTagHFStats1_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFStats1down, MVA+"_mu_"+file_tag[i]+"_bTagHFStats1_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFStats2up, MVA+"_mu_"+file_tag[i]+"_bTagHFStats2_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagHFStats2down, MVA+"_mu_"+file_tag[i]+"_bTagHFStats2_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFStats1up, MVA+"_mu_"+file_tag[i]+"_bTagLFStats1_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFStats1down, MVA+"_mu_"+file_tag[i]+"_bTagLFStats1_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFStats2up, MVA+"_mu_"+file_tag[i]+"_bTagLFStats2_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagLFStats2down, MVA+"_mu_"+file_tag[i]+"_bTagLFStats2_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagcfErr1up, MVA+"_mu_"+file_tag[i]+"_bTagcfErr1_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagcfErr1down, MVA+"_mu_"+file_tag[i]+"_bTagcfErr1_Down", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagcfErr2up, MVA+"_mu_"+file_tag[i]+"_bTagcfErr2_Up", file_FitHisto, nBINS, BINS, lumi);
// 		Rebinning(histoSys_atanhBDT_bTagcfErr2down, MVA+"_mu_"+file_tag[i]+"_bTagcfErr2_Down", file_FitHisto, nBINS, BINS, lumi);
		file->Close();
		 file_PUup->Close();
		 file_PUdown->Close();
		 file_QCDup ->Close();
		 file_QCDdown->Close();
		 file_JESup->Close();
		 file_JESdown->Close();
		 file_JERup->Close();
		 file_JERdown->Close();
// 		 file_bTagJESup->Close();
// 		 file_bTagJESdown->Close();

    }
	
    TH1F * histo_data_obs  = (TH1F*) Proiezione(MVA, file_data_name, k, increment);//file_data->Get(MVA);
//     histo_data_obs->GetXaxis()->SetRange(0, BINS[nBINS - 1]);
// 	histo_data_obs = (TH1F*)histo_data_obs->Rebin(nBINS, "hnew_BDT_ATANH", BINS);
	std::cout<<"data = ";
    printf(" = %10.5f\n", histo_data_obs->Integral());

	TString data_name = MVA + "_mu_data_obs";
    hnew_BDT_ATANH->SetName(data_name);
    file_FitHisto->cd();
    hnew_BDT_ATANH->Write();
    file_data->Close();

    
    file_FitHisto->Close();
    
    k+=increment;
    
	}


}
        
void macroFitHisto_TH2F() { 

	TString dir = "BDT_17_07_10var_Final";
	TString MVA = "hBDT_VBF_atanh";
// 	macroFitHisto_2(dir, MVA, nBDT_ATANH_BINS, BDT_ATANH_BINS);
// 	MVA = "hBDT_VBF";
// 	macroFitHisto_2(dir, MVA, nBDT_BINS, BDT_BINS);
// 	
// 	dir = "BDTG_17_07_12var_Final";
// 	MVA = "hBDTG_VBF_atanh";
// 	macroFitHisto_2(dir, MVA, nBDTG_ATANH_BINS, BDTG_ATANH_BINS);
// 	MVA = "hBDTG_VBF";
// 	macroFitHisto_2(dir, MVA, nBDTG_BINS, BDTG_BINS);
// 
// 	dir = "MLP_17_07_16var_Final";
// 	MVA = "hMLP_VBF_atanh";
// 	macroFitHisto_2(dir, MVA, nMLP_ATANH_BINS, MLP_ATANH_BINS);
// 	MVA = "hMLP_VBF";
// 	macroFitHisto_2(dir, MVA, nMLP_BINS, MLP_BINS);
	
// 	for(int i = 0; i < 43; i++)
// 		MLL[i] = i + 108;


// 		TCanvas *c1 = new TCanvas("c1", "c1", 500 ,500);
// 		TH1D *proj = (TH1D*) Proiezione("hHll_vs_MVA", "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/TH2F/TT_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root", 1);
// 		TFile *file = new TFile("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/TH2F/TT_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
// 		TH2F * th2f = (TH2F*) file->Get("hHll_vs_MVA")->Clone();
// 		TH1D* proiezione = th2f->ProjectionX("prova",0,1);
// 		proiezione->Draw();
// 		proj->Draw();

// 		TCanvas *c2 = new TCanvas("c2", "c2", 500 ,500);
// 		proj = (TH1D*) Proiezione("hHll_vs_MVA", "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/TH2F/TT_mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root", 2);
// 		TH2F * th2f = (TH2F*) file->Get(MVA)->Clone;
// 		TH1D* proiezione_2 = th2f->ProjectionX("prova2",1,2);
// 		proiezione_2->Draw();
// 		proj->Draw();
// 		return;
	
	for(int i = 0; i < 10; i++)
		MLL[i] = i + 115;
	dir = "Agnese_SameParameter_bTagGiulio_Binning_variabiliCorrette_NobTagWeight_NoMass_ForTH2";
	MVA = "hHll_vs_MVA";
	macroFitHisto_2(dir, MVA, nMLL, MLL);


}  


void addErrorToHist (TH1F * histo, float sigma, int n) {
        histo->SetBinContent(n, histo->GetBinContent(n) + histo->GetBinError(n)*sigma);
}

void writeStatisticalErrors (TH1F* h1, TString BDTversionName, TString fileTag, TFile * file_FitHisto,Int_t nBINS, Double_t BINS[], TFile * file, float lumi) {
    
    
//         TH1F * histo_BDT_nom = (TH1F*) file->Get(BDTversionName)->Clone();
        TH1F * histo_BDT_nom = (TH1F*) h1->Clone();

//         histo_BDT_nom->GetXaxis()->SetRange(0, BINS[nBINS - 1]);
        
        for (int n = 0; n < nBINS; ++n) {
//         for (int n = 0; n < histo_BDT_nom->GetNbinsX()+1; ++n) {
            TH1F * histoStat_BDT_Up = (TH1F*) h1->Clone();//file->Get(BDTversionName)->Clone();
            TH1F * histoStat_BDT_Down = (TH1F*) h1->Clone();//file->Get(BDTversionName)->Clone();

            std::string binNumber = std::to_string(n);
                        
            addErrorToHist(histoStat_BDT_Up,1,n);
            addErrorToHist(histoStat_BDT_Down,-1,n);
            
            
            histoStat_BDT_Up->Scale(lumi);
            histoStat_BDT_Down->Scale(lumi);
        
                
            file_FitHisto->cd();

// 			histoStat_BDT_Up = (TH1F*)histoStat_BDT_Up->Rebin(nBINS, "hnew_BDT_ATANH", BINS);
			histoStat_BDT_Up->SetName((BDTversionName+"_mu_"+fileTag+"_Stat"+fileTag+binNumber+"_Up").Data());
            histoStat_BDT_Up->Write();
// 			histoStat_BDT_Down = (TH1F*)histoStat_BDT_Down->Rebin(nBINS, "hnew_BDT_ATANH", BINS);
			histoStat_BDT_Down->SetName((BDTversionName+"_mu_"+fileTag+"_Stat"+fileTag+binNumber+"_Down").Data());
            histoStat_BDT_Down->Write();
        }
}

void Rebinning(TH1F *histo, TString fileTag, TFile * file_FitHisto, Int_t nBINS, Double_t BINS[], float lumi, int integral = 0){

// 	    histo->GetXaxis()->SetRange(0, BINS[nBINS - 1]);

// 		histo = (TH1F*)histo->Rebin(nBINS, "hnew_BDT_ATANH", BINS);

        histo->Scale(lumi);  
	    histo->SetName(fileTag);

	    file_FitHisto->cd();	    

	    histo->Write();

		if(integral){
    	    printf("%10.3f\t", histo->Integral());
    	}
}

TH1D* Proiezione(TString fileTag, TString file, int bin, int increment){
		
		TString NOME = Form("prova_%i", bin);
		TFile *file1 = new TFile(file);
		TH2F * th2f = (TH2F*) file1->Get(fileTag)->Clone();
		TH1D* proj = th2f->ProjectionX(NOME, bin, bin+increment);	
		return proj;
}

float DY_scale(TString histo_Dir, TString systematic_name, TString MVA, float lumi){

    float dataInt = 0;;
    Double_t dataError = 0;
   	float MCint = 0;
   	Double_t MCError = 0;
    float DYint = 0;
    Double_t DYError = 0;
    
	TString  nome_data = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/" + histo_Dir + "/SingleMuon"  + "_mu" + systematic_name + ".root";
// 	std::cout<<nome_data<<std::endl;
   	TFile *file_initial_data = TFile::Open(nome_data);
	TH1F *hist_data = (TH1F*)file_initial_data->Get(MVA)->Clone("data");
	dataInt = hist_data->IntegralAndError(0,hist_data->GetNbinsX()+1, dataError); 	
   	
    for (int fileIterator = 0; fileIterator < nfiles; fileIterator++) {
    
     	TString  nome_file = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/";
    	nome_file = nome_file + histo_Dir + "/" + file_names_2[fileIterator] + "_mu" + systematic_name + ".root";
//     	std::cout<<nome_file;
   	   	TFile *file_initial_mc = TFile::Open(nome_file);
   	   	
//    	   	TH1F *hist_mc = (TH1F*)file_initial_mc->Get(MVA)->Clone("mc");
   	   	TH1F *hist_mc = (TH1F*)file_initial_mc->Get("hHll_mass")->Clone("mc");
// 		std::cout<<hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, MCError)<<std::endl;

       	hist_mc->Scale(lumi);
       	if (file_names_2[fileIterator].CompareTo("DYJetstoLL_amc_Filter105")==0){
       		DYint= hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, DYError);
// 			std::cout<<MCint<<"\t"<<file_names_2[fileIterator]<<"\t"<<hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, MCError)<<std::endl; 
		}
       	else{ 
       		MCint+=hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, MCError); 
//        		std::cout<<MCint<<"\t"<<file_names_2[fileIterator]<<"\t"<<hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, MCError)<<std::endl; 
//        		std::cout<<file_names_2[fileIterator]<<"\t"<<hist_mc->IntegralAndError(0, hist_mc->GetNbinsX()+1, MCError)<<std::endl; 
       	}

    }
    
//     std::cout<<"ratio = "<<(dataInt-MCint)/DYint<<"\t"<<MCint<<std::endl;
//     std::cout<<"After normalisation = "<<(dataInt-MCint)<<std::endl;

    return (dataInt-MCint)/DYint;

}
  
