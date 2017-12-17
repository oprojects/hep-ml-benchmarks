{

    // create the Reader object
    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

   Float_t img[162];
   for(UInt_t i=0;i<162;i++)
   reader->AddVariable( Form("img[%d]",i), &img[i] );
   reader->BookMVA( "BDTG", "dataset/weights/TMVARegression_BDTG.weights.xml" ); 
   
   TH1F pred("pred","pred",100,0,350);
    auto input = TFile::Open("regtest.root"); 
   
   //
   TTree* reg = (TTree*)input->Get("reg");
   std::cout << "--- Select signal sample" << std::endl;
   for(UInt_t i=0;i<162;i++)
   reg->SetBranchAddress("img", img);

   std::cout << "--- Processing: " << reg->GetEntries() << " events" << std::endl;

   TStopwatch sw;
   sw.Start();
   for (Long64_t ievt=0; ievt<reg->GetEntries();ievt++) {

      if (ievt%1000 == 0) {
         std::cout << "--- ... Processing event: " << ievt << std::endl;
      }

      reg->GetEntry(ievt);

      Float_t val = (reader->EvaluateRegression( "BDTG" ))[0];
      pred.Fill( val );    
   }
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();
   pred.Draw();
}
