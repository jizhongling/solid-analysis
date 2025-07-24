void DrawTrackResolution()
{
  const Float_t emin = 1;
  const Float_t emax = 10;
  const Float_t amin = 12;
  const Float_t amax = 13;

  auto h_total = new TH1F("h_total", "Total particles;Energy (GeV);", 10, -0.5, 9.5);
  auto h_pass = new TH1F("h_pass", "Reconstructed particles;Energy (GeV);", 10, -0.5, 9.5);
  auto h_res = new TH1F("h_res", "Reco Seed Resolution;E_{reco}/E_{truth}-1;", 1000, -0.5, 0.5);

  for(Int_t runno = 0; runno < 100; runno++)
  {
    auto data_file = new TFile(Form("data-u-plane/rec_e-_mom_%g_%gGeV_theta_%g_%gdeg-%d.root", emin, emax, amin, amax, runno));
    if(!data_file->IsOpen())
      cout << "Error with open file with runno " << runno << endl;
    else
      cout << "Processing file with runno " << runno << endl;

    auto events = (TTree*)data_file->Get("events");
    // Check the type of variables
    const Int_t max_track = 10;
    Float_t edep[1000], mc_mom[4][max_track], rec_mom[4][max_track];
    events->SetBranchAddress("GEMTrackerHits.eDep", (Float_t*)edep);
    events->SetBranchAddress("GeneratedParticles.momentum.x", (Float_t*)mc_mom[0]);
    events->SetBranchAddress("GeneratedParticles.momentum.y", (Float_t*)mc_mom[1]);
    events->SetBranchAddress("GeneratedParticles.momentum.z", (Float_t*)mc_mom[2]);
    events->SetBranchAddress("GeneratedParticles.energy", (Float_t*)mc_mom[3]);
    events->SetBranchAddress("ReconstructedChargedWithoutPIDParticles.momentum.x", (Float_t*)rec_mom[0]);
    events->SetBranchAddress("ReconstructedChargedWithoutPIDParticles.momentum.y", (Float_t*)rec_mom[1]);
    events->SetBranchAddress("ReconstructedChargedWithoutPIDParticles.momentum.z", (Float_t*)rec_mom[2]);
    events->SetBranchAddress("ReconstructedChargedWithoutPIDParticles.energy", (Float_t*)rec_mom[3]);

    for(Long64_t ien = 0; ien < events->GetEntries(); ien++)
    {
      edep[5] = -1;
      rec_mom[3][0] = -1;
      events->GetEntry(ien);

      // Require at lease 6 hits (6 U planes without V planes)
      if(edep[5] < 0)
        continue;

      h_total->Fill(mc_mom[3][0]);
      if(rec_mom[3][0] > 0)
      {
        h_pass->Fill(mc_mom[3][0]);
        Float_t res = rec_mom[3][0] / mc_mom[3][0] - 1;
        h_res->Fill(res);
      }
    } // ien

    data_file->Close();
  } // runno

  auto c0 = new TCanvas("c0", "c0", 1200, 600);
  c0->Divide(2, 1);

  c0->cd(1);
  auto g_eff = new TGraphAsymmErrors(h_pass, h_total);
  g_eff->GetXaxis()->SetTitle("Energy (GeV)");
  g_eff->SetMarkerStyle(20);
  g_eff->SetMarkerColor(1);
  g_eff->SetMarkerSize(1);
  g_eff->Draw("APE");

  c0->cd(2);
  gStyle->SetOptFit(1);
  h_res->SetLineStyle(2);
  h_res->SetLineColor(1);
  h_res->SetLineWidth(1);
  h_res->Fit("gaus");

  c0->Print("plots/TrackResolution-u-plane.pdf");
}
