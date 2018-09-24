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




void addErrorToHist (TH1F * histo, float sigma, int n) {
        histo->SetBinContent(n, histo->GetBinContent(n) + histo->GetBinError(n)*sigma);
}

void writeStatisticalErrors (TString BDTversionName, TString fileTag, TFile * file_FitHisto, TFile * file, float lumi) {
    
    
        TH1F * histo_BDT_nom = (TH1F*) file->Get(BDTversionName)->Clone();
        
        for (int n = 0; n < histo_BDT_nom->GetNbinsX()+1; ++n) {
            TH1F * histoStat_BDT_Up = (TH1F*) file->Get(BDTversionName)->Clone();
            TH1F * histoStat_BDT_Down = (TH1F*) file->Get(BDTversionName)->Clone();

            std::string binNumber = std::to_string(n);
            
            histoStat_BDT_Up->SetName((BDTversionName+"_mu_"+fileTag+"_Stat"+fileTag+binNumber+"_Up").Data());
            histoStat_BDT_Down->SetName((BDTversionName+"_mu_"+fileTag+"_Stat"+fileTag+binNumber+"_Down").Data());

            
            addErrorToHist(histoStat_BDT_Up,1,n);
            addErrorToHist(histoStat_BDT_Down,-1,n);
            
            
            histoStat_BDT_Up->Scale(lumi);
            histoStat_BDT_Down->Scale(lumi);
        
                
            file_FitHisto->cd();
                        
            histoStat_BDT_Up->Write();
            histoStat_BDT_Down->Write();
        }
}


void macroFitHisto_NoTan () {
    std::cout << "I can see here" << std::endl;

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


const int nfiles  = 9;
// TString file_names[nfiles] = {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
TString file_names[nfiles] = {"WJetsToLNu","WW","ZZ","WZ","ST","TT","DYJetstoLL_amc_Filter105","VBF_HToMuMu", "GluGlu_HToMuMu"};
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


TString file_data_name = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/SingleMuon";
TString file_tag[nfiles];

//TString file_names_QCDup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_QCDdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_JESup[nfiles] =  {"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};
//TString file_names_JESdown[nfiles] ={"SingleMuon","WJetsToLNu","WW","ZZ","WZ","ST","TT",DY_file_name,"VBF_HToMuMu", "GluGlu_HToMuMu"};


for (int i=0;i<nfiles;i++){
    file_tag[i] = file_names[i];
//    file_names[i].Prepend("/afs/pi.infn.it/user/mandorli/mandorli/Hmumu/CMSSW_8_0_28/src/giulioMandorli/histoFileDir/10_07_Agnese/"); 

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
    
    file_names[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_PUdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_PUup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_QCDup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_QCDdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_JESup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_JESdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_JERup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_JERdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagJESup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagJESdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFup[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFdown[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFStats1up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFStats1down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFStats2up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagHFStats2down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFStats1up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFStats1down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFStats2up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagLFStats2down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagcfErr1up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagcfErr1down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagcfErr2up[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");
    file_names_bTagcfErr2down[i].Prepend("/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/histoFileDir/10_07_Agnese/");

    file_names[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_v25_reskim.root");
    file_names_PUup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUup_v25_reskim.root");
    file_names_PUdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUdown_v25_reskim.root");
    file_names_QCDup[i].Append("_QCDScaleup_JESnom_bTagJESnom_JERnom_PUnom_v25_reskim.root");
    file_names_QCDdown[i].Append("_QCDScaledown_JESnom_bTagJESnom_JERnom_PUnom_v25_reskim.root");
    file_names_JESup[i].Append("_QCDScalenom_JESup_bTagJESnom_JERnom_PUnom_v25_reskim.root");
    file_names_JESdown[i].Append("_QCDScalenom_JESdown_bTagJESnom_JERnom_PUnom_v25_reskim.root");
    file_names_JERup[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERup_PUnom_v25_reskim.root");
    file_names_JERdown[i].Append("_QCDScalenom_JESnom_bTagJESnom_JERdown_PUnom_v25_reskim.root");
    file_names_bTagJESup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagJESdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFup[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFdown[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFStats1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFStats1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFStats2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagHFStats2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFStats1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFStats1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFStats2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagLFStats2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagcfErr1up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagcfErr1down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");
    file_names_bTagcfErr2up[i].Append("_QCDScalenom_JESnom_bTagJESup_JERnom_PUnom_v25_reskim.root");
    file_names_bTagcfErr2down[i].Append("_QCDScalenom_JESnom_bTagJESdown_JERnom_PUnom_v25_reskim.root");

}
    file_data_name.Append("_mu");
    file_data_name.Append("_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom_v25_reskim.root");

//     file_tag[6] = "DYJetsToLL";



//    TFile * fileDY_Jet   = new TFile (fileDY_name_Jet.c_str());
//    TFile * fileDY_Pt    = new TFile (fileDY_name_Pt.c_str());
    TFile * file_FitHisto= new TFile ("hBDTG_VBF_Systematics.root", "recreate");
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
        TFile * file_bTagJESup  = new TFile (file_names_bTagJESup[i]);
        TFile * file_bTagJESdown   = new TFile (file_names_bTagJESdown[i]);
        TFile * file_bTagHFup   = new TFile (file_names_bTagHFup[i]);
        TFile * file_bTagHFdown   = new TFile (file_names_bTagHFdown[i]);
        TFile * file_bTagLFup   = new TFile (file_names_bTagLFup[i]);
        TFile * file_bTagLFdown   = new TFile (file_names_bTagLFdown[i]);
        TFile * file_bTagHFStats1up   = new TFile (file_names_bTagHFStats1up[i]);
        TFile * file_bTagHFStats1down   = new TFile (file_names_bTagHFStats1down[i]);
        TFile * file_bTagHFStats2up   = new TFile (file_names_bTagHFStats2up[i]);
        TFile * file_bTagHFStats2down   = new TFile (file_names_bTagHFStats2down[i]);
        TFile * file_bTagLFStats1up   = new TFile (file_names_bTagLFStats1up[i]);
        TFile * file_bTagLFStats1down   = new TFile (file_names_bTagLFStats1down[i]);
        TFile * file_bTagLFStats2up   = new TFile (file_names_bTagLFStats2up[i]);
        TFile * file_bTagLFStats2down   = new TFile (file_names_bTagLFStats2down[i]);
        TFile * file_bTagcfErr1up   = new TFile (file_names_bTagcfErr1up[i]);
        TFile * file_bTagcfErr1down   = new TFile (file_names_bTagcfErr1down[i]);
        TFile * file_bTagcfErr2up   = new TFile (file_names_bTagcfErr2up[i]);
        TFile * file_bTagcfErr2down   = new TFile (file_names_bTagcfErr2down[i]);
 
        
        TString histTitle = "hBDTG_VBF_";
        for (int j=10;j<Nsyst_NoConst;++j){
			std::cout<<(histTitle+"mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Up").Data()<<std::endl;
            TH1F * histoSys_atanhBDT_Up = (TH1F*) file->Get((histTitle+"mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Up").Data())->Clone();
            TH1F * histoSys_atanhBDT_Down = (TH1F*) file->Get((histTitle+"mu_"+file_tag[i]+"_"+uncertainty_name[j]+"_Down").Data())->Clone();

            file_FitHisto->cd();

            histoSys_atanhBDT_Up->Scale(lumi);
            histoSys_atanhBDT_Down->Scale(lumi);
            
            histoSys_atanhBDT_Up->Write();
            histoSys_atanhBDT_Down->Write();

        }
             
        TH1F * histo_atanhBDT_nom = (TH1F*) file->Get("hBDTG_VBF")->Clone();
        histo_atanhBDT_nom->SetName((histTitle+"mu_"+file_tag[i]).Data());

        TString histTitleLinear = "hBDTG_VBF";
        writeStatisticalErrors (histTitleLinear, file_tag[i], file_FitHisto, file, lumi);

//         TH1F * histoSys_atanhBDT_Up_ = (TH1F*) file->Get((histTitle+"mu_"+file_tag[i]+"_"+uncertainty_name[0]+"_Up").Data())->Clone();        

        TH1F * histoSys_atanhBDT_PUup      = (TH1F*) file_PUup->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_PUup_get->Clone(histoSys_atanhBDT_PUup_get->GetName());
        TH1F * histoSys_atanhBDT_PUdown    = (TH1F*) file_PUdown->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_PUdown_get->Clone(histoSys_atanhBDT_PUdown_get->GetName());                
        TH1F * histoSys_atanhBDT_QCDup      = (TH1F*) file_QCDup->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_QCDup_get->Clone(histoSys_atanhBDT_QCDup_get->GetName());
        TH1F * histoSys_atanhBDT_QCDdown    = (TH1F*) file_QCDdown->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_QCDdown_get->Clone(histoSys_atanhBDT_QCDdown_get->GetName());
        TH1F * histoSys_atanhBDT_JESup      = (TH1F*) file_JESup->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_JESup_get->Clone(histoSys_atanhBDT_JESup_get->GetName());
        TH1F * histoSys_atanhBDT_JESdown    = (TH1F*) file_JESdown->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_JESdown_get->Clone(histoSys_atanhBDT_JESdown_get->GetName());
        TH1F * histoSys_atanhBDT_JERup      = (TH1F*) file_JERup->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_JERup_get->Clone(histoSys_atanhBDT_JERup_get->GetName());
        TH1F * histoSys_atanhBDT_JERdown    = (TH1F*) file_JERdown->Get("hBDTG_VBF");//(TH1F*) histoSys_atanhBDT_JERdown_get->Clone(histoSys_atanhBDT_JERdown_get->GetName());
        TH1F * histoSys_atanhBDT_bTagJESup      = (TH1F*) file_bTagJESup->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagJESdown    = (TH1F*) file_bTagJESdown->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFup      = (TH1F*) file_bTagHFup->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFdown    = (TH1F*) file_bTagHFdown->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFup      = (TH1F*) file_bTagLFup->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFdown    = (TH1F*) file_bTagLFdown->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFStats1up      = (TH1F*) file_bTagHFStats1up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFStats1down    = (TH1F*) file_bTagHFStats1down->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFStats2up      = (TH1F*) file_bTagHFStats2up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagHFStats2down    = (TH1F*) file_bTagHFStats2down->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFStats1up      = (TH1F*) file_bTagLFStats1up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFStats1down    = (TH1F*) file_bTagLFStats1down->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFStats2up      = (TH1F*) file_bTagLFStats2up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagLFStats2down    = (TH1F*) file_bTagLFStats2down->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagcfErr1up      = (TH1F*) file_bTagcfErr1up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagcfErr1down    = (TH1F*) file_bTagcfErr1down->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagcfErr2up      = (TH1F*) file_bTagcfErr2up->Get("hBDTG_VBF");
        TH1F * histoSys_atanhBDT_bTagcfErr2down    = (TH1F*) file_bTagcfErr2down->Get("hBDTG_VBF");

        histoSys_atanhBDT_PUup->SetName((histTitle+"mu_"+file_tag[i]+"_PU_Up").Data());
        histoSys_atanhBDT_PUdown->SetName((histTitle+"mu_"+file_tag[i]+"_PU_Down").Data());                
        histoSys_atanhBDT_QCDup->SetName((histTitle+"mu_"+file_tag[i]+"_QCD_Up").Data());
        histoSys_atanhBDT_QCDdown->SetName((histTitle+"mu_"+file_tag[i]+"_QCD_Down").Data());
        histoSys_atanhBDT_JESup->SetName((histTitle+"mu_"+file_tag[i]+"_JES_Up").Data());
        histoSys_atanhBDT_JESdown->SetName((histTitle+"mu_"+file_tag[i]+"_JES_Down").Data());
        histoSys_atanhBDT_JERup->SetName((histTitle+"mu_"+file_tag[i]+"_JER_Up").Data());
        histoSys_atanhBDT_JERdown->SetName((histTitle+"mu_"+file_tag[i]+"_JER_Down").Data());
        histoSys_atanhBDT_bTagJESup->SetName((histTitle+"mu_"+file_tag[i]+"_bTagJES_Up").Data());
        histoSys_atanhBDT_bTagJESdown->SetName((histTitle+"mu_"+file_tag[i]+"_bTagJES_Down").Data());
        histoSys_atanhBDT_bTagHFup->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHF_Up").Data());
        histoSys_atanhBDT_bTagHFdown->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHF_Down").Data());
        histoSys_atanhBDT_bTagLFup->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLF_Up").Data());
        histoSys_atanhBDT_bTagLFdown->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLF_Down").Data());
        histoSys_atanhBDT_bTagHFStats1up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHFStats1_Up").Data());
        histoSys_atanhBDT_bTagHFStats1down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHFStats1_Down").Data());
        histoSys_atanhBDT_bTagHFStats2up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHFStats2_Up").Data());
        histoSys_atanhBDT_bTagHFStats2down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagHFStats2_Down").Data());
        histoSys_atanhBDT_bTagLFStats1up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLFStats1_Up").Data());
        histoSys_atanhBDT_bTagLFStats1down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLFStats1_Down").Data());
        histoSys_atanhBDT_bTagLFStats2up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLFStats2_Up").Data());
        histoSys_atanhBDT_bTagLFStats2down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagLFStats2_Down").Data());
        histoSys_atanhBDT_bTagcfErr1up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagcfErr1_Up").Data());
        histoSys_atanhBDT_bTagcfErr1down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagcfErr1_Down").Data());
        histoSys_atanhBDT_bTagcfErr2up->SetName((histTitle+"mu_"+file_tag[i]+"_bTagcfErr2_Up").Data());
        histoSys_atanhBDT_bTagcfErr2down->SetName((histTitle+"mu_"+file_tag[i]+"_bTagcfErr2_Down").Data());

//         std::cout << histoSys_BDT_JESup << "      "+histTitle+"mu_"+file_tag[i]+"_JER_Down \t" << histoSys_BDT_JESup->GetEntries() << std::endl;

        histoSys_atanhBDT_PUup->Scale(lumi);
        histoSys_atanhBDT_PUdown->Scale(lumi);
        histoSys_atanhBDT_QCDup->Scale(lumi);
        histoSys_atanhBDT_QCDdown->Scale(lumi);
        histoSys_atanhBDT_JESup->Scale(lumi);
        histoSys_atanhBDT_JESdown->Scale(lumi);
        histoSys_atanhBDT_JERup->Scale(lumi);
        histoSys_atanhBDT_JERdown->Scale(lumi);
        histoSys_atanhBDT_bTagJESup->Scale(lumi);
        histoSys_atanhBDT_bTagJESdown->Scale(lumi);
        histoSys_atanhBDT_bTagHFup->Scale(lumi);
        histoSys_atanhBDT_bTagHFdown->Scale(lumi);
        histoSys_atanhBDT_bTagLFup->Scale(lumi);
        histoSys_atanhBDT_bTagLFdown->Scale(lumi);
        histoSys_atanhBDT_bTagHFStats1up->Scale(lumi);
        histoSys_atanhBDT_bTagHFStats1down->Scale(lumi);
        histoSys_atanhBDT_bTagHFStats2up->Scale(lumi);
        histoSys_atanhBDT_bTagHFStats2down->Scale(lumi);
        histoSys_atanhBDT_bTagLFStats1up->Scale(lumi);
        histoSys_atanhBDT_bTagLFStats1down->Scale(lumi);
        histoSys_atanhBDT_bTagLFStats2up->Scale(lumi);
        histoSys_atanhBDT_bTagLFStats2down->Scale(lumi);
        histoSys_atanhBDT_bTagcfErr1up->Scale(lumi);
        histoSys_atanhBDT_bTagcfErr1down->Scale(lumi);
        histoSys_atanhBDT_bTagcfErr2up->Scale(lumi);
        histoSys_atanhBDT_bTagcfErr2down->Scale(lumi);
        
        
        
//         histoSys_atanhBDT_Up_->Scale(lumi);

        histo_atanhBDT_nom->Scale(lumi);   
                
        file_FitHisto->cd();

        
        histoSys_atanhBDT_PUup->Write();
        histoSys_atanhBDT_PUdown->Write();
        histoSys_atanhBDT_QCDup->Write();
        histoSys_atanhBDT_QCDdown->Write();
        histoSys_atanhBDT_JESup->Write();
        histoSys_atanhBDT_JESdown->Write();
        histoSys_atanhBDT_JERup->Write();
        histoSys_atanhBDT_JERdown->Write();
        histoSys_atanhBDT_bTagJESup->Write();
        histoSys_atanhBDT_bTagJESdown->Write();
        histoSys_atanhBDT_bTagHFup->Write();
        histoSys_atanhBDT_bTagHFdown->Write();
        histoSys_atanhBDT_bTagLFup->Write();
        histoSys_atanhBDT_bTagLFdown->Write();
        histoSys_atanhBDT_bTagHFStats1up->Write();
        histoSys_atanhBDT_bTagHFStats1down->Write();
        histoSys_atanhBDT_bTagHFStats2up->Write();
        histoSys_atanhBDT_bTagHFStats2down->Write();
        histoSys_atanhBDT_bTagLFStats1up->Write();
        histoSys_atanhBDT_bTagLFStats1down->Write();
        histoSys_atanhBDT_bTagLFStats2up->Write();
        histoSys_atanhBDT_bTagLFStats2down->Write();
        histoSys_atanhBDT_bTagcfErr1up->Write();
        histoSys_atanhBDT_bTagcfErr1down->Write();
        histoSys_atanhBDT_bTagcfErr2up->Write();
        histoSys_atanhBDT_bTagcfErr2down->Write();

//         histoSys_atanhBDT_Up_->Write();

        histo_atanhBDT_nom->Write();         

    }
    
    
    
    TFile * file_data   = new TFile (file_data_name);
    TH1F * histo_data_obs  = (TH1F*) file_data->Get("hBDTG_VBF");
    histo_data_obs->SetName("hBDTG_VBF_mu_data_obs");
// 
    file_FitHisto->cd();
    histo_data_obs->Write();
    
    
    
    
    file_FitHisto->Close();
}
        
        
