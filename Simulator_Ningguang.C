#include "top.hh"


using namespace std;


void Simulator_Ningguang()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	MemoryOfDust* weapon = new MemoryOfDust();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();
	
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

    Character* simChar = new Ningguang(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);

	simChar->MakeEffectionArray();

	SuperArtifactList artifactSuperList;
	artifactSuperList.flower = {};
	artifactSuperList.feather = {};
	artifactSuperList.clock = {};
	artifactSuperList.cup = {};
	artifactSuperList.crown = {};


	// simulation number
	int simNum = 100;
	
	// the number of artifacts to get
	constexpr int artifactNum = 600; // 4.7925 per day (150 ~ month)
	
	// maxDamage, binNum
	int binNum = 100;
	double minDamage = 100000.;
	double maxDamage = 300000.;

	// Nth-histogram
	
	TH1D* N_Histogram[artifactNum];
	for (int i = 0; i < artifactNum; i++)
	{
		N_Histogram[i] = new TH1D(Form("%d-th trial", i+1), "", binNum, minDamage, maxDamage);
	}
	


	// Code Execution Time Evaluation
	double TIMELIST[4] = { 0. };
	double start, finish;

	// Simulation Part
	for (int i = 0; i < simNum; i++)
	{
		double bestDamage = 0;
		
		artifactSuperList.flower = {};
		artifactSuperList.feather = {};
		artifactSuperList.clock = {};
		artifactSuperList.cup = {};
		artifactSuperList.crown = {};

		ArtFlower* bestFlower = new ArtFlower();
		ArtFeather* bestFeather = new ArtFeather();
		ArtClock* bestClock = new ArtClock();
		ArtCup* bestCup = new ArtCup();
		ArtCrown* bestCrown = new ArtCrown();
		
		for (int j = 0; j < artifactNum; j++)
		{
			ArtFlower* bestTryFlower = new ArtFlower();
			ArtFeather* bestTryFeather = new ArtFeather();
			ArtClock* bestTryClock = new ArtClock();
			ArtCup* bestTryCup = new ArtCup();
			ArtCrown* bestTryCrown = new ArtCrown();
			
			start = clock(); 
			Artifact* gennedArtifact = GenerateRandomArtifact();
			finish  = clock();
			TIMELIST[0] += (double)(finish - start) / CLOCKS_PER_SEC;

			start = finish; 
			bool whetherAppend = CheckWhetherAppendAndDelete(simChar, gennedArtifact, artifactSuperList);
			finish  = clock();
			TIMELIST[1] += (double)(finish - start) / CLOCKS_PER_SEC;

			if (whetherAppend)
			{				
				start = clock(); 
				double comparedDamage = CalLoopArtifact(simChar, gennedArtifact, artifactSuperList,
											bestTryFlower, bestTryFeather, bestTryClock, bestTryCup, bestTryCrown);
				finish  = clock();
				TIMELIST[2] += (double)(finish - start) / CLOCKS_PER_SEC;

				if (comparedDamage >= bestDamage)
				{
					bestDamage = comparedDamage;
					bestFlower = bestTryFlower;
					bestFeather = bestTryFeather;
					bestClock = bestTryClock;
					bestCup = bestTryCup;
					bestCrown = bestTryCrown;
				}
				
				start = finish; 
				AppendArtifactList(gennedArtifact, artifactSuperList);
				finish  = clock();
				TIMELIST[3] += (double)(finish - start) / CLOCKS_PER_SEC;
			}
			else delete gennedArtifact;

			N_Histogram[j]->Fill(bestDamage);

			double beforePercent = (double)(i * artifactNum + j - 1)/(double)(simNum * artifactNum) * 100.;
			double percent = (double)(i * artifactNum + j)/(double)(simNum * artifactNum) * 100.;
			if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
		}

		cout << i << "-th result =============================================" << endl;
		cout << "bestDamage : " << bestDamage << endl;
		cout << "============== Last Artifacts ===============" << endl;
		PrintArtifact(*bestFlower);
		PrintArtifact(*bestFeather);
		PrintArtifact(*bestClock);
		PrintArtifact(*bestCup);
		PrintArtifact(*bestCrown);
		cout << "============== Character Stat ===============" << endl;
		PrintStat(simChar->GetStat());
		
		for (int j = 0; j < artifactSuperList.flower.size(); j++) delete artifactSuperList.flower[j];
		for (int j = 0; j < artifactSuperList.feather.size(); j++) delete artifactSuperList.feather[j];
		for (int j = 0; j < artifactSuperList.clock.size(); j++) delete artifactSuperList.clock[j];
		for (int j = 0; j < artifactSuperList.cup.size(); j++) delete artifactSuperList.cup[j];
		for (int j = 0; j < artifactSuperList.crown.size(); j++) delete artifactSuperList.crown[j];
	}

	cout << TIMELIST[0] << "s, " << TIMELIST[1] << "s, " << TIMELIST[2] << "s, " << TIMELIST[3] << "s" << endl;
	cout << CALLOOPTIMELIST[0] << "s, " << CALLOOPTIMELIST[1] << "s" << endl;
	cout << ARTINITTIMELIST[0] << "s, " << ARTINITTIMELIST[1] << "s, " << ARTINITTIMELIST[2] << "s, "
	<< ARTINITTIMELIST[3] << "s, " << ARTINITTIMELIST[4] << "s" << endl;

	// Plot Part
	TH2D* VisualHistogram = new TH2D("Visual", "", artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			VisualHistogram->SetBinContent(i + 1, j + 1, N_Histogram[i]->GetBinContent(j + 1));
		}
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
	
	TFile* file = new TFile("GenshinArtifactSimulator", "recreate");
	VisualHistogram->Write();
	file->Close();

}