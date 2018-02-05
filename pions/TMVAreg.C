// #include<TMVA/PyMethodBase.h>
#include<TMVA/DataLoader.h>
#include<TMVA/Factory.h>
#include<TMVA/Config.h>
#include<TMVA/Tools.h>


#include<TFile.h>
#include<TString.h>


#include<iostream>
void TMVAreg(){

// TMVA::PyMethodBase::PyInitialize();

TMVA::Tools::Instance();
TString outfileName( "TMVAReg.root" );
TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

TMVA::Factory *factory = new TMVA::Factory( "TMVARegression", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:AnalysisType=Regression:!ModelPersistence" );

TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

for(UInt_t i=0;i<162;i++)
dataloader->AddVariable( Form("img[%d]",i));
 
 
dataloader->AddTarget( "target" );

auto input = TFile::Open("regtrain.root");

TTree *reg = (TTree*)input->Get("reg");

// global event weights per tree (see below for setting event-wise weights)
Double_t regWeight  = 1.0;

dataloader->AddRegressionTree( reg, regWeight );
 

dataloader->PrepareTrainingAndTestTree( "",
                                         "nTrain_Regression=35000:nTest_Regression=1000:SplitMode=Random:NormMode=NumEvents:!V" );  



{
    // Input Layout
TString inputLayoutString("InputLayout=1|1|162");

// General layout.
TString layoutString("Layout=CONV|8|3|3|1|1|1|1|RELU,CONV|8|3|3|1|1|1|1|RELU,MAXPOOL|4|4|4|4,RESHAPE|1|1|320|FLAT,DENSE|64|RELU,DENSE|32|"
                    "RELU,DENSE|1|LINEAR");
// TString layoutString("Layout=RESHAPE|1|1|162|FLAT,DENSE|64|RELU,DENSE|32|"
//                      "RELU,DENSE|1|LINEAR");
// TString layoutString("Layout=RESHAPE|1|1|162|FLAT,DENSE|64|RELU,DENSE|32|"
//                      "TANH,LINEAR");

// reshape was 12 4 4 
   // Batch Layout
   TString batchLayoutString("BatchLayout=32|1|162");

   // Training strategies.
   TString training0("LearningRate=1e-1,Momentum=0.0,Repetitions=1,"
                     "ConvergenceSteps=500,BatchSize=32,TestRepetitions=10,"
                     "MaxEpochs=500,"
                     "WeightDecay=1e-4,Regularization=None");
//                     "DropConfig=0.0+0.0+0.0+0.0+0.2+0.2");
   TString training1("LearningRate=1e-2,Momentum=0.9,Repetitions=1,"
                     "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                     "WeightDecay=1e-4,Regularization=L2,"
                     "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
   TString training2("LearningRate=1e-3,Momentum=0.0,Repetitions=1,"
                     "ConvergenceSteps=20,BatchSize=256,TestRepetitions=10,"
                     "WeightDecay=1e-4,Regularization=L2,"
                     "DropConfig=0.0+0.0+0.0+0.0, Multithreading=True");
   TString trainingStrategyString ("TrainingStrategy=");
   trainingStrategyString += training0; // + "|" + training1 + "|" + training2;

   // General Options.
   TString rnnOptions ("!H:V:ErrorStrategy=SUMOFSQUARES:VarTransform=None:"
                       "WeightInitialization=XAVIERUNIFORM");

   rnnOptions.Append(":"); rnnOptions.Append(inputLayoutString);
   rnnOptions.Append(":"); rnnOptions.Append(batchLayoutString);
   rnnOptions.Append(":"); rnnOptions.Append(layoutString);
   rnnOptions.Append(":"); rnnOptions.Append(trainingStrategyString);
   rnnOptions.Append(":Architecture=CPU");

   factory->BookMethod(dataloader, TMVA::Types::kDL, "CNN_CPU123", rnnOptions);

}

{
//  TString layoutString ("Layout=RELU|300,RELU|100,RELU|30,RELU|10,LINEAR");
        TString layoutString("Layout=RESHAPE|1|1|162,TANH|256,LINEAR");

TString training0("LearningRate=1e-5,Momentum=0.5,Repetitions=1,ConvergenceSteps=25,BatchSize=256,"
                        "TestRepetitions=7,WeightDecay=0.01,Regularization=NONE,DropConfig=0.5+0.5+0.5+0.5,"
                        "DropRepetitions=2, Multithreading=True");

// TString training0("LearningRate=1e-5,Momentum=0.5,Repetitions=1,ConvergenceSteps=50,BatchSize=162,"
//                         "TestRepetitions=7,Regularization=NONE,"
//                         " Multithreading=True");

TString trainingStrategyString("TrainingStrategy=");
trainingStrategyString += training0 ;

//       TString trainingStrategyString
//       ("TrainingStrategy=LearningRate=1e-1,Momentum=0.3,Repetitions=3,ConvergenceSteps=20,BatchSize=30,TestRepetitions=7,WeightDecay=0.0,L1=false,DropFraction=0.0,DropRepetitions=5");

TString nnOptions("!H:V:ErrorStrategy=SUMOFSQUARES:VarTransform=I:WeightInitialization=XAVIERUNIFORM:Architecture=CPU");
nnOptions.Append(":");
nnOptions.Append(layoutString);
nnOptions.Append(":");
nnOptions.Append(trainingStrategyString);

// factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", nnOptions); // NN
}
//factory->BookMethod( dataloader,  TMVA::Types::kBDT, "BDTG",
//                           "!H:!V:NTrees=2000::BoostType=Grad:Shrinkage=0.1:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=3:MaxDepth=4" );
//factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras123","H:!V:VarTransform=None:FilenameModel=model_cnn.h5:FilenameTrainedModel=trained_model_cnn.h5:NumEpochs=100:BatchSize=32");

// Train MVAs using the set of training events
factory->TrainAllMethods();

// Evaluate all MVAs using the set of test events
factory->TestAllMethods();

// Evaluate and compare performance of all configured MVAs
factory->EvaluateAllMethods();

// --------------------------------------------------------------

// Save the output
outputFile->Close();

}

int main()
{
    TMVAclass();
    return 0;
}
