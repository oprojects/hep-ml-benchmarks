//Author Omar.Zapata@cern.ch 2017
{
    
TFile f("../ptclassdata.root");
auto trainsgn=(TTree*)f.Get("trainsgn");
auto n=trainsgn->GetEntries();
Float_t inv_GEN_pt_trg;
trainsgn->SetBranchAddress("inv_GEN_pt_trg",&inv_GEN_pt_trg);
for(UInt_t i=0;i<n;i++)
{
    trainsgn->GetEntry(i);
    if(inv_GEN_pt_trg<0.008)
    {
        std::cout<<1.0/inv_GEN_pt_trg<<" "<<inv_GEN_pt_trg<<std::endl;
    }
}
}
