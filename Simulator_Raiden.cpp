#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TMath.h"
#include "TF1.h"
#include <thread>


using namespace std;


double logXperX(double* x, double* p)
{
	return p[1] * TMath::Log(x[0]) * TMath::Power(x[0], p[0]);
}


void Simulator_Raiden()
{
	gStyle->SetOptStat(false);
	gRandom->SetSeed(0);

	std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	// the number of artifacts to get
	int simNum = 10000;
	int artifactNum = 600; // 4.7925 per day (150 ~ month)

	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 100000.;
	double maxDamage = 300000.;
	
    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);
	// simulator->SetNumThread(8); default : max
	simulator->SetSeeLastArtifact(false);
	simulator->SetSeeTimeConsumption(true);
	simulator->SetScoreIndexMode(JANGDDOL);
	simulator->SetBundleNum(5);

	TH2D* VisualHistogram = simulator->RunSimulationMultiThreads(simNum, artifactNum, binNum, minDamage, maxDamage);
	// TH2D* VisualHistogram = simulator->RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage, "");

	int numContent = 0;
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			numContent += VisualHistogram->GetBinContent(i + 1, j + 1);
		}
	}
	VisualHistogram->SetEntries(numContent);

	// Set TGraph for appendableRate
	vector<double> appendableRate = simulator->GetAppendableRate();
	TGraph* gAR = new TGraph();
	for (int i = 0; i < appendableRate.size(); i++)
	{
		gAR->SetPoint(i, (double)(i + 1), appendableRate[i]);
	}

	// fitting
	TF1* fitFunc1 = new TF1("fit", logXperX, 100, artifactNum, 2);
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
	
	VisualHistogram->SetName("Visual");
	VisualHistogram->GetXaxis()->SetTitle("The number of artifact to get");
	VisualHistogram->GetYaxis()->SetTitle("Damage");
	VisualHistogram->GetZaxis()->SetTitle("Count");
	VisualHistogram->Draw("COL4Z");

	TFile* file = new TFile("GenshinArtifactSimulator_Raiden.root", "recreate");
	VisualHistogram->Write();
	file->Close();
}


int main()
{
	Simulator_Raiden();
	return 0;
}