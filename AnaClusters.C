/*
 * Macro to analyze the calorimeters output from solid_recon.
 * Run it by "root -l -b -q AnaClusters.C".
 * It can take one parameter "proc", which is the index of ROOT files.
 * It creates 2D histograms with X-axis as the energy and Y-axis as the theta angle.
 * Four calorimeters: "LAEC_PrSh", "LAEC_Sh", "FAEC_PrSh", "FAEC_Sh".
 * Three types of outputs: "RecHits", "TruthClusters", "Clusters".
 * "RecHits" is the energy sum of reconstructed hits.
 * "TruthClusters" is the energy of the energy of the truth cluster.
 * "Clusters" is the energy of the reconstructed clusters.
 */

void AnaClusters(const Int_t proc = 0)
{
  // Energy ranges
  const vector<pair<Double_t, Double_t>> v_energy{{1, 10}};
  const Double_t theta_min = 5;
  const Double_t theta_max = 20;
  const char *particle = "e-";

  const Int_t ndet = 4;
  const Int_t ntype = 3;
  const char *det_name[ndet] = {"LAEC_PrSh", "LAEC_Sh", "FAEC_PrSh", "FAEC_Sh"};
  const char *clus_name[ntype] = {"RecHits", "TruthClusters", "Clusters"};

  // TODO: Change the data directory "dir_solid" to where you stored the ROOT files.
  //       "mkdir histos" in the current directory to store the output ROOT files.
  const char *dir_solid = "/w/halla-scshelf2102/solid/zji/sim/analysis/data-reco-seed";
  auto f_out = new TFile(Form("histos/clus_%s_theta_%g_%gdeg-%d.root", particle, theta_min, theta_max, proc), "RECREATE");

  for(const auto &[energy_min, energy_max] : v_energy)
  {
    f_out->cd();
    TH2 *h2_edep[ndet][ntype];
    for(Int_t id = 0; id < ndet; id++)
      for(Int_t it = 0; it < ntype; it++)
        h2_edep[id][it] = new TH2F(
            Form("h2_edep_%s%s", det_name[id], clus_name[it]),
            Form("%s%s;E [GeV];#theta", det_name[id], clus_name[it]), 
            60, 0., 3.,
            static_cast<Int_t>(theta_max - theta_min)*4, theta_min, theta_max);

    TString file_name;
    file_name.Form("%s/rec_%s_mom_%g_%gGeV_theta_%g_%gdeg-%d.root", dir_solid, particle, energy_min, energy_max, theta_min, theta_max, proc);

    // Reserve enough space to avoid segmentation fault
    const Int_t max_track = 1000;
    TFile *data_file = TFile::Open(file_name);
    if(!data_file)
    {
      cerr << "Cannot open " << file_name << endl;
      continue;
    }
    cout << "Opening " << file_name << endl;

    auto events = (TTree*)data_file->Get("events");
    // Check the type of variables
    Double_t pmc[3][max_track];
    Float_t edep[ndet][ntype][max_track];
    events->SetBranchAddress("MCParticles.momentum.x", (Double_t*)pmc[0]);
    events->SetBranchAddress("MCParticles.momentum.y", (Double_t*)pmc[1]);
    events->SetBranchAddress("MCParticles.momentum.z", (Double_t*)pmc[2]);
    for(Int_t id = 0; id < ndet; id++)
      for(Int_t it = 0; it < ntype; it++)
        events->SetBranchAddress(Form("%s%s.energy", det_name[id], clus_name[it]), (Float_t*)edep[id][it]);

    for(Long64_t i = 0; i < events->GetEntries(); i++)
    {
      events->GetEntry(i);
      // Check MCParticles for the index of the generated particle
      ROOT::Math::XYZVector v3_pmc(pmc[0][0], pmc[1][0], pmc[2][0]);
      Double_t theta = v3_pmc.Theta() * 180. / TMath::Pi();
      if( theta < theta_min || theta > theta_max )
        continue;
      for(Int_t it = 0; it < ntype; it++)
        for(Int_t id = 0; id < ndet; id++)
        {
          Float_t edep_sum = 0.;
          for(Long64_t j = 0; j < events->GetLeaf(Form("%s%s.energy", det_name[id], clus_name[it]))->GetLen(); j++)
          {
            // for RecHits
            if(it == 0)
              edep_sum += edep[id][it][j];
            // for Clusters
            else
              h2_edep[id][it]->Fill(edep[id][it][j], theta);
          } // leaf
          if(it == 0)
            h2_edep[id][it]->Fill(edep_sum, theta);
        } // det, type
    } // event

    data_file->Close();
  } // energy

  f_out->Write();
  f_out->Close();
}
