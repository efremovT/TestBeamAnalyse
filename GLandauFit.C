#define monanalyse_cxx
#include "monanalyse.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Riostream.h"
#include <TF1.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TMinuit.h>
#include <TColor.h>
#include <TLine.h>
#include "/home/nikolic/ATLAS/style/AtlasStyle.C"
#include "/home/nikolic/ATLAS/style/AtlasUtils.C"
#include "/home/nikolic/ATLAS/style/AtlasLabels.C"

#include "RooBinIntegrator.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooFFTConvPdf.h"
#include "RooFit.h"
#include "RooGaussian.h"
#include "RooIntegrator1D.h"
#include "RooLandau.h"
#include "RooMsgService.h"
#include "RooNumIntConfig.h"
#include "RooNumber.h"
#include "RooPlot.h"
#include "RooRealVar.h"

#include "TAxis.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TVirtualFFT.h"
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TStyle.h>

using namespace RooFit;

//--------------------------------For 205 Analysis------------------------------
//==============================================================================
//-------------------------------Définitions------------------------------------
//==============================================================================
const Double_t micron = 18.5 *1e-3;

const Double_t shiftX[2] = {536., 554.};
const Double_t shiftY[2] = {125., 167.};

Double_t convertX(Double_t x, Int_t i) {
	return (x - shiftX[i])*micron;
}

Double_t convertY(Double_t y, Int_t i) {
	return (y - shiftY[i])*micron;
}

//==============================================================================
//-------------------------------Définitions------------------------------------
//==============================================================================

void monanalyse::Loop()
{
//   In a ROOT session, you can do:
//      root> .L monanalyse.C
//      root> monanalyse t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

//==============================================================================
//-------------------------------Initialisations--------------------------------
//==============================================================================


   //---------------TXT Output          ----------------------------------------
   
     ofstream ChargeFitFBKTxt("ChargeFitFBK_50Bin_min50Event.txt");
     ofstream ChargeFitIMETxt("ChargeFitIME_50Bin_min50Event.txt");
     
     ofstream ChargeFitFBKTxt_gt2("ChargeFitFBK_50Bin_min50Event_gt2.txt");
     ofstream ChargeFitIMETxt_gt2("ChargeFitIME_50Bin_min50Event_gt2.txt");
     
   //---------------TXT Output          ----------------------------------------
   
   
   
   //---------------Standards variables ----------------------------------------
   const Int_t NDUT=2; //Number DUT
   const Int_t Nbin{100}; //Std Bin for 1D bin
   const Int_t NbinBox[3]{20,50,100};//number of bins for 2D plots change first variable only
   const Int_t NbinCenter{NbinBox[0]/2};
   Double_t BinSizePos[2]; //Taille bin position
   Double_t NEventMin[2]{50,50}; //Number of event minimum for fitting each DUT
   
   //NOM DUT
   TString DUTname[3]{"FBK_W19 ","IMEv2_W7Q2 ","SIPM "};
   TString Fitname[3]{" fit range >0fC chi square"," fit range >1fC chi square","  fit range >2fC chi square"};
   TString DUTNumberBox[3]{"20","50","100"};
   
   TString tirrsensor[3];
   tirrsensor[0] = TString("Irr. 4E14");
   tirrsensor[1] = TString("Irr. 8E14");
   tirrsensor[2] = TString("");
   //---------------Standards variables ----------------------------------------
   

   
   //--------- Window of plotting XTR YTR for each DUT--------------------------
   Double_t xmin[2]{538.,548.};
   Double_t xmax[2]={600.,622.};
   Double_t ymin[2]={74.,116.};
   Double_t ymax[2]={136.,184.};	
   Double_t xmin_bis[2]{-1,-1};
   Double_t ymin_bis[2]{-1,-1};	
   Double_t xmax_bis[2]{1,1};
   Double_t ymax_bis[2]{1,1};
   Double_t xmin_bis_zoom[2]{-0.6,-0.6};
   Double_t ymin_bis_zoom[2]{-0.6,-0.6};	
   Double_t xmax_bis_zoom[2]{0.6,0.6};
   Double_t ymax_bis_zoom[2]{0.6,0.6};
   
   /*for (Int_t i=0; i<NDUT ; i++) {
      xmin_bis[i] = convertX(xmin[i],i);
      xmax_bis[i] = convertX(xmax[i],i);
      ymin_bis[i] = convertY(ymin[i],i);
      ymax_bis[i] = convertY(ymax[i],i);
      }
   */
   
   //--------- Window of plotting XTR YTR for each DUT--------------------------
   
   
   //-----------   CUT ---------------------------------------------------------
   //Cut Time difference between SiPM and DUT 
   Float_t TimeMin[2]{2800.,2500.};
   Float_t TimeMax[2]{4800.,4500.};
   
   //Float_t ChargeCut[NDUT]{-10,-10}; //Charge cut for each DUT
   Float_t ChargeCut[NDUT]{2,2};
   RooRealVar x("x", "Charge [fC]", 2, 50);
   //RooRealVar x("x", "Charge [fC]", 0, 50); //change range fit
   
   Float_t PulseHeightCut[2]{7.,770.};
   
   //---------- CUT  -----------------------------------------------------------
   
   
   //----------- Histogram -----------------------------------------------------
   //Define histo
   TH1F *ChDUT0 = new TH1F("ChDUT0","charge of FBK_W19 DUT[0] with cut on time and pulse height",100,-5,50);
   TH1F *HTimeAtMaxDiff[NDUT];
   
   
   //Charge histograms
   TH1F *HCharge[NDUT];
   TH1F *HCharge_event[NDUT]; // charge distribution if N > NEventMin[NDUT]
   TH2F *HOccupancy[NDUT];
   TH2F *HOccupancymm[NDUT];
   TH1F *HErrGain[NDUT];
   //Box Histograms
   TH1F *HChargeBox[Nbin][Nbin][NDUT];
   TH2F *HMpvBox[NDUT];
   TH2F *HMpvBoxErr[NDUT];
   TH1F *HChargeFit[NbinBox[0]][NbinBox[0]][NDUT];


   //Charge
   Double_t Clandau[Nbin][Nbin][NDUT];
   Double_t Cerrlandau[Nbin][Nbin][NDUT];
   Double_t Cerr[NDUT];
   
   
   //Loop initialisation
   for (Int_t i=0; i<NDUT ; i++) {
      // charge histogram 
      HCharge[i] = new TH1F(DUTname[i] +" h1",DUTname[i] + "charge",100,-5,50) ;
      HCharge_event[i] = new TH1F(DUTname[i] +" h1_event",DUTname[i] + "charge_event",100,-5,50) ;
      HOccupancy[i] = new TH2F(DUTname[i] + " h2",DUTname[i] + " XtrYtr",NbinBox[0], xmin[i], xmax[i], NbinBox[0], ymin[i], ymax[i]);
      HOccupancymm[i] = new TH2F(DUTname[i] + " h3",DUTname[i] + "pos_mm" ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]); //position in mm
      
      
      
      //GetBinSize while we're at it
      BinSizePos[i] = HOccupancymm[i]->GetXaxis()->GetBinWidth(Nbin);
      
      
      
      
      //Time histogram
      HTimeAtMaxDiff[i] = new TH1F("h4  " + DUTname[i] ,"Time of SiPM -" + DUTname[i],100,xmin_bis[0],xmax_bis[0]) ;
   
      
   //DUT cutting in box
      HMpvBox[i] = new TH2F(DUTname[i] + "h5 " ,DUTname[i] + " Mpv "  ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]);
      
      HMpvBoxErr[i] = new TH2F(DUTname[i] + "h6 "  ,DUTname[i] + " Mpv Errors in % "  ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]);
      
      
      HErrGain[i] = new TH1F("HErrGain"+DUTname[i]," Abso Error on the Gain in"+DUTname[i],50,0.,2.);
      
      for (Int_t k=0;k<Nbin;k++) {
       for (Int_t j=0;j<Nbin;j++){
            HChargeBox[k][j][i] = new TH1F(DUTname[i] +"X"+k+"y"+j,DUTname[i]+"X"+k+"y"+j,25,0.,50.);
            Clandau[k][j][i] = 0;
            Cerrlandau[k][j][i] = 0;
         
      }
      }
   
   
   }// loop on DUT  
   //----------- Histogram ----------------------------------------------------
    
//==============================================================================
//-------------------------------Initialisations--------------------------------
//==============================================================================    
        
        
        
        
//==============================================================================
//-------------------------------LOOP-------------------------------------------
//==============================================================================        
        
   //------------------------Code start-----------------------------------------
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   //---------------------------------------------------------------------------   
   
   
   
      //--------------Charge histogram with cut on time and ph------------------
      if (pulseHeight[2] >= PulseHeightCut[0]  && pulseHeight[2] <= PulseHeightCut[1]) { // pulse SiPM 
	  
	      for (Int_t i=0 ; i<NDUT ; i++) { // for each DUT
	    
            if ( (timeAtMax[2]-timeAtMax[i]) > TimeMin[i] && (timeAtMax[2]-timeAtMax[i]) < TimeMax[i] ) { //If constraint on time are respected
            

               if (charge[i] >= ChargeCut[i]) { //Charge cut
                  
                  HOccupancymm[i]->Fill(convertX(Xtr[i],i),convertY(Ytr[i],i));
                  //Box filling
                  for (Int_t ix=0;ix<NbinBox[0];ix++){ 
                  for (Int_t iy=0;iy<NbinBox[0];iy++){
                  if  ((convertX(Xtr[i],i)> xmin_bis[i]+BinSizePos[i]*ix) && (convertX(Xtr[i],i)<xmin_bis[i]+BinSizePos[i]*(ix+1)) && (convertY(Ytr[i],i)>ymin_bis[i]+BinSizePos[i]*iy) &&(convertY(Ytr[i],i)<ymin_bis[i]+BinSizePos[i]*(iy+1))){ //move on the DUT
                  
                  
                  HChargeBox[ix][iy][i]-> Fill(charge[i]);
                  
                  
                  }
                  }
                  }
                  //end box filling
                  
               } // end Charge condition
            } // end time condition
         } // end for loop on DUT
      } // end IF pulse height SiPM
   } // end event loop
   
   //------------------------Code end  -----------------------------------------
   
//==============================================================================
//-------------------------------LOOP-------------------------------------------
//==============================================================================   



//==============================================================================
//-------------------------------Analysis---------------------------------------
//==============================================================================

//Initialisation of charge fit histogram
for (Int_t i=0 ; i<NDUT ; i++) { //DUT
for (Int_t ix=0;ix<NbinBox[0];ix++){ //XPOS
for (Int_t iy=0;iy<NbinBox[0];iy++){ //YPOS
   HChargeFit[ix][iy][i] = (TH1F*) HChargeBox[ix][iy][i]->Clone();
   HChargeFit[ix][iy][i]->SetName(  DUTname[i] );
   HChargeFit[ix][iy][i]->SetTitle(  "fit of " + DUTname[i]  + "position = " + ix +" x " + iy +" y ");
}
}
}


RooPlot *xframemulti[NbinBox[0]][NbinBox[0]][NDUT];
RooDataHist *dhm[NbinBox[0]][NbinBox[0]][NDUT];
Double_t mean_landau[2]{20,10};
RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
RooMsgService::instance().setSilentMode(true);

//Fitting and histogram of MvP
for (Int_t i=0 ; i<NDUT ; i++) { //DUT
for (Int_t ix=0;ix<NbinBox[0];ix++){ //XPOS
for (Int_t iy=0;iy<NbinBox[0];iy++){ //YPOS
   //if (HChargeFit[ix][iy][i][0]->GetEntries() >3000){//condition on number of event
   
 if (HChargeFit[ix][iy][i] -> GetEntries() > NEventMin[i]){//end if event 

RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL); // Silence Roofit output
RooMsgService::instance().setSilentMode(true);

   //roofit each bin
    //RooRealVar x("x", "Charge [fC]", 2, 50);
    //RooRealVar x("x", "Charge [fC]", 0, 50); //change range fit
    
    xframemulti[ix][iy][i] = x.frame(Name("xframe"), Title(DUTname[i] +"charge fit with landau x gauss"));
    dhm[ix][iy][i] = new RooDataHist("roofit of " + DUTname[i]  + "position = " + ix +" x " + iy +" y ","roofit of " + DUTname[i]  + "position = " + ix +" x " + iy +" y ", x,Import(*HChargeFit[ix][iy][i]));
    
    //landau
    RooRealVar meanl("MPV", "mean landau", mean_landau[i],
                  mean_landau[i] - 20, mean_landau[i] + 20); //lower bound and upper bound
    RooRealVar sigl("sl", "sigma landau", 1, 0.1, 10);
    RooLandau landau("lx", "lx", x, meanl, sigl);
    
    //gaus
    RooRealVar meang("mg", "mg", 0);
    RooRealVar sigg("sg", "sg", 2, 0.1, 10);
    RooGaussian gauss("gauss", "gauss", x, meang, sigg);
    
    
    RooFFTConvPdf glandau("glandau", "landau (X) gauss", x, landau, gauss);
    RooFitResult *res = glandau.fitTo(*dhm[ix][iy][i]);
    
    
    // Export parameter on histogram
    dhm[ix][iy][i]->plotOn(xframemulti[ix][iy][i]); 
    glandau.paramOn(xframemulti[ix][iy][i], Parameters(meanl), Layout(0.5, 0.85, 0.7));
    glandau.paramOn(xframemulti[ix][iy][i], Parameters(sigl), Layout(0.5, 0.85, 0.6));
    glandau.paramOn(xframemulti[ix][iy][i], Parameters(sigg), Layout(0.5, 0.85, 0.5));
    glandau.plotOn(xframemulti[ix][iy][i]);

    Clandau[ix][iy][i] = meanl.getVal(); // Mpv from convolution
    Cerr[i] = meanl.getAsymErrorHi();
    Cerrlandau[ix][iy][i] = 100.*Cerr[i]/(Clandau[ix][iy][i]);
   
    HMpvBox[i]-> SetBinContent(ix+1,iy+1,Clandau[ix][iy][i]);

    HMpvBoxErr[i]-> SetBinContent(ix+1,iy+1,Cerrlandau[ix][iy][i]);

    HErrGain[i]->Fill(Cerr[i]); 
    
    
    

   if (i==0) {
   ChargeFitFBKTxt << "In Box " << ix << ", " << iy << ", N:  "<< HChargeFit[ix][iy][i] -> GetEntries() << ", MPV:  " << Clandau[ix][iy][i]<< "  error: " << Cerr[i] << "  percent error  " << Cerrlandau[ix][iy][i]  << ",  Landau:  " <<Clandau[ix][iy][i] <<   endl;
   }
      if (i==1) {
   ChargeFitIMETxt << "In Box " << ix << ", " << iy << ", N:  "<< HChargeFit[ix][iy][i] -> GetEntries() << ", MPV:  " << Clandau[ix][iy][i]<< "  error: " << Cerr[i] << "  percent error  " << Cerrlandau[ix][iy][i]  << ",  Landau:  " <<Clandau[ix][iy][i] <<   endl;
   }
// txt print
   
   }//end if event
   
   
   // if there is not enough event we hardcode a outlier value
      if (HChargeFit[ix][iy][i] -> GetEntries() < NEventMin[i]){
   Clandau[ix][iy][i] = -1000;
   Cerr[i] = 1000;
   Cerrlandau[ix][iy][i] = 100.*Cerr[i]/(Clandau[ix][iy][i]);
   
   HMpvBox[i]-> SetBinContent(ix+1,iy+1,Clandau[ix][iy][i]);
   HMpvBoxErr[i]-> SetBinContent(ix+1,iy+1,Cerrlandau[ix][iy][i]);

   HErrGain[i]->Fill(Cerr[i]);
   }
   
   
} // end for posx
} // end for posy
} // end for each dut



//==============================================================================
//-------------------------------Analysis---------------------------------------
//==============================================================================


//==============================================================================
//-------------------------------Plot-------------------------------------------
//==============================================================================

/*

syntax for histograms :


Histogram_raw[Dut_Number]

Histogram_fit[Fit_option][Dut_Number]

Histogram_fit_localised[x][y][Fit_option][Dut_Number]
*/
   
   //------------------------ Plot Start ---------------------------------------
   
   TStyle *MyStyle = new TStyle("MyStyle","My own Style");
   MyStyle->SetPalette(1,0);
   MyStyle->SetStatFont(42);
   MyStyle->SetStatBorderSize(1);
   MyStyle->SetStatColor(10);
   MyStyle->SetStatFontSize(0.03);
   MyStyle->SetTitleBorderSize(1);
   MyStyle->SetTitleFont(52);
   MyStyle->SetTitleFontSize(0.04);
   MyStyle->SetOptFit(1111);  
   MyStyle->SetOptStat(1111);
   MyStyle->SetLabelSize(0.005);
  
   MyStyle->SetLabelSize(0.03,"xyz"); 
   MyStyle->SetTitleSize(0.035,"xyz"); 
   MyStyle->SetTitleFont(22,"xyz"); 
   MyStyle->SetTitleOffset(1.2,"yz");
   MyStyle->SetPadLeftMargin(0.16); 
   MyStyle->SetPadRightMargin(0.15); 
   MyStyle->SetPalette(55); 
   MyStyle->SetStatX(0.85);
   MyStyle-> SetStatY(0.9);
   
   //   MyStyle->SetPalette(kAlpine); 
   gROOT->SetStyle("MyStyle");
   cout << "styles are set " << endl;
  
  
  
   TCanvas *c2mm = new TCanvas("c2mm","Occupancy");
   gStyle-> SetStatY(0.9);
   c2mm->Divide(2,1);
   c2mm->cd(1);
   c2mm->SetCanvasSize(1600, 600);
   c2mm->SetWindowSize(1630, 630);
   
   HOccupancymm[0]->Draw("colz");
   HOccupancymm[0]-> SetYTitle("y in mm");
   HOccupancymm[0]-> SetXTitle("x in mm");
   HOccupancymm[0]-> SetTitleSize(0.042,"x in mm");
   HOccupancymm[0]-> SetTitleSize(0.042,"y in mm");
   HOccupancymm[0]-> SetZTitle("Events");
   HOccupancymm[0]-> SetMinimum(50);
   c2mm->cd(2);
   HOccupancymm[1]->Draw("colz");
   HOccupancymm[1]-> SetYTitle("y in mm");
   HOccupancymm[1]-> SetXTitle("x in mm");
   HOccupancymm[1]-> SetTitleSize(0.042,"x in mm");
   HOccupancymm[1]-> SetTitleSize(0.042,"y in mm");
   HOccupancymm[1]-> SetZTitle("Events");
   HOccupancymm[1]-> SetMinimum(50);
   
   c2mm->Print("fig/Occupation_on_pad_DUT_8x8_cut.png");
   
   
    //TEST MPV
   TCanvas *c4 = new TCanvas("c4","Mpv0");
   c4->Divide(2,1);
   c4->cd(1);
   c4->SetCanvasSize(1600, 600);
   c4->SetWindowSize(1630, 630);
   HMpvBox[1]-> SetMinimum(2);
   HMpvBox[1]-> SetMaximum(9);
   HMpvBox[0]-> SetMinimum(6);
   
   
   
    
   HMpvBox[0]->Draw("colz");
   HMpvBox[0]-> SetYTitle("y in mm");
   HMpvBox[0]-> SetXTitle("x in mm");
   HMpvBox[0]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[0]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[0]-> SetZTitle("MpV of charge");
   
   c4->cd(2);

   HMpvBoxErr[0]->Draw("colz");
   HMpvBoxErr[0]-> SetYTitle("y in mm");
   HMpvBoxErr[0]-> SetXTitle("x in mm");
   HMpvBoxErr[0]-> SetTitleSize(0.042,"x in mm");
   HMpvBoxErr[0]-> SetTitleSize(0.042,"y in mm");
   HMpvBoxErr[0]-> SetZTitle("MpVerr in %");
   HMpvBoxErr[0]-> SetMaximum(100);
   HMpvBoxErr[0]-> SetMinimum(0);
   
   c4->Print("fig/MPV_DUT_" + DUTNumberBox[0] + "_FBK.png");
   
   
   
       //TEST MPVErr
   TCanvas *c4err = new TCanvas("c4err","Mpv_err");
   c4err->Divide(2,1);
   c4err->cd(1);
   c4err->SetCanvasSize(1600, 600);
   c4err->SetWindowSize(1630, 630);
   
   HMpvBox[1]->Draw("colz");
   HMpvBox[1]-> SetYTitle("y in mm");
   HMpvBox[1]-> SetXTitle("x in mm");
   HMpvBox[1]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[1]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[1]-> SetZTitle("MpV of charge");
   
   c4err->cd(2);
   HMpvBoxErr[1]->Draw("colz");
   HMpvBoxErr[1]-> SetYTitle("y in mm");
   HMpvBoxErr[1]-> SetXTitle("x in mm");
   HMpvBoxErr[1]-> SetTitleSize(0.042,"x in mm");
   HMpvBoxErr[1]-> SetTitleSize(0.042,"y in mm");
   HMpvBoxErr[1]-> SetZTitle("MpVerr in %");
   HMpvBoxErr[1]-> SetMaximum(100);
   HMpvBoxErr[1]-> SetMinimum(0);
   
   c4err->Print("fig/MPV_DUTErr_" + DUTNumberBox[0] + "_IME.png");

   
    //with roofit
   TCanvas *fitoccupationroo = new TCanvas("fitoccupationroo","fitoccupationroo");
   gStyle->SetOptFit(0010);  
   gStyle->SetOptStat(0000);
   gStyle-> SetStatY(0.9);
   gStyle->SetOptFit(11);  
   gStyle->SetOptStat(10);
   gStyle-> SetStatY(0.9);
   fitoccupationroo->SetRightMargin(0.);
   fitoccupationroo->SetLeftMargin(0.);
   fitoccupationroo->SetTopMargin(0.);
   fitoccupationroo->SetBottomMargin(0.);
   fitoccupationroo->Divide(NbinBox[0],NbinBox[0]);
   fitoccupationroo->SetCanvasSize(8000, 4600);
   fitoccupationroo->SetWindowSize(8030, 4630);
   fitoccupationroo->SetFrameBorderMode(0);
   
   Int_t ix=0;
   Int_t iy=NbinBox[0]-1;
   for (Int_t c=1 ; c<=NbinBox[0]*NbinBox[0] ; c++){
  
      if (ix >= NbinBox[0]) {
      ix = 0;
      iy -= 1;
      }
      
       if (HChargeFit[ix][iy][0] -> GetEntries() > NEventMin[0]){
       
         fitoccupationroo->cd(c);
         xframemulti[ix][iy][0]->Draw("");
         xframemulti[ix][iy][0]-> SetYTitle("Number of events");
         xframemulti[ix][iy][0]-> SetXTitle("Charge in fC");
         xframemulti[ix][iy][0]-> SetTitleSize(0.042,"x");
         xframemulti[ix][iy][0]-> SetTitleSize(0.042,"y");
         }
         
         
       else {
         fitoccupationroo->cd(c);
         HChargeFit[ix][iy][0]-> Draw("");
         HChargeFit[ix][iy][0]-> SetYTitle("Number of events");
         HChargeFit[ix][iy][0]-> SetXTitle("Charge in fC");
         HChargeFit[ix][iy][0]-> SetTitleSize(0.042,"x");
         HChargeFit[ix][iy][0]-> SetTitleSize(0.042,"y");
         }
        
           
      ix+=1;
      }
      
   fitoccupationroo->Print("fig/roofit_FBK.png");
   
     //with roofit
   TCanvas *fitoccupationroo1 = new TCanvas("fitoccupationroo1","fitoccupationroo1");
   gStyle->SetOptFit(0010);  
   gStyle->SetOptStat(0000);
   gStyle-> SetStatY(0.9);
   gStyle->SetOptFit(11);  
   gStyle->SetOptStat(10);
   gStyle-> SetStatY(0.9);
   fitoccupationroo1->SetRightMargin(0.);
   fitoccupationroo1->SetLeftMargin(0.);
   fitoccupationroo1->SetTopMargin(0.);
   fitoccupationroo1->SetBottomMargin(0.);
   fitoccupationroo1->Divide(NbinBox[0],NbinBox[0]);
   fitoccupationroo1->SetCanvasSize(8000, 4600);
   fitoccupationroo1->SetWindowSize(8030, 4630);
   fitoccupationroo1->SetFrameBorderMode(0);
   
   ix=0;
   iy=NbinBox[0]-1;
   
   for (Int_t c=1 ; c<=NbinBox[0]*NbinBox[0] ; c++){
  
      if (ix >= NbinBox[0]) {
      ix = 0;
      iy -= 1;
      }
      
       if (HChargeFit[ix][iy][1] -> GetEntries() > NEventMin[0]){
       
         fitoccupationroo1->cd(c);
         xframemulti[ix][iy][1]->Draw("");
         xframemulti[ix][iy][1]-> SetYTitle("Number of events");
         xframemulti[ix][iy][1]-> SetXTitle("Charge in fC");
         xframemulti[ix][iy][1]-> SetTitleSize(0.042,"x");
         xframemulti[ix][iy][1]-> SetTitleSize(0.042,"y");
         }
         
         
       else {
         fitoccupationroo1->cd(c);
         HChargeFit[ix][iy][1]-> Draw("");
         HChargeFit[ix][iy][1]-> SetYTitle("Number of events");
         HChargeFit[ix][iy][1]-> SetXTitle("Charge in fC");
         HChargeFit[ix][iy][1]-> SetTitleSize(0.042,"x");
         HChargeFit[ix][iy][1]-> SetTitleSize(0.042,"y");
         }
        
           
      ix+=1;
      }
      
   fitoccupationroo1->Print("fig/roofit_IME.png");
   
//==============================================================================
//-------------------------------Plot-------------------------------------------
//==============================================================================  
} // end function 
