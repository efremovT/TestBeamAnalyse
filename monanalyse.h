//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 30 16:48:14 2022 by ROOT version 6.24/02
// from TTree tree/Test beam
// found on file: tree_February2022_201.root
//////////////////////////////////////////////////////////

#ifndef monanalyse_h
#define monanalyse_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class monanalyse {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           FEI4x;
   Int_t           FEI4y;
   Int_t           runNumber;
   Int_t           eventNumber;
   Int_t           cycleNumber;
   Float_t         eventTime;
   Int_t           nChannel;
   Float_t         pulseHeightNoise[3];   //[nChannel]
   Float_t         pulseHeight[3];   //[nChannel]
   Float_t         pulseHeightMin[3];   //[nChannel]
   Float_t         timeCFD[3];   //[nChannel]
   Float_t         timeZCD[3];   //[nChannel]
   Float_t         timeCFD20[3];   //[nChannel]
   Float_t         timeCFD70[3];   //[nChannel]
   Float_t         timeZCD20[3];   //[nChannel]
   Float_t         timeCFD50[3];   //[nChannel]
   Float_t         timeZCD50[3];   //[nChannel]
   Float_t         timeCTD[3];   //[nChannel]
   Float_t         timeAtMax[3];   //[nChannel]
   Float_t         pedestal[3];   //[nChannel]
   Float_t         noise[3];   //[nChannel]
   Float_t         noiseZCD[3];   //[nChannel]
   Float_t         charge[3];   //[nChannel]
   Float_t         jitter[3];   //[nChannel]
   Float_t         jitterCFD20[3];   //[nChannel]
   Float_t         jitterCFD50[3];   //[nChannel]
   Float_t         jitterCTD[3];   //[nChannel]
   Float_t         riseTime1090[3];   //[nChannel]
   Float_t         derivative[3];   //[nChannel]
   Float_t         derivativeCFD20[3];   //[nChannel]
   Float_t         derivativeCFD50[3];   //[nChannel]
   Float_t         derivativeCTD[3];   //[nChannel]
   Float_t         derivativeZCD[3];   //[nChannel]
   Float_t         width[3];   //[nChannel]
   Float_t         TOT[3];   //[nChannel]
   Int_t           nALTIROCChannel;
   Int_t           nBoards;
   Int_t           seqCnt[2];   //[nBoards]
   Int_t           trigCnt[2];   //[nBoards]
   Float_t         arBoardNb[30];   //[nALTIROCChannel]
   Float_t         arChannelNb[30];   //[nALTIROCChannel]
   Float_t         arTOA[30];   //[nALTIROCChannel]
   Float_t         arTOAOV[30];   //[nALTIROCChannel]
   Float_t         arTOT[30];   //[nALTIROCChannel]
   Float_t         arTOTOV[30];   //[nALTIROCChannel]
   Float_t         arTOTF[30];   //[nALTIROCChannel]
   Float_t         arTOTC[30];   //[nALTIROCChannel]
   Double_t        NTrack;
   Double_t        Xtr[8];
   Double_t        Ytr[8];
   Double_t        DeltaXtr[8];
   Double_t        DeltaYtr[8];
   Double_t        axz_down;
   Double_t        ayz_down;
   Double_t        axz_up;
   Double_t        ayz_up;
   Double_t        Zkink;
   Double_t        DRMin;

   // List of branches
   TBranch        *b_FEI4x;   //!
   TBranch        *b_FEI4y;   //!
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_cycleNumber;   //!
   TBranch        *b_eventTime;   //!
   TBranch        *b_nChannel;   //!
   TBranch        *b_pulseHeightNoise;   //!
   TBranch        *b_pulseHeight;   //!
   TBranch        *b_pulseHeightMin;   //!
   TBranch        *b_timeCFD;   //!
   TBranch        *b_timeZCD;   //!
   TBranch        *b_timeCFD20;   //!
   TBranch        *b_timeCFD70;   //!
   TBranch        *b_timeZCD20;   //!
   TBranch        *b_timeCFD50;   //!
   TBranch        *b_timeZCD50;   //!
   TBranch        *b_timeCTD;   //!
   TBranch        *b_timeAtMax;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_noise;   //!
   TBranch        *b_noiseZCD;   //!
   TBranch        *b_charge;   //!
   TBranch        *b_jitter;   //!
   TBranch        *b_jitterCFD20;   //!
   TBranch        *b_jitterCFD50;   //!
   TBranch        *b_jitterCTD;   //!
   TBranch        *b_riseTime1090;   //!
   TBranch        *b_derivative;   //!
   TBranch        *b_derivativeCFD20;   //!
   TBranch        *b_derivativeCFD50;   //!
   TBranch        *b_derivativeCTD;   //!
   TBranch        *b_derivativeZCD;   //!
   TBranch        *b_width;   //!
   TBranch        *b_TOT;   //!
   TBranch        *b_nALTIROCChannel;   //!
   TBranch        *b_nBoards;   //!
   TBranch        *b_seqCnt;   //!
   TBranch        *b_trigCnt;   //!
   TBranch        *b_arBoardNb;   //!
   TBranch        *b_arChannelNb;   //!
   TBranch        *b_arTOA;   //!
   TBranch        *b_arTOAOV;   //!
   TBranch        *b_arTOT;   //!
   TBranch        *b_arTOTOV;   //!
   TBranch        *b_arTOTF;   //!
   TBranch        *b_arTOTC;   //!
   TBranch        *b_NTrack;   //!
   TBranch        *b_Xtr;   //!
   TBranch        *b_Ytr;   //!
   TBranch        *b_DeltaXtr;   //!
   TBranch        *b_DeltaYtr;   //!
   TBranch        *b_axz_down;   //!
   TBranch        *b_ayz_down;   //!
   TBranch        *b_axz_up;   //!
   TBranch        *b_ayz_up;   //!
   TBranch        *b_Zkink;   //!
   TBranch        *b_DRMin;   //!

   monanalyse(TTree *tree=0);
   virtual ~monanalyse();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef monanalyse_cxx
monanalyse::monanalyse(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tree_February2022_201.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tree_February2022_201.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

monanalyse::~monanalyse()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t monanalyse::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t monanalyse::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void monanalyse::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("FEI4x", &FEI4x, &b_FEI4x);
   fChain->SetBranchAddress("FEI4y", &FEI4y, &b_FEI4y);
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("cycleNumber", &cycleNumber, &b_cycleNumber);
   fChain->SetBranchAddress("eventTime", &eventTime, &b_eventTime);
   fChain->SetBranchAddress("nChannel", &nChannel, &b_nChannel);
   fChain->SetBranchAddress("pulseHeightNoise", pulseHeightNoise, &b_pulseHeightNoise);
   fChain->SetBranchAddress("pulseHeight", pulseHeight, &b_pulseHeight);
   fChain->SetBranchAddress("pulseHeightMin", pulseHeightMin, &b_pulseHeightMin);
   fChain->SetBranchAddress("timeCFD", timeCFD, &b_timeCFD);
   fChain->SetBranchAddress("timeZCD", timeZCD, &b_timeZCD);
   fChain->SetBranchAddress("timeCFD20", timeCFD20, &b_timeCFD20);
   fChain->SetBranchAddress("timeCFD70", timeCFD70, &b_timeCFD70);
   fChain->SetBranchAddress("timeZCD20", timeZCD20, &b_timeZCD20);
   fChain->SetBranchAddress("timeCFD50", timeCFD50, &b_timeCFD50);
   fChain->SetBranchAddress("timeZCD50", timeZCD50, &b_timeZCD50);
   fChain->SetBranchAddress("timeCTD", timeCTD, &b_timeCTD);
   fChain->SetBranchAddress("timeAtMax", timeAtMax, &b_timeAtMax);
   fChain->SetBranchAddress("pedestal", pedestal, &b_pedestal);
   fChain->SetBranchAddress("noise", noise, &b_noise);
   fChain->SetBranchAddress("noiseZCD", noiseZCD, &b_noiseZCD);
   fChain->SetBranchAddress("charge", charge, &b_charge);
   fChain->SetBranchAddress("jitter", jitter, &b_jitter);
   fChain->SetBranchAddress("jitterCFD20", jitterCFD20, &b_jitterCFD20);
   fChain->SetBranchAddress("jitterCFD50", jitterCFD50, &b_jitterCFD50);
   fChain->SetBranchAddress("jitterCTD", jitterCTD, &b_jitterCTD);
   fChain->SetBranchAddress("riseTime1090", riseTime1090, &b_riseTime1090);
   fChain->SetBranchAddress("derivative", derivative, &b_derivative);
   fChain->SetBranchAddress("derivativeCFD20", derivativeCFD20, &b_derivativeCFD20);
   fChain->SetBranchAddress("derivativeCFD50", derivativeCFD50, &b_derivativeCFD50);
   fChain->SetBranchAddress("derivativeCTD", derivativeCTD, &b_derivativeCTD);
   fChain->SetBranchAddress("derivativeZCD", derivativeZCD, &b_derivativeZCD);
   fChain->SetBranchAddress("width", width, &b_width);
   fChain->SetBranchAddress("TOT", TOT, &b_TOT);
   fChain->SetBranchAddress("nALTIROCChannel", &nALTIROCChannel, &b_nALTIROCChannel);
   fChain->SetBranchAddress("nBoards", &nBoards, &b_nBoards);
   fChain->SetBranchAddress("seqCnt", seqCnt, &b_seqCnt);
   fChain->SetBranchAddress("trigCnt", trigCnt, &b_trigCnt);
   fChain->SetBranchAddress("arBoardNb", arBoardNb, &b_arBoardNb);
   fChain->SetBranchAddress("arChannelNb", arChannelNb, &b_arChannelNb);
   fChain->SetBranchAddress("arTOA", arTOA, &b_arTOA);
   fChain->SetBranchAddress("arTOAOV", arTOAOV, &b_arTOAOV);
   fChain->SetBranchAddress("arTOT", arTOT, &b_arTOT);
   fChain->SetBranchAddress("arTOTOV", arTOTOV, &b_arTOTOV);
   fChain->SetBranchAddress("arTOTF", arTOTF, &b_arTOTF);
   fChain->SetBranchAddress("arTOTC", arTOTC, &b_arTOTC);
   fChain->SetBranchAddress("NTrack", &NTrack, &b_NTrack);
   fChain->SetBranchAddress("Xtr", Xtr, &b_Xtr);
   fChain->SetBranchAddress("Ytr", Ytr, &b_Ytr);
   fChain->SetBranchAddress("DeltaXtr", DeltaXtr, &b_DeltaXtr);
   fChain->SetBranchAddress("DeltaYtr", DeltaYtr, &b_DeltaYtr);
   fChain->SetBranchAddress("axz_down", &axz_down, &b_axz_down);
   fChain->SetBranchAddress("ayz_down", &ayz_down, &b_ayz_down);
   fChain->SetBranchAddress("axz_up", &axz_up, &b_axz_up);
   fChain->SetBranchAddress("ayz_up", &ayz_up, &b_ayz_up);
   fChain->SetBranchAddress("Zkink", &Zkink, &b_Zkink);
   fChain->SetBranchAddress("DRMin", &DRMin, &b_DRMin);
   Notify();
}

Bool_t monanalyse::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void monanalyse::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t monanalyse::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef monanalyse_cxx
