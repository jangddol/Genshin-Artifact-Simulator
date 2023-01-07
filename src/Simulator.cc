#include "../header/Simulator.hh"
#include "../header/top.hh"

#include <iostream>
#include <cstdio>
#include <std::vector>
#include <algorithm>
#include <ctime>


struct SuperArtifactList
{
	std::vector<ArtFlower*> flower;
	std::vector<ArtFeather*> feather;
	std::vector<ArtClock*> clock;
	std::vector<ArtCup*> cup;
	std::vector<ArtCrown*> crown;
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
	// 20% È®·ü·Î ºÎ??§¸¦ °áÁ¤.
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
			std::cout << "Error : gRandom occur some errors at GenerateRandomArtifact : temp = " << temp << std::endl;
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


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtFlower*> flowerList)
{
	std::vector<Artifact*> returnVector(flowerList.size());
	for (int i = 0; i < flowerList.size() ; i++) returnVector[i] = (Artifact*)flowerList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtFeather*> featherList)
{
	std::vector<Artifact*> returnVector(featherList.size());
	for (int i = 0; i < featherList.size() ; i++) returnVector[i] = (Artifact*)featherList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtClock*> clockList)
{
	std::vector<Artifact*> returnVector(clockList.size());
	for (int i = 0; i < clockList.size() ; i++) returnVector[i] = (Artifact*)clockList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtCup*> cupList)
{
	std::vector<Artifact*> returnVector(cupList.size());
	for (int i = 0; i < cupList.size() ; i++) returnVector[i] = (Artifact*)cupList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtCrown*> crownList)
{
	std::vector<Artifact*> returnVector(crownList.size());
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
	else std::cout << "Error : gennedArtifact has wrong Type at EraseSuperArtifactList : numType = " << numType << std::endl;
}


bool CheckWhetherAppendAndDelete(Character* character, Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
	int mainType = gennedArtifact->GetMainType();

	if (CheckEffectiveOption(character, mainType) == false && (mainType != 3 && mainType != 6)) return false; 

	int numType = gennedArtifact->GetType();

	std::vector<Artifact*> selectedList;

	if      (numType == 1) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.flower);
	else if (numType == 2) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.feather);
	else if (numType == 3) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.clock);
	else if (numType == 4) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.cup);
	else if (numType == 5) selectedList = Convert2VecArtifactPointer(ArtifactSuperList.crown);
	else
	{
		std::cout << "Error : gennedArtifact has wrong Type at CheckWhetherAppendAndDelete : numType = " << numType << std::endl;
	}
	
	int effectiveList[10] = { 0 };
	int effListSize;
	MakeEffectiveOptionList(effectiveList, effListSize, character);
	
	/*
	std::cout << "================================================================================" << std::endl;
	std::cout << "Main" << "\t|\t" << "SubOpt" << std::endl;
	std::cout << STATSTRING[mainType] << "\t|\t" << "CR" << "\t"
										<< "CB" << "\t"
										<< STATSTRING[2] << "\t"
										<< STATSTRING[3] << "\t"
										<< "EC" << "\t"
										<< STATSTRING[5] << "\t"
										<< STATSTRING[6] << "\t"
										<< "EM" << "\t"
										<< STATSTRING[8] << "\t"
										<< STATSTRING[9] << std::endl;
	std::cout << "gennedArtifact" << std::endl;
	std::cout << gennedArtifact->GetMainStat().GetOption(mainType) << "\t|\t" 
		<< gennedArtifact->GetSubStat().GetOption(0) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(1) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(2) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(3) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(4) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(5) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(6) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(7) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(8) << "\t"
		<< gennedArtifact->GetSubStat().GetOption(9) << std::endl;
	std::cout << "listArtifact" << std::endl;
	for (int i = 0; i < selectedList.size(); i++)
	{
		std::cout << selectedList[i]->GetMainStat().GetOption(mainType) << "\t|\t" 
			<< selectedList[i]->GetSubStat().GetOption(0) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(1) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(2) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(3) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(4) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(5) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(6) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(7) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(8) << "\t"
			<< selectedList[i]->GetSubStat().GetOption(9) << std::endl;
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
				delete selectedList[i];
				selectedList.erase(selectedList.begin() + i);
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
		std::cout << "Error : gennedArtifact has wrong Type (@ AppendArtifactList)" << std::endl; 
	}
}


double CALLOOPTIMELIST[2] = { 0. };
double CALLOOPSTART, CALLOOPFINISH;
double CalLoopArtifact(Character* character, Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList,
	ArtFlower* &oFlower, ArtFeather* &oFeather, ArtClock* &oClock, ArtCup* &oCup, ArtCrown* &oCrown)
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
		std::cout << "Error : gennedArtifact has wrong Type (@ CalLoopArtifact) : numType = " << numType << std::endl; 
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
						CALLOOPTIMELIST[0] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;
						
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = character->GetDamage();
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[1] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;

						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
							oFlower = loopList.flower[i1];
							oFeather = loopList.feather[i2];
							oClock = loopList.clock[i3];
							oCup = loopList.cup[i4];
							oCrown = loopList.crown[i5];
						}
					}
				}
			}
		}
	}
	return bestDamage;
}


TH2D* RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage
    bool seeLastArtifact, bool seeTimeConsumption)
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

	SuperArtifactList artifactSuperList;

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
			if ((int)beforePercent != (int)percent) std::std::cout << (int)percent << "% end" << std::std::endl;
		}

        if (seeLastArtifact)
        {
            std::cout << i << "-th result =============================================" << std::endl;
            std::cout << "bestDamage : " << bestDamage << std::endl;
            std::cout << "============== Last Artifacts ===============" << std::endl;
            PrintArtifact(*bestFlower);
            PrintArtifact(*bestFeather);
            PrintArtifact(*bestClock);
            PrintArtifact(*bestCup);
            PrintArtifact(*bestCrown);
            std::cout << "============== Character Stat ===============" << std::endl;
            PrintStat(simChar->GetStat());
        }
		
		for (int j = 0; j < artifactSuperList.flower.size(); j++) delete artifactSuperList.flower[j];
		for (int j = 0; j < artifactSuperList.feather.size(); j++) delete artifactSuperList.feather[j];
		for (int j = 0; j < artifactSuperList.clock.size(); j++) delete artifactSuperList.clock[j];
		for (int j = 0; j < artifactSuperList.cup.size(); j++) delete artifactSuperList.cup[j];
		for (int j = 0; j < artifactSuperList.crown.size(); j++) delete artifactSuperList.crown[j];
	}

    if (seeTimeConsumption)
    {
        std::cout << TIMELIST[0] << "s, " << TIMELIST[1] << "s, " << TIMELIST[2] << "s, " << TIMELIST[3] << "s" << std::endl;
        std::cout << CALLOOPTIMELIST[0] << "s, " << CALLOOPTIMELIST[1] << "s" << std::endl;
        std::cout << ARTINITTIMELIST[0] << "s, " << ARTINITTIMELIST[1] << "s, " << ARTINITTIMELIST[2] << "s, "
        << ARTINITTIMELIST[3] << "s, " << ARTINITTIMELIST[4] << "s" << std::endl;
    }

	// Plot Part
	TH2D* VisualHistogram = new TH2D("Visual", "", artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			VisualHistogram->SetBinContent(i + 1, j + 1, N_Histogram[i]->GetBinContent(j + 1));
		}
	}

    return VisualHistogram
}