#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TF1.h"


using namespace std;


double logXperX(double* x, double* p)
{
	return p[1] * TMath::Log(x[0]) * TMath::Power(x[0], p[0]);
}


void Simulator_Kokomi()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	EverlastingMoonglow* weapon = new EverlastingMoonglow();
	cout << "weapon generated" << endl;

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

	ArtSetStat* artSetStat = new ArtSetStat();
    artSetStat->SetZero();
    artSetStat->SetOption(18, 15);

	Character* simChar = new Kokomi(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);
	cout << "character generated" << endl;
	


    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	int simNum = 10;
	
	// the number of artifacts to get
	constexpr int artifactNum = 900; // 4.7925 per day (150 ~ month)
	
	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 100000.;
	double maxDamage = 180000.;

    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);

	// Plot Part
	TH2D* VisualHistogram = simulator->RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage);

	// Set TGraph for appendableRate
	vector<double> appendableRate = simulator->GetAppendableRate();
	TGraph* gAR = new TGraph();
	for (int i = 0; i < appendableRate.size(); i++)
	{
		gAR->SetPoint(i, (double)(i + 1), appendableRate[i]);
	}

	// fitting
	TF1* fitFunc1 = new TF1("fit2", logXperX, 100, artifactNum, 2);
	fitFunc1->SetParameters(-1, 1);
	fitFunc1->SetParNames("Power", "Amplitude");
	gAR->Fit(fitFunc1);

	// Plot part
	TCanvas* can1 = new TCanvas("appendableRate", "appendableRate", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

	TH1D* hFrame = new TH1D("hFrame", "", 100, 0, artifactNum);
	hFrame->GetXaxis()->SetTitle("artifact number");
	hFrame->GetYaxis()->SetTitle("appendable Rate");
	hFrame->Draw();
	gAR->Draw("lSAME");
	fitFunc1->SetLineColor(3);
	fitFunc1->Draw("lSAME");

	TCanvas* can2 = new TCanvas("canvas", "canvas", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

	VisualHistogram->GetXaxis()->SetTitle("The number of artifact to get");
	VisualHistogram->GetYaxis()->SetTitle("Damage");
	VisualHistogram->GetZaxis()->SetTitle("Count");
	VisualHistogram->Draw("COL4Z");
	
	TFile* file = new TFile("GenshinArtifactSimulator_Kokomi.root", "recreate");
	VisualHistogram->Write();
	file->Close();
}