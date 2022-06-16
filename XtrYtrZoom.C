/* Credit to Pepe_Le_Pew on the CERN ROOT forum for the scaling axis
code*/


Double_t ScaleX(Double_t x)
{
  Double_t v;
  v = 18.5*1e-3* x; // for mimosa resolution to mm
  return v;
}

Double_t ScaleY(Double_t y)
{
  Double_t v;
  v = 18.5*1e-3* y; // for mimosa resolution to mm
  return v;
}

Double_t ScaleZ(Double_t z)
{
  Double_t v;
  v = 18.5*1e-3* z; // for mimosa resolution to mm
  return v;
}

void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t))
{
  if (!a) return; // just a precaution
  if (a->GetXbins()->GetSize())
    {
      // an axis with variable bins
      // note: bins must remain in increasing order, hence the "Scale"
      // function must be strictly (monotonically) increasing
      TArrayD X(*(a->GetXbins()));
      for(Int_t i = 0; i < X.GetSize(); i++) X[i] = Scale(X[i]);
      a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
    }
  else
    {
      // an axis with fix bins
      // note: we modify Xmin and Xmax only, hence the "Scale" function
      // must be linear (and Xmax must remain greater than Xmin)
      a->Set( a->GetNbins(),
              Scale(a->GetXmin()), // new Xmin
              Scale(a->GetXmax()) ); // new Xmax
    }
  return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetXaxis(), Scale);
  return;
}

void ScaleYaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetYaxis(), Scale);
  return;
}

void ScaleZaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetZaxis(), Scale);
  return;
}



void XtrYtrZoom()
{    //Importation du tree donnée test beam
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree = (TTree*)input->Get("tree");
    
    //création canvas double histogramme pour les deux DUT
    TCanvas *c3 = new TCanvas();
    c3->Divide(2,1);
    c3->SetCanvasSize(1200, 600);
    c3->SetWindowSize(1260, 630);
    gStyle-> SetStatX(0.8);
    gStyle->SetPalette(55);
    //gStyle->SetPalette(kDarkBodyRadiator);
    //drakbodyradiatior is prettier
    
    
    // coupure
    
    TCut pulse = "(pulseHeight[2] > 7.  && pulseHeight[2] < 770) ";
    TCut time0 = "((timeAtMax[2]-timeAtMax[0]) > 3000 && (timeAtMax[2]-timeAtMax[0]) < 5000)";
    TCut time1 = "((timeAtMax[2]-timeAtMax[1]) > 2800 && (timeAtMax[2]-timeAtMax[1]) < 4800)";

    
    //Création histogramme DUT[0] et mise en forme
    c3->cd(1);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH2F *h = new TH2F("h","Xtrace vs YTrace DUT[0]",60,515,620,60,53,158);
    h-> SetYTitle("y in mm");
    h-> SetXTitle("x in mm");
    h-> SetTitleSize(0.042,"x in mm");
    h-> SetTitleSize(0.042,"y in mm");
    h-> SetZTitle("Events");
    h-> SetStats();
    h-> SetMinimum(5);
    h-> GetZaxis()->SetNdivisions(-507,kTRUE);
    h-> GetZaxis()->SetMaxDigits(3);
    tree->Draw("Ytr[0]:Xtr[0]>>h",pulse && time0 ,"Colz");
    ScaleXaxis(h,ScaleX);
    ScaleYaxis(h,ScaleY);
    
    
    
    //Création histogramme DUT[1] et mise en forme
    c3->cd(2);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH2F *h1 = new TH2F("h1","Xtrace vs YTrace DUT[1]",60,525,645,60,90,210);
    h1-> SetYTitle("y in mm");
    h1-> SetXTitle("x in mm");
    h1-> SetTitleSize(0.042,"x in mm");
    h1-> SetTitleSize(0.042,"y in mm");
    h1-> SetZTitle("Events");
    h1-> SetStats();
    h1-> SetMinimum(5);
    h1-> GetZaxis()->SetNdivisions(-507,kTRUE);
    h1-> GetZaxis()->SetMaxDigits(3);
    h1-> GetZaxis()->SetTitleOffset(1);
    
    tree->Draw("Ytr[1]:Xtr[1]>>h1",pulse && time1,"Colz");
    ScaleXaxis(h1,ScaleX);
    ScaleYaxis(h1,ScaleY);
    
    c3->Print("fig/XtrYtrZoom.png");
    
    /*c3->cd();
    TPad *newpad=new TPad("newpad","a transparent pad",0,0,1,1);
    newpad->SetFillStyle(4000);
    newpad->Draw();
    newpad->cd();
    TPaveText *title = new TPaveText(0.29,0.15,0.52,0.4) ;
    title -> AddText(" pulseHeight[2] > 7. ");
    title -> AddText(" pulseHeight[2] < 770 ");
    title -> AddText(" (timeAtMax[2]-timeAtMax[DUT]) > 3000 ");
    title -> AddText("(timeAtMax[2]-timeAtMax[DUT]) < 5000)");
    title->SetFillColor(16);
    title->SetTextFont(70);
    title->SetTextSize(0.023);
    title->Draw();*/
    
    // On a un millieu a y =105.9 x=568.7 DUT0
    //On a un millieu a y =150.2 x=585 DUT0
}
