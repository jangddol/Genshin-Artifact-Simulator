#include "top.hh"
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctime>


using namespace std;


struct SuperArtifactList
{
	vector<ArtFlower*> flower;
	vector<ArtFeather*> feather;
	vector<ArtClock*> clock;
	vector<ArtCup*> cup;
	vector<ArtCrown*> crown;
};


Artifact* GenRandArtf_1()
{
	ArtFlower* tempArtf = new ArtFlower();
	tempArtf->Generation();
	Artifact* returnArtf = (Artifact*)tempArtf;
	return returnArtf;
}


Artifact* GenRandArtf_2()
{
	ArtFeather* tempArtf = new ArtFeather();
	tempArtf->Generation();
	Artifact* returnArtf = (Artifact*)tempArtf;
	return returnArtf;
}


Artifact* GenRandArtf_3()
{
	ArtClock* tempArtf = new ArtClock();
	tempArtf->Generation();
	Artifact* returnArtf = (Artifact*)tempArtf;
	return returnArtf;
}


Artifact* GenRandArtf_4()
{
	ArtCup* tempArtf = new ArtCup();
	tempArtf->Generation();
	Artifact* returnArtf = (Artifact*)tempArtf;
	return returnArtf;
}


Artifact* GenRandArtf_5()
{
	ArtCrown* tempArtf = new ArtCrown();
	tempArtf->Generation();
	Artifact* returnArtf = (Artifact*)tempArtf;
	return returnArtf;
}


Artifact* GenerateRandomArtifact()
{
	// 20% 확률로 부위를 결정.
	// int temp = gRandom->Integer(5);
	int temp = uni(rng);

	switch (temp)
	{
	case 0: return GenRandArtf_1(); 
	case 1: return GenRandArtf_2();
	case 2: return GenRandArtf_3();
	case 3: return GenRandArtf_4();
	case 4: return GenRandArtf_5();
	default:
		{
			cout << "Error : gRandom occur some errors at GenerateRandomArtifact : temp = " << temp << endl;
			return new Artifact();
		}
	}
}


bool CheckEffectiveOption(Character* character, int index)
{
	if (character->GetEffection(index) > 0) return true;
	else return false;
}


void MakeEffectiveOptionList(int* oEffectiveList, int& oSize, Character* character)
{
	int j = 0;
	for (int i = 0; i < 10; i++)
	{
		if (CheckEffectiveOption(character, i))
		{
			oEffectiveList[j] = i;
			j += 1;
		}
	}
	oSize = j;
}


bool CheckBetterSubOpt(Stat betterOpt, Stat worseOpt, int effectiveList[], int effListSize)
{
	int betterOptNum = 0;
	for (int j = 0; j < effListSize; j++)
	{
		if (betterOpt.GetOption(effectiveList[j]) >= worseOpt.GetOption(effectiveList[j])) betterOptNum++;
	}
	if (betterOptNum == effListSize) return true;
	else return false;
}


vector<Artifact*> Convert2VecArtifactPointer(vector<ArtFlower*> flowerList)
{
	vector<Artifact*> returnVector(flowerList.size());
	for (int i = 0; i < flowerList.size() ; i++) returnVector[i] = (Artifact*)flowerList[i];
	return returnVector;
}


vector<Artifact*> Convert2VecArtifactPointer(vector<ArtFeather*> featherList)
{
	vector<Artifact*> returnVector(featherList.size());
	for (int i = 0; i < featherList.size() ; i++) returnVector[i] = (Artifact*)featherList[i];
	return returnVector;
}


vector<Artifact*> Convert2VecArtifactPointer(vector<ArtClock*> clockList)
{
	vector<Artifact*> returnVector(clockList.size());
	for (int i = 0; i < clockList.size() ; i++) returnVector[i] = (Artifact*)clockList[i];
	return returnVector;
}


vector<Artifact*> Convert2VecArtifactPointer(vector<ArtCup*> cupList)
{
	vector<Artifact*> returnVector(cupList.size());
	for (int i = 0; i < cupList.size() ; i++) returnVector[i] = (Artifact*)cupList[i];
	return returnVector;
}


vector<Artifact*> Convert2VecArtifactPointer(vector<ArtCrown*> crownList)
{
	vector<Artifact*> returnVector(crownList.size());
	for (int i = 0; i < crownList.size() ; i++) returnVector[i] = (Artifact*)crownList[i];
	return returnVector;
}


void EraseSuperArtifactList(SuperArtifactList& ArtifactSuperList, int numType, int index)
{
	if      (numType == 1) ArtifactSuperList.flower.erase(ArtifactSuperList.flower.begin() + index);
	else if (numType == 2) ArtifactSuperList.feather.erase(ArtifactSuperList.feather.begin() + index);
	else if (numType == 3) ArtifactSuperList.clock.erase(ArtifactSuperList.clock.begin() + index);
	else if (numType == 4) ArtifactSuperList.cup.erase(ArtifactSuperList.cup.begin() + index);
	else if (numType == 5) ArtifactSuperList.crown.erase(ArtifactSuperList.crown.begin() + index);
	else cout << "Error : gennedArtifact has wrong Type at EraseSuperArtifactList : numType = " << numType << endl;
}


bool CheckWhetherAppendAndDelete(Character* character, Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
	int mainType = gennedArtifact->GetMainType();

	if (CheckEffectiveOption(character, mainType) == false && (mainType != 3 && mainType != 6)) return false; 

	int numType = gennedArtifact->GetType();

	vector<Artifact*> selectedList;

	if      (numType == 1) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.flower);
	else if (numType == 2) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.feather);
	else if (numType == 3) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.clock);
	else if (numType == 4) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.cup);
	else if (numType == 5) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.crown);
	else
	{
		cout << "Error : gennedArtifact has wrong Type at CheckWhetherAppendAndDelete : numType = " << numType << endl;
	}
	
	int effectiveList[10] = { 0 };
	int effListSize;
	MakeEffectiveOptionList(effectiveList, effListSize, character);
	
	/*
	cout << "================================================================================" << endl;
	cout << "Main" << "\t|\t" << "SubOpt" << endl;
	cout << STATSTRING[mainType] << "\t|\t" << "CR" << "\t"
										<< "CB" << "\t"
										<< STATSTRING[2] << "\t"
										<< STATSTRING[3] << "\t"
										<< "EC" << "\t"
										<< STATSTRING[5] << "\t"
										<< STATSTRING[6] << "\t"
										<< "EM" << "\t"
										<< STATSTRING[8] << "\t"
										<< STATSTRING[9] << endl;
	cout << "gennedArtifact" << endl;
	cout << gennedArtifact->GetMainStat().GetOption(mainType) << "\t|\t" 
		<< gennedArtifact->GetSubStat().GetOption(0) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(1) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(2) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(3) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(4) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(5) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(6) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(7) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(8) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(9) << endl;
	cout << "listArtifact" << endl;
	for (int i = 0; i < selectedList.size(); i++)
	{
		cout << selectedList[i]->GetMainStat().GetOption(mainType) << "\t|\t" 
			<< selectedList[i]->GetSubStat().GetOption(0) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(1) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(2) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(3) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(4) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(5) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(6) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(7) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(8) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(9) << endl;
	}*/

	Stat gennedSubOpt = gennedArtifact->GetSubStat();

	// artifact not to append to list = there exist better artifact in list
		// better : for every effective option, '>=' is true
	// artifact to delete from the list = worse than new artifact
		// worse : for every effective option, '<=' is true
	
	// check append
	bool existBetterArt = false;
	for (int i = 0; i < selectedList.size(); i++)
	{
		if (selectedList[i]->GetMainType() == mainType)
		{
			Stat tempSubOpt = selectedList[i]->GetSubStat();
			if (!existBetterArt) existBetterArt = CheckBetterSubOpt(tempSubOpt, gennedSubOpt, effectiveList, effListSize);
		}
	}
	bool whetherAppend = !existBetterArt;
	// cout << "Append? : " << whetherAppend << endl;

	// check delete
	for (int i = 0; i < selectedList.size(); i++)
	{
		if (selectedList[i]->GetMainType() == mainType)
		{
			Stat tempSubOpt = selectedList[i]->GetSubStat();
			bool isNeed2Delete = CheckBetterSubOpt(gennedSubOpt, tempSubOpt, effectiveList, effListSize);
			// 1st and 2nd arg is flipped -> CheckWorse
			if (isNeed2Delete)
			{
				EraseSuperArtifactList(ArtifactSuperList, numType, i);
				selectedList.erase(selectedList.begin() + i);
				// cout << i << "-th deleted" << endl;
				i--;
			}
		}
	}
	return whetherAppend;
}


void AppendArtifactList(Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
	int numType = gennedArtifact->GetType();
	

	if      (numType == 1) ArtifactSuperList.flower.push_back((ArtFlower*)gennedArtifact);
	else if (numType == 2) ArtifactSuperList.feather.push_back((ArtFeather*)gennedArtifact);
	else if (numType == 3) ArtifactSuperList.clock.push_back((ArtClock*)gennedArtifact);
	else if (numType == 4) ArtifactSuperList.cup.push_back((ArtCup*)gennedArtifact);
	else if (numType == 5) ArtifactSuperList.crown.push_back((ArtCrown*)gennedArtifact);
	else
	{
		cout << "Error : gennedArtifact has wrong Type (@ AppendArtifactList)" << endl; 
	}
}


double CALLOOPTIMELIST[3] = { 0. };
double CALLOOPSTART, CALLOOPFINISH;
double CalLoopArtifact(Character* character, Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList)
{
	SuperArtifactList loopList = ArtifactSuperList;
	int numType = gennedArtifact->GetType();
	
	if (numType == 1) loopList.flower = { (ArtFlower*)gennedArtifact };
	else if (numType == 2) loopList.feather = { (ArtFeather*)gennedArtifact };
	else if (numType == 3) loopList.clock = { (ArtClock*)gennedArtifact };
	else if (numType == 4) loopList.cup = { (ArtCup*)gennedArtifact };
	else if (numType == 5) loopList.crown = { (ArtCrown*)gennedArtifact };
	else
	{
		cout << "Error : gennedArtifact has wrong Type (@ CalLoopArtifact) : numType = " << numType << endl; 
		return 0.;
	}

	double tempDamage, bestDamage;
	bestDamage = 0;
	for (int i1 = 0; i1 < loopList.flower.size(); i1++)
	{
		character->SetArtFlower(loopList.flower[i1]);
		for (int i2 = 0; i2 < loopList.feather.size(); i2++)
		{
			character->SetArtFeather(loopList.feather[i2]);
			for (int i3 = 0; i3 < loopList.clock.size(); i3++)
			{
				character->SetArtClock(loopList.clock[i3]);
				for (int i4 = 0; i4 < loopList.cup.size(); i4++)
				{
					character->SetArtCup(loopList.cup[i4]);
					for (int i5 = 0; i5 < loopList.crown.size(); i5++)
					{
						character->SetArtCrown(loopList.crown[i5]);
						
						CALLOOPSTART = clock();
						character->Initialization();
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[1] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;
						
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = character->GetDamage();
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[2] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;

						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
						}
					}
				}
			}
		}
	}
	return bestDamage;
}


void Simulator()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	SolarPearl* weapon = new SolarPearl();
	cout << "weapon generated" << endl;

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

	Character* simChar = new Ningguang(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);
	cout << "character generated" << endl;
	
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
	cout << "Effection List : " << simChar->GetEffection(0) << ", "
								<< simChar->GetEffection(1) << ", "
								<< simChar->GetEffection(2) << ", "
								<< simChar->GetEffection(3) << ", "
								<< simChar->GetEffection(4) << ", "
								<< simChar->GetEffection(5) << ", "
								<< simChar->GetEffection(6) << ", "
								<< simChar->GetEffection(7) << ", "
								<< simChar->GetEffection(8) << ", "
								<< simChar->GetEffection(9) << endl;

	SuperArtifactList artifactSuperList;
	artifactSuperList.flower = {};
	artifactSuperList.feather = {};
	artifactSuperList.clock = {};
	artifactSuperList.cup = {};
	artifactSuperList.crown = {};


	// simulation number
	int simNum = 1000;
	
	// the number of artifacts to get
	constexpr int artifactNum = 900; // 5.112 per day (150 ~ month)
	
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
		
		for (int j = 0; j < artifactNum; j++)
		{
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
				double comparedDamage = CalLoopArtifact(simChar, gennedArtifact, artifactSuperList);
				finish  = clock();
				TIMELIST[2] += (double)(finish - start) / CLOCKS_PER_SEC;

				if (comparedDamage >= bestDamage)
				{
					bestDamage = comparedDamage; 
				}
				
				start = finish; 
				AppendArtifactList(gennedArtifact, artifactSuperList);
				finish  = clock();
				TIMELIST[3] += (double)(finish - start) / CLOCKS_PER_SEC;
			}

			N_Histogram[j]->Fill(bestDamage);

			double beforePercent = (double)(i * artifactNum + j - 1)/(double)(simNum * artifactNum) * 100.;
			double percent = (double)(i * artifactNum + j)/(double)(simNum * artifactNum) * 100.;
			if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
		}

		// cout << "============== Last Artifacts ===============" << endl;
		// PrintArtifact(*(simChar->GetArtFlower()));
		// PrintArtifact(*(simChar->GetArtFeather()));
		// PrintArtifact(*(simChar->GetArtClock()));
		// PrintArtifact(*(simChar->GetArtCup()));
		// PrintArtifact(*(simChar->GetArtCrown()));
	}

	cout << TIMELIST[0] << "s, " << TIMELIST[1] << "s, " << TIMELIST[2] << "s, " << TIMELIST[3] << "s" << endl;
	cout << CALLOOPTIMELIST[0] << "s, " << CALLOOPTIMELIST[1] << "s, " << CALLOOPTIMELIST[2] << "s" << endl;
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