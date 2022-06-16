void ChargeDUTCoupure()
{
    //Importation du tree donnée test beam
    TFile *input = new TFile("tree_February2022_201.root");
    TTree *tree = (TTree*)input->Get("tree");
    //création canvas double histogramme pour les deux DUT
    TCanvas *c3 = new TCanvas();
    c3->Divide(2,1);
    c3->SetCanvasSize(1600, 600);
    c3->SetWindowSize(1630, 630);
    
    // coupure
    TCut pulse = "(pulseHeight[2] > 7.  && pulseHeight[2] < 770) ";
    TCut time0 = "((timeAtMax[2]-timeAtMax[0]) > 3000 && (timeAtMax[2]-timeAtMax[0]) < 5000)";
    TCut time1 = "((timeAtMax[2]-timeAtMax[1]) > 2800 && (timeAtMax[2]-timeAtMax[1]) < 4800)";

    //Création histogramme et mise en forme
    c3->cd(1);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH1F *h = new TH1F("h","charge of FBK_W19 DUT[0] with cut on time and pulse height",100,-5,50);
    h -> SetLineColor(2);
    h-> SetFillColor(2);
    h-> SetFillStyle(3001);
    h-> SetLineWidth(2);
    h->SetYTitle("Number of event");
    h->SetXTitle("Charge in fC");
    h->SetTitleSize(0.042,"x");
    h->SetTitleSize(0.042,"y");
    h->SetStats();
    
    tree->Draw("charge[0]>>h",pulse && time0);
    
    h-> Fit("landau");
    h->GetFunction("landau")->SetLineColor(1);
    h->GetFunction("landau")->SetLineWidth(3);
    h->GetFunction("landau")->SetLineStyle(10);
    

    
    
    //Deuxième histogramme
    c3->cd(2);
    gPad-> SetTickx();
    gPad-> SetTicky();
    gPad-> SetGrid();
    
    gPad->SetRightMargin(0.13);
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.13);
    
    TH1F *h1 = new TH1F("h1","charge of IMEv2_W7Q2 DUT[1] with cut on time and pulse height ",      100,-5,50);
    h1-> SetFillColor(4);
    h1-> SetFillStyle(3002);
    h1-> SetLineWidth(2);
    h1->SetYTitle("Number of event");
    h1->SetXTitle("Charge in fC");
    h1->SetTitleSize(0.042,"x");
    h1->SetTitleSize(0.042,"y");
    h1->SetStats();
    
    tree->Draw("charge[1]>>h1",pulse && time1,"");
    
    h1-> Fit("landau","RL","",-5,50);
    h1->GetFunction("landau")->SetLineColor(6);
    h1->GetFunction("landau")->SetLineWidth(3);
    h1->GetFunction("landau")->SetLineStyle(10);
    
        
    // On affiche les cuts
    c3->cd();
    TPad *newpad=new TPad("newpad","a transparent pad",0,0,1,1);
    newpad->SetFillStyle(4000);
    newpad->Draw();
    newpad->cd();
    TPaveText *title = new TPaveText(0.6,0.6,0.93,0.8) ;
    title -> AddText(" pulseHeight[2] > 7. ");
    title -> AddText(" pulseHeight[2] < 770 ");
    title -> AddText(" (timeAtMax[2]-timeAtMax[DUT]) > 0 ");
    title -> AddText("(timeAtMax[2]-timeAtMax[DUT]) < 2000)");
    title->SetFillColor(16);
    title->SetTextFont(70);
    title->SetTextSize(0.025);
    title->Draw();
    
    //Stack des deux histogrammes
    TCanvas *c2 = new TCanvas();
    c2-> SetTickx();
    c2-> SetTicky();
    c2-> SetGridx();
    c2-> SetGridy();
    c2->SetCanvasSize(800, 500);
    c2->SetWindowSize(820,520);
    
    
    THStack *a = new THStack("a","Charge of both DUT");
    TH1F *copy = (TH1F*)h->Clone();
    TH1F *copy1 = (TH1F*)h1->Clone();
    copy->SetStats(0);
    copy1->SetStats(0);
    
    a->Add(copy);
    a->Add(copy1);
    a->Draw("nostack");
    a->GetXaxis()->SetTitle("Charge in fC");
    a->GetYaxis()->SetTitle("Number of event");
    
    
    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg -> AddEntry(h,"FBK_W19","f");
    leg -> AddEntry(h1,"IMEv2_W7Q2","f");
    leg -> Draw();

}
