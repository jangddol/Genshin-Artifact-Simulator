#include "top.hh"
#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include <thread>
#include "TStyle.h"
#include "TRandom.h"


using namespace std;


/*
// the number of artifacts to get
constexpr int SIMNUM = 1000;
constexpr int artifactNum = 300; // 4.7925 per day (150 ~ month)

// maxDamage, binNum
constexpr int binNum = 100;
constexpr double minDamage = 100000.;
constexpr double maxDamage = 300000.;

TH2D* SimulationWorker(UInt_t workerID, int workerNum)
{
	MemoryOfDust* weapon = new MemoryOfDust();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();
	
    Character* simChar = new Ningguang(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat artSetStat = Stat();
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
	int simNum = SIMNUM / workerNum;
	
    Simulator* simulator = new Simulator();
    simulator->SetCharacter(simChar);

	// Plot Part
	TString histName = Form("Visual-%d", workerID);
	TH2D* VisualHistogram = simulator->RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage,
														false, true, histName);

	// cout << "Appendable Rate : " << simulator->GetAppendableRate() << endl;

	TFile* f = new TFile(Form("MultiThread_%u.root", workerID), "RECREATE");
	VisualHistogram->Write();
	f->Close();

	return VisualHistogram;
}


void Simulator_Multithread()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	cout << std::thread::hardware_concurrency() << endl;

	TH2D* VisualHistogram = new TH2D("VisualHistogram", "", artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);
	std::vector<TH2D> HistogramArray = {};


	// const UInt_t numThreads = 8U;

	// ROOT::EnableThreadSafety();
	// // We define our work item
	// auto workItem = [](UInt_t workerID) {
	// 	TFile* f = new TFile(Form("MultiThread_%u.root", workerID), "RECREATE");
	// 	TH2D* tempHist = SimulationWorker(workerID, numThreads);
	// 	tempHist->Write();
	// 	f->Close();
	// 	return 0;
	// };

	// // Create the pool of threads
	// ROOT::TThreadExecutor pool(numThreads);
	// // Fill the pool with work
	
	// pool.Map(workItem, ROOT::TSeqI(numThreads));

	std::vector<std::thread> threads;
	const int numThreads = 2;
	// Create and launch 8 threads
	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back(SimulationWorker, i, numThreads);
	}

	// Join the threads
	for (auto& thread : threads) {
		thread.join();
	}

	for (int i = 0; i < numThreads; i++)
	{
		TFile* f = new TFile(Form("MultiThread_%u.root", i), "read");
		TH2D readHist = *((TH2D*)f->Get(Form("Visual-%d", i)));
		f->Close();
		remove(Form("MultiThread_%u.root", i));
		HistogramArray.push_back(readHist);
	}


	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			VisualHistogram->SetBinContent(i + 1, j + 1, HistogramArray[0].GetBinContent(i + 1, j + 1));
		}
	}

	for (int i = 1; i < numThreads; i++)
	{
		VisualHistogram->Add(&HistogramArray[i]);
	}


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
*/

void Simulator_Multithread()
{
	// gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	cout << std::thread::hardware_concurrency() << endl;

	MemoryOfDust* weapon = new MemoryOfDust();

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
	int simNum = 5000;
	int artifactNum = 900; // 4.7925 per day (150 ~ month)

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