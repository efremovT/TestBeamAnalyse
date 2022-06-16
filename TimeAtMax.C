void TimeAtMax()
{
    //Importation du tree donnée test beam
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree =(TTree*)input->Get("tree");
    
    //histogramme
    TCanvas *c1 = new TCanvas();
    c1->SetWindowSize(1000, 1000);
    c1->Divide(1,2);
    TCut time0 = "(timeAtMax[2]-timeAtMax[0]) > 3000 && (timeAtMax[2]-timeAtMax[0]) < 5000";
    TCut time1 = "(timeAtMax[2]-timeAtMax[1]) > 2800 && (timeAtMax[2]-timeAtMax[1]) < 4800";
    TCut pulse = "pulseHeight[2]<770 && pulseHeight[2]>7";
    
    c1->cd(1);
    TH1F *h = new TH1F("h","SiPM - FBK_W19 DUT[0] ",100,3000,5000);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
        
    h -> SetLineColor(4);
    h-> SetFillColor(4);
    h-> SetFillStyle(3001);
    h-> SetLineWidth(2);
    h->SetYTitle("Number of event");
    h->SetXTitle("Time in ps");
    h->SetTitleSize(0.042,"x");
    h->SetTitleSize(0.042,"y");
    h->SetStats();
    
    tree->Draw("(timeAtMax[2]-timeAtMax[0])>>h",pulse && time0);
    
    
    c1->cd(2);
    TH1F *h1 = new TH1F("h1","SiPM - IMEv2_W7Q2 DUT[1] ",100,2800,4800);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
        
    h1 -> SetLineColor(2);
    h1-> SetFillColor(2);
    h1-> SetFillStyle(3001);
    h1-> SetLineWidth(2);
    h1->SetYTitle("Number of event");
    h1->SetXTitle("Time in ps");
    h1->SetTitleSize(0.042,"x");
    h1->SetTitleSize(0.042,"y");
    h1->SetStats();
    
    tree->Draw("(timeAtMax[2]-timeAtMax[1])>>h1",pulse && time1);
    
    
    //On applique un cut entre 3000 ps et 5000ps en gros
    //TCut time = "abs(timeAtMax[2]-timeAtMax[1]) > 3000 && abs(timeAtMax[2]-timeAtMax[1]) < 5000"
    
}
