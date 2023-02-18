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
	int simNum = 1000;
	int artifactNum = 900; // 4.7925 per day (150 ~ month)

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