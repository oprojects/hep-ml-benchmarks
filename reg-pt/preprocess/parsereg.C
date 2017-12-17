//Author Omar.Zapata@cern.ch 2017
//Macro to remove the noise 128.
//to perform regression
//
{
TFile f("../PtRegression_Apr_2017_06_05_invPtTarg_invPtWgt_MODE_15_bitCompr_RPC.root");
auto ds=(TDirectoryFile*)f.Get("f_MODE_15_invPtTarg_invPtWgt_bitCompr_RPC");
ds->cd();
auto TrainTree = (TTree*)ds->Get("TrainTree");
auto TestTree  = (TTree*)ds->Get("TestTree");

TFile  f2("../ptregdata.root","RECREATE");
TTree *regtrain = TrainTree->CopyTree("(1/inv_GEN_pt_trg)!=128");
TTree *regtest  = TestTree->CopyTree("(1/inv_GEN_pt_trg)!=128");

regtrain->Write("regtrain");
regtest->Write("regtest");
f2.Close();
}

