#ifndef ANALYZEBOOSTEDTOPPOL_H
#define ANALYZEBOOSTEDTOPPOL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include "TH2.h"

class AnalyzeBoostedTopPol : public NtupleVariables{

 public:
  AnalyzeBoostedTopPol(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~AnalyzeBoostedTopPol();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *);
  void     BookHistogram(const char *);
  
  TFile *oFile;
  TH1F *h_NParticles;
  TH1F *h_b_energyfrac;
  TH1F *h_w_energyfrac;
  TH2D *h_Ak15_mass_tvsjet;
  TH2D *h_Ak12_mass_tvsjet;
  TH2D *h_Ak15_pt_tvsjet;
  TH2D *h_Ak12_pt_tvsjet;
  TH1F *h_Ak15_jetcount;
  TH1F *h_Ak12_jetcount;
  TH2D *h_temp;
  TH2D *h_Ak12_pttvsmassjet;
  TH2D *h_Ak15_pttvsmassjet;
};
#endif

#ifdef ANALYZEBOOSTEDTOPPOL_cxx

void AnalyzeBoostedTopPol::BookHistogram(const char *outFileName) {

//  char hname[200], htit[200];
//  double xlow = 0.0,  xhigh = 2000.0;
//  int nbins = 2000;

  oFile = new TFile(outFileName, "recreate");
  char hname[200] , hname1[200],hname2[200],hname3[200];
  sprintf(hname, "h_NParticles");
  h_NParticles = new TH1F(hname, hname, 100, 0, 1000);
  sprintf(hname1, "h_b_energyfrac");
  h_b_energyfrac = new TH1F(hname1, hname1 , 100 , 0 , 1);
  sprintf(hname2, "h_w_energyfrac");
  h_w_energyfrac = new TH1F(hname2, hname2 , 100 , 0 , 1);
  sprintf(hname3, "h_Ak15_mass_tvsjet");
  h_Ak15_mass_tvsjet = new TH2D(hname3, hname3 , 250 , 0 , 500 , 250 , 0 , 500);
  h_Ak12_mass_tvsjet = new TH2D("h_Ak12_mass_tvsjet","h_Ak12_mass_tvsjet",250,0,500,250,0,500);
  h_Ak15_pt_tvsjet = new TH2D("h_Ak15_pt_tvsjet","h_Ak15_pt_tvsjet",250,0,500,250,0,500);
  h_Ak12_pt_tvsjet = new TH2D("h_Ak12_pt_tvsjet","h_Ak12_pt_tvsjet",250,0,500,250,0,500);
  h_Ak15_jetcount = new TH1F("h_Ak15_jetcount","h_Ak15_jetcount",10 ,0,10);
  h_Ak12_jetcount = new TH1F("h_Ak12_jetcount","h_Ak12_jetcount",10 ,0,10);
  h_Ak15_pttvsmassjet= new TH2D("h_Ak15_pttvsmassjet","h_Ak15_pttvsmassjet",250,0,500,250,0,500);
  h_Ak12_pttvsmassjet= new TH2D("h_Ak12_pttvsmassjet","h_Ak12_pttvsmassjet",250,0,500,250,0,500);
  h_temp = new TH2D("h_temp","h_temp",250,0,500,250,0,500);
}


AnalyzeBoostedTopPol::AnalyzeBoostedTopPol(const TString &inputFileList, const char *outFileName, const char* dataset) {

  TChain *tree = new TChain("Delphes");

  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  NtupleVariables::Init(tree);

  BookHistogram(outFileName);
  
}

Bool_t AnalyzeBoostedTopPol::FillChain(TChain *chain, const TString &inputFileList) {

  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //std::cout << "Adding tree from " << buffer.c_str() << std::endl;                                                              
    chain->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in this tree : " << chain->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t AnalyzeBoostedTopPol::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }
  return centry;
}

AnalyzeBoostedTopPol::~AnalyzeBoostedTopPol() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif
