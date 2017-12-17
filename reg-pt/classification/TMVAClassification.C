//Author Omar.Zapata@cern.ch 2017
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

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int TMVAClassification( )
{
   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();


   // Read training and test data
   // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
   TFile *input(0);
   TString fname = "../ptclassdata.root";
   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   }
   std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

   // Register the training and test trees

   TTree *trainsgn     = (TTree*)input->Get("trainsgn");
   TTree *trainbkg     = (TTree*)input->Get("trainbkg");
   TTree *testsgn     = (TTree*)input->Get("testsgn");
   TTree *testbkg     = (TTree*)input->Get("testbkg");

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification" );

TMVA::DataLoader dl("dataset");
    
    
    // dl.AddVariable("classID","I");
    // dl.AddVariable("className","C");
    dl.AddVariable("theta",'F');
    dl.AddVariable("St1_ring2",'F');
    dl.AddVariable("dPhi_12",'F');
    dl.AddVariable("dPhi_23",'F');
    dl.AddVariable("dPhi_34",'F');
    dl.AddVariable("dPhi_13",'F');
    dl.AddVariable("dPhi_14",'F');
    dl.AddVariable("dPhi_24",'F');
    dl.AddVariable("FR_1",'F');
    dl.AddVariable("bend_1",'F');
    dl.AddVariable("dPhiSum4",'F');
    dl.AddVariable("dPhiSum4A",'F');
    dl.AddVariable("dPhiSum3",'F');
    dl.AddVariable("dPhiSum3A",'F');
    dl.AddVariable("outStPhi",'F');
    dl.AddVariable("dTh_14",'F');
    dl.AddVariable("RPC_1",'F');
    dl.AddVariable("RPC_2",'F');
    dl.AddVariable("RPC_3",'F');
    dl.AddVariable("RPC_4",'F');
//     dl.AddVariable("GEN_pt",'F');
//     dl.AddVariable("EMTF_pt",'F');
//     dl.AddVariable("GEN_eta",'F');
//     dl.AddVariable("EMTF_eta",'F');
//     dl.AddVariable("TRK_eta",'F');
//     dl.AddVariable("GEN_charge",'F');
//     dl.AddVariable("EMTF_charge",'F');
//     dl.AddVariable("EMTF_mode",'F');
//     dl.AddVariable("EMTF_mode_CSC",'F');
//     dl.AddVariable("EMTF_mode_RPC",'F');
//     dl.AddVariable("TRK_mode",'F');//is a constant
//     dl.AddVariable("TRK_mode_CSC",'F');
//     dl.AddVariable("TRK_mode_RPC",'F');
//     dl.AddVariable("dPhi_sign",'F');
//     dl.AddVariable("evt_weight",'F');
//     dl.AddVariable("inv_GEN_pt_trg",'F');
//     dl.AddVariable("inv_GEN_pt_trg",'F');//target
//     dl.AddVariable("weight",'F');//weight expression
 
    // spectators
    dl.AddSpectator("EMTF_pt",'F');
    dl.AddSpectator("EMTF_eta",'F');
    dl.AddSpectator("EMTF_charge",'F');
    dl.AddSpectator("EMTF_mode",'F');
    dl.AddSpectator("EMTF_mode_CSC",'F');
    dl.AddSpectator("EMTF_mode_RPC",'F');
    dl.AddSpectator("TRK_eta",'F');
    dl.AddSpectator("GEN_pt",'F');
    dl.AddSpectator("GEN_eta",'F');
    dl.AddSpectator("GEN_charge",'F');
    dl.AddSpectator("TRK_mode",'F');//is a constant
    dl.AddSpectator("classID",'I');
    dl.AddSpectator("className",'C');
    dl.AddSpectator("TRK_mode_CSC",'F');
    dl.AddSpectator("TRK_mode_RPC",'F');
    dl.AddSpectator("dPhi_sign",'F');
    dl.AddSpectator("evt_weight",'F');
    
    dl.AddSpectator( "inv_GEN_pt_trg" );
    //target
   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   dl.AddSignalTree    ( trainsgn,     signalWeight );
   dl.AddSignalTree    ( testsgn ,     signalWeight );

   dl.AddBackgroundTree( trainbkg, backgroundWeight );
   dl.AddBackgroundTree( testbkg, backgroundWeight );


   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

   dl.PrepareTrainingAndTestTree( mycuts, mycutb,
                                        "nTrain_Signal=860000:nTrain_Background=860000:nTest_Signal=860000:nTest_Background=860000:SplitMode=Block:NormMode=NumEvents:!V" );


   // Boosted Decision Trees
      factory->BookMethod( &dl, TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );

      factory->BookMethod( &dl, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

   // Now you can tell the factory to train, test, and evaluate the MVAs
   //
   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
//    delete dataloader;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

   return 0;
}
