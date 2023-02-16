#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TRandom.h"
#include <thread>


using namespace std;


void Simulator_Raiden()
{
	gStyle->SetOptStat(false);
	gRandom->SetSeed(0);

	EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	// the number of artifacts to get
	int simNum = 1000;
	int artifactNum = 300; // 4.7925 per day (150 ~ month)

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

	TCanvas* can1 = new TCanvas("canvas", "canvas", 1200, 800);
	can1->SetLeftMargin(0.12);
	can1->SetBottomMargin(0.12);
	can1->SetRightMargin(0.08);
	can1->SetTopMargin(0.05);
	
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