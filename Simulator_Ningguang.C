#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TF1.h"
#include "TStyle.h"
#include "TRandom.h"


using namespace std;


double logXperX(double* x, double* p)
{
	return p[1] * TMath::Log(x[0]) * TMath::Power(x[0], p[0]);
}


void Simulator_Ningguang()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	MemoryOfDust* weapon = new MemoryOfDust();

	ArtFlower* artinit1 = new ArtFlower();
	artinit1->SetMainType(6);
	ArtFeather* artinit2 = new ArtFeather();
	artinit2->SetMainType(3);
	ArtClock* artinit3 = new ArtClock();
	artinit3->SetMainType(2);
	ArtCup* artinit4 = new ArtCup();
	artinit4->SetMainType(15);
	ArtCrown* artinit5 = new ArtCrown();
	artinit5->SetMainType(1);
	
	ArtSetStat* artSetStat = new ArtSetStat();
    artSetStat->SetZero();
    artSetStat->SetOption(2, 18.);
    artSetStat->SetOption(23, 20.);

    Character* simChar = new Ningguang(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetResistCut(20.);
    resonanceStat.SetGeoBonus(15.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();
	simChar->MakeScoreFunction();

	// simulation number
	int simNum = 10000;
	
	// the number of artifacts to get
	constexpr int artifactNum = 300; // 4.7925 per day (150 ~ month)
	
	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 100000.;
	double maxDamage = 300000.;

    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);
	// simulator->SetNumThread(8); default : max
	simulator->SetSeeLastArtifact(false);
	simulator->SetSeeTimeConsumption(true);

	TH2D* VisualHistogram = simulator->RunSimulationMultiThreads(simNum, artifactNum, binNum, minDamage, maxDamage);
	// TH2D* VisualHistogram = simulator->RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage);

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


	TCanvas* can2 = new TCanvas("Distribution", "Distribution", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

	VisualHistogram->GetXaxis()->SetTitle("The number of artifact to get");
	VisualHistogram->GetYaxis()->SetTitle("Damage");
	VisualHistogram->GetZaxis()->SetTitle("Count");
	VisualHistogram->Draw("COL4Z");
	
	TFile* file = new TFile("GenshinArtifactSimulator_Ningguang.root", "recreate");
	VisualHistogram->Write();
	file->Close();

	cout << "28 score : " << simChar->GetScoreFunction(28) << endl;
}