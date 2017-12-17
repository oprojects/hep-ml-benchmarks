//Author Omar.Zapata@cern.ch 2017
{
TFile f("TMVAReg2x32.root");
f.cd("dataset");
TCanvas c;                                                                                                                                                       
TestTree->Draw("(inv_GEN_pt_trg-DNN_GPU)/inv_GEN_pt_trg"); 
}
