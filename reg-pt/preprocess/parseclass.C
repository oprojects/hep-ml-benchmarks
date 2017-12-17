//Author Omar.Zapata@cern.ch 2017
//Macro to split data in two classes according to inv_GEN_pt_trg and removing the noise 128.
//to perform two class classification
{
TFile f("../PtRegression_Apr_2017_06_05_invPtTarg_invPtWgt_MODE_15_bitCompr_RPC.root");
auto ds=(TDirectoryFile*)f.Get("f_MODE_15_invPtTarg_invPtWgt_bitCompr_RPC");
ds->cd();
auto TrainTree = (TTree*)ds->Get("TrainTree");
auto TestTree  = (TTree*)ds->Get("TestTree");

TFile  f2("../ptclassdata.root","RECREATE");
TTree *trainsgn = TrainTree->CopyTree("(1/inv_GEN_pt_trg)>=20 && (1/inv_GEN_pt_trg)!=128");
TTree *trainbkg = TrainTree->CopyTree("(1/inv_GEN_pt_trg)<20 && (1/inv_GEN_pt_trg)!=128");

TTree *testsgn = TestTree->CopyTree("(1/inv_GEN_pt_trg)>=20 && (1/inv_GEN_pt_trg)!=128");
TTree *testbkg = TestTree->CopyTree("(1/inv_GEN_pt_trg)<20 && (1/inv_GEN_pt_trg)!=128");

trainsgn->Write("trainsgn");
trainbkg->Write("trainbkg");
testsgn->Write("testsgn");
testbkg->Write("testbkg");
f2.Close();
}
