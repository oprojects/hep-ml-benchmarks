//Author Omar.Zapata@cern.ch 2017
using namespace TMVA;

void regression(int n=32){
    
    TMVA::Tools::Instance();
    // Create a new root output file
    auto outfile = TFile::Open( Form("TMVAReg2x%d.root",n), "RECREATE" );
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
    
    TFile f("../../PtRegression_Apr_2017_06_05_invPtTarg_invPtWgt_MODE_15_bitCompr_RPC.root");
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

   

      TString layoutString(Form("Layout=TANH|%d,TANH|%d,TANH|%d,LINEAR",n,n,n));

      TString training0("LearningRate=1e-5,Momentum=0.5,Repetitions=1,ConvergenceSteps=500,BatchSize=50,"
                        "TestRepetitions=7,WeightDecay=0.01,Regularization=NONE,DropConfig=0.5+0.5+0.5+0.5,"
                        "DropRepetitions=2");
//       TString training1("LearningRate=1e-5,Momentum=0.9,Repetitions=1,ConvergenceSteps=170,BatchSize=30,"
//                         "TestRepetitions=7,WeightDecay=0.01,Regularization=L2,DropConfig=0.1+0.1+0.1,DropRepetitions="
//                         "1");
//       TString training2("LearningRate=1e-5,Momentum=0.3,Repetitions=1,ConvergenceSteps=150,BatchSize=40,"
//                         "TestRepetitions=7,WeightDecay=0.01,Regularization=NONE");
//       TString training3("LearningRate=1e-6,Momentum=0.1,Repetitions=1,ConvergenceSteps=500,BatchSize=100,"
//                         "TestRepetitions=7,WeightDecay=0.0001,Regularization=NONE");

      TString trainingStrategyString("TrainingStrategy=");
      trainingStrategyString += training0;//+ "|" + training1 + "|" + training2 + "|" + training3;

      //       TString trainingStrategyString
      //       ("TrainingStrategy=LearningRate=1e-1,Momentum=0.3,Repetitions=3,ConvergenceSteps=20,BatchSize=30,TestRepetitions=7,WeightDecay=0.0,L1=false,DropFraction=0.0,DropRepetitions=5");

      TString nnOptions(
         "!H:V:ErrorStrategy=SUMOFSQUARES:VarTransform=G:WeightInitialization=XAVIERUNIFORM:Architecture=GPU");
      //       TString nnOptions ("!H:V:VarTransform=Normalize:ErrorStrategy=CHECKGRADIENTS");
      nnOptions.Append(":");
      nnOptions.Append(layoutString);
      nnOptions.Append(":");
      nnOptions.Append(trainingStrategyString);

      factory->BookMethod(&dl, TMVA::Types::kDNN, "DNN_GPU", nnOptions); // NN


   
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
