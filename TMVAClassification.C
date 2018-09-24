// @(#)root/tmva $Id: TMVAClassification.C 38895 2011-04-18 11:59:54Z evt $
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

// #include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void TMVAClassification( TString myMethodList = "" )
{
   // The explicit loading of the shared libTMVA is done in TMVAlogon.C, defined in .rootrc
   // if you use your private .rootrc, or run from a different directory, please copy the
   // corresponding lines from .rootrc

   // methods to be processed can be given as an argument; use format:
   //
   // mylinux~> root -l TMVAClassification.C\(\"myMethod1,myMethod2,myMethod3\"\)
   //
   // if you like to use a method via the plugin mechanism, we recommend using
   //
   // mylinux~> root -l TMVAClassification.C\(\"P_myMethod\"\)
   // (an example is given for using the BDT as plugin (see below),
   // but of course the real application is when you write your own
   // method based)

   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 1; 
   Use["MLP"]             = 1;
    
   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "Prova.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is 
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

   // Define the input variables that shall be used for the MVA training
   // note that you may also use variable expressions, such as: "3*var1/var2*abs(var3)"
   // [all types of expressions that can also be parsed by TTree::Draw( "expression" )]

   factory->AddVariable( "ll_mass", "ll_mass", "units", 'F' );
   factory->AddVariable( "ll_pt", "ll_pt", "units", 'F' );
//    factory->AddVariable( "ll_eta", "ll_eta", "units", 'F' );
   factory->AddVariable( "ll_ystar", "ll_ystar", "units", 'F' );
   factory->AddVariable( "ll_zstar", "ll_zstar", "units", 'F' );
   factory->AddVariable( "MqqLog", "MqqLog", "units", 'F' );
   factory->AddVariable( "Rpt", "Rpt", "units", 'F' );
   factory->AddVariable( "softActivityEWK_njets5", "softActivityEWK_njets5", "units", 'F' );
//    factory->AddVariable( "qq_pt", "qq_pt", "units", 'F' );   
   factory->AddVariable( "W_mass_virtual1Log", "W_mass_virtual1Log", "units", 'F' );
   factory->AddVariable( "W_mass_virtual2", "W_mass_virtual2", "units", 'F' );
//    factory->AddVariable( "qgl_1q", "qgl_1q", "units", 'F' );
//    factory->AddVariable( "qgl_2q", "qgl_2q", "units", 'F' );
//    factory->AddVariable( "theta1", "theta1", "units", 'F' );
//    factory->AddVariable( "theta2", "theta2", "units", 'F' );
//    factory->AddVariable( "deltaMRel", "deltaMRel", "units", 'F' );
//    factory->AddVariable( "DeltaEtaQQ", "DeltaEtaQQ", "units", 'F' );
//    factory->AddVariable( "Jet1q_pt", "Jet1q_pt", "units", 'F' );
//    factory->AddVariable( "Jet2q_pt", "Jet2q_pt", "units", 'F' );
//    factory->AddVariable( "Jet3_pt", "Jet3_pt", "units", 'F' );
//    factory->AddVariable( "Jet3_eta", "Jet3_eta", "units", 'F' );
//    factory->AddVariable( "mumujj_pt", "mumujj_pt", "units", 'F' );
//    factory->AddVariable( "X_parton1", "X_parton1", "units", 'F' );
//    factory->AddVariable( "X_parton2", "X_parton2", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual1", "W_Pt_virtual1", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual2", "W_Pt_virtual2", "units", 'F' );
//    factory->AddVariable( "W_eta_virtual1", "W_eta_virtual1", "units", 'F' );
//    factory->AddVariable( "W_eta_virtual2", "W_eta_virtual2", "units", 'F' );
//    factory->AddVariable( "W_phi_virtual1", "W_phi_virtual1", "units", 'F' );
//    factory->AddVariable( "W_phi_virtual2", "W_phi_virtual2", "units", 'F' );
//    factory->AddVariable( "lepton1_pt", "lepton1_pt", "units", 'F' );
//    factory->AddVariable( "lepton2_pt", "lepton2_pt", "units", 'F' );
//    factory->AddVariable( "WWmass", "WWmass", "units", 'F' );
//    factory->AddVariable( "impulsoZ", "impulsoZ", "units", 'F' );

/////////////			AGNESE
//    factory->AddVariable( "ll_mass", "ll_mass", "units", 'F' );
//    factory->AddVariable( "MqqLog", "MqqLog", "units", 'F' );
//    factory->AddVariable( "Rpt", "Rpt", "units", 'F' );
//    factory->AddVariable( "W_mass_virtual1Log", "W_mass_virtual1Log", "units", 'F' );
//    factory->AddVariable( "ll_zstar", "ll_zstar", "units", 'F' );
//    factory->AddVariable( "softActivityEWK_njets5", "softActivityEWK_njets5", "units", 'F' );
//    factory->AddVariable( "ll_pt", "ll_pt", "units", 'F' );

/////////////			MIO BDT
//    factory->AddVariable( "ll_mass", "ll_mass", "units", 'F' );
//    factory->AddVariable( "ll_pt", "ll_pt", "units", 'F' );
//    factory->AddVariable( "ll_ystar", "ll_ystar", "units", 'F' );
//    factory->AddVariable( "Mqq", "Mqq", "units", 'F' );
//    factory->AddVariable( "Rpt", "Rpt", "units", 'F' );
//    factory->AddVariable( "W_mass_virtual2", "W_mass_virtual2", "units", 'F' );
//    factory->AddVariable( "ll_zstar", "ll_zstar", "units", 'F' );
//    factory->AddVariable( "deltaMRel", "deltaMRel", "units", 'F' );
//    factory->AddVariable( "DeltaEtaQQ", "DeltaEtaQQ", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual1", "W_Pt_virtual1", "units", 'F' );


/////////////			MIO MLP
//    factory->AddVariable( "ll_mass", "ll_mass", "units", 'F' );
//    factory->AddVariable( "ll_pt", "ll_pt", "units", 'F' );
//    factory->AddVariable( "Mqq", "Mqq", "units", 'F' );
//    factory->AddVariable( "Rpt", "Rpt", "units", 'F' );
//    factory->AddVariable( "softActivityEWK_njets5", "softActivityEWK_njets5", "units", 'F' );
//    factory->AddVariable( "W_mass_virtual1", "W_mass_virtual1", "units", 'F' );
//    factory->AddVariable( "W_mass_virtual2", "W_mass_virtual2", "units", 'F' );
//    factory->AddVariable( "ll_zstar", "ll_zstar", "units", 'F' );
//    factory->AddVariable( "deltaMRel", "deltaMRel", "units", 'F' );
//    factory->AddVariable( "Jet1q_pt", "Jet1q_pt", "units", 'F' );
//    factory->AddVariable( "Jet2q_pt", "Jet2q_pt", "units", 'F' );
//    factory->AddVariable( "Jet3_pt", "Jet3_pt", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual1", "W_Pt_virtual1", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual2", "W_Pt_virtual2", "units", 'F' );
//    factory->AddVariable( "lepton1_pt", "lepton1_pt", "units", 'F' );
//    factory->AddVariable( "lepton2_pt", "lepton2_pt", "units", 'F' );
/////////////			MIO MLP

/////////////			MIO BDTG
//    factory->AddVariable( "ll_mass", "ll_mass", "units", 'F' );
//    factory->AddVariable( "ll_pt", "ll_pt", "units", 'F' );
//    factory->AddVariable( "Mqq", "Mqq", "units", 'F' );
//    factory->AddVariable( "Rpt", "Rpt", "units", 'F' );
//    factory->AddVariable( "softActivityEWK_njets5", "softActivityEWK_njets5", "units", 'F' );
//    factory->AddVariable( "W_mass_virtual2", "W_mass_virtual2", "units", 'F' );
//    factory->AddVariable( "ll_zstar", "ll_zstar", "units", 'F' );
//    factory->AddVariable( "deltaMRel", "deltaMRel", "units", 'F' );
//    factory->AddVariable( "DeltaEtaQQ", "DeltaEtaQQ", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual1", "W_Pt_virtual1", "units", 'F' );
//    factory->AddVariable( "W_Pt_virtual2", "W_Pt_virtual2", "units", 'F' );
//    factory->AddVariable( "impulsoZ", "impulsoZ", "units", 'F' );



///   factory->AddVariable( "myvar1 := var1+var2", 'F' );
///   factory->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
///   factory->AddVariable( "var3",                "Variable 3", "units", 'F' );
///   factory->AddVariable( "var4",                "Variable 4", "units", 'F' );

   // You can add so-called "Spectator variables", which are not used in the MVA training,
   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
   // input variables, the response values of all trained MVAs, and the spectator variables

///   factory->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
///   factory->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );

   // Read training and test data
   // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
///   TString fname = "./tmva_class_example.root";
//    TString fname = "./Prova.root";
   
//    if (gSystem->AccessPathName( fname ))  // file does not exist in local directory
//       gSystem->Exec("wget http://root.cern.ch/files/tmva_class_example.root");
   
//    TFile *input = TFile::Open( fname );
   
//    std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;
   
   // --- Register the training and test trees

   TString fname_signal_train = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/mvaTree/MVA_Agnese_NobTagWeight/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom.root";
   TString fname_bg_train = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/mvaTree/MVA_Agnese_NobTagWeight/main_tmva_tree_DYJetsToLL_M_v25mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom.root";
   TFile *input_signal_train = TFile::Open( fname_signal_train );   
   TFile *input_bg_train = TFile::Open( fname_bg_train ); 
   TTree *signal_train     = (TTree*)input_signal_train->Get("TMVAtree");
   TTree *bg_train = (TTree*)input_bg_train->Get("TMVAtree");

	// test        
	TString fname_signal_test = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/mvaTree/MVA_Agnese_NobTagWeight/main_tmva_tree_VBF_HToMuMu_v25mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom.root";
	TString fname_bg_test = "/afs/cern.ch/work/f/ferrico/private/Hmumu_Git/CMSSW_8_0_28/src/Hmumu/mvaTree/MVA_Agnese_NobTagWeight/main_tmva_tree_DYJetsToLL_M_v25mu_QCDScalenom_JESnom_bTagJESnom_JERnom_PUnom.root";
   TFile *input_signal_test = TFile::Open( fname_signal_test );
   TFile *input_bg_test = TFile::Open( fname_bg_test );
   TTree *signal_test     = (TTree*)input_signal_test->Get("TMVAtree");
   TTree *bg_test = (TTree*)input_bg_test->Get("TMVAtree");
   
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;
   
   // You can add an arbitrary number of signal or background trees
//    factory->AddSignalTree    ( signal,     signalWeight     );
//    factory->AddBackgroundTree( background, backgroundWeight );
    factory->AddSignalTree( signal_train, signalWeight, "Training" );
    factory->AddBackgroundTree( bg_train, backgroundWeight, "Training" );
    factory->AddSignalTree( signal_test,     signalWeight,  "Test" );
    factory->AddBackgroundTree( bg_test, backgroundWeight, "Test" );
   
   // To give different trees for training and testing, do as follows:
   //    factory->AddSignalTree( signalTrainingTree, signalTrainWeight, "Training" );
   //    factory->AddSignalTree( signalTestTree,     signalTestWeight,  "Test" );
   
   // Use the following code instead of the above two or four lines to add signal and background
   // training and test events "by hand"
   // NOTE that in this case one should not give expressions (such as "var1+var2") in the input
   //      variable definition, but simply compute the expression before adding the event
   //
   //     // --- begin ----------------------------------------------------------
   //     std::vector<Double_t> vars( 4 ); // vector has size of number of input variables
   //     Float_t  treevars[4], weight;
   //     
   //     // Signal
   //     for (UInt_t ivar=0; ivar<4; ivar++) signal->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<signal->GetEntries(); i++) {
   //        signal->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < signal->GetEntries()/2.0) factory->AddSignalTrainingEvent( vars, signalWeight );
   //        else                              factory->AddSignalTestEvent    ( vars, signalWeight );
   //     }
   //   
   //     // Background (has event weights)
   //     background->SetBranchAddress( "weight", &weight );
   //     for (UInt_t ivar=0; ivar<4; ivar++) background->SetBranchAddress( Form( "var%i", ivar+1 ), &(treevars[ivar]) );
   //     for (UInt_t i=0; i<background->GetEntries(); i++) {
   //        background->GetEntry(i);
   //        for (UInt_t ivar=0; ivar<4; ivar++) vars[ivar] = treevars[ivar];
   //        // add training and test events; here: first half is training, second is testing
   //        // note that the weight can also be event-wise
   //        if (i < background->GetEntries()/2) factory->AddBackgroundTrainingEvent( vars, backgroundWeight*weight );
   //        else                                factory->AddBackgroundTestEvent    ( vars, backgroundWeight*weight );
   //     }
         // --- end ------------------------------------------------------------
   //
   // --- end of tree registration 

   // Set individual event weights (the variables must exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
//    factory->SetBackgroundWeightExpression( "weight" );

   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used 
   // for training, and the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // To also specify the number of testing events, use:
   //    factory->PrepareTrainingAndTestTree( mycut,
   //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
   factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable


   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=100:MinNodeSize=0.05:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
                           
// 							  "!H:!V:NTrees=200:MinNodeSize=0.05:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");  //Agnese						
//   							"!H:!V:NTrees=200:MinNodeSize=0.75:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20"); //Mio

  if (Use["BDTG"]) // Gradient Boost
  		factory->BookMethod( TMVA::Types::kBDT, "BDTG",
							  "!H:!V:NTrees=100:MinNodeSize=0.05:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");
// 								"!H:!V:NTrees=230:MinNodeSize=0.75:BoostType=Grad:Shrinkage=.1:UseBaggedBoost:BaggedSampleFraction=0.6:MaxDepth=4:nCuts=20:NegWeightTreatment=IgnoreNegWeightsInTraining"); //MIO
   if (Use["MLP"]) // Mio MLP
//       factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=1250:HiddenLayers=N+2:TestRate=30:!UseRegulator" );
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=100:HiddenLayers=N+5:TestRate=5:!UseRegulator" );



   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events

   // factory->OptimizeAllMethods("SigEffAt001","Scan");
   // factory->OptimizeAllMethods("ROCIntegral","GA");

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;

   // Launch the GUI for the root macros
//    if (!gROOT->IsBatch()) TMVAGui( outfileName );
}