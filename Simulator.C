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


void MakeEffectiveOptionList(int* oEffectiveList, int oSize, Character* character)
{
	int j = 0;
	for (int i = 0; i < 10; i++)
	{
		if (CheckEffectiveOption(character, i) == true)
		{
			oEffectiveList[j] = i;
			j += 1;
		}
	}
	oSize = j + 1;
}


void Convert_pArtFlowerList2pArtifact(vector<Artifact*>& selectedList, SuperArtifactList& ArtifactSuperList)
{
	int length = ArtifactSuperList.flower.size();
	for(int i = 0; i < length; i++) selectedList.push_back((Artifact*)ArtifactSuperList.flower[i]);
}


void Convert_pArtFeatherList2pArtifact(vector<Artifact*>& selectedList, SuperArtifactList& ArtifactSuperList)
{
	int length = ArtifactSuperList.feather.size();
	for(int i = 0; i < length; i++) selectedList.push_back((Artifact*)ArtifactSuperList.feather[i]);
}


void Convert_pArtClockList2pArtifact(vector<Artifact*>& selectedList, SuperArtifactList& ArtifactSuperList)
{
	int length = ArtifactSuperList.clock.size();
	for(int i = 0; i < length; i++) selectedList.push_back((Artifact*)ArtifactSuperList.clock[i]);
}


void Convert_pArtCupList2pArtifact(vector<Artifact*>& selectedList, SuperArtifactList& ArtifactSuperList)
{
	int length = ArtifactSuperList.cup.size();
	for(int i = 0; i < length; i++) selectedList.push_back((Artifact*)ArtifactSuperList.cup[i]);
}


void Convert_pArtCrownList2pArtifact(vector<Artifact*>& selectedList, SuperArtifactList& ArtifactSuperList)
{
	int length = ArtifactSuperList.crown.size();
	for(int i = 0; i < length; i++) selectedList.push_back((Artifact*)ArtifactSuperList.crown[i]);
}


bool CheckWhetherAppend(Character* character, Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList)
{
	int numType = gennedArtifact->GetType();
	vector<Artifact*> selectedList = {};

	if (numType == 1) Convert_pArtFlowerList2pArtifact(selectedList, ArtifactSuperList);
	else if (numType == 2) Convert_pArtFeatherList2pArtifact(selectedList, ArtifactSuperList);
	else if (numType == 3) Convert_pArtClockList2pArtifact(selectedList, ArtifactSuperList);
	else if (numType == 4) Convert_pArtCupList2pArtifact(selectedList, ArtifactSuperList);
	else if (numType == 5) Convert_pArtCrownList2pArtifact(selectedList, ArtifactSuperList);
	else
	{
		cout << "Error : gennedArtifact has wrong Type at CheckWhetherAppend : numType = " << numType << endl;
		return false;
	}

	/*
	switch (numType)
	{
	case 1: Convert_pArtFlowerList2pArtifact(selectedList, ArtifactSuperList);
	case 2: Convert_pArtFeatherList2pArtifact(selectedList, ArtifactSuperList);
	case 3: Convert_pArtClockList2pArtifact(selectedList, ArtifactSuperList);
	case 4: Convert_pArtCupList2pArtifact(selectedList, ArtifactSuperList);
	case 5: Convert_pArtCrownList2pArtifact(selectedList, ArtifactSuperList);
	default:
		{
			cout << "Error : gennedArtifact has wrong Type at CheckWhetherAppend : numType = " << numType << endl;
			return false;
		}
	}*/

	int mainType = gennedArtifact->GetMainType();

	if (CheckEffectiveOption(character, mainType) == false && (mainType != 3 && mainType != 6)) return false; 

	int effectiveList[10] = { 0 };
	int effListSize;
	MakeEffectiveOptionList(effectiveList, effListSize, character);

	Stat gennedSubOpt = gennedArtifact->GetSubStat();
	bool returnBool = true;
	for (int i = 0; i < selectedList.size(); i++)
	{
		Stat tempSubOpt = selectedList[i]->GetSubStat();
		for (int j = 0; j < effListSize; j++)
		{
			if (tempSubOpt.GetOption(effectiveList[j]) < gennedSubOpt.GetOption(effectiveList[j]))
			{
				returnBool = false;
				break;
			}
		}
		if (!returnBool) break;
	}
	return returnBool;
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

	/*
	switch (numType)
	{
	case 1: loopList.flower = { (ArtFlower*)gennedArtifact };
	case 2: loopList.feather = { (ArtFeather*)gennedArtifact };
	case 3: loopList.clock = { (ArtClock*)gennedArtifact };
	case 4: loopList.cup = { (ArtCup*)gennedArtifact };
	case 5: loopList.crown = { (ArtCrown*)gennedArtifact };
	default:
		{
			cout << "Error : gennedArtifact has wrong Type (@ CalLoopArtifact) : numType = " << numType << endl; 
			return 0.;
		}
	}*/

	double tempDamage, bestDamage;
	bestDamage = 0;
	for (int i1 = 0; i1 < loopList.flower.size(); i1++)
	{
		ArtFlower* tempArtf1 = loopList.flower[i1];
		for (int i2 = 0; i2 < loopList.feather.size(); i2++)
		{
			ArtFeather* tempArtf2 = loopList.feather[i2];
			for (int i3 = 0; i3 < loopList.clock.size(); i3++)
			{
				ArtClock* tempArtf3 = loopList.clock[i3];
				for (int i4 = 0; i4 < loopList.cup.size(); i4++)
				{
					ArtCup* tempArtf4 = loopList.cup[i4];
					for (int i5 = 0; i5 < loopList.crown.size(); i5++)
					{
						ArtCrown* tempArtf5 = loopList.crown[i5];
						
						CALLOOPSTART = clock();
						character->SetArtifact( tempArtf1, tempArtf2, tempArtf3, tempArtf4, tempArtf5);
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[0] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;

						CALLOOPSTART = CALLOOPFINISH;
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


void AppendArtifactList(Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
	int numType = gennedArtifact->GetType();
	

	if (numType == 1) ArtifactSuperList.flower.push_back((ArtFlower*)gennedArtifact);
	else if (numType == 2) ArtifactSuperList.feather.push_back((ArtFeather*)gennedArtifact);
	else if (numType == 3) ArtifactSuperList.clock.push_back((ArtClock*)gennedArtifact);
	else if (numType == 4) ArtifactSuperList.cup.push_back((ArtCup*)gennedArtifact);
	else if (numType == 5) ArtifactSuperList.crown.push_back((ArtCrown*)gennedArtifact);
	else
	{
		cout << "Error : gennedArtifact has wrong Type (@ AppendArtifactList)" << endl; 
	}
	
	/*
	switch (numType)
	{
	case 1: ArtifactSuperList.flower.push_back((ArtFlower*)gennedArtifact);
	case 2: ArtifactSuperList.feather.push_back((ArtFeather*)gennedArtifact);
	case 3: ArtifactSuperList.clock.push_back((ArtClock*)gennedArtifact);
	case 4: ArtifactSuperList.cup.push_back((ArtCup*)gennedArtifact);
	case 5: ArtifactSuperList.crown.push_back((ArtCrown*)gennedArtifact);
	default:
		{
			cout << "Error : gennedArtifact has wrong Type (@ AppendArtifactList)" << endl; 
		}
	}*/
}


void Simulator()
{
	// gStyle->SetOptStat(kFALSE);
	// gRandom->SetSeed(0);

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
	int simNum = 100;
	
	// the number of artifacts to get
	constexpr int artifactNum = 100; // 5.112 per day
	
	// maxDamage, binNum
	int binNum = 50;
	double maxDamage = 200000.;

	// Nth-histogram
	
	TH1D* N_Histogram[artifactNum];
	for (int i = 0; i < artifactNum; i++)
	{
		N_Histogram[i] = new TH1D(Form("%d-th trial", i+1), "", binNum, 0, maxDamage);
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
			bool whetherAppend = CheckWhetherAppend(simChar, gennedArtifact, artifactSuperList);
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
	}

	cout << TIMELIST[0] << "s, " << TIMELIST[1] << "s, " << TIMELIST[2] << "s, " << TIMELIST[3] << "s" << endl;
	cout << CALLOOPTIMELIST[0] << "s, " << CALLOOPTIMELIST[1] << "s, " << CALLOOPTIMELIST[2] << "s" << endl;
	cout << ARTINITTIMELIST[0] << "s, " << ARTINITTIMELIST[1] << "s, " << ARTINITTIMELIST[2] << "s, "
	<< ARTINITTIMELIST[3] << "s, " << ARTINITTIMELIST[4] << "s" << endl;

	// Plot Part
	TH2D* VisualHistogram = new TH2D("Visual", "", artifactNum, 0, artifactNum, binNum, 0, maxDamage);
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
	
}