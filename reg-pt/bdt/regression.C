//Author Omar.Zapata@cern.ch 2017
using namespace TMVA;
void regression(){
    TMVA::Tools::Instance();
    // Create a new root output file
    auto outfile = TFile::Open( "TMVAReg.root", "RECREATE" );
    auto factory = new TMVA::Factory( "TMVARegression", outfile,"!V:!Silent:Color:DrawProgressBar:AnalysisType=Regression" );
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
    
    //target
    dl.AddTarget( "inv_GEN_pt_trg" );
    
    TFile f("../PtRegression_Apr_2017_06_05_invPtTarg_invPtWgt_MODE_15_bitCompr_RPC.root");
    auto datadir=(TFile*)f.Get("f_MODE_15_invPtTarg_invPtWgt_bitCompr_RPC");
    auto TrainTree = (TTree*)datadir->Get("TrainTree");
    auto TestTree = (TTree*)datadir->Get("TestTree");
    
   Double_t regWeight  = 1.0;

   dl.AddRegressionTree( TrainTree, regWeight );
   dl.AddRegressionTree( TestTree, regWeight );

   // This would set individual event weights (the variables defined in the
   // expression need to exist in the original TTree)
    dl.SetWeightExpression(1.0);    

   TCut mycut = ""; // for example: TCut mycut = "abs(var1)<0.5 && abs(var2-0.5)<1";

   // tell the DataLoader to use all remaining events in the trees after training for testing:
   dl.PrepareTrainingAndTestTree( mycut,"nTrain_Regression=1999999:nTest_Regression=7204349:SplitMode=Block:V:Correlations=False" );
   
//    factory->BookMethod( &dl,  TMVA::Types::kBDT, "BDT",
//                            "!H:!V:NTrees=200:MinNodeSize=1.0%:BoostType=AdaBoostR2:SeparationType=RegressionVariance:nCuts=20:PruneMethod=CostComplexity:PruneStrength=30" );

//    factory->BookMethod( &dl,  TMVA::Types::kBDT, "BDTG",
//                            "!H:!V:NTrees=200::BoostType=Grad:Shrinkage=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3:MaxDepth=4" );

   factory->BookMethod( &dl, TMVA::Types::kBDT, "BDTG_AWB_Sq","!H:!V:NTrees=400::BoostType=Grad:Shrinkage=0.1:nCuts=1000:MaxDepth=5:MinNodeSize=0.000001:"
                                                "RegressionLossFunctionBDTG=LeastSquares" );

  // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();
   outfile->Close();

   std::cout << "==> Wrote root file: " << "TMVAReg.root" << std::endl;
   std::cout << "==> TMVARegression is done!" << std::endl;

   delete factory;  
   delete outfile;
}
