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
        
TH1D* Proiezione(TString fileTag, TString file, int bin){
		
		TString NOME = Form("prova_%i", bin);
		TFile *file1 = new TFile(file);
		TH2F * th2f = (TH2F*) file1->Get(fileTag)->Clone();
		TH1D* proj = th2f->ProjectionX(NOME, bin, bin);		
		return proj;
}    
        
using namespace std;

int main(int argc, char* argv[]){

    
std::cout << "I can see here" << std::endl;

gROOT->ProcessLine(".x setTDRStyle.C");

int set_type=atoi(argv[1]); // 0 - analysis, 1 - control region , top

TString dir_save = "./TH2F_Prove/";

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


TString file_names[nfiles] =        {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_UPup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_UPdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_QCDup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_QCDdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_bTagJESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_bTagJESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JERup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names_JERdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};

int bg_begin;
int qcd_begin=18;
 bg_begin=1;

int FILLCOLOR[nfiles] = {1,kSpring+7,kSpring+8, kSpring+5, kGreen-3, kAzure+1,kBlue-4,kOrange-2,kRed-4,kMagenta+1};
int LINECOLOR[nfiles] = {1,kSpring+7,kSpring+8, kSpring+5, kGreen-3,kAzure+1,kBlue-4,kOrange-2,kRed-4,kMagenta+1};
int LINESTYLE[nfiles] = {1,1,1,1,1,1,1,1,1,2};
int LINEWIDTH[nfiles] = {1,1,1,1,1,1,1,1,3,3};
int FILLSTYLE[nfiles] = {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001};

float signalMultipyFactor = 20.;
	
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
//         file_names[i].Prepend(directory);
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

const int nhistos =  26; //79 //40//52

Double_t NPROVA = 10;
Double_t PROVA_BINS[] = {110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130};

Double_t BDT_BINS[] = {0, 0.2, 0.4, 0.6, 0.8, 1.0};//{-1.002, -0.768, -0.666, -0.564, -0.462, -0.36, -0.26, -0.158, -0.058, 0.042, 0.144, 0.244, 0.346, 0.446, 0.548, 0.65, 0.75};
Double_t BDT_ATANH_BINS[] = {-0.005, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.255, 1.355};
Double_t BDTG_BINS[] = {-1.002, -0.948, -0.846, -0.744, -0.642, -0.542, -0.442, -0.34, -0.238, -0.136, -0.036, 0.064, 0.164, 0.264, 0.366, 0.468, 0.57, 0.672, 0.774, 0.876, 0.978};
Double_t BDTG_ATANH_BINS[] = {-0.005, 0, 0.105, 0.21, 0.315, 0.42, 0.525, 0.63, 0.73, 0.835, 0.94, 1.045, 1.15, 1.25, 1.35, 1.45, 1.555, 1.66, 1.765, 1.865, 1.965, 2.07, 2.175, 2.28, 2.38, 2.485, 2.585};
Double_t MLP_BINS[] = {-0.001, 0.095, 0.196, 0.297, 0.397, 0.498, 0.599, 0.699, 0.799, 0.9, 1};
Double_t MLP_ATANH_BINS[] = {-0.005, 0.525, 0.625, 0.725, 0.83, 0.93, 1.035, 1.14, 1.24, 1.34, 1.44, 1.545, 1.65, 1.755, 1.855, 1.955, 2.06, 2.165, 2.265, 2.37, 2.475, 2.58, 2.685, 2.79, 2.89, 2.995, 3.1, 3.205, 3.31, 3.415, 3.515, 3.62, 3.725, 3.83, 3.935, 4.765};
Int_t nBDT_BINS = 5;//16;
Int_t nBDT_ATANH_BINS = 13;
Int_t nBDTG_BINS = 20;
Int_t nBDTG_ATANH_BINS = 26;
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

TString MVA = "hHll_vs_MVA";

TCanvas *canvas = new TCanvas("ola", "ola", 200,10,700,500);
TH1F * proj;
TH1F * data;
TH1F * vbf;
TH1F * ggF;
TH1F * vbf_2;
TH1F * ggF_2;
TH1F * sum;
TH1F * bkgErr;
TH1F * bkgErrUp;
TH1F * bkgErrLo;
TH1F * ratio;

TH1F *histos_for_legened;
float xmin = 0;//108;
float xmax = 2.5;//150;
float ymax, ymin;
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
TLegend *leg = new TLegend(0.77,0.65,0.92,0.9); //without writing about SF
leg->SetFillColor(0);
leg->SetBorderSize(0);
leg->SetTextFont(42);
leg->SetTextSize(0.025);

TFile * file_data   = new TFile (file_names[0]);
TH2F* th2f = (TH2F*)file_data->Get(MVA);

int Xbins = th2f->GetNbinsY();

int rebin_value = 10;


for(int binIterator = 1; binIterator < Xbins/rebin_value+1; binIterator++){
	THStack *stacks = new THStack(MVA,"");
	TString bhu = Form("_%i", binIterator);
	TString NOME = MVA;
 	TCanvas *canvas = new TCanvas(NOME, NOME, 500, 500);
	gPad->SetLogy(1);
	canvas->SetBottomMargin(.3);
	canvas->SetRightMargin(.25);
	
	std::cout<<" ---------------- "<<binIterator<<std::endl;

	for(int fileIterator = 0; fileIterator < nfiles; fileIterator++){
		TFile * nome_file   = new TFile (file_names[fileIterator]);
		TH2F* th2f = (TH2F*)nome_file->Get(MVA);
		th2f = (TH2F*)th2f->RebinY(rebin_value);//NPROVA, "th2f", PROVA_BINS);
		
		
		NOME = Form("Ciao_%.0f", th2f->GetYaxis()->GetBinLowEdge(binIterator));
		
		if(fileIterator == 0) std::cout<<binIterator<<"\t"<<binIterator+1<<std::endl;
		proj = (TH1F*)th2f->ProjectionX(NOME, binIterator, binIterator+1);
// 		proj = (TH1F*)proj->Rebin(NPROVA, "proj", PROVA_BINS);
		if(binIterator > 10){
			TCanvas *c1 = new TCanvas("c1", "c1", 500 ,500);
			proj->Draw();
			c1->Print("ola.png");
			}

		if (fileIterator >0){ 
			proj->Scale(lumi); 
			if (fileIterator==1) 	sum = (TH1F*)proj->Clone(NOME);
			if (fileIterator>1)	sum->Add(proj); 
			proj->Sumw2(kFALSE); 
		}

		if(fileIterator == 0){
			data = (TH1F*)th2f->ProjectionX(NOME, binIterator, binIterator+1);
// 			data = (TH1F*)data->Rebin(NPROVA, "data", PROVA_BINS);
			add_underFlow_overFlow(data);
		}	

		if(fileIterator == nfiles-2){
			vbf = (TH1F*)th2f->ProjectionX(NOME, binIterator, binIterator+1);
// 			vbf = (TH1F*)vbf->Rebin(NPROVA, "vbf", PROVA_BINS);

			vbf_2 = (TH1F*) vbf->Clone();

			vbf->Scale(lumi);
			vbf->Scale(signalMultipyFactor);
			vbf->Sumw2(kFALSE);
			vbf->SetLineColor(LINECOLOR[fileIterator]);
// 			vbf->SetLineWidth(LINEWIDTH[fileIterator]);
			vbf->SetLineStyle(LINESTYLE[fileIterator]);			
			add_underFlow_overFlow(vbf);

			vbf_2->Scale(lumi);
			vbf_2->Scale(signalMultipyFactor);
			vbf_2->Sumw2(kFALSE);
			vbf_2->SetLineColor(LINECOLOR[fileIterator]);
			vbf_2->SetLineWidth(LINEWIDTH[fileIterator]);
			vbf_2->SetLineStyle(LINESTYLE[fileIterator]);			
			add_underFlow_overFlow(vbf_2);

		}
		
		if(fileIterator == nfiles-1){
			ggF = (TH1F*)th2f->ProjectionX(NOME, binIterator, binIterator+1);
// 			ggF = (TH1F*)ggF->Rebin(NPROVA, "ggF", PROVA_BINS);

			ggF_2 = (TH1F*) ggF->Clone();

			ggF->Scale(lumi);
			ggF->Scale(signalMultipyFactor);
			ggF->Sumw2(kFALSE);
			ggF->SetLineColor(LINECOLOR[fileIterator]);
// 			ggF->SetLineWidth(LINEWIDTH[fileIterator]);
			ggF->SetLineStyle(LINESTYLE[fileIterator]);			
			add_underFlow_overFlow(ggF);
			
			ggF_2->Scale(lumi);
			ggF_2->Scale(signalMultipyFactor);
			ggF_2->Sumw2(kFALSE);
			ggF_2->SetLineColor(LINECOLOR[fileIterator]);
			ggF_2->SetLineWidth(LINEWIDTH[fileIterator]);
			ggF_2->SetLineStyle(LINESTYLE[fileIterator]);			
			add_underFlow_overFlow(ggF_2);

		}
		
		proj->SetLineColor(LINECOLOR[fileIterator]);
		proj->SetLineStyle(LINESTYLE[fileIterator]);
		proj->SetLineWidth(LINEWIDTH[fileIterator]);
		proj->SetFillStyle(FILLSTYLE[fileIterator]);
		if ((fileIterator!=0)) proj->SetFillColor(FILLCOLOR[fileIterator]);

		if (fileIterator==0) {
			proj->SetMarkerStyle(20);
			data->SetLineColor(1);
			data->SetMarkerStyle(10);
			data->SetMarkerSize(.7);
		}
		
	 	if (fileIterator>=1) {
// 					if (hist_names[hist].CompareTo("hBDT_VBF")==0){
// 			proj = (TH1F*)proj->Rebin(NPROVA, "ggF", PROVA_BINS);
			stacks->Add(proj);
	    }

		if(binIterator == 1){
				if(fileIterator == 0)
					leg->AddEntry(data,leg_names[fileIterator],"P");
				if (fileIterator > 0 && fileIterator < nfiles-2)
					leg->AddEntry(proj,leg_names[fileIterator],"F");
		}

	}


  	bkgErr = (TH1F*)sum->Clone("hbbkgErr");
	bkgErrUp = (TH1F*)sum->Clone("hBkgUncUp");
	bkgErrLo = (TH1F*)sum->Clone("hBkgUncLo");

  	for(int i=0;i<bkgErrUp->GetNbinsX();i++) {
  		float e = 0.0;
    	if (sum->GetBinContent(i+1) != 0)
    		e = sum->GetBinError(i+1)/sum->GetBinContent(i+1);

    	bkgErrUp->SetBinContent(i+1,e);
    	bkgErrLo->SetBinContent(i+1,-e);
	}

  	bkgErr->SetMarkerSize(0);
 	bkgErr->SetFillColor(kBlack);
 	bkgErr->SetFillStyle(3004);
 	bkgErrUp->SetLineColor(kBlack);
 	bkgErrUp->SetLineWidth(1);
 	bkgErrUp->SetFillColor(kBlack);
 	bkgErrUp->SetFillStyle(3004);
	bkgErrLo->SetLineColor(kBlack);
 	bkgErrLo->SetLineWidth(1);
  	bkgErrLo->SetFillColor(kBlack);
	bkgErrLo->SetFillStyle(3004);

	ratio = (TH1F*)data->Clone("new");
	ratio->Add(sum,-1);
	ratio->Divide(sum);
	for (int j=0;j<ratio->GetNbinsX();j++){
		if (sum->GetBinContent(j+1)!=0)
			ratio->SetBinError(j+1, TMath::Sqrt(data->GetBinContent(j+1))/sum->GetBinContent(j+1));
		else 
			if (data->GetBinContent(j+1)!=0)
				ratio->SetBinError(j+1, TMath::Sqrt(data->GetBinContent(j+1))/data->GetBinContent(j+1));
			else 
				ratio->SetBinError(j+1,0.); 
	}

	if(binIterator == 1){
		leg->AddEntry(vbf,leg_names[nfiles-2],"L");
		leg->AddEntry(ggF,leg_names[nfiles-1],"L");
		leg->AddEntry(bkgErr,"MC stat. unc.","F");
	}
	
	TH1F *frame = new TH1F("frame","",1,xmin,xmax);
	TGaxis::SetExponentOffset(-0.07,0,"xy");
	frame->Reset();
	frame->SetMinimum(0.01);
	frame->SetMaximum(std::max(data->GetMaximum()*1.2,stacks->GetMaximum()*1.2));
	TGaxis::SetMaxDigits(4);
	frame->GetXaxis()->SetTitleOffset(0.91);
	frame->SetStats(0);
	frame->GetYaxis()->SetNdivisions(505);
	frame->GetXaxis()->SetLabelSize(0.0);
	
	char name[1000];
// // 		if (tmp_hist_name.find("GeV")==std::string::npos) {
// // 			if (data_histos[i]->GetBinWidth(1)>1) sprintf(name,"Events / %1.0f",data_histos[i]->GetBinWidth(1));
// // 			else sprintf(name,"Events / %1.2f",data_histos[i]->GetBinWidth(1));
// // 		} else {
// // 		}
	sprintf(name,"Events / %1.0f %s", data->GetBinWidth(1),"GeV");
	frame->GetYaxis()->SetTitle(name);

    frame->Draw();
	tex->Draw();
	tex1->Draw();
	tex2->Draw();
	pCMSset.Draw("same");

	stacks->Draw("same");
// 	data->Draw("PEsame");
    TH1F *h_below_115 = (TH1F*)data->Clone("Mll_below_115");
//     TH1F *h_above_130 = (TH1F*)data->Clone("Mll_above_130");                
    h_below_115->GetXaxis()->SetRangeUser(0, 0.8692);
//     h_above_130->GetXaxis()->SetRangeUser(130,150);
    h_below_115->Draw("PEsame");
//     h_above_130->Draw("PEsame");

	vbf_2->Draw("same");
	ggF_2->Draw("same");
	bkgErr->Draw("same E2");
	leg->Draw("same");
	
	TPad* pad2 = new TPad("pad2", "pad2", 0., 0., 1., 1.);
	pad2->SetTopMargin(0.73);
	pad2->SetRightMargin(0.25);
	pad2->SetFillColor(0);
	pad2->SetFillStyle(0);
	pad2->Draw();
 	pad2->cd(0);
 	gPad->SetGridy();
	TH1F *frame2 = new TH1F("frame2","",1,xmin,xmax);
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
// 	frame2->SetXTitle(vbf->GetXaxis()->GetTitle());
// 	frame2->SetXTitle("m_{ll} GeV");
	frame2->SetXTitle("MVA");
	frame2->SetYTitle("Data / MC - 1");
	frame2->SetMinimum(-1.);
 	frame2->SetMaximum(1.);
	frame2->Draw();	
   	bkgErrUp->Draw("HIST same");
	bkgErrLo->Draw("HIST same");
// 	ratio->Draw("PEsame");
    h_below_115 = (TH1F*)ratio->Clone("Mll_below_115");
//     h_above_130 = (TH1F*)ratio->Clone("Mll_above_130");                
    h_below_115->GetXaxis()->SetRangeUser(0,0.8692);
//     h_above_130->GetXaxis()->SetRangeUser(130,150);
    h_below_115->Draw("Psame");
//     h_above_130->Draw("Psame");



	std::cout<<"prima = "<<binIterator;
// 	binIterator +=rebin_value-1;
	if(binIterator==1)
 		canvas->Print(dir_save + "Ciao.pdf[");
	canvas->Print(dir_save + "Ciao.pdf");
 	if(binIterator == Xbins/rebin_value)
 		canvas->Print(dir_save + "Ciao.pdf]");
 	canvas->Print();
 	
	std::cout<<"\tdopo = "<<binIterator<<std::endl;
}
	
	return 0;
}
