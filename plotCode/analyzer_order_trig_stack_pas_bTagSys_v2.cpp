#include <stdio.h>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TTree.h"
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TMarker.h>
#include <THStack.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TCut.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TF1.h>
#include "TROOT.h"
#include "TPaveText.h"
#include "TPad.h"
#include "TGaxis.h"
// #include <boost/format.hpp>
#include <array>
#include <algorithm>
#include <iterator>
#include <sstream>

#define SWAP(A, B) { Float_t t = A; A = B; B = t; }
#define SWAP2(A, B) { B.swap(A); }

void bubblesort(Float_t *a, std::string *str, int n)
{
  int i, j;
  for (i = n - 1; i >= 0; i--)
  {
    for (j = 0; j < i; j++)
    {
      if (a[j] < a[j + 1]){
        SWAP( a[j], a[j + 1] );
		SWAP2( str[j], str[j+1]);
		}
    }
  }
}


Float_t Discr(TH1F *h1, TH1F *h2){  
  Int_t h1s = h1->GetNbinsX();
  Int_t h2s = h2->GetNbinsX();
  if (h1s != h2s) {
  	printf("h1 size %d   h2 size %d \n",h1s,h2s);
  	return -1;
  }
  if (h1->Integral()!=0 ) h1->Scale(1./h1->Integral());
  if (h2->Integral()!=0 ) h2->Scale(1./h2->Integral());
  Float_t adiff = 0;
  for (Int_t i=0;i<h1s;i++){
	adiff +=  TMath::Abs(h1->GetBinContent(i) - h2->GetBinContent(i));
	}
  return adiff/2;
}


Float_t add_underFlow_overFlow(TH1F *h){ 
    
    h->SetBinContent(1, h->GetBinContent(0) + h->GetBinContent(1));
    h->SetBinContent(h->GetNbinsX(), h->GetBinContent(h->GetNbinsX()) + h->GetBinContent(h->GetNbinsX() + 1)); 
    
}
    
    
    
    
    
void draw2Dhistos(TH2F * h2Dhistos_DY, TH2F * h2Dhistos_VBF, std::string figName) {
       
    TCanvas * c = new TCanvas("c", "c", 1200, 600);
    c->Divide(2,1);
    gPad->SetLogz();
    c->cd(1);
    gStyle->SetOptStat(0000);  
    gPad->SetLogz();
    gPad->SetRightMargin(.2);
    h2Dhistos_DY->Draw("colz");
//     h2Dhistos_DY->Draw("CONT1Z");
    c->cd(2);
    gStyle->SetOptStat(0000);  
    gPad->SetLogz();
    gPad->SetRightMargin(.2);
//     h2Dhistos_VBF->Draw("CONT1Z");
    h2Dhistos_VBF->Draw("colz");
    c->Print(("plotsDirectory/"+figName+".png").c_str());
    
}
    
    
    
    
    
using namespace std;

int main(int argc, char* argv[]){

    
std::cout << "I can see here" << std::endl;

gROOT->ProcessLine(".x setTDRStyle.C");


int set_type=atoi(argv[1]); // 0 - analysis, 1 - control region , top


const int nfiles  = 10; //9;  //11;
//TString leg_names[nfiles] = {"Data","VBF Z #rightarrow ll (#times 10)","WW + jets","ZZ + jets","WZ + jets", "t#bar{t}", "Z + jets"};
//TString leg_names[nfiles] = {"Data","tZq #rightarrow ll","TTZ #rightarrow ll #nu#nu","W(l#nu) + jets","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets","VBF Z #rightarrow ll"};

//TString leg_names[nfiles] = {"Data","W(l#nu) + jets","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets (MDG)","VBF Z #rightarrow ll"};
//TString leg_names[nfiles] = {"Data","W(l#nu) + jets","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets (AMC)","VBF Z #rightarrow ll"};

TString leg_names[nfiles] = {"Data","W(l#nu) + jets","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets","VBF H #rightarrow ll x20","GluGlu H #rightarrow ll x20"};
TString leg_names_2000[nfiles] = {"Data","W(l#nu) + jets","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets","VBF H #rightarrow ll x20","GluGlu H #rightarrow ll x20"};

//---------------------------------------------------------------------------------------------------------------------- OK
/// change EWK to VBF bla
//TString leg_names[nfiles] = {"Data","WW + jets","ZZ + jets","WZ + jets","Single Top", "t#bar{t}", "Z + jets (AMC)","VBF Z #rightarrow ll"};
TString set_names[2] = {"Dimuon","Dielectron"}; 
//if (set_type==0)leg_names[0] = "Data (DoubleB)";
//if (set_type==1)leg_names[0] = "Data (SingleB)";



/// change EWK to VBF bla-----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//TString file_names[nfiles] =        {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc","EWK_LLJJ"};

TString DY_name = "DYInclusivetoLL";
TString DY_file_name = "DYInclusivetoLL_M";

//DY_name = "DYJetstoLL_amc";
//DY_file_name = "DYJetstoLL_amc";

DY_name = "DYJetstoLL_amc_Filter105";
DY_file_name = "DYJetstoLL_amc_Filter105";

TString file_names[nfiles] =        {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_UPup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_UPdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_QCDup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_QCDdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_bTagJESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_bTagJESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JERup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JERdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};

int bg_begin;
int qcd_begin=18;
 bg_begin=1;

int FILLCOLOR[nfiles] = {1,kSpring+7,kSpring+8, kSpring+5, kGreen-3, kAzure+1,kBlue-4,kOrange-2,kRed-4,kMagenta+1};
int LINECOLOR[nfiles] = {1,kSpring+7,kSpring+8, kSpring+5, kGreen-3,kAzure+1,kBlue-4,kOrange-2,kRed-4,kMagenta+1};
int LINESTYLE[nfiles] = {1,1,1,1,1,1,1,1,1,2};
int LINEWIDTH[nfiles] = {1,1,1,1,1,1,1,1,3,3};
int FILLSTYLE[nfiles] = {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001};


	
//int order[nfiles] = {0,1,2,3,4,5,6,7,8,9,10};// number in file_names array as they should appear in the legend
int order[nfiles] = {0,1,2,3,4,5,6,7,8,9};// number in file_names array as they should appear in the legend
int order_legend[nfiles]; 
for (int i=0;i<nfiles;i++){
	order_legend[order[i]]=i;
}


TString data_name[2] = {"SingleMuon","SingleElectron"};

TString set[3]={"_mu","_el"}; 

TString directory = "../histoFileDir/Agnese_SameParameter_bTagGiulio_Binning_variabiliCorrette_NobTagWeight_NoMass_ForTH2/";

for (int i=0;i<nfiles;i++){
	if (i==0) file_names[i] = data_name[set_type];
//         file_names[i].Prepend("../histoFileDir/MLP_17_07_16var_Final/"); 
//         file_names[i].Prepend("../histoFileDir/BDTG_17_07_12var_Final/"); 
//         file_names[i].Prepend("../histoFileDir/BDTG_1_08_7var_Agnese_SameParameter_bTagGiulio_Binning_variabiliCorrette/"); 
//         file_names[i].Prepend("../histoFileDir/MLP_17_07_16var_Final_CUT/"); 
        file_names[i].Prepend(directory); 

	file_names[i].Append(set[set_type]);
	file_names_UPup[i].Prepend(directory); 
	file_names_UPdown[i].Prepend(directory); 
	file_names_QCDup[i].Prepend(directory); 
	file_names_QCDdown[i].Prepend(directory); 
	file_names_JESup[i].Prepend(directory); 
	file_names_JESdown[i].Prepend(directory); 
	file_names_JERup[i].Prepend(directory); 
	file_names_JERdown[i].Prepend(directory); 
	file_names_bTagJESup[i].Prepend(directory); 
	file_names_bTagJESdown[i].Prepend(directory); 

	file_names_UPup[i].Append("_mu");
	file_names_UPdown[i].Append("_mu");
	file_names_QCDup[i].Append("_mu");
	file_names_QCDdown[i].Append("_mu");
	file_names_JESup[i].Append("_mu");
	file_names_JESdown[i].Append("_mu");
	file_names_JERup[i].Append("_mu");
	file_names_JERdown[i].Append("_mu");
	file_names_bTagJESup[i].Append("_mu");
	file_names_bTagJESdown[i].Append("_mu");


	file_names[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_UPup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUup_WEIGHTnom_v25_reskim.root");
	file_names_UPdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUdown_WEIGHTnom_v25_reskim.root");
	file_names_QCDup[i].Append("_QCDScaleup_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_QCDdown[i].Append("_QCDScaledown_JESnom_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_JESup[i].Append("_QCDScalenom_JESup_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_JESdown[i].Append("_QCDScalenom_JESdown_bTagJESnom_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_JERup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERup_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_JERdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERdown_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_bTagJESup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_WEIGHTnom_v25_reskim.root");
	file_names_bTagJESdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_WEIGHTnom_v25_reskim.root");

}

TString dir_name= "plotsDirectory";

dir_name.Append("/");
//TString dir_name = "plots_amc/";
Float_t lumi = 35900;



TLegend *leg = new TLegend(0.77,0.65,0.92,0.9); //without writing about SF
leg->SetFillColor(0);
leg->SetBorderSize(0);
leg->SetTextFont(42);
leg->SetTextSize(0.025);
TLegend *leg_2000 = new TLegend(0.77,0.65,0.92,0.9); //with  writing about SF
leg_2000 ->SetFillColor(0);
leg_2000 ->SetBorderSize(0);
leg_2000 ->SetTextFont(42);
leg_2000 ->SetTextSize(0.025);


const int nhistos =  28; //79 //40//52
//TString hist_names[nhistos]={"hMqq", "hHll_mass", "hEtaQQ","hHTsoftEWK","hSoft_n2EWK","hSoft_n5EWK","hSoft_n10EWK","hPVs", "hJet1q_pt", "hJet1q_eta", "hJet1q_phi", "hJet2q_phi", "hJet1q_ptd", "hJet1q_axis2", "hJet1q_mult", "hJet2q_pt", "hJet2q_eta", "hJet2q_ptd", "hJet2q_axis2", "hJet2q_mult", "hmet",   "hJet1q_leadTrackPt", "hJet2q_leadTrackPt", "hqq_pt", "hqgl", "hqgl2", "hqglAtanh", "hqgl2Atanh", "hZll_pt", "hZll_phi", "hZll_eta",  "hrho", "hlepton1_pt", "hlepton2_pt", "hlepton1_eta", "hlepton2_eta","hlepton1_iso03", "hlepton2_iso03", "hDeltaRelQQ", "hRpt", "hRptAtanh", "hEtaQQSum", "hPhiZQ1", "hZll_y", "hZll_ystar", "hZll_zstar", "hMqq_log", "hlheV_pt","hPhiQQ", "hJets12_pt","hJets12_pt_log", "hJet1q_pt_log", "hJet2q_pt_log", "hHT","hlheHT_log", "hlheNj", "hNAdJets", "hJet3_pt", "hJet3_pt_log", "hsoftleadTrackEta", "hAdJetHT", "hJet3_eta", "hJet3_pt_new", "hMaxJetBTagCSV","hVirtual_Pt1","hVirtual_Pt2","hVirtual_Pt1_log", "hVirtual_Pt2_log", "hMaxJetBTagCMVA","hthetastar_W1","hthetastar_W2", "hMaxSecondJetBTagCMVA", "hMaxSecondJetBTagCSV","hVirtual_eta1","hVirtual_eta2", "hThetaStarJet", "hThetaPlanes", "hThetaStar", "hThetaStarJetAtanh", "hThetaPlanesAtanh","hDiffmass", "hThetaStarAbs","hTheta_HiggsJ1","hTheta_HiggsJ2","hthetastar_W2toHW1","hthetastar_W1toHW2","hthetastar_HtoWW", "hmumujj_pt", "hmumujj_pt", "hmumujj_ptLog", "hEnergy_fraction_Parton1_log", "hEnergy_fraction_Parton2_log","hdeltaM", "hdeltaMRel", "hTotalEnergy","hTotalEnergylog","hVirtual_phi1","hVirtual_phi2","hWWmass", "hEnergy_fraction_Parton1","hPz","hPzAbs", "hPzAbsLog","hVirtual_Wmass1","hVirtual_Wmass2", "hVirtual_Wmass1_log", "hVirtual_Wmass2_log",  "hInvariant_MassLog","hInvariant_Mass" , "hEnergy_fraction_Parton2", "hBDT_VBF" , "hBDT_VBF_atanh"};

/*, "hJet1q_mcFlavour",  "hJet2q_mcFlavour"*/
TString hist_names[nhistos]={"hHll_mass", "hMqq", "hHll_eta", "hlepton1_pt", "hlepton2_pt",
			"hHll_zstar", "hEtaQQ", "hHll_pt", "hBDiscriminator_CMVAV2", "hVirtual_Wmass1", 
			"hRpt", "hHll_pt", "hBDT_VBF", "hBDT_VBF_atanh", "hBDTG_VBF", 
// 			"hbtagWeight_Up", "hbtagWeight_Down", "hbtagWeight_Up_perJet", 
// 			"hbtagWeight_Down_perJet", "hbtagWeight_UpMenoCentrale_perJet", "hbtagWeight_DownMenoCentrale_perJet", 
		"hBDTG_VBF_atanh", "hMLP_VBF", "hMLP_VBF_atanh", "hInvariant_Mass", "hTheta_HiggsJ1",
		"hTheta_HiggsJ2", "hqgl", "hqgl2", "hdeltaMRel", "hqq_pt",
		"hSoft_n5EWK", "hHll_phi", "hPVs",
		};


// Double_t BDT_BINS[] = {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
// Double_t BDTG_BINS[] = {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
// Double_t MLP_BINS[] = {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};
// Double_t BDT_ATANH_BINS[] = {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
// Double_t BDTG_ATANH_BINS[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.5}; //2.4, 
// Double_t MLP_ATANH_BINS[] = {0.0, 0.25, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1., 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.75, 3, 3.5, 4.25, 5};
Int_t nBDTG_ATANH_BINS = 19; 
Double_t BDTG_ATANH_BINS[] = {0, 0.0689, 0.1689, 0.2689, 0.369, 0.469, 0.569, 0.6691, 0.7691, 0.8692, 0.9693, 1.0693, 1.1694, 1.2694, 1.3694, 1.4695, 1.5696, 1.6696, 1.7806, 2.5};

Int_t nBDT_ATANH_BINS = 15; 
Double_t BDT_ATANH_BINS[] = {0,     0.0225,         0.1224,         0.2223,         0.3222,         0.4221,         0.522,  0.6219,         0.7218,         0.8217,        0.9216,         1.0215,         1.1214,         1.2213,         1.3842,         2.5};

Double_t BDT_BINS[] = {-1.002, -0.768, -0.666, -0.564, -0.462, -0.36, -0.26, -0.158, -0.058, 0.042, 0.144, 0.244, 0.346, 0.446, 0.548, 0.65, 0.75};
// Double_t BDT_ATANH_BINS[] = {-0.005, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.255, 1.355};
Double_t BDTG_BINS[] = {-1.002, -0.948, -0.846, -0.744, -0.642, -0.542, -0.442, -0.34, -0.238, -0.136, -0.036, 0.064, 0.164, 0.264, 0.366, 0.468, 0.57, 0.672, 0.774, 0.876, 0.978};
// Double_t BDTG_ATANH_BINS[] = {-0.005, 0, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.25, 1.35, 1.45, 1.555, 1.66, 1.765, 1.865, 1.965, 2.07, 2.175, 2.28, 2.38, 2.485, 2.585};
Double_t MLP_BINS[] = {-0.001, 0.095, 0.196, 0.297, 0.397, 0.498, 0.599, 0.699, 0.799, 0.9, 1};
Double_t MLP_ATANH_BINS[] = {-0.005, 0.525, 0.625, 0.725, 0.83, 0.93, 1.035, 1.14, 1.24, 1.34, 1.44, 1.545, 1.65, 1.755, 1.855, 1.955, 2.06, 2.165, 2.265, 2.37, 2.475, 2.58, 2.685, 2.79, 2.89, 2.995, 3.1, 3.205, 3.31, 3.415, 3.515, 3.62, 3.725, 3.83, 3.935, 4.765};
Int_t nBDT_BINS = 16;
// Int_t nBDT_ATANH_BINS = 13;
Int_t nBDTG_BINS = 20;
// Int_t nBDTG_ATANH_BINS = 26;
Int_t nMLP_BINS = 10;
Int_t nMLP_ATANH_BINS = 35;

TH1F* hnew_BDT = new TH1F("hnew_BDT", "hnew_BDT", nBDT_BINS, -1, 1);
TH1F* hnew_BDTG = new TH1F("hnew_BDTG", "hnew_BDTG", nBDTG_BINS, -1, 1);
TH1F* hnew_MLP = new TH1F("hnew_MLP", "hnew_MLP", nMLP_BINS, -1, 1);
TH1F* hnew_BDT_ATANH = new TH1F("hnew_BDT_ATANH", "hnew_BDT_ATANH", nBDT_ATANH_BINS, -1, 1);
TH1F* hnew_BDTG_ATANH = new TH1F("hnew_BDTG_ATANH", "hnew_BDTG_ATANH", nBDTG_ATANH_BINS, -1, 1);
TH1F* hnew_MLP_ATANH = new TH1F("hnew_MLP_ATANH", "hnew_MLP_ATANH", nMLP_ATANH_BINS, -1, 1);

TH1F* hnew_BDT_2 = new TH1F("hnew_BDT_2", "hnew_BDT_2", nBDT_BINS, -1, 1);
TH1F* hnew_BDTG_2 = new TH1F("hnew_BDTG_2", "hnew_BDTG", nBDTG_BINS, -1, 1);
TH1F* hnew_MLP_2 = new TH1F("hnew_MLP_2", "hnew_MLP_2", nMLP_BINS, -1, 1);
TH1F* hnew_BDT_ATANH_2 = new TH1F("hnew_BDT_ATANH_2", "hnew_BDT_ATANH_2", nBDT_ATANH_BINS, -1, 1);
TH1F* hnew_BDTG_ATANH_2 = new TH1F("hnew_BDTG_ATANH_2", "hnew_BDTG_ATANH_2", nBDTG_ATANH_BINS, -1, 1);
TH1F* hnew_MLP_ATANH_2 = new TH1F("hnew_MLP_ATANH_2", "hnew_MLP_ATANH_2", nMLP_ATANH_BINS, -1, 1);



std::vector<std::string> variablesName_in_2D_plot = {"Zll_mass", "deltaM", "Xparton1Log", "Xparton2Log", "RpT", "zStar" };
std::vector<std::string> hist2D_names;
std::vector<TH2F*> h2Dhistos_DY;
std::vector<TH2F*> h2Dhistos_VBF;
 
for (int i = 0; i < variablesName_in_2D_plot.size(); i++) {
    for (int j = i+1; j < variablesName_in_2D_plot.size(); j++) {

        hist2D_names.push_back(("histo2D_"+variablesName_in_2D_plot[i]+"_"+variablesName_in_2D_plot[j]).c_str());        
        
    }
}
        
        




std::array<int,200> LOGY_array = {};


// ,"hpdgId"

TString hist_names_sum[nhistos]={};
TString sum_histos_names[nhistos]={};
std::string hist_names_sort[nhistos];


for (int i=0;i<nhistos;i++){
	hist_names_sort[i] = hist_names[i];
	hist_names_sum[i] = hist_names[i];
	hist_names_sum[i].Append("_sum");
	sum_histos_names[i] = hist_names[i];
	sum_histos_names[i].Append("_sum0");
}


TString stacks_names[nhistos];
for (int i=0;i<nhistos;i++){
	stacks_names[i] = hist_names[i];
	stacks_names[i].Prepend("s");
}


TString output_names[nhistos];
for (int i=0;i<nhistos;i++){
	output_names[i] = hist_names[i];
	output_names[i].Prepend(dir_name);
	output_names[i].Append("_v25.png");
}





TH1F *data_histos[nhistos];
TH1F *data_histos2[nhistos];
TH1F *data_histosbTagJESup[nhistos];
TH1F *data_histosbTagJESlo[nhistos];

TH1F *data_histosTrig[nhistos];
TH1F *signal_histos[nhistos];
TH1F *signal_histos2[nhistos];
TH1F *gluonFu_histos[nhistos];
TH1F *tthbb_histos[nhistos];
TH1F *tthnbb_histos[nhistos];
TH1F *gf_histos[nhistos];
TH1F *sum_histos[nhistos];
TH1F *sum_histosUp[nhistos];
TH1F *sum_histosDown[nhistos];
TH1F *histos_forUnc[nhistos];
TH1F *histos_for_legened[nhistos];
TH1F *discr_histos[nhistos];//Mqq,delta eta, delta phi, qgl, btag //12,13,14,21,22
TH1F *hBkgVis[nhistos];
TH1F *hBkgUncUp[nhistos];
TH1F *hBkgUncLo[nhistos];
TH1F *hBkgUncUpTrig[nhistos];
TH1F *hBkgUncLoTrig[nhistos];
TH1F *hBkgbTagJESUp[nhistos];
TH1F *hBkgbTagJESLo[nhistos];



int files=0; 
THStack *stacks[nhistos];
TH1F * stackHisto[nhistos];
for (int i=0;i<nhistos;++i){
	stacks[i] = new THStack(stacks_names[i],"");
}

Double_t totalBG=0.;
Double_t totalQCD=0.;
Double_t totalMC=0.;
Double_t totalData=0.;
Double_t totalDataQCD=0.;
ofstream out_efficiency;
ofstream out_efficiency_up;
ofstream out_efficiency_down;
ofstream out_discrimination;
out_efficiency.open(dir_name+"efficiency.txt"); 
out_efficiency_up.open(dir_name+"efficiency_up.txt"); 
out_efficiency_down.open(dir_name+"efficiency_down.txt"); 


// DY bg ratio is calculated here
float ratio[11];
float ratioError[11];
TString file_names_mc[nfiles];
for (int counter_ratio=0; counter_ratio < 11;counter_ratio++) {

    float dataInt = 0;
    float MCint = 0;
    float DYint =0;
    float MCsign =0;
    
    double dataError = 0;
    double MCError = 0;
    double DYError =0;
    
    
    //Set file_names_mc
    file_names_mc[0] = file_names[0];
    for (int i=1;i<nfiles;i++) {
        if (counter_ratio==0) file_names_mc[i] = file_names[i];
        if (counter_ratio==1) file_names_mc[i] = file_names_UPup[i];
        if (counter_ratio==2) file_names_mc[i] = file_names_UPdown[i];
        if (counter_ratio==3) file_names_mc[i] = file_names_QCDup[i];
        if (counter_ratio==4) file_names_mc[i] = file_names_QCDdown[i];
        if (counter_ratio==5) file_names_mc[i] = file_names_JESup[i];
        if (counter_ratio==6) file_names_mc[i] = file_names_JESdown[i];
        if (counter_ratio==7) file_names_mc[i] = file_names_JERup[i];
        if (counter_ratio==8) file_names_mc[i] = file_names_JERdown[i];
        if (counter_ratio==9) file_names_mc[i] = file_names_bTagJESup[i];
        if (counter_ratio==10) file_names_mc[i] = file_names_bTagJESdown[i];

//         if (((counter_ratio==1) || (counter_ratio==2))&& (i==5)) {
//             file_names_mc[i]=file_names[i];
//             cout << " No QCD up e down \n " << file_names[i] << endl;
//         }
    }

    files=0;
    for (int fileIterator = 0; fileIterator < nfiles; fileIterator++) {

        TFile *file_initial_mc;
//         std::cout << file_names[fileIterator] << std::endl;

        file_initial_mc = TFile::Open(file_names_mc[fileIterator]);

        string file_name_tag = file_names_mc[fileIterator].Data();
        TH1F *histos_mc[100];
        int hist=0;
        histos_mc[hist] = (TH1F*)file_initial_mc->Get(hist_names[hist])->Clone("mc");

                        
        if (fileIterator==0) dataInt = histos_mc[hist]->IntegralAndError(0,histos_mc[hist]->GetNbinsX()+1, dataError); 
        if (fileIterator!=0) {
        
//         	std::cout<<histos_mc[hist]->Integral(0,histos_mc[hist]->GetNbinsX()+1)<<"\t"<<hist_names[hist]<<std::endl;
        
            histos_mc[hist]->Scale(lumi);
            
/*            if (file_name_tag.find("HToMuMu")==std::string::npos) MCint+=histos_mc[hist]->Integral(0,histos_mc[hist]->GetNbinsX()+1); 
            else MCsign +=histos_mc[hist]->Integral(0,histos_mc[hist]->GetNbinsX()+1);*/ 
            
            if (file_name_tag.find(DY_name)!=std::string::npos) {
                DYint= histos_mc[hist]->IntegralAndError(0,histos_mc[hist]->GetNbinsX()+1, DYError);
// 				std::cout<<counter_ratio<<" ><><<><><><><><><><><><><><><><> ====== "<<DYint<<std::endl;              
            }
            else MCint+=histos_mc[hist]->IntegralAndError(0,histos_mc[hist]->GetNbinsX()+1, MCError); 

// 			std::cout<<counter_ratio<<" ><><<><><><><><><><><><><><><><> ====== "<<file_name_tag<<"\t"<<histos_mc[hist]->IntegralAndError(0,histos_mc[hist]->GetNbinsX()+1, MCError)<<"\t"<<MCint<<std::endl;
            
        }

    }


    float tmp_ratio = (dataInt-MCint)/DYint;
    ratio[counter_ratio] = tmp_ratio;
    ratioError[counter_ratio] = DYError/DYint*ratio[counter_ratio];

	///////// NORMALIZZAZIONE DY ///////////////
//     ratio[counter_ratio] = 1.;
    ratioError[counter_ratio] = 0;
	///////// NORMALIZZAZIONE DY ///////////////
	
//     if(counter_ratio == 0)  cout << "ratio  "  << counter_ratio << " : " << ratio[0] <<   " \t\t DYint : " << DYint <<   " \t\t MCint : " << MCint  <<   " \t\t dataInt : " << dataInt << endl;
//     if(counter_ratio == 1)  cout << "PU up ratio  "  << counter_ratio << " : " << ratio[1] <<   " \t\t DYint : " << DYint <<   " \t\t MCint : " << MCint  <<   " \t\t dataInt : " << dataInt << endl;
//     if(counter_ratio == 2)  cout << "PU down ratio  "  << counter_ratio << " : " << ratio[2] <<   " \t\t DYint : " << DYint <<   " \t\t MCint : " << MCint  <<   " \t\t dataInt : " << dataInt << endl;
// 	std::cout<<" ------------------------------------------------------------------------"<<ratio[counter_ratio]<<std::endl;
	}




TH1F *histos_check[nhistos];
TH1F *histos_JESup_check[nhistos];


files=0;
for (int fileIterator = 0; fileIterator < nfiles; fileIterator++) {

	TFile *file_initial;
//         std::cout << file_names[fileIterator] << std::endl;
  	file_initial = TFile::Open(file_names[fileIterator]);
	string file_name_tag = file_names[fileIterator].Data();
	string leg_name_tag = leg_names[fileIterator].Data();

	std::cout<<file_name_tag<<std::endl;


        TH1F *histos[nhistos];

	for (int hist=0;hist<nhistos;++hist){
//         if(hist == 0) std::cout<<"Nom = "<<file_name_tag;

	//	if (hist_names[hist].CompareTo("hlepton1_eta")==0) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(4);
	//	if (hist_names[hist].CompareTo("hlepton2_eta")==0) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(4);
// 		if (hist_names[hist].CompareTo("hbdt")==0) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(4);
// 		if (hist_names[hist].CompareTo("hbdt_atanh")==0) ((TH1F*)file_initial->Get(hist_names[hist]))->Rebin(10);

		histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("h");
        if(file_name_tag.find("DYJetstoLL")!=std::string::npos)   histos_check[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("h");
                    
                


                if (fileIterator==0) {
                    data_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("data");
                    add_underFlow_overFlow(data_histos[hist]);
                    
                }
		if (fileIterator>0)histos[hist]->Scale(lumi); 

		if (file_name_tag.find("DYJetstoLL")!=std::string::npos)  histos[hist]->Scale(ratio[0]);  
//                 if ((file_name_tag.find("HToMuMu")==std::string::npos) && (file_name_tag.find("SingleMuon")==std::string::npos))  histos[hist]->Scale(ratio[0]);  
                
                
                
                
                
                add_underFlow_overFlow(histos[hist]);

//                 histos[hist]->SetBinContent(1, histos[hist]->GetBinContent(0) + histos[hist]->GetBinContent(1));
//                 histos[hist]->SetBinContent(histos[hist]->GetNbinsX(), histos[hist]->GetBinContent(histos[hist]->GetNbinsX()) + histos[hist]->GetBinContent(histos[hist]->GetNbinsX() + 1));
    
    
// 		if ((hist_names[hist].CompareTo("hNAdJets_mjj1")==0) && ((file_name_tag.find("DYJetstoLL")!=std::string::npos)|| (file_name_tag.find("EWK_LLJJ")!=std::string::npos)) ) 
// 			cout<< " mjj > 1500 ,Integral " <<leg_name_tag<<"   "<< histos[hist]->Integral()<<endl; 
// 		if ((hist_names[hist].CompareTo("hNAdJets_mjj2")==0) && ((file_name_tag.find("DYJetstoLL")!=std::string::npos)|| (file_name_tag.find("EWK_LLJJ")!=std::string::npos)) ) 
// 			cout<< " mjj > 2500 ,Integral " <<leg_name_tag<<"   "<< histos[hist]->Integral()<<endl; 
		if ((hist_names[hist].CompareTo("hNAdJets_bdt")==0) && ((file_name_tag.find(DY_name)!=std::string::npos)|| (file_name_tag.find("EWK_LLJJ")!=std::string::npos)) ) 
			cout<< " bdt > 0.92 ,Integral " <<leg_name_tag<<"   "<< histos[hist]->Integral()<<endl; 
		if ((hist_names[hist].CompareTo("hNAdJets_bdt2")==0) && ((file_name_tag.find(DY_name)!=std::string::npos)|| (file_name_tag.find("EWK_LLJJ")!=std::string::npos)) ) 
			cout<< " bdt > 0.84 ,Integral " <<leg_name_tag<<"   "<< histos[hist]->Integral()<<endl; 
////////////////////////////
////////////////////////////
////////////////////////////
// Signal now is included in the sum total MC


		if (fileIterator==1) 	sum_histos[hist] = (TH1F*)histos[hist]->Clone(sum_histos_names[hist]);
		if (fileIterator>1)	sum_histos[hist]->Add(histos[hist]); 



////////////////////////////
////////////////////////////
////////////////////////////
		if (fileIterator>0) histos[hist]->Sumw2(kFALSE);    //  COMMENT THIS FOR ERRORS
// 		if (fileIterator>0) histos[hist]->Sumw2();
//		if (hist==1) cout<<fileIterator<<"   "<<histos[1]->Integral() <<endl;


        float signalMultipyFactor = 20.;

		if (fileIterator==nfiles-2) {
			signal_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"newhist");
			signal_histos[hist]->Scale(lumi);

			signal_histos[hist]->Scale(signalMultipyFactor);
			signal_histos[hist]->Sumw2(kFALSE);
// 			signal_histos[hist]->Sumw2();
			signal_histos[hist]->SetLineColor(LINECOLOR[fileIterator]);
			signal_histos[hist]->SetLineWidth(LINEWIDTH[fileIterator]);
// 			signal_histos[hist]->SetLineColor(kRed+2);
			signal_histos[hist]->SetLineStyle(LINESTYLE[fileIterator]);
			signal_histos2[hist]=(TH1F*)signal_histos[hist]->Clone("signalHist2");
			
			add_underFlow_overFlow(signal_histos[hist]);
			add_underFlow_overFlow(signal_histos2[hist]);
			
			



		}
  


                if (fileIterator==nfiles-1) {
                        gluonFu_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone(hist_names_sum[hist]+"newhist");
                        gluonFu_histos[hist]->Scale(lumi);

			gluonFu_histos[hist]->Scale(signalMultipyFactor);
			gluonFu_histos[hist]->Sumw2(kFALSE);
// 			gluonFu_histos[hist]->Sumw2();
			gluonFu_histos[hist]->SetLineColor(LINECOLOR[fileIterator]);
            gluonFu_histos[hist]->SetLineWidth(LINEWIDTH[fileIterator]);
            gluonFu_histos[hist]->SetLineStyle(2);
			gluonFu_histos[hist]->SetLineStyle(LINESTYLE[fileIterator]);
            add_underFlow_overFlow(gluonFu_histos[hist]);
		}


		histos[hist]->SetLineColor(LINECOLOR[fileIterator]);
		histos[hist]->SetLineStyle(LINESTYLE[fileIterator]);
		histos[hist]->SetLineWidth(LINEWIDTH[fileIterator]);
		histos[hist]->SetFillStyle(FILLSTYLE[fileIterator]);
		if ((fileIterator!=0)) histos[hist]->SetFillColor(FILLCOLOR[fileIterator]);

		if (fileIterator==0) {
			histos[hist]->SetMarkerStyle(20);
			data_histos[hist]->SetLineColor(1);
			data_histos[hist]->SetMarkerStyle(10);
			data_histos[hist]->SetMarkerSize(.8);
		}

// 	 	if (files>=bg_begin) stacks[hist]->Add(histos[hist]);
	 	if (fileIterator>=1) {
					if (hist_names[hist].CompareTo("hBDT_VBF")==0){
						hnew_BDT = (TH1F*)histos[hist]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
	                    stacks[hist]->Add(hnew_BDT);
	                }
					else if (hist_names[hist].CompareTo("hBDTG_VBF")==0){
						hnew_BDTG = (TH1F*)histos[hist]->Rebin(nBDTG_BINS, "hnew_BDTG", BDTG_BINS);
						stacks[hist]->Add(hnew_BDTG);
					}
					else if (hist_names[hist].CompareTo("hMLP_VBF")==0){
						hnew_MLP = (TH1F*)histos[hist]->Rebin(nMLP_BINS, "hnew_MLP", MLP_BINS);
	                    stacks[hist]->Add(hnew_MLP);
	                }
					else if (hist_names[hist].CompareTo("hBDT_VBF_atanh")==0){
						hnew_BDT_ATANH = (TH1F*)histos[hist]->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
	                    stacks[hist]->Add(hnew_BDT_ATANH);
	                }
					else if (hist_names[hist].CompareTo("hBDTG_VBF_atanh")==0){
						hnew_BDTG_ATANH = (TH1F*)histos[hist]->Rebin(nBDTG_ATANH_BINS, "hnew_BDTG_ATANH", BDTG_ATANH_BINS);
						stacks[hist]->Add(hnew_BDTG_ATANH);
					}
					else if (hist_names[hist].CompareTo("hMLP_VBF_atanh")==0){
						hnew_MLP_ATANH = (TH1F*)histos[hist]->Rebin(nMLP_ATANH_BINS, "hnew_MLP_ATANH", MLP_ATANH_BINS);
	                    stacks[hist]->Add(hnew_MLP_ATANH);
	                }
                    else
                    	stacks[hist]->Add(histos[hist]);




                    if (fileIterator==1)stackHisto[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("stackHisto");
                    else stackHisto[hist]->Add(histos[hist]);
                }
		if (hist==0) histos_for_legened[fileIterator] = (TH1F*)histos[0]->Clone("newd");
		if (fileIterator==bg_begin)	discr_histos[hist] = (TH1F*)file_initial->Get(hist_names[hist])->Clone("discr");
		if ((fileIterator>bg_begin)&&(fileIterator!=(nfiles-2))){
			discr_histos[hist]->Add(histos[hist]); 
                }                

            }

            for(int n=0; n<hist2D_names.size(); n++) {
                if (file_name_tag.find("DYJetstoLL")!=std::string::npos)  {
                    TH2F* h =  (TH2F*)file_initial->Get(hist2D_names[n].c_str())->Clone(("h2D"+hist2D_names[n]).c_str());
                    h->Scale(ratio[0]);
                    h2Dhistos_DY.push_back(h); 
                    
                }
                if (file_name_tag.find("VBF_HToMuMu")!=std::string::npos)  {
                    TH2F* h =  (TH2F*)file_initial->Get(hist2D_names[n].c_str())->Clone(("h2D"+hist2D_names[n]).c_str());
                    h2Dhistos_VBF.push_back(h); 
                }
            }
            
            
            double bkgIntegralError = 0;
            if (fileIterator>=bg_begin) totalBG+=histos[0]->Integral(1,histos[0]->GetNbinsX());   //from 1 to histos[0]->GetNbinsX() because I added overflow and underflow
            if (fileIterator>0) totalMC+=histos[0]->Integral(1,histos[0]->GetNbinsX());
            if (fileIterator==0) {totalData+=histos[0]->Integral(1,histos[0]->GetNbinsX()); }
            if (fileIterator==0) out_efficiency<<"Sample  \t\t\t yield(per "<< lumi<<" pb^-1)"<<endl;
//             if (fileIterator==0) out_efficiency<<leg_names[order[fileIterator]]<<"\t \t \t"<< std::setprecision(6)<<histos[0]->Integral(1,histos[0]->GetNbinsX()) <<endl;
//             else out_efficiency<<leg_names[order[fileIterator]]<<"\t\t\t  "<<std::setprecision(6)<<histos[0]->Integral(1,histos[0]->GetNbinsX())<<endl;
            float bkgIntegral = histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError);
            out_efficiency<<leg_names[order[fileIterator]]<<"\t\t\t"<< std::setprecision(6)<<histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError) << " +- " << bkgIntegralError  <<endl;
// 			std::cout<<"\t"<<histos[0]->Integral(1,histos[0]->GetNbinsX())<<std::endl;
            if (fileIterator==nfiles-1) out_efficiency<<"Total BG"<<"\t \t \t  "<<std::setprecision(6)<<totalBG<<endl;
            if (fileIterator==nfiles-1) out_efficiency<<"Total MC"<<"\t \t \t  "<<std::setprecision(6)<<totalMC<<endl;
//            if (fileIterator==nfiles-1) out_efficiency<<"Data/MC"<<"\t \t \t  "<<std::setprecision(3)<<totalData/totalMC<<endl;
            if (fileIterator==nfiles-1) out_efficiency<<"Data/MC"<<"\t \t \t \t  "<<std::setprecision(6)<<ratio[0]<< " +- " << ratioError[0] <<endl;
//            if (fileIterator==nfiles-1) out_efficiency<<"DY MC scaled with "<<"\t \t \t  "<<std::setprecision(13)<<ratio[0]<<endl;
                if(file_name_tag.find("DYJetstoLL")!=std::string::npos  ) cout << "ratio4  " << fileIterator << "  "  << ratio[0]  << " +- " << ratioError[0] <<   " \t\t integral : " << histos[0]->Integral(0,histos[0]->GetNbinsX()+1) << endl;


}

out_efficiency.close();

std::cout<<"OK1"<<std::endl;
// for(int n=0; n<hist2D_names.size(); n++) {

// draw2Dhistos(h2Dhistos_DY[n], h2Dhistos_VBF[n], hist2D_names[n]);
// }

/////////////////////////////////////

//----------------------------------------------------------------------------------------------------------------------
///JES and QCD part ->no need

files=1;
for (int fileIterator = 1; fileIterator < nfiles; fileIterator++) {

	TFile *file_initial_up;		
	string file_name_tag = file_names_bTagJESup[fileIterator].Data();
//   	if (file_name_tag.find("ST_")!=std::string::npos) {
// 		file_names_bTagJESup[fileIterator] = file_names[fileIterator];
// 		string file_name_tag = file_names_bTagJESup[fileIterator].Data();
// 	}
   file_initial_up = TFile::Open(file_names_bTagJESup[fileIterator]);
//	cout<<file_names_bTagJESup[fileIterator] <<endl;
	TH1F *histos_bTagJESup[nhistos];
	for (int hist=0;hist<nhistos;++hist){

// 		if (hist_names[hist].CompareTo("hbdt")==0) ((TH1F*)file_initial_up->Get(hist_names[hist]))->Rebin(4);
// 		if (hist_names[hist].CompareTo("hbdt_atanh")==0) ((TH1F*)file_initial_up->Get(hist_names[hist]))->Rebin(10);
		histos_bTagJESup[hist] = (TH1F*)file_initial_up->Get(hist_names[hist])->Clone("hup");
		histos_bTagJESup[hist]->Scale(lumi);  //top
//		if (hist==0) cout<<histos_bTagJESup[hist]->Integral() <<endl;
		if (file_name_tag.find("DYJetstoLL")!=std::string::npos)  histos_bTagJESup[hist]->Scale(ratio[1]);  
//                 if (file_name_tag.find("HToMuMu")==std::string::npos)  histos_bTagJESup[hist]->Scale(ratio[1]);  

// 		if (file_name_tag.find("EWK_LLJJ")!=std::string::npos)	histos_bTagJESup[hist]->Scale(0.97957); //new signal x-sec
		if (fileIterator==1) 	hBkgbTagJESUp[hist] = (TH1F*)histos_bTagJESup[hist]->Clone(sum_histos_names[hist]+"bTagJESup");
		if (fileIterator>1)	hBkgbTagJESUp[hist]->Add(histos_bTagJESup[hist]);
		hBkgbTagJESUp[hist]->SetLineColor(kRed);
		hBkgbTagJESUp[hist]->SetLineStyle(2);
	}
	TFile *file_initial_down;
	file_name_tag = file_names_bTagJESdown[fileIterator].Data();
//   	if (file_name_tag.find("ST_")!=std::string::npos) {
// 		file_names_bTagJESdown[fileIterator] = file_names[fileIterator];
// 		string file_name_tag = file_names_bTagJESdown[fileIterator].Data();
// 	}
//	cout<<file_names_bTagJESdown[fileIterator] <<endl;
  	file_initial_down = TFile::Open(file_names_bTagJESdown[fileIterator]);
	TH1F *histos_bTagJESdown[nhistos];
	for (int hist=0;hist<nhistos;++hist){
// 		if (hist_names[hist].CompareTo("hbdt")==0) ((TH1F*)file_initial_down->Get(hist_names[hist]))->Rebin(4);
// 		if (hist_names[hist].CompareTo("hbdt_atanh")==0) ((TH1F*)file_initial_down->Get(hist_names[hist]))->Rebin(10);
		histos_bTagJESdown[hist] = (TH1F*)file_initial_down->Get(hist_names[hist])->Clone("hdown");
		histos_bTagJESdown[hist]->Scale(lumi);  //top
	//	if (hist==0) cout<<histos_bTagJESdown[hist]->Integral() <<endl;
		if (file_name_tag.find("DYJetstoLL")!=std::string::npos)  histos_bTagJESdown[hist]->Scale(ratio[2]);  
//                 if (file_name_tag.find("HToMuMu")==std::string::npos)  histos_bTagJESdown[hist]->Scale(ratio[2]);  

// 		if (file_name_tag.find("EWK_LLJJ")!=std::string::npos)	histos_bTagJESdown[hist]->Scale(0.97957); //new signal x-sec
		if (fileIterator==1) 	hBkgbTagJESLo[hist] = (TH1F*)histos_bTagJESdown[hist]->Clone(sum_histos_names[hist]+"bTagJESdown");
		if (fileIterator>1)	hBkgbTagJESLo[hist]->Add(histos_bTagJESdown[hist]);
		hBkgbTagJESLo[hist]->SetLineColor(kBlue);
		hBkgbTagJESLo[hist]->SetLineStyle(3);
	}

}


for (int hist=0;hist<nhistos;hist++){
// 	if (hist_names[i].CompareTo("hBDT_VBF")==0){
// 			hnew_BDT = (TH1F*)data_histos[i]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
// 			TH1F *hnew_BDT_data = (TH1F*)hnew_BDT->Clone("new");
// 			hnew_BDT_2 = (TH1F*)sum_histos[i]->Rebin(nBDT_BINS, "hnew_BDT_2", BDT_BINS);

	hBkgUncUp[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncUp");
	hBkgUncLo[hist] = (TH1F*)sum_histos[hist]->Clone("hBkgUncLo");
  	hBkgVis[hist]   = (TH1F*)sum_histos[hist]->Clone("hbkgVis");

	if (hist_names[hist].CompareTo("hBDT_VBF")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nBDT_BINS, "hBkgVis", BDT_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nBDT_BINS, "hBkgUncUp", BDT_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nBDT_BINS, "hBkgUncLo", BDT_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nBDT_BINS, "sum_histos", BDT_BINS);
	}
	if (hist_names[hist].CompareTo("hBDTG_VBF")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nBDTG_BINS, "hBkgVis", BDTG_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nBDTG_BINS, "hBkgUncUp", BDTG_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nBDTG_BINS, "hBkgUncLo", BDTG_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nBDTG_BINS, "sum_histos", BDTG_BINS);
	}
	if (hist_names[hist].CompareTo("hMLP_VBF")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nMLP_BINS, "hBkgVis", MLP_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nMLP_BINS, "hBkgUncUp", MLP_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nMLP_BINS, "hBkgUncLo", MLP_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nMLP_BINS, "sum_histos", MLP_BINS);
	}
	if (hist_names[hist].CompareTo("hBDT_VBF_atanh")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nBDT_ATANH_BINS, "hBkgVis", BDT_ATANH_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nBDT_ATANH_BINS, "hBkgUncUp", BDT_ATANH_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nBDT_ATANH_BINS, "hBkgUncLo", BDT_ATANH_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nBDT_ATANH_BINS, "sum_histos", BDT_ATANH_BINS);
	}
	if (hist_names[hist].CompareTo("hBDTG_VBF_atanh")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nBDTG_ATANH_BINS, "hBkgVis", BDTG_ATANH_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nBDTG_ATANH_BINS, "hBkgUncUp", BDTG_ATANH_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nBDTG_ATANH_BINS, "hBkgUncLo", BDTG_ATANH_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nBDTG_ATANH_BINS, "sum_histos", BDTG_ATANH_BINS);
	}
	if (hist_names[hist].CompareTo("hMLP_VBF_atanh")==0){
			hBkgVis[hist] = (TH1F*)hBkgVis[hist]->Rebin(nMLP_ATANH_BINS, "hBkgVis", MLP_ATANH_BINS);
			hBkgUncUp[hist] = (TH1F*)hBkgUncUp[hist]->Rebin(nMLP_ATANH_BINS, "hBkgUncUp", MLP_ATANH_BINS);
			hBkgUncLo[hist] = (TH1F*)hBkgUncLo[hist]->Rebin(nMLP_ATANH_BINS, "hBkgUncLo", MLP_ATANH_BINS);
			sum_histos[hist] = (TH1F*)sum_histos[hist]->Rebin(nMLP_ATANH_BINS, "sum_histos", MLP_ATANH_BINS);
	}

  	for(int i=0;i<hBkgUncUp[hist]->GetNbinsX();i++) {
  		float e = 0.0;
    	if (sum_histos[hist]->GetBinContent(i+1) != 0) {
      	e = sum_histos[hist]->GetBinError(i+1)/sum_histos[hist]->GetBinContent(i+1);
   	}
    	hBkgUncUp[hist]->SetBinContent(i+1,e);
    	hBkgUncLo[hist]->SetBinContent(i+1,-e);
	}
  	hBkgVis[hist]->SetMarkerSize(0);
 	hBkgVis[hist]->SetFillColor(kBlack);
 	hBkgVis[hist]->SetFillStyle(3004);
 	hBkgUncUp[hist]->SetLineColor(kBlack);
 	hBkgUncUp[hist]->SetLineWidth(1);
 	hBkgUncUp[hist]->SetFillColor(kBlack);
 	hBkgUncUp[hist]->SetFillStyle(3004);
	hBkgUncLo[hist]->SetLineColor(kBlack);
 	hBkgUncLo[hist]->SetLineWidth(1);
  	hBkgUncLo[hist]->SetFillColor(kBlack);
	hBkgUncLo[hist]->SetFillStyle(3004);
}

Float_t TSF[2] = {1.,1.};
Float_t kfactors[2] = {0.,0.};
//kfactors[set_type] = 1./(MC_data[set_type] * TSF[set_type]);
kfactors[set_type] = 1./(ratio[0] * TSF[set_type]);
//cout<<"kfactor = "<<kfactors[set_type]<<endl;
//cout<<"Data/MC = "<< MC_data[set_type]<<endl;


for (int i=0;i<nfiles-2;i++){
	if (i==0) { 
            leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"P");
            leg_2000->AddEntry(histos_for_legened[order_legend[i]],leg_names_2000[i],"P");
        }
	if (i>=bg_begin) {
            leg->AddEntry(histos_for_legened[order_legend[i]],leg_names[i],"F");
            leg_2000->AddEntry(histos_for_legened[order_legend[i]],leg_names_2000[i],"F");
        }
}
	leg->AddEntry(histos_for_legened[nfiles-2],leg_names[nfiles-2],"L");
	leg->AddEntry(histos_for_legened[nfiles-1],leg_names[nfiles-1],"L");
        leg_2000->AddEntry(histos_for_legened[nfiles-2],leg_names_2000[nfiles-2],"L");
	leg_2000->AddEntry(histos_for_legened[nfiles-1],leg_names_2000[nfiles-1],"L");
        
leg->AddEntry(hBkgUncUp[0],"MC stat. unc.","F");
leg_2000->AddEntry(hBkgUncUp[0],"MC stat. unc.","F");


std::cout<<"OK2"<<std::endl;
// COMPUTE d VALUES FOR ALL THE HISTOGRAMS
Float_t discriminators[nhistos];
for (int d=0;d<nhistos;d++){
	discriminators[d] = Discr(discr_histos[d],signal_histos2[d]);
}

bubblesort(discriminators, hist_names_sort,nhistos);
//out_discrimination.open("Aftertriggercorr2/"+trigger[set_type]+dir_name+"discrimination.txt");
out_discrimination.open(dir_name+"discrimination.txt");
for (int d=0;d<nhistos;d++){
	if (d==0) out_discrimination<<"Variable &\t d"<<endl;
	out_discrimination<<"$"<<hist_names_sort[d]<<"$"<<" & \t "<< std::setprecision(6)<< discriminators[d]<<endl;
}
out_discrimination.close();
TLatex* tex = new TLatex(0.75,0.95,"35.9 fb^{-1} (13 TeV)");
tex->SetNDC();
tex->SetTextAlign(35);
tex->SetTextFont(42);
tex->SetTextSize(0.035);
tex->SetLineWidth(2);
TLatex *tex1 = new TLatex(0.17,0.95,"CMS");
tex1->SetNDC();
tex1->SetTextAlign(20);
tex1->SetTextFont(61);
tex1->SetTextSize(0.04);
tex1->SetLineWidth(2);
TLatex* tex2 = new TLatex(0.27,0.89,"Work in progress");
tex2->SetNDC();
tex2->SetTextAlign(20);
tex2->SetTextFont(52);
tex2->SetTextSize(0.035);
tex2->SetLineWidth(2);
TString temp_str;
temp_str.Form("%2.2f",kfactors[set_type]);
//		temp_str.Form("%2.2f",(1./MC_data[set_type]));
TString k_factor_str;
TLatex* tex_set = new TLatex(0.667,0.86,set_names[set_type]);
tex_set->SetNDC();
tex_set->SetTextAlign(20);
tex_set->SetTextFont(42);
tex_set->SetTextSize(0.03);
tex_set->SetLineWidth(2);
TLatex* tex_k = new TLatex(0.63,0.89,k_factor_str);
tex_k->SetNDC();
tex_k->SetTextAlign(20);
tex_k->SetTextFont(42);
tex_k->SetTextSize(0.03);
tex_k->SetLineWidth(2);
	float left2 = gStyle->GetPadLeftMargin();
	float right2 = gStyle->GetPadRightMargin();
	float top2 = gStyle->GetPadTopMargin();
	float bottom2 = gStyle->GetPadBottomMargin();
	TPaveText pCMSset(0.57,1.-top2*2.,0.67,0.92,"NDC");
	pCMSset.SetTextFont(42);
	pCMSset.SetTextSize(top2*0.75);
	pCMSset.SetTextAlign(12);
	pCMSset.SetFillStyle(-1);
	pCMSset.SetBorderSize(0);
	pCMSset.AddText(set_names[set_type]);

	

    
        
        
        
//------------------------------------- DRAW THE HISTOGRAMS -----------------------------------------------------------
        

for (int i=0;i<nhistos;i++){
//	for (int i=0;i<4;i++){
		//temp_str.Form("%2.2f",Discr(discr_histos[i],signal_histos2[i]));
		Float_t d_value = Discr((TH1F*)data_histos[i]->Clone("data_clone_"+hist_names_sum[i]),signal_histos2[i]);
		temp_str.Form("%2.2f",d_value);
		TString disc_value = temp_str.Prepend(" d = ");
		TLatex *disc_value_text = new TLatex(0.62,0.83,disc_value);
      disc_value_text->SetNDC();
      disc_value_text->SetTextAlign(20);
      disc_value_text->SetTextFont(42);
      disc_value_text->SetTextSize(0.03);
      disc_value_text->SetLineWidth(2);
		
		temp_str.Form("%2d",i);
		TString can_name="c1";
		can_name.Append(temp_str);
		TCanvas *c1 = new TCanvas(can_name,"",900,750);
		c1->cd();
		gPad->SetLogy(0);
		c1->SetBottomMargin(.3);
		c1->SetRightMargin(.25);
	
	//	bool LOGY=false;
	///	if (hist_names[i].CompareTo("hMqq")==0) LOGY=true; 
	//	if (hist_names[i].CompareTo("hqq_pt")==0) LOGY=true; 
		bool LOGY=true;

		Double_t xmin = signal_histos[i]->GetBinCenter(0);
		Double_t xmax = signal_histos[i]->GetBinCenter(signal_histos[i]->GetNbinsX())+signal_histos[i]->GetBinWidth(signal_histos[i]->GetNbinsX());
		if (hist_names[i].CompareTo("hPVs")==0) {
			xmax=30;
			LOGY=false;
		}
		if (hist_names[i].CompareTo("hBDT_VBF")==0){ xmin = BDT_BINS[0];  xmax = BDT_BINS[nBDT_BINS];}
		if (hist_names[i].CompareTo("hBDTG_VBF")==0){ xmin = BDTG_BINS[0];  xmax = BDTG_BINS[nBDTG_BINS ];}
		if (hist_names[i].CompareTo("hMLP_VBF")==0){ xmin = MLP_BINS[0];  xmax = MLP_BINS[nMLP_BINS ];}
		if (hist_names[i].CompareTo("hBDT_VBF_atanh")==0){ xmin = BDT_ATANH_BINS[0];  xmax = BDT_ATANH_BINS[nBDT_ATANH_BINS];}
		if (hist_names[i].CompareTo("hBDTG_VBF_atanh")==0){ xmin = BDTG_ATANH_BINS[0];  xmax = BDTG_ATANH_BINS[nBDTG_ATANH_BINS];}
		if (hist_names[i].CompareTo("hMLP_VBF_atanh")==0){ xmin = MLP_ATANH_BINS[0];  xmax = MLP_ATANH_BINS[nMLP_ATANH_BINS];}

		if ((hist_names[i].CompareTo("hThetaStar")==0) || (hist_names[i].CompareTo("hThetaStarAbs")==0)) {
			LOGY=false;
		}
		string tmp_hist_name = hist_names[i].Data();
		if ((hist_names[i].CompareTo("hAdJetHT_bdt")==0) || (hist_names[i].CompareTo("hNAdJetHT_bdt")==0) ||  (hist_names[i].CompareTo("hJet3_pt_bdt")==0)|| (hist_names[i].CompareTo("hJet3_eta_bdt")==0) ||(tmp_hist_name.find("EWK_bdt")!=std::string::npos) ||(tmp_hist_name.find("_bdt2")!=std::string::npos ) ||(tmp_hist_name.find("_mjj1")!=std::string::npos) ||(tmp_hist_name.find("_mjj2")!=std::string::npos ) ) {
			LOGY=false;
		}
		if (hist_names[i].CompareTo("hMqq_log")==0) {
			xmin=4.5;
			xmax=9;
		}
		if (hist_names[i].CompareTo("hBDT_VBF_atanh")==0 || hist_names[i].CompareTo("hBDTG_VBF_atanh")==0) {
			xmin=-0.1;
			xmax=3;
		}
		if (hist_names[i].CompareTo("hbdt_atanh")==0) {
			xmin=0;
			xmax=3.5;
		}
//		if (hist_names[i].CompareTo("hAdJetHT")==0) {
//			xmax=200;
//		}

		TH1F *frame = new TH1F("frame","",1,xmin,xmax);
		TGaxis::SetExponentOffset(-0.07,0,"xy");
		frame->Reset();
		frame->SetMinimum(0.01);
                frame->SetMaximum(std::max(data_histos[i]->GetMaximum()*1.2,sum_histos[i]->GetMaximum()*1.2) );
		if (LOGY==true) {
			gPad->SetLogy();	
                        frame->SetMaximum(std::max(data_histos[i]->GetMaximum()*100,sum_histos[i]->GetMaximum()*100) );
			if (hist_names[i].CompareTo("hHT")==0) frame->SetMaximum(sum_histos[i]->GetMaximum()*100 );
		}
		TGaxis::SetMaxDigits(4);
                frame->GetXaxis()->SetTitleOffset(0.91);
                frame->SetStats(0);
		frame->GetYaxis()->SetNdivisions(505);
	 	frame->GetXaxis()->SetLabelSize(0.0);
		char name[1000];
		if (tmp_hist_name.find("GeV")==std::string::npos) {
			if (data_histos[i]->GetBinWidth(1)>1) sprintf(name,"Events / %1.0f",data_histos[i]->GetBinWidth(1));
			else sprintf(name,"Events / %1.2f",data_histos[i]->GetBinWidth(1));
		} else {
      	sprintf(name,"Events / %1.0f %s",data_histos[i]->GetBinWidth(1),"GeV");
		}
		frame->GetYaxis()->SetTitle(name);

      frame->Draw();
		tex->Draw();
		tex1->Draw();
		tex2->Draw();
                pCMSset.Draw("same");
	//	tex_k->Draw();
//		tex_set->Draw();
                
                
		if ((d_value>0.01)&&(d_value<1.)) disc_value_text->Draw();

        float totalSensitivity=0.;

		stacks[i]->Draw("same");                       
		if(hist_names[i].CompareTo("hBDT_VBF")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(-1,0);
        }
		if(hist_names[i].CompareTo("hBDTG_VBF")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nBDTG_BINS, "hnew_BDTG", BDTG_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nBDTG_BINS, "hnew_BDTG", BDTG_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDTG_BINS, "hnew_BDTG", BDTG_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(-1,0);
		}
		if(hist_names[i].CompareTo("hMLP_VBF")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nMLP_BINS, "hnew_MLP", MLP_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nMLP_BINS, "hnew_MLP", MLP_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nMLP_BINS, "hnew_MLP", MLP_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
        }
		if(hist_names[i].CompareTo("hBDT_VBF_atanh")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDT_ATANH_BINS, "hnew_BDT_ATANH", BDT_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
        }
		if(hist_names[i].CompareTo("hBDTG_VBF_atanh")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nBDTG_ATANH_BINS, "hnew_BDTG", BDTG_ATANH_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nBDTG_ATANH_BINS, "hnew_BDTG", BDTG_ATANH_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDTG_ATANH_BINS, "hnew_BDTG", BDTG_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
		}
		if(hist_names[i].CompareTo("hMLP_VBF_atanh")==0){
			signal_histos[i] = (TH1F*)signal_histos[i]->Rebin(nMLP_ATANH_BINS, "hnew_BDTG_ATANH", MLP_ATANH_BINS);
			gluonFu_histos[i] = (TH1F*)gluonFu_histos[i]->Rebin(nMLP_ATANH_BINS, "hnew_BDTG_ATANH", MLP_ATANH_BINS);
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nMLP_ATANH_BINS, "hnew_BDTG_ATANH", MLP_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,2.5);
        }
        if (hist_names[i].CompareTo("hHll_mass")==0) {
        	TH1F *h_below_115 = (TH1F*)data_histos[i]->Clone("Mll_below_115");
            TH1F *h_above_130 = (TH1F*)data_histos[i]->Clone("Mll_above_130");
                    
            h_below_115->GetXaxis()->SetRangeUser(108,115);
            h_above_130->GetXaxis()->SetRangeUser(135,160);

            h_below_115->Draw("Psame");
            h_above_130->Draw("Psame");
			signal_histos[i]->Draw("same");
			gluonFu_histos[i]->Draw("same");
			hBkgVis[i]->Draw("same E2");

        } 
        else{
			signal_histos[i]->Draw("same");
			gluonFu_histos[i]->Draw("same");
        	data_histos[i]->Draw("Psame");
			hBkgVis[i]->Draw("same E2");
		}
                        
                if (i>=nhistos-2) {
                    float totalSensitivitySquared=0.;   
                    float totalSensitivitySquaredErrorSquared = 0.;
                        for (int n = 1; n <= signal_histos[i]->GetXaxis()->GetNbins(); ++n) {
                            float histoBin = signal_histos[i]->GetBinContent(n)/20.;  //  (signal_histos[i]->GetBinContent(n) + gluonFu_histos[i]->GetBinContent(n))/20.;
                            float histoBinGlu = gluonFu_histos[i]->GetBinContent(n)/20.;
                            float staskPos = stackHisto[i]->GetBinContent(n) - signal_histos[i]->GetBinContent(n)/20. - gluonFu_histos[i]->GetBinContent(n)/20.;
                            
                            float histoBinVBFError  = signal_histos[i]->GetBinError(n)/20.;
                            float histoBinGluError  = gluonFu_histos[i]->GetBinError(n)/20.;
                            float histoBinSigError  = sqrt(histoBinVBFError*histoBinVBFError + histoBinGluError*histoBinGluError);
                            float histoSTACK_Error  = stackHisto[i]->GetBinError(n);
                            float histoBinBKGError  = sqrt(histoSTACK_Error*histoSTACK_Error - histoBinSigError*histoBinSigError);
                            

                            
                            
                            float Signal_RelativeError = histoBinSigError/(histoBin + histoBinGlu);
                            float BKG_RelativeError = histoBinBKGError/staskPos;
                            float SensitivityRelativeError = (histoBin > 0.00001 && staskPos > 0.00001) ? (4*Signal_RelativeError*Signal_RelativeError + BKG_RelativeError*BKG_RelativeError) : 0.;
                            float SensitivitySquaredErrorSquared = SensitivityRelativeError*histoBin/staskPos*histoBin/staskPos;
                            totalSensitivitySquaredErrorSquared += SensitivitySquaredErrorSquared;
                        
                            
//                             std::cout <<"bin " << n << " \t\t signal VBF " << histoBin  <<" \t +-  " << histoBinVBFError <<  std::endl;
//                             std::cout <<"\t\t signal GLU " << histoBinGlu  <<" \t +-  " << histoBinGluError <<  std::endl;
//                             std::cout <<"\t\t  VBF + GLU " << histoBin + histoBinGlu  <<" \t +-  " << histoBinSigError <<  std::endl;
//                             std::cout <<"\t\t stack      " << staskPos + histoBin + histoBinGlu <<" \t +-  " << histoSTACK_Error <<  std::endl;
//                             std::cout <<"\t\t BKG        " << staskPos  <<" \t +-  " << histoBinBKGError <<  std::endl;
//                             std::cout <<"\t\t relErrors  " <<  Signal_RelativeError <<" \t  " << BKG_RelativeError  << " \t  " << SensitivityRelativeError <<  std::endl;
                            
                            
//                             TFile *file_initial;
//                             file_initial = TFile::Open(file_names[7]);
//                             TH1F * h_check = (TH1F *) file_initial->Get("hBDT_VBF_atanh");
//                             staskPos = h_check->GetBinContent(n)*lumi* ratio[0];
                            
//                             if (staskPos < 0.) {staskPos = 0.; std::cout << "ATTENTION: staskPos < 0.       -----------------------------------------------------------" <<  std::endl;}
                            
                            float binSensitivitySquared = (histoBin > 0.00001 && staskPos > 0.00001) ? histoBin*histoBin/(staskPos) : 0.;
                            totalSensitivitySquared += binSensitivitySquared;
//                             std::cout <<"bin " << n << " \t signal          " << histoBin  <<" \t bkg    " << staskPos << "  \t " << "bin sensitivity  " << binSensitivitySquared << " \t +- " << SensitivitySquaredErrorSquared << std::endl;
//                             std::cout <<"\t Sensitivity^2   " <<   totalSensitivitySquared<< " \t +-  " << totalSensitivitySquaredErrorSquared  <<  std::endl;

                        }
                        
                        
                        
                        
//                        data_histos[nhistos-1]->GetXaxis()->SetRangeUser(-1.,0.5);
//                         data_histos[i]->Draw("Psame");


                        std::ostringstream sigmaString;
//                         sigmaString<<std::setprecision(3)<<totalSensitivity;
                        sigmaString<<std::setprecision(3)<<sqrt(totalSensitivitySquared) ;//<< " +-" << sqrt(totalSensitivitySquaredErrorSquared/2./sqrt(totalSensitivitySquared));
                        TLatex* texSig = new TLatex(0.50,0.85,("Tot sigma = " + sigmaString.str() ).c_str());
                        texSig->SetNDC();
                        texSig->SetTextAlign(35);
                        texSig->SetTextFont(42);
                        texSig->SetTextSize(0.04);
                        texSig->SetLineWidth(2);

                        texSig->Draw();
                    }		

		leg->Draw("same");
                    
        gPad->RedrawAxis();
	
  		TPad* pad2 = new TPad("pad2", "pad2", 0., 0., 1., 1.);
 		pad2->SetTopMargin(0.73);
 		pad2->SetRightMargin(0.25);
 		pad2->SetFillColor(0);
  		pad2->SetFillStyle(0);
  		pad2->Draw();
  		pad2->cd(0);
  		gPad->SetGridy();

		TH1F *frame2 = new TH1F("frame2","",1,xmin,xmax);
		if (hist_names[i].CompareTo("hbtagWeight")==0){
			frame2->SetMinimum(-.25);
    	  	frame2->SetMaximum(.25);
    	}
    	else{
    		frame2->SetMinimum(-1);
    		frame2->SetMaximum(1);
    	}
		if ((hist_names[i].CompareTo("hAdJetHT_bdt")==0)||(hist_names[i].CompareTo("hJet3_pt_bdt")==0)) {
			frame2->SetMinimum(-1.);
    	 	frame2->SetMaximum(1.);
		}
      frame2->SetStats(0);
      frame2->SetTitleFont(42,"x");
		frame2->SetTitleFont(42,"y");
      frame2->SetTitleSize(0.13, "XYZ");
		frame2->GetYaxis()->SetNdivisions(505);
 		frame2->GetYaxis()->SetTickLength(0.06);
  		frame2->GetYaxis()->SetTitleSize(0.04);
  		frame2->GetYaxis()->SetTitleOffset(1.5);
 		frame2->GetYaxis()->SetLabelSize(0.03);
  		frame2->GetYaxis()->CenterTitle(kTRUE);
  		frame2->GetXaxis()->SetTitleSize(0.05);
  		frame2->GetXaxis()->SetLabelSize(0.04);
		frame2->SetXTitle(signal_histos[i]->GetXaxis()->GetTitle());
		if (hist_names[i].CompareTo("hNAdJets_bdt2")==0 ) 
	   	frame2->SetXTitle("N of jets, BDT > 0.84");
// 		if (hist_names[i].CompareTo("hNAdJets_mjj1")==0 ) 
// 	   	frame2->SetXTitle("N of jets, m(qq) > 1500");
// 		if (hist_names[i].CompareTo("hNAdJets_mjj2")==0 ) 
// 	   	frame2->SetXTitle("N of jets, m(qq) > 2500");
		frame2->SetYTitle("Data / MC - 1");
		frame2->Draw();	

 		Double_t aa[2] = {xmin,xmax};
                Double_t bb[2] = {0,0};
                TGraph *cons = new TGraph(2,aa,bb);
                cons->SetLineStyle(2);
		cons->Draw("Lsame");


		if (hist_names[i].CompareTo("hBDT_VBF")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDT_BINS, "data_histos", BDT_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nBDT_BINS, "sum_histos", BDT_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(-1,0);
// 			hnew_BDT = (TH1F*)data_histos[i]->Rebin(nBDT_BINS, "hnew_BDT", BDT_BINS);
// 			TH1F *hnew_BDT_data = (TH1F*)hnew_BDT->Clone("new");
// 			hnew_BDT_2 = (TH1F*)sum_histos[i]->Rebin(nBDT_BINS, "hnew_BDT_2", BDT_BINS);
// 			hnew_BDT->Add(hnew_BDT_2, -1);
// 			hnew_BDT->Divide(hnew_BDT_2);
// 			for (int j=0;j<hnew_BDT->GetNbinsX();j++){
// 				if (hnew_BDT_2->GetBinContent(j+1)!=0) hnew_BDT->SetBinError(j+1, TMath::Sqrt(hnew_BDT_data->GetBinContent(j+1))/hnew_BDT_2->GetBinContent(j+1)) ;
// 				else 
//             	        if (hnew_BDT_data->GetBinContent(j+1)!=0) hnew_BDT->SetBinError(j+1, TMath::Sqrt(hnew_BDT_data->GetBinContent(j+1))/hnew_BDT_data->GetBinContent(j+1)) ;
// 						else hnew_BDT->SetBinError(j+1,0.); 
// 			}
        }
		if (hist_names[i].CompareTo("hBDTG_VBF")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDTG_BINS, "data_histos", BDTG_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nBDTG_BINS, "sum_histos", BDTG_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(-1,0);
		}
		if (hist_names[i].CompareTo("hMLP_VBF")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nMLP_BINS, "data_histos", MLP_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nMLP_BINS, "sum_histos", MLP_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
		}
		if (hist_names[i].CompareTo("hBDT_VBF_atanh")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDT_ATANH_BINS, "data_histos", BDT_ATANH_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nBDT_ATANH_BINS, "sum_histos", BDT_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
		}
		if (hist_names[i].CompareTo("hBDTG_VBF_atanh")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nBDTG_ATANH_BINS, "data_histos", BDTG_ATANH_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nBDTG_ATANH_BINS, "sum_histos", BDTG_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,0.5);
		}
		if (hist_names[i].CompareTo("hMLP_VBF_atanh")==0){			
			data_histos[i] = (TH1F*)data_histos[i]->Rebin(nMLP_ATANH_BINS, "data_histos", MLP_ATANH_BINS);
			sum_histos[i] = (TH1F*)sum_histos[i]->Rebin(nMLP_ATANH_BINS, "sum_histos", MLP_ATANH_BINS);
			data_histos[i]->GetXaxis()->SetRangeUser(0,2.5);
		}


		data_histos2[i] = (TH1F*)data_histos[i]->Clone("new");
		data_histos2[i]->Add(sum_histos[i],-1);
		data_histos2[i]->Divide(sum_histos[i]);

		for (int j=0;j<data_histos2[i]->GetNbinsX();j++){
			if (sum_histos[i]->GetBinContent(j+1)!=0) data_histos2[i]->SetBinError(j+1, TMath::Sqrt(data_histos[i]->GetBinContent(j+1))/sum_histos[i]->GetBinContent(j+1)) ;
			else 
      	        if (data_histos[i]->GetBinContent(j+1)!=0) data_histos2[i]->SetBinError(j+1, TMath::Sqrt(data_histos[i]->GetBinContent(j+1))/data_histos[i]->GetBinContent(j+1)) ;
				else data_histos2[i]->SetBinError(j+1,0.); 
		}
		float chi2_data_mc = 1;//data_histos2[i]->Chi2Test(sum_histos[i],"UWCHI2/NDF");
		float ks = 1;//data_histos2[i]->KolmogorovTest(sum_histos[i]);

		if (hist_names[i].CompareTo("hHll_mass")==0) {
            TH1F *h_below_115_2 = (TH1F*)data_histos2[i]->Clone("Mll_below_115");
            TH1F *h_above_130_2 = (TH1F*)data_histos2[i]->Clone("Mll_above_130");

            h_below_115_2->GetXaxis()->SetRangeUser(90,115);
            h_above_130_2->GetXaxis()->SetRangeUser(135,160);

            h_below_115_2->Draw("PEsame");
            h_above_130_2->Draw("PEsame");
   			hBkgUncUp[i]->Draw("HIST same");
			hBkgUncLo[i]->Draw("HIST same");

        }
        else{
   			hBkgUncUp[i]->Draw("HIST same");
			hBkgUncLo[i]->Draw("HIST same");
			data_histos2[i]->Draw("PEsame");
		}

////////// Per disegnare il ratio plot ///////////
/* 		if ((hist_names[i].CompareTo("hBDT_VBF")!=0) && (hist_names[i].CompareTo("hBDT_VBF_atanh")!=0) && (hist_names[i].CompareTo("hHll_mass")!=0)) 
                  data_histos2[i]->Draw("PEsame");


		if (hist_names[i].CompareTo("hBDT_VBF")==0) {
             data_histos2[i]->GetXaxis()->SetRangeUser(0.,0.5);
             data_histos2[i]->Draw("Psame");
        }

		if (hist_names[i].CompareTo("hBDT_VBF_atanh")==0) {
             data_histos2[i]->GetXaxis()->SetRangeUser(0.,0.5);
             data_histos2[i]->Draw("Psame");
        }

		if (hist_names[i].CompareTo("hHll_mass")==0) {
            data_histos2[i]->GetXaxis()->SetRangeUser(100.,115);
            TH1F *h_below_115_2 = (TH1F*)data_histos2[i]->Clone("Mll_below_115");
            TH1F *h_above_130_2 = (TH1F*)data_histos2[i]->Clone("Mll_above_130");

            h_below_115_2->GetXaxis()->SetRangeUser(90,115);
            h_above_130_2->GetXaxis()->SetRangeUser(130,160);

            h_below_115_2->Draw("Psame");
            h_above_130_2->Draw("Psame");
        }
*/
////////// Per disegnare il ratio plot ///////////
		data_histosbTagJESup[i] = (TH1F*)hBkgbTagJESUp[i]->Clone("newbTagJESup");
		data_histosbTagJESup[i]->SetLineColor(kBlue);
		data_histosbTagJESup[i]->SetLineStyle(2);
		data_histosbTagJESlo[i] = (TH1F*)hBkgbTagJESLo[i]->Clone("newbTagJESlow");
		data_histosbTagJESlo[i]->SetLineStyle(2);
		data_histosbTagJESlo[i]->SetLineColor(kBlue);
// 		cout<<data_histosbTagJESlo[i]->Integral()<< "    "<<data_histosbTagJESup[i]->Integral()<<endl;
		data_histosbTagJESup[i]->Add(sum_histos[i],-1);
		data_histosbTagJESup[i]->Divide(sum_histos[i]);
// 		data_histosbTagJESup[i]->Draw("HISTsame");
		data_histosbTagJESlo[i]->Add(sum_histos[i],-1);
		data_histosbTagJESlo[i]->Divide(sum_histos[i]);
		data_histosbTagJESlo[i]->SetLineColor(kRed);
// 		data_histosbTagJESlo[i]->Draw("HISTsame");
		
		temp_str.Form("%2.2f",chi2_data_mc);
		TString chi2_str = temp_str.Prepend("#chi^{2} = ");
		TLatex *chi2_latex = new TLatex(0.21,0.23,temp_str);
 	  	chi2_latex->SetNDC();
  	 	chi2_latex->SetTextAlign(20);
  		chi2_latex->SetTextFont(42);
   	chi2_latex->SetTextSize(0.025);
   	chi2_latex->SetLineWidth(2);
// 		chi2_latex->Draw();
		temp_str.Form("%2.2f",ks);
		TString ks_str = temp_str.Prepend(", KS = ");
		TLatex *ks_latex = new TLatex(0.31,0.23,temp_str);
 	  	ks_latex->SetNDC();
  	 	ks_latex->SetTextAlign(20);
  		ks_latex->SetTextFont(42);
   	ks_latex->SetTextSize(0.025);
   	ks_latex->SetLineWidth(2);
// 		ks_latex->Draw();
		TLegend *leg_jes = new TLegend(0.16,0.14,0.3,0.17); //without writing about SF
		leg_jes->SetFillStyle(0);
		leg_jes->SetBorderSize(0);
		leg_jes->SetTextFont(42);
		leg_jes->SetTextSize(0.02);
		leg_jes->AddEntry(data_histosbTagJESup[i],"bTag JES scale up","L");
		leg_jes->AddEntry(data_histosbTagJESlo[i],"bTag JES scale down","L");
// 		leg_jes->Draw();
		


		pad2->RedrawAxis();
		c1->Print(output_names[i]);
		c1->Delete();
	}


return 0;



files=0;
totalBG = 0;
totalMC = 0;
totalData = 0;
for (int fileIterator = 0; fileIterator < nfiles; fileIterator++) {

	TFile *file_initial;
  	file_initial = TFile::Open(file_names_bTagJESup[fileIterator]);
	string file_name_tag = file_names_bTagJESup[fileIterator].Data();
	std::cout<<"Up = "<<file_name_tag;
	
	TH1F *histos[1];
	
	histos[0] = (TH1F*)file_initial->Get(hist_names[0])->Clone("h");
	add_underFlow_overFlow(histos[0]);
	if (fileIterator>0)histos[0]->Scale(lumi); 
	if (fileIterator>0) histos[0]->Sumw2(kFALSE);    //  COMMENT THIS FOR ERRORS

	double bkgIntegralError = 0;            
	if (fileIterator>=bg_begin) totalBG+=histos[0]->Integral(1,histos[0]->GetNbinsX());   //from 1 to histos[0]->GetNbinsX() because I added overflow and underflow
	if (fileIterator>0) totalMC+=histos[0]->Integral(1,histos[0]->GetNbinsX());
	if (fileIterator==0) {totalData+=histos[0]->Integral(1,histos[0]->GetNbinsX()); }
	if (fileIterator==0) out_efficiency_up<<"Sample  \t\t\t yield(per "<< lumi<<" pb^-1)"<<endl;
	float bkgIntegral = histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError);
	out_efficiency_up<<leg_names[order[fileIterator]]<<"\t\t\t"<< std::setprecision(6)<<histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError) << " +- " << bkgIntegralError  <<endl;
	std::cout<<"\t"<<histos[0]->Integral(1,histos[0]->GetNbinsX())<<std::endl;
	if (fileIterator==nfiles-1) out_efficiency_up<<"Total BG"<<"\t \t \t  "<<std::setprecision(6)<<totalBG<<endl;
	if (fileIterator==nfiles-1) out_efficiency_up<<"Total MC"<<"\t \t \t  "<<std::setprecision(6)<<totalMC<<endl;
	if (fileIterator==nfiles-1) out_efficiency_up<<"Data/MC"<<"\t \t \t \t  "<<std::setprecision(6)<<ratio[0]<< " +- " << ratioError[0] <<endl;
}

std::cout<<""<<std::endl;

files=0;
totalBG = 0;
totalMC = 0;
totalData = 0;
for (int fileIterator = 0; fileIterator < nfiles; fileIterator++) {

	TFile *file_initial;
  	file_initial = TFile::Open(file_names_bTagJESdown[fileIterator]);
	string file_name_tag = file_names_bTagJESdown[fileIterator].Data();
	std::cout<<"Down = "<<file_name_tag;
	
	TH1F *histos[1];
	
	histos[0] = (TH1F*)file_initial->Get(hist_names[0])->Clone("h");
	add_underFlow_overFlow(histos[0]);
	if (fileIterator>0)histos[0]->Scale(lumi); 
	if (fileIterator>0) histos[0]->Sumw2(kFALSE);    //  COMMENT THIS FOR ERRORS

	double bkgIntegralError = 0;            
	if (fileIterator>=bg_begin) totalBG+=histos[0]->Integral(1,histos[0]->GetNbinsX());   //from 1 to histos[0]->GetNbinsX() because I added overflow and underflow
	if (fileIterator>0) totalMC+=histos[0]->Integral(1,histos[0]->GetNbinsX());
	if (fileIterator==0) {totalData+=histos[0]->Integral(1,histos[0]->GetNbinsX()); }
	if (fileIterator==0) out_efficiency_down<<"Sample  \t\t\t yield(per "<< lumi<<" pb^-1)"<<endl;
	float bkgIntegral = histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError);
	out_efficiency_down<<leg_names[order[fileIterator]]<<"\t\t\t"<< std::setprecision(6)<<histos[0]->IntegralAndError(1,histos[0]->GetNbinsX(), bkgIntegralError) << " +- " << bkgIntegralError  <<endl;
	std::cout<<"\t"<<histos[0]->Integral(1,histos[0]->GetNbinsX())<<std::endl;
	if (fileIterator==nfiles-1) out_efficiency_down<<"Total BG"<<"\t \t \t  "<<std::setprecision(6)<<totalBG<<endl;
	if (fileIterator==nfiles-1) out_efficiency_down<<"Total MC"<<"\t \t \t  "<<std::setprecision(6)<<totalMC<<endl;
	if (fileIterator==nfiles-1) out_efficiency_down<<"Data/MC"<<"\t \t \t \t  "<<std::setprecision(6)<<ratio[0]<< " +- " << ratioError[0] <<endl;
}











return 0;
}
