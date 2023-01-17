#include "top.hh"
// #include "TH2D.h"
// #include "TCanvas.h"
// #include "TFile.h"
#include <thread>


using namespace std;


void Simulator_Raiden()
{
	gStyle->SetOptStat(false);
	gRandom->SetSeed(0);

	EngulfingLightning* weapon = new EngulfingLightning();
	
    Raiden* simChar = new Raiden(weapon);

    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();
    simChar->SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	// the number of artifacts to get
	int simNum = 1;
	int artifactNum = 600; // 4.7925 per day (150 ~ month)

	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 0.;
	double maxDamage = 8000.;
	
    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);
	// simulator->SetNumThread(8); default : max
	simulator->SetSeeLastArtifact(true);
	simulator->SetSeeTimeConsumption(true);

	// TH2D* VisualHistogram = simulator->RunSimulationMultiThreads(simNum, artifactNum, binNum, minDamage, maxDamage);
	TH2D* VisualHistogram = simulator->RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage);

	int numContent = 0;
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			numContent += VisualHistogram->GetBinContent(i + 1, j + 1);
		}
	}
	VisualHistogram->SetEntries(numContent);

	TCanvas* can1 = new TCanvas("canvas", "canvas", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);
	
	VisualHistogram->GetXaxis()->SetTitle("The number of artifact to get");
	VisualHistogram->GetYaxis()->SetTitle("Damage");
	VisualHistogram->GetZaxis()->SetTitle("Count");
	VisualHistogram->Draw("COL4Z");

	TFile* file = new TFile("GenshinArtifactSimulator_Multithread.root", "recreate");
	VisualHistogram->Write();
	file->Close();
}