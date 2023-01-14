#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include <thread>


using namespace std;


void Simulator_Multithread()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	cout << std::thread::hardware_concurrency() << endl;

	EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();
	
    Ningguang* simChar = new Ningguang(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);

    ArtSetStat artSetStat = ArtSetStat();
    artSetStat.SetZero();
    artSetStat.SetAttackPer(18);
    artSetStat.SetQBonus(20);
    simChar->SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetResistCut(20.);
    resonanceStat.SetGeoBonus(15.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	// the number of artifacts to get
	int simNum = 100;
	int artifactNum = 300; // 4.7925 per day (150 ~ month)

	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 100000.;
	double maxDamage = 300000.;
	
    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);
	simulator->SetNumThread(8);
	simulator->SetSeeLastArtifact(false);
	simulator->SetSeeTimeConsumption(true);

	TH2D* VisualHistogram = simulator->RunSimulationMultiThreads(simNum, artifactNum, binNum, minDamage, maxDamage);

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