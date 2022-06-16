void pulseSIPM()
{
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree =(TTree*)input->Get("tree");

    TCanvas *c1 = new TCanvas();
    c1->SetWindowSize(1000, 500);
    TH1F *h = new TH1F("h","Pulse height of SiPM",100,0,50);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    h -> SetLineColor(4);
    h-> SetFillColor(4);
    h-> SetFillStyle(3001);
    h-> SetLineWidth(2);
    h->SetYTitle("Number of event");
    h->SetXTitle("Pulse Height");
    h->SetTitleSize(0.042,"x");
    h->SetTitleSize(0.042,"y");
    h->SetStats();
    
    
    tree->Draw("pulseHeight[2]>>h");
//Il faut donc faire une coupure entre 7 et 770
}

