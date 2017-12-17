//Author Omar.Zapata@cern.ch 2017
// {
// TFile f("TMVAReg.root");
// auto ds=(TDirectoryFile*)f.Get("dataset");
// ds->cd();
// auto testtree=(TTree*)ds->Get("TestTree");
// testtree->SetLineWidth(2);
// // TCanvas c1("c1");
// // testtree->Draw("(1./max(BDTG,0.01)) : min(GEN_pt, 100)","( GEN_eta > -5 )","colz");
// // c1.Draw();
// 
// TCanvas c2("c2");
// testtree->SetAlias("emtfLoss","pow(log(GEN_pt) - log(min(1./BDTG, 256.)), 2)");
// testtree->Draw("emtfLoss >> LOSS(70,0,35)","( GEN_pt > 1 && GEN_pt < 256 )*1./GEN_pt","normhist");
// c2.Draw();
// 
// TCanvas c3("c3");
// testtree->Draw("emtfLoss >> WGT_LOSS(70,0,35)","( GEN_pt > 1 && GEN_pt < 256 )*emtfLoss/GEN_pt","hist");
// c3.Draw();
// 
// WGT_LOSS->Integral()
// // std::cout<<"Integral:"<<inte<<std::endl;
// }
{
TFile f("TMVAReg.root");
auto ds=(TDirectoryFile*)f.Get("dataset");
ds->cd();
auto TestTree=(TTree*)ds->Get("TestTree");
// TestTree->Draw("(1./max(BDTG_AWB_Sq,0.01)) : min(GEN_pt, 100)","( GEN_eta > -5 )","colz")

TestTree->SetAlias("emtfLoss","pow(log(GEN_pt) - log(min(1./BDTG_AWB_Sq, 256.)), 2)");

// TestTree->Draw("emtfLoss >> LOSS(70,0,35)","( GEN_pt > 1 && GEN_pt < 256 )*1./GEN_pt","normhist");

TCanvas c;
c.SetLogy();
TestTree->Draw("emtfLoss >> WGT_LOSS(70,0,35)","( GEN_pt > 1 && GEN_pt < 256 )*emtfLoss/GEN_pt","hist");
auto wgt_loss=(TH1F*)gDirectory->Get("WGT_LOSS");

std::cout<<wgt_loss->Integral()<<std::endl;
}
