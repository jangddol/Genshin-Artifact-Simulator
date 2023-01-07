#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"


using namespace std;


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

	Character* simChar = new Kokomi(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);
	cout << "character generated" << endl;
	
	Stat artSetStat = Stat();
    artSetStat.SetZero();
    artSetStat.SetHealBonus(15);
    simChar->SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetResistCut(20.);
    resonanceStat.SetGeoBonus(15.);
    simChar->SetResonanceStat(resonanceStat);

	simChar->MakeEffectionArray();

	// simulation number
	int simNum = 1000;
	
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

	cout << "Appendable Rate : " << simulator->GetAppendableRate() << endl;

	TCanvas* can1 = new TCanvas("canvas", "canvas", 1200, 800);
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