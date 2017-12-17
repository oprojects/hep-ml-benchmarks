//Author Omar.Zapata@cern.ch 2017
{
TFile f("TMVAReg.root");
f.cd("dataset");
TCanvas c;                                                                                                                                                       
TestTree->Draw("(inv_GEN_pt_trg-BDTG_AWB_Sq)/inv_GEN_pt_trg"); 
}
