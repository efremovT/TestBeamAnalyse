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

//==============================================================================
//-------------------------------Définitions------------------------------------
//==============================================================================
const Double_t micron = 18.5 *1e-3;

const Double_t shiftX[2] = {568.9, 584.8};
const Double_t shiftY[2] = {106.4, 150.1};

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

   
   //---------------Standards variables ----------------------------------------
   const Int_t NDUT=2; //Number DUT
   const Int_t Nbin{100}; //Std Bin
   const Int_t NbinBox[3]{20,50,100};//bin for box only change first variable
   const Int_t NbinCenter{NbinBox[0]/2};
   const Int_t NFitOption{3};
   Double_t BinSizePos[2]; //Taille bin position
   Double_t NEventMin[2]{50,50};
   
   //NOM DUT
   TString DUTname[3]{"FBK_W19 ","IMEv2_W7Q2 ","SIPM "};
   TString Fitname[3]{" >1fC chi square"," >2fC chi square"," >3fC chi square"};
   
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
   Double_t xmin_bis[2];
   Double_t ymin_bis[2];	
   Double_t xmax_bis[2];
   Double_t ymax_bis[2];
   Double_t xmin_bis_zoom[2]{-0.6,-0.6};
   Double_t ymin_bis_zoom[2]{-0.6,-0.6};	
   Double_t xmax_bis_zoom[2]{0.6,0.6};
   Double_t ymax_bis_zoom[2]{0.6,0.6};
   
   for (Int_t i=0; i<NDUT ; i++) {
      xmin_bis[i] = convertX(xmin[i],i);
      xmax_bis[i] = convertX(xmax[i],i);
      ymin_bis[i] = convertY(ymin[i],i);
      ymax_bis[i] = convertY(ymax[i],i);
      }
   
   
   //--------- Window of plotting XTR YTR for each DUT--------------------------
   
   
   //-----------   CUT ---------------------------------------------------------
   //Cut Time difference between SiPM and DUT 
   Float_t TimeMin[2]{3000.,2800.};
   Float_t TimeMax[2]{5000.,4800.};
   
   Float_t ChargeCut[NDUT]{-3,-3};
   Float_t FitCut[NFitOption]{0,1,2};

   Float_t PulseHeightCut[2]{7.,770.};
   
   //---------- CUT  -----------------------------------------------------------
   
   
   //----------- Histogram -----------------------------------------------------
   //Define histo
   TH1F *ChDUT0 = new TH1F("ChDUT0","charge of FBK_W19 DUT[0] with cut on time and pulse height",100,-5,50);
   TH1F *HTimeAtMaxDiff[NDUT];
   
   
   //Charge histograms
   TH1F *HCharge[NDUT];
   TH2F *HOccupancy[NDUT];
   TH2F *HOccupancymm[NDUT];
   TH1F *HErrGain[NDUT][NFitOption];
   //Box Histograms
   TH1F *HChargeBox[Nbin][Nbin][NDUT];
   TH2F *HMpvBox[NDUT][NFitOption];
   TH2F *HMpvBoxErr[NDUT][NFitOption];
   TH1F *HChargeFit[NbinBox[0]][NbinBox[0]][NDUT][NFitOption];


   //Charge
   Double_t Clandau[Nbin][Nbin][NDUT][NFitOption];
   Double_t Cerrlandau[Nbin][Nbin][NDUT][NFitOption];
   Double_t Cerr[NDUT][NFitOption];
   
   
   //Loop initialisation
   for (Int_t i=0; i<NDUT ; i++) {
      // charge histogram 
      HCharge[i] = new TH1F(DUTname[i] +" h1",DUTname[i] + "charge",100,-5,50) ;
      HOccupancy[i] = new TH2F(DUTname[i] + " h2",DUTname[i] + " XtrYtr",NbinBox[0], xmin[i], xmax[i], NbinBox[0], ymin[i], ymax[i]);
      HOccupancymm[i] = new TH2F(DUTname[i] + " h3",DUTname[i] + "posmm" ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]); //position in mm
      
      
      
      //GetBinSize while we're at it
      BinSizePos[i] = HOccupancymm[i]->GetXaxis()->GetBinWidth(Nbin);
      
      
      
      
      //Time histogram
      HTimeAtMaxDiff[i] = new TH1F("h4  " + DUTname[i] ,"Time of SiPM -" + DUTname[i],100,xmin_bis[0],xmax_bis[0]) ;
   
      
   //DUT cutting in box
   for (Int_t f=0;f<NFitOption;f++) {
      HMpvBox[i][f] = new TH2F(DUTname[i] + "h5 " + f,DUTname[i] + " MpvOnBox " + Fitname[f] ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]);
      
      HMpvBoxErr[i][f] = new TH2F(DUTname[i] + "h6 " + f,DUTname[i] + " MpvOnBoxErr " + Fitname[f] ,NbinBox[0], xmin_bis[i], xmax_bis[i], NbinBox[0], ymin_bis[i], ymax_bis[i]);
      
      
      HErrGain[i][f] = new TH1F("HErrGain"+DUTname[i]+ Fitname[f]," Abso Error on the Gain in"+DUTname[i],50,0.,2.);
      }
      
      for (Int_t k=0;k<Nbin;k++) {
       for (Int_t j=0;j<Nbin;j++){
         HChargeBox[k][j][i] = new TH1F(DUTname[i] +"X"+k+"y"+j,DUTname[i]+"X"+k+"y"+j,50,0.,50.);
         for (Int_t f=0;f<NFitOption;f++) {
            Clandau[k][j][i][f] = 0;
            Cerrlandau[k][j][i][f] = 0;
         }
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
               
                  HCharge[i]->Fill(charge[i]);
                  HOccupancy[i]->Fill(Xtr[i],Ytr[i]);
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
for (Int_t f=0 ; f<NFitOption ; f++) {
   HChargeFit[ix][iy][i][f] = (TH1F*) HChargeBox[ix][iy][i]->Clone();
   HChargeFit[ix][iy][i][f]->SetName(  DUTname[i] + f);
   HChargeFit[ix][iy][i][f]->SetTitle(  "fit of " + DUTname[i] + Fitname[f] + "position = " + ix +" x " + iy +" y ");
}
}
}
}


RooPlot *xframemulti[NbinBox[0]][NbinBox[0]][NDUT][NFitOption];
RooDataHist *dhm[NbinBox[0]][NbinBox[0]][NDUT][NFitOption];
Double_t mean_landau[2]{14,7};

//Fitting and histogram of MvP
for (Int_t i=0 ; i<NDUT ; i++) { //DUT
for (Int_t ix=0;ix<NbinBox[0];ix++){ //XPOS
for (Int_t iy=0;iy<NbinBox[0];iy++){ //YPOS
for (Int_t f=0 ; f<NFitOption ; f++) {   //what type of fitting
   //if (HChargeFit[ix][iy][i][0]->GetEntries() >3000){//condition on number of event
   if (HChargeFit[ix][iy][i][f] -> GetEntries() > NEventMin[i]){
   
   HChargeFit[ix][iy][i][f]->Fit("landau","R","",FitCut[f],50);//condition on charge
  
   Clandau[ix][iy][i][f] = HChargeFit[ix][iy][i][f]->GetFunction("landau")->GetParameter(1);
   Cerr[i][f] = HChargeFit[ix][iy][i][f]->GetFunction("landau")->GetParError(1);
   Cerrlandau[ix][iy][i][f] = 100.*Cerr[i][f]/(Clandau[ix][iy][i][f]);
   
   HMpvBox[i][f]-> SetBinContent(ix+1,iy+1,Clandau[ix][iy][i][f]);
   HMpvBox[0][f]-> SetMinimum(8);
   HMpvBox[1][f]-> SetMinimum(2.5);
   
   HMpvBoxErr[i][f]-> SetBinContent(ix+1,iy+1,Cerrlandau[ix][iy][i][f]);

   HErrGain[i][f]->Fill(Cerr[i][f]);
   } //end if event 

   //roofit each bin
    RooRealVar x("x", "Charge [fC]", -5, 50);
    xframemulti[ix][iy][i][f] = x.frame(Name("xframe"), Title(DUTname[i] +"charge fit with landau x gauss"));
    dhm[ix][iy][i][f] = new RooDataHist("roofit of " + DUTname[i] + Fitname[f] + "position = " + ix +" x " + iy +" y ","roofit of " + DUTname[i] + Fitname[f] + "position = " + ix +" x " + iy +" y ", x,Import(*HChargeFit[ix][iy][i][f]));
    
    //landau
    RooRealVar meanl("MPV", "mean landau", mean_landau[i],
                  mean_landau[i] - 5, mean_landau[i] + 5);
    RooRealVar sigl("sl", "sigma landau", 1, 0.1, 10);
    RooLandau landau("lx", "lx", x, meanl, sigl);
    
    //gaus
    RooRealVar meang("mg", "mg", 0);
    RooRealVar sigg("sg", "sg", 2, 0.1, 10);
    RooGaussian gauss("gauss", "gauss", x, meang, sigg);
    
    
    RooFFTConvPdf glandau("glandau", "landau (X) gauss", x, landau, gauss);
    RooFitResult *res = glandau.fitTo(*dhm[ix][iy][i][f]);
    
    dhm[ix][iy][i][f]->plotOn(xframemulti[ix][iy][i][f]);
    glandau.paramOn(xframemulti[ix][iy][i][f], Parameters(meanl), Layout(0.5, 0.85, 0.7));
    glandau.paramOn(xframemulti[ix][iy][i][f], Parameters(sigl), Layout(0.5, 0.85, 0.6));
    glandau.paramOn(xframemulti[ix][iy][i][f], Parameters(sigg), Layout(0.5, 0.85, 0.5));
    glandau.plotOn(xframemulti[ix][iy][i][f]);
} //End for fitting option
} // end for posx
} // end for posy
} // end for each dut


RooPlot *xframe[NDUT];
RooDataHist *dh[NDUT];
  for (Int_t i=0 ; i<NDUT ; i++) {
    RooRealVar x("x", "Charge [fC]", -5, 50);
    xframe[i] = x.frame(Name("xframe"), Title(DUTname[i] +"charge fit with landau x gauss"));
    dh[i] = new RooDataHist("dh" + DUTname[i],"dh" + DUTname[i], x,Import(*HCharge[i]));
    
    //landau
    RooRealVar ml("MPV", "mean landau", mean_landau[i],
                  mean_landau[i] - 5, mean_landau[i] + 5);
    RooRealVar sl("sl", "sigma landau", 1, 0.1, 10);
    RooLandau landau("lx", "lx", x, ml, sl);
    
    //gaus
    RooRealVar mg("mg", "mg", 0);
    RooRealVar sg("sg", "sg", 2, 0.1, 10);
    RooGaussian gauss("gauss", "gauss", x, mg, sg);
    
    
    RooFFTConvPdf lxg("lxg", "landau (X) gauss", x, landau, gauss);
    RooFitResult *res = lxg.fitTo(*dh[i]);
    
    dh[i]->plotOn(xframe[i]);
    lxg.paramOn(xframe[i], Parameters(ml), Layout(0.5, 0.85, 0.7));
    lxg.paramOn(xframe[i], Parameters(sl), Layout(0.5, 0.85, 0.6));
    lxg.paramOn(xframe[i], Parameters(sg), Layout(0.5, 0.85, 0.5));
    lxg.plotOn(xframe[i]);
  }

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
   
   
   //   MyStyle->SetPalette(kAlpine); 
   gROOT->SetStyle("MyStyle");
   cout << "styles are set " << endl;
  
  
  
   //SetAtlasStyle();
   
    //Test Charge
   TCanvas *c1 = new TCanvas("c1","test");
   gPad-> SetTickx();
   gPad-> SetTicky();
   gPad-> SetGrid();
   gPad->SetRightMargin(0.13);
   gPad->SetLeftMargin(0.13);
   gPad->SetBottomMargin(0.13);
   c1->SetCanvasSize(1600, 600);
   c1->SetWindowSize(1630, 630);
   c1->Divide(2,1);
   
   c1->cd(1);
   HCharge[0]->Draw("");
   HCharge[0]-> SetLineColor(4);
   HCharge[0]-> SetFillColor(4);
   HCharge[0]-> SetFillStyle(3001);
   HCharge[0]-> SetLineWidth(2);
   HCharge[0]->SetYTitle("Number of event");
   HCharge[0]->SetXTitle("Charge in fC");
   HCharge[0]->SetTitleSize(0.042,"x");
   HCharge[0]->SetTitleSize(0.042,"y");
   HCharge[0]->SetStats();
   
   c1->cd(2);
   HCharge[1]->Draw("");
   HCharge[1]-> SetLineColor(4);
   HCharge[1]-> SetFillColor(4);
   HCharge[1]-> SetFillStyle(3001);
   HCharge[1]-> SetLineWidth(2);
   HCharge[1]->SetYTitle("Number of event");
   HCharge[1]->SetXTitle("Charge in fC");
   HCharge[1]->SetTitleSize(0.042,"x");
   HCharge[1]->SetTitleSize(0.042,"y");
   HCharge[1]->SetStats();
    
   c1->Print("fig/Charge_Dut_0_205.png");
   
   
   /*
   TCanvas *c2 = new TCanvas("c2","XtrYtr0");
   c2->Divide(2,1);
   c2->cd(1);
   c2->SetCanvasSize(1600, 600);
   c2->SetWindowSize(1630, 630);
   
   HOccupancy[0]->Draw("colz");
   HOccupancy[0]-> SetYTitle("y in mm");
   HOccupancy[0]-> SetXTitle("x in mm");
   HOccupancy[0]-> SetTitleSize(0.042,"x in mm");
   HOccupancy[0]-> SetTitleSize(0.042,"y in mm");
   HOccupancy[0]-> SetZTitle("Events");
   HOccupancy[0]-> SetMinimum(0);
   c2->cd(2);
   HOccupancy[1]->Draw("colz");
   HOccupancy[1]-> SetYTitle("y in mm");
   HOccupancy[1]-> SetXTitle("x in mm");
   HOccupancy[1]-> SetTitleSize(0.042,"x in mm");
   HOccupancy[1]-> SetTitleSize(0.042,"y in mm");
   HOccupancy[1]-> SetZTitle("Events");
   HOccupancy[1]-> SetMinimum(0);
   
   //c2->Print("fig/monanalyse/Occupation_on_pad_DUT_6x6.png");
   */
   
   TCanvas *c2mm = new TCanvas("c2mm","XtrYtr0");
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
   HOccupancymm[0]-> SetMinimum(0);
   c2mm->cd(2);
   HOccupancymm[1]->Draw("colz");
   HOccupancymm[1]-> SetYTitle("y in mm");
   HOccupancymm[1]-> SetXTitle("x in mm");
   HOccupancymm[1]-> SetTitleSize(0.042,"x in mm");
   HOccupancymm[1]-> SetTitleSize(0.042,"y in mm");
   HOccupancymm[1]-> SetZTitle("Events");
   HOccupancymm[1]-> SetMinimum(0);
   
   c2mm->Print("fig/Occupation_on_pad_DUT_8x8_cut.png");
   
   /*
    //Test fit center
   
   TCanvas *c3 = new TCanvas("c3","1BoxCharge");
   gPad-> SetTickx();
   gPad-> SetTicky();
   gPad-> SetGrid();
   gStyle-> SetStatY(0.9);
   c3->Divide(2,1);
   c3->cd(1);
   c3->SetCanvasSize(1600, 600);
   c3->SetWindowSize(1630, 630);
   
   HChargeFit[NbinCenter][NbinCenter][0][0]->Draw("");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetYTitle("Number of events");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetXTitle("Charge in fC");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetTitleSize(0.042,"x");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetTitleSize(0.042,"y");
   
   c3->cd(2);
   
   HChargeFit[NbinCenter][NbinCenter][1][0]->Draw("");
   HChargeFit[NbinCenter][NbinCenter][1][0]-> SetYTitle("Number of events");
   HChargeFit[NbinCenter][NbinCenter][1][0]-> SetXTitle("Charge in fC");
   HChargeFit[NbinCenter][NbinCenter][1][0]-> SetTitleSize(0.042,"x");
   HChargeFit[NbinCenter][NbinCenter][1][0]-> SetTitleSize(0.042,"y");

   c3->Print("fig/monanalyse/FitMPVlikelihood.png");
   
   */
   
    //TEST MPV
   TCanvas *c4 = new TCanvas("c4","Mpv0");
   c4->Divide(2,1);
   c4->cd(1);
   c4->SetCanvasSize(1600, 600);
   c4->SetWindowSize(1630, 630);
   
   HMpvBox[0][0]->Draw("colz");
   HMpvBox[0][0]-> SetYTitle("y in mm");
   HMpvBox[0][0]-> SetXTitle("x in mm");
   HMpvBox[0][0]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[0][0]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[0][0]-> SetZTitle("MpV of charge");
   
   c4->cd(2);
   HMpvBox[1][0]->Draw("colz");
   HMpvBox[1][0]-> SetYTitle("y in mm");
   HMpvBox[1][0]-> SetXTitle("x in mm");
   HMpvBox[1][0]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[1][0]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[1][0]-> SetZTitle("MpV of charge");
   
   c4->Print("fig/MPV_DUT_8*8.png");
       //TEST MPVErr
   TCanvas *c4err = new TCanvas("c4err","Mpv0err");
   c4err->Divide(2,1);
   c4err->cd(1);
   c4err->SetCanvasSize(1600, 600);
   c4err->SetWindowSize(1630, 630);
   
   HMpvBoxErr[0][0]->Draw("colz");
   HMpvBoxErr[0][0]-> SetYTitle("y in mm");
   HMpvBoxErr[0][0]-> SetXTitle("x in mm");
   HMpvBoxErr[0][0]-> SetTitleSize(0.042,"x in mm");
   HMpvBoxErr[0][0]-> SetTitleSize(0.042,"y in mm");
   HMpvBoxErr[0][0]-> SetZTitle("MpVerr in percent");
   HMpvBoxErr[1][0]-> SetMaximum(100);
   
   c4err->cd(2);
   HMpvBoxErr[1][0]->Draw("colz");
   HMpvBoxErr[1][0]-> SetYTitle("y in mm");
   HMpvBoxErr[1][0]-> SetXTitle("x in mm");
   HMpvBoxErr[1][0]-> SetTitleSize(0.042,"x in mm");
   HMpvBoxErr[1][0]-> SetTitleSize(0.042,"y in mm");
   HMpvBoxErr[1][0]-> SetZTitle("MpVerr in percent");
   HMpvBoxErr[1][0]-> SetMaximum(100);
   c4err->Print("fig/MPV_DUTErr_8*8.png");
   
   
   TCanvas *err = new TCanvas("err","err");
   HErrGain[0][0]->Draw("");
   /* //Test FIT   
      
   TCanvas *c5 = new TCanvas("c5","Mpv_Fit_0");
   c5->Divide(3,2);
   c5->cd(1);
   c5->SetCanvasSize(1600, 600);
   c5->SetWindowSize(1630, 630);
   
   HMpvBox[0][0]->Draw("colz");
   HMpvBox[0][0]-> SetYTitle("y in mm");
   HMpvBox[0][0]-> SetXTitle("x in mm");
   HMpvBox[0][0]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[0][0]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[0][0]-> SetZTitle("MpV of charge");
   
   c5->cd(4);
   HChargeFit[NbinCenter][NbinCenter][0][0]->Draw("");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetYTitle("Number of events");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetXTitle("Charge in fC");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetTitleSize(0.042,"x");
   HChargeFit[NbinCenter][NbinCenter][0][0]-> SetTitleSize(0.042,"y");
   

   c5->cd(2);
   c5->SetCanvasSize(1600, 600);
   c5->SetWindowSize(1630, 630);
   
   HMpvBox[0][1]->Draw("colz");
   HMpvBox[0][1]-> SetYTitle("y in mm");
   HMpvBox[0][1]-> SetXTitle("x in mm");
   HMpvBox[0][1]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[0][1]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[0][1]-> SetZTitle("MpV of charge");
   
   c5->cd(5);
   HChargeFit[NbinCenter][NbinCenter][0][1]->Draw("");
   HChargeFit[NbinCenter][NbinCenter][0][1]-> SetYTitle("Number of events");
   HChargeFit[NbinCenter][NbinCenter][0][1]-> SetXTitle("Charge in fC");
   HChargeFit[NbinCenter][NbinCenter][0][1]-> SetTitleSize(0.042,"x");
   HChargeFit[NbinCenter][NbinCenter][0][1]-> SetTitleSize(0.042,"y");
   
   
   c5->cd(3);
   c5->SetCanvasSize(1600, 600);
   c5->SetWindowSize(1630, 630);
   
   HMpvBox[0][2]->Draw("colz");
   HMpvBox[0][2]-> SetYTitle("y in mm");
   HMpvBox[0][2]-> SetXTitle("x in mm");
   HMpvBox[0][2]-> SetTitleSize(0.042,"x in mm");
   HMpvBox[0][2]-> SetTitleSize(0.042,"y in mm");
   HMpvBox[0][2]-> SetZTitle("MpV of charge");
   
   c5->cd(6);
   HChargeFit[NbinCenter][NbinCenter][0][2]->Draw("");
   HChargeFit[NbinCenter][NbinCenter][0][2]-> SetYTitle("Number of events");
   HChargeFit[NbinCenter][NbinCenter][0][2]-> SetXTitle("Charge in fC");
   HChargeFit[NbinCenter][NbinCenter][0][2]-> SetTitleSize(0.042,"x");
   HChargeFit[NbinCenter][NbinCenter][0][2]-> SetTitleSize(0.042,"y");
   
   
   c5->Print("fig/monanalyse/MPV_Fit_0_likelihood.png");
   
   cout << BinSizePos[0] << "BinSize" << endl;	
   
   */
   
   /*HChargeBox[0][0][0] -> SetLineColor(4);
   HChargeBox[0][0][0]-> SetFillColor(4);
   HChargeBox[0][0][0]-> SetFillStyle(3001);
   HChargeBox[0][0][0]-> SetLineWidth(2);
   HChargeBox[0][0][0]->SetYTitle("Number of event");
   HChargeBox[0][0][0]->SetXTitle("Charge in fC");
   HChargeBox[0][0][0]->SetTitleSize(0.042,"x");
   HChargeBox[0][0][0]->SetTitleSize(0.042,"y");
   HChargeBox[0][0][0]->SetStats();*/
   
   
   /*
   TCanvas *fitoccupation = new TCanvas("fitoccupation","FitOccupation");
   gStyle->SetOptFit(0010);  
   gStyle->SetOptStat(0000);
   gStyle-> SetStatY(0.9);
   fitoccupation->Divide(NbinBox[0],NbinBox[0]);
   fitoccupation->SetCanvasSize(2000, 1600);
   fitoccupation->SetWindowSize(2030, 1630);
   
   Int_t xpos=0;
   Int_t ypos=NbinBox[0]-1;
   for (Int_t c=1 ; c<=NbinBox[0]*NbinBox[0] ; c++){
  
      if (xpos >= NbinBox[0]) {
      xpos = 0;
      ypos -= 1;
      }
      
      fitoccupation->cd(c);
      HChargeFit[xpos][ypos][0][0]->Draw("");
      HChargeFit[xpos][ypos][0][0]-> SetYTitle("Number of events");
      HChargeFit[xpos][ypos][0][0]-> SetXTitle("Charge in fC");
      HChargeFit[xpos][ypos][0][0]-> SetTitleSize(0.042,"x");
      HChargeFit[xpos][ypos][0][0]-> SetTitleSize(0.042,"y");
      
      xpos+=1;
   }
   
  // fitoccupation->Print("fig/rapport/Fit_rapport.png");
  */
  
    //with roofit
   TCanvas *fitoccupation = new TCanvas("fitoccupation","FitOccupation");
   gStyle->SetOptFit(0010);  
   gStyle->SetOptStat(0000);
   gStyle-> SetStatY(0.9);
   fitoccupation->Divide(NbinBox[0],NbinBox[0]);
   fitoccupation->SetCanvasSize(2000, 1600);
   fitoccupation->SetWindowSize(2030, 1630);
   
   Int_t xpos=0;
   Int_t ypos=NbinBox[0]-1;
   for (Int_t c=1 ; c<=NbinBox[0]*NbinBox[0] ; c++){
  
      if (xpos >= NbinBox[0]) {
      xpos = 0;
      ypos -= 1;
      }
      
      fitoccupation->cd(c);
      xframemulti[xpos][ypos][0][0]->Draw("");
      xframemulti[xpos][ypos][0][0]-> SetYTitle("Number of events");
      xframemulti[xpos][ypos][0][0]-> SetXTitle("Charge in fC");
      xframemulti[xpos][ypos][0][0]-> SetTitleSize(0.042,"x");
      xframemulti[xpos][ypos][0][0]-> SetTitleSize(0.042,"y");
      
      xpos+=1;
      }
      
   fitoccupation->Print("fig/roofit/6*6roofit.png");
   
//==============================================================================
//-------------------------------Plot-------------------------------------------
//==============================================================================  
} // end function 
