#define ANALYZEBOOSTEDTOPPOL_cxx

#include <iostream>
#include <vector>
#include <cstring>
#include "AnalyzeBoostedTopPol.h"
#include <TMath.h>
#include <algorithm>
#include "TLorentzVector.h"

using namespace std;

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  AnalyzeBoostedTopPol hgcmuons(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;

  hgcmuons.EventLoop(data);

  return 0;
}

void AnalyzeBoostedTopPol::EventLoop(const char *data) {

  std::vector<Float_t> b_energy;
  std::vector<Float_t> t_energy;                                      
  std::vector<Float_t> W_energy;                                      
  std::vector<Float_t> t_pt;                                      
  std::vector<Float_t> t_eta;                                      
  std::vector<Float_t> t_phi;                                      
  std::vector<Float_t> jet_pt;                                      
  std::vector<Float_t> jet_phi;                                      
  std::vector<Float_t> jet_eta;                                      
  std::vector<Float_t> total_jet_pt;                                    
 std::vector<Float_t> jet_count;                                      
  
  


  
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;

  Long64_t nbytes = 0, nb = 0;
  int decade = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" << endl;
    decade = k;
    
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;


     // ================ define TLorentzVectors and filling jets pt eta phi and M ============= //

    vector<TLorentzVector> v_Ak15jet;
    for (int i=0 ; i<Ak15Jet_size ; i++)
      {
	TLorentzVector temp_v;
	temp_v.SetPtEtaPhiM(Ak15Jet_PT[i],Ak15Jet_Eta[i],Ak15Jet_Phi[i],Ak15Jet_Mass[i]);
	v_Ak15jet.push_back(temp_v);

      }

    vector<TLorentzVector> v_Ak12jet;
    for (int i=0 ; i<Ak12Jet_size ; i++)
      {
	TLorentzVector temp_v;
	temp_v.SetPtEtaPhiM(Ak12Jet_PT[i],Ak12Jet_Eta[i],Ak12Jet_Phi[i],Ak12Jet_Mass[i]);
	v_Ak12jet.push_back(temp_v);

      }

    //=================Fill  pt eta phi E in vectors for t b W ==============================
    std::cout << jentry << " Event_ " << Event_ << " Particle_size " << Particle_size << std::endl;
    h_NParticles->Fill(Particle_size);
    ////
    for (int i=0 ; i< Particle_size ; i++)
      {
	if (Particle_PID[i] == 5 && Particle_Status[i] < 30 && Particle_Status[i] > 20)
	  b_energy.push_back(Particle_E[i]);
	if (Particle_PID[i] == 6 && Particle_Status[i] < 30 && Particle_Status[i] > 20 ){
	  t_energy.push_back(Particle_E[i]);
	  t_pt.push_back(Particle_PT[i]);
	  t_eta.push_back(Particle_Eta[i]);
	  t_phi.push_back(Particle_Phi[i]);
	 
	}
	if (Particle_PID[i] == 24 && Particle_Status[i] < 30 && Particle_Status[i] > 20)
	  W_energy.push_back(Particle_E[i]);
       
      }
    // //--------Filling jets     
    // for (int i=0; i < Jet_size ; i++)
    //   {
    // 	// sum_jet_pt += Jet_PT[i];
    // 	// if (sum_jet_pt > 200) break;

    //     jet_pt.push_back(Jet_PT[i]);	
    // 	jet_phi.push_back(Jet_Phi[i]);
    // 	jet_eta.push_back(Jet_Eta[i]);
    //   }
    // // total_jet_pt.push_back(sum_jet_pt);    
   

//=================to calculate dr between top and jets and put cut on del_r

    int Ak15_jetcount =0,Ak12_jetcount =0;
    float del_r=0;
    TLorentzVector total_v_Ak15jet; 
    TLorentzVector total_v_Ak12jet; 
    double_t total_jetmass = 0;
    for (int i =0; i<Particle_size;i++)
      if (Particle_PID[i] == 6 && Particle_Status[i] < 30 && Particle_Status[i] > 20 && Particle_PT[i] > 20)
	{
	  for (int j =0; j<Ak15Jet_size;j++)
	  {
	    del_r =sqrt(pow((Particle_Eta[i]- v_Ak15jet[j].Eta()),2)+pow((Particle_Phi[i]- v_Ak15jet[j].Phi()),2));
	    if (del_r < 2.0)
	      { Ak15_jetcount++;
		total_v_Ak15jet += v_Ak15jet[j];
		
	      }

	   
	  }
	  for (int j =0; j<Ak12Jet_size;j++)
	  {
	    del_r =sqrt(pow((Particle_Eta[i]- v_Ak12jet[j].Eta()),2)+pow((Particle_Phi[i]- v_Ak12jet[j].Phi()),2));
	    if (del_r < 2.0)
	      { Ak12_jetcount++;
		total_v_Ak12jet += v_Ak12jet[j];
		//	t_pt.push_back(Particle_PT[i]);
		total_jetmass += Ak12Jet_Mass[j];
	      }

	  
	  }
	  if( total_v_Ak15jet.M() == 0) goto lb1;
	  h_Ak15_mass_tvsjet->Fill(Particle_Mass[i],total_v_Ak15jet.M());	
	  h_Ak15_pt_tvsjet->Fill(Particle_PT[i],total_v_Ak15jet.Pt());
	  h_Ak15_pttvsmassjet->Fill(Particle_PT[i],total_v_Ak15jet.M());
	lb1:
	  if( total_v_Ak12jet.M() == 0) continue;
	  h_Ak12_mass_tvsjet->Fill(Particle_Mass[i],total_v_Ak12jet.M());
	  h_Ak12_pt_tvsjet->Fill(Particle_PT[i],total_v_Ak12jet.Pt());
	  h_temp->Fill(Particle_Mass[i],total_jetmass);	
          h_Ak12_pttvsmassjet->Fill(Particle_PT[i],total_v_Ak12jet.M());

	}
    // cout<<jentry <<"  Jet _size " << jetcount <<endl;
    h_Ak15_jetcount->Fill(Ak15_jetcount);
    h_Ak12_jetcount->Fill(Ak12_jetcount);
   
  } // loop over entries


 for (int i=0 ; i<t_energy.size(); i++)
    { 
      
      h_b_energyfrac->Fill(b_energy[i]/t_energy[i]);
      h_w_energyfrac->Fill(W_energy[i]/t_energy[i]);
      
    }     

 
  for (int i=0 ; i<t_energy.size(); i++)
    {
      h_b_energyfrac->Scale(1/h_b_energyfrac->Integral());
      h_w_energyfrac->Scale(1/h_w_energyfrac->Integral());
    }


  // cout<<"enteries for q from w----> "<<q_energy.size()<<endl;
  // cout<<"enteries for qbar from w----> "<<qbar_energy.size()<<endl;
  cout<<"enteries for b from w  "<<b_energy.size()<<endl;
}

