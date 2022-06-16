void XtrYtr()
{    //Importation du tree donnée test beam
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree = (TTree*)input->Get("tree");
    
    //création canvas double histogramme pour les deux DUT
    TCanvas *c3 = new TCanvas();
    c3->Divide(2,1);
    c3->SetCanvasSize(1400, 600);
    c3->SetWindowSize(1430, 630);
    gStyle-> SetStatX(0.8);

    // coupure
    TCut pulse = "(pulseHeight[2] > 7.  && pulseHeight[2] < 770) ";
    TCut time0 = "((timeAtMax[2]-timeAtMax[0]) > 3000 && (timeAtMax[2]-timeAtMax[0]) < 5000)";
    TCut time1 = "((timeAtMax[2]-timeAtMax[1]) > 2800 && (timeAtMax[2]-timeAtMax[1]) < 4800)";
    
    //Création histogramme et mise en forme
    c3->cd(1);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    
    TH2F *h = new TH2F("h","Xtrace vs YTrace DUT[0]",100,0,1000,100,0,1000);
    h-> SetYTitle("y");
    h-> SetXTitle("x");
    h-> SetTitleSize(0.042,"x");
    h-> SetTitleSize(0.042,"y");
    h-> SetStats();
    
    tree->Draw("Xtr[0]:Ytr[0]>>h",pulse && time0,"Colz");
    
    c3->cd(2);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    
    TH2F *h1 = new TH2F("h1","Xtrace vs YTrace DUT[1]",100,0,1000,100,0,1000);
    h1-> SetYTitle("y");
    h1-> SetXTitle("x");
    h1-> SetTitleSize(0.042,"x");
    h1-> SetTitleSize(0.042,"y");
    h1-> SetStats();
    
    tree->Draw("Xtr[1]:Ytr[1]>>h1",pulse && time1,"Colz");
    
    
    
    c3->cd();
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
    title->Draw();
    
}
