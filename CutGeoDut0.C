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



void CutGeoDut0()
{    //Importation du tree donnée test beam
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree = (TTree*)input->Get("tree");
    
    //création canvas double histogramme pour les deux DUT
    TCanvas *c3 = new TCanvas("c3","test");
    c3->Divide(3,2);
    c3->SetCanvasSize(1600, 900);
    c3->SetWindowSize(1630, 930);
    gStyle-> SetStatX(0.8);
    gStyle->SetPalette(55);
    gStyle->SetOptFit(1111);  
    gStyle->SetOptStat(1111);
    //gStyle->SetPalette(kDarkBodyRadiator);
    //drakbodyradiatior is prettier
    
    
    // coupure
    
    TCut pulse = "(pulseHeight[2] > 7.  && pulseHeight[2] < 770) ";
    TCut time0 = "((timeAtMax[2]-timeAtMax[0]) > 3000 && (timeAtMax[2]-timeAtMax[0]) < 5000)";
    TCut posx1 = "Xtr[0]*18.5*1e-3 <= (10.52+0.5) &&  Xtr[0]*18.5*1e-3 >= (10.52-0.5)";
    TCut posy1 = "Ytr[0]*18.5*1e-3 <= (1.967+0.5) &&  Ytr[0]*18.5*1e-3 >= (1.967-0.5)";
    TCut posx08 = "Xtr[0]*18.5*1e-3 <= (10.52+0.4) &&  Xtr[0]*18.5*1e-3 >= (10.52-0.4)";
    TCut posy08 = "Ytr[0]*18.5*1e-3 <= (1.967+0.4) &&  Ytr[0]*18.5*1e-3 >= (1.967-0.4)";
    TCut posx05 = "Xtr[0]*18.5*1e-3 <= (10.52+0.25) &&  Xtr[0]*18.5*1e-3 >= (10.52-0.25)";
    TCut posy05 = "Ytr[0]*18.5*1e-3 <= (1.967+0.25) &&  Ytr[0]*18.5*1e-3 >= (1.967-0.25)";
    
    //Q<2
    
    c3->cd(1);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH1F *h = new TH1F("h","charge of FBK_W19 DUT[0] 1mm cut ",100,-5,50);
    h -> SetLineColor(2);
    h-> SetFillColor(2);
    h-> SetFillStyle(3001);
    h-> SetLineWidth(2);
    h->SetYTitle("Number of event");
    h->SetXTitle("Charge in fC");
    h->SetTitleSize(0.042,"x");
    h->SetTitleSize(0.042,"y");
    h->SetStats();
    
    tree->Draw("charge[0]>>h",pulse && time0 && posx1 && posy1);
    
    h-> Fit("landau","R","",2,50);
    h->GetFunction("landau")->SetLineColor(1);
    h->GetFunction("landau")->SetLineWidth(3);
    h->GetFunction("landau")->SetLineStyle(10);
    
    c3->cd(4);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH2F *h1 = new TH2F("h1","DUT[0] 1mm cut",100,465,670,100,-3,208);
    h1-> SetYTitle("y in mm");
    h1-> SetXTitle("x in mm");
    h1-> SetTitleSize(0.042,"x in mm");
    h1-> SetTitleSize(0.042,"y in mm");
    h1-> SetZTitle("Events");
    h1-> SetStats();
    h1-> SetMinimum(2);
    h1-> GetZaxis()->SetNdivisions(-507,kTRUE);
    h1-> GetZaxis()->SetMaxDigits(3);
    
    tree->Draw("Ytr[0]:Xtr[0]>>h1",pulse && time0 && posx1 && posy1,"Colz");
    ScaleXaxis(h1,ScaleX);
    ScaleYaxis(h1,ScaleY);
    
    //Q<3
    
    c3->cd(2);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH1F *h2 = new TH1F("h2","charge of FBK_W19 DUT[0] 0.8mm cut ",100,-5,50);
    h2 -> SetLineColor(2);
    h2-> SetFillColor(2);
    h2-> SetFillStyle(3001);
    h2-> SetLineWidth(2);
    h2->SetYTitle("Number of event");
    h2->SetXTitle("Charge in fC");
    h2->SetTitleSize(0.042,"x");
    h2->SetTitleSize(0.042,"y");
    h2->SetStats();
    
    tree->Draw("charge[0]>>h2",pulse && time0 && posx08 && posy08);
    
    h2-> Fit("landau","R","",2,50);
    h2->GetFunction("landau")->SetLineColor(1);
    h2->GetFunction("landau")->SetLineWidth(3);
    h2->GetFunction("landau")->SetLineStyle(10);
    
    c3->cd(5);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH2F *h3 = new TH2F("h3","DUT[0] 0.8mm cut",100,465,670,100,-3,208);
    h3-> SetYTitle("y in mm");
    h3-> SetXTitle("x in mm");
    h3-> SetTitleSize(0.042,"x in mm");
    h3-> SetTitleSize(0.042,"y in mm");
    h3-> SetZTitle("Events");
    h3-> SetStats();
    h3-> SetMinimum(2);
    h3-> GetZaxis()->SetNdivisions(-507,kTRUE);
    h3-> GetZaxis()->SetMaxDigits(3);
    
    tree->Draw("Ytr[0]:Xtr[0]>>h3",pulse && time0 && posx08 && posy08,"Colz");
    ScaleXaxis(h3,ScaleX);
    ScaleYaxis(h3,ScaleY);
    
    
    c3->cd(3);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH1F *h4 = new TH1F("h4","charge of FBK_W19 DUT[0] 0.5mm cut ",100,-5,50);
    h4 -> SetLineColor(2);
    h4-> SetFillColor(2);
    h4-> SetFillStyle(3001);
    h4-> SetLineWidth(2);
    h4->SetYTitle("Number of event");
    h4->SetXTitle("Charge in fC");
    h4->SetTitleSize(0.042,"x");
    h4->SetTitleSize(0.042,"y");
    h4->SetStats();
    
    tree->Draw("charge[0]>>h4",pulse && time0 && posx05 && posy05);
    
    h4-> Fit("landau","R","",2,50);
    h4->GetFunction("landau")->SetLineColor(1);
    h4->GetFunction("landau")->SetLineWidth(3);
    h4->GetFunction("landau")->SetLineStyle(10);
    
    c3->cd(6);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH2F *h5 = new TH2F("h5","DUT[0] 0.5mm cut",100,465,670,100,-3,208);
    h5-> SetYTitle("y in mm");
    h5-> SetXTitle("x in mm");
    h5-> SetTitleSize(0.042,"x in mm");
    h5-> SetTitleSize(0.042,"y in mm");
    h5-> SetZTitle("Events");
    h5-> SetStats();
    h5-> SetMinimum(2);
    h5-> GetZaxis()->SetNdivisions(-507,kTRUE);
    h5-> GetZaxis()->SetMaxDigits(3);
    
    tree->Draw("Ytr[0]:Xtr[0]>>h5",pulse && time0 && posx05 && posy05,"Colz");
    ScaleXaxis(h5,ScaleX);
    ScaleYaxis(h5,ScaleY);
   
    c3->cd();
    TPad *newpad=new TPad("newpad","a transparent pad",0,0,1,1);
    newpad->SetFillStyle(4000);
    newpad->Draw();
    newpad->cd();

    TLatex *tex = new TLatex(0.4,0.5,"All data is cut on SiPM Pulse height and TimeAtMax");
    tex->SetNDC();
    tex->SetTextSize(0.02);
    tex->Draw();
    
    c3->Print("fig/CutGeoDUT0.png");
    

    
    // On a un millieu a y =105.9 x=568.7 DUT0
    //On a un millieu a y =150.2 x=585 DUT0
}
