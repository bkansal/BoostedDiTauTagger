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
};
#endif

#ifdef ANALYZEBOOSTEDTOPPOL_cxx

void AnalyzeBoostedTopPol::BookHistogram(const char *outFileName) {

//  char hname[200], htit[200];
//  double xlow = 0.0,  xhigh = 2000.0;
//  int nbins = 2000;

  oFile = new TFile(outFileName, "recreate");
  char hname[200];
  sprintf(hname, "h_NParticles");
  h_NParticles = new TH1F(hname, hname, 100, 0, 1000);
  
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
