#include "../header/Simulator.hh"
#include "../header/RandomInitialization.hh"
#include "../header/Character.hh"
#include "../header/Artifact.hh"
#include "../header/Skill.hh"
#include "../header/Stat.hh"
#include "../header/Weapon.hh"
#include "../header/Artifact/ArtFlower.hh"
#include "../header/Artifact/ArtFeather.hh"
#include "../header/Artifact/ArtClock.hh"
#include "../header/Artifact/ArtCup.hh"
#include "../header/Artifact/ArtCrown.hh"

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctime>
#include "TH1D.h"
#include "TH2D.h"


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
	if (character->GetEffection(index) > 1.e-5) return true;
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


bool CheckBetterSubOpt(Stat betterOpt, Stat worseOpt, int effectiveList[], int effListSize, Character* character)
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
    // Return false if the effective option check fails and the main type is not 3 or 6
    if (!CheckEffectiveOption(character, gennedArtifact->GetMainType()) && (gennedArtifact->GetMainType() != 3 && gennedArtifact->GetMainType() != 6))
    {
        return false;
    }

    // Get the list of artifacts for the given type
    std::vector<Artifact*> selectedList;
    switch (gennedArtifact->GetType())
    {
        case 1:
            selectedList = Convert2VecArtifactPointer(ArtifactSuperList.flower);
            break;
        case 2:
            selectedList = Convert2VecArtifactPointer(ArtifactSuperList.feather);
            break;
        case 3:
            selectedList = Convert2VecArtifactPointer(ArtifactSuperList.clock);
            break;
        case 4:
            selectedList = Convert2VecArtifactPointer(ArtifactSuperList.cup);
            break;
        case 5:
            selectedList = Convert2VecArtifactPointer(ArtifactSuperList.crown);
            break;
        default:
            std::cout << "Error : gennedArtifact has wrong Type at CheckWhetherAppendAndDelete : numType = " << gennedArtifact->GetType() << std::endl;
    }

    // Make a list of effective options
    int effectiveList[10] = { 0 };
    int effListSize;
    MakeEffectiveOptionList(effectiveList, effListSize, character);

    // Check whether the generated artifact should be appended to or deleted from the list
    bool whetherAppend = true;
    Stat gennedSubOpt = gennedArtifact->GetSubStat();
    for (int i = 0; i < selectedList.size(); i++)
    {
        if (selectedList[i]->GetMainType() == gennedArtifact->GetMainType())
        {
            Stat tempSubOpt = selectedList[i]->GetSubStat();
            if (CheckBetterSubOpt(tempSubOpt, gennedSubOpt, effectiveList, effListSize, character))
            {
                // Existing artifact is better, do not append the generated artifact
                whetherAppend = false;
            }
            else if (CheckBetterSubOpt(gennedSubOpt, tempSubOpt, effectiveList, effListSize, character))
            {
                // Generated artifact is better, delete the existing artifact
                EraseSuperArtifactList(ArtifactSuperList, gennedArtifact->GetType(), i);
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


double Simulator::CalLoopArtifact(Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList,
	ArtFlower* &oFlower, ArtFeather* &oFeather, ArtClock* &oClock, ArtCup* &oCup, ArtCrown* &oCrown)
{
	double CALLOOPSTART, CALLOOPFINISH;
	// Set the generated artifact as the only artifact in the loop list for its type
    SuperArtifactList loopList = ArtifactSuperList;
    switch (gennedArtifact->GetType())
    {
        case 1:
            loopList.flower = { (ArtFlower*)gennedArtifact };
            break;
        case 2:
            loopList.feather = { (ArtFeather*)gennedArtifact };
            break;
        case 3:
            loopList.clock = { (ArtClock*)gennedArtifact };
            break;
        case 4:
            loopList.cup = { (ArtCup*)gennedArtifact };
            break;
        case 5:
            loopList.crown = { (ArtCrown*)gennedArtifact };
            break;
        default:
            std::cout << "Error : gennedArtifact has wrong Type (@ CalLoopArtifact) : numType = " << gennedArtifact->GetType() << std::endl; 
            return 0;
    }

	double tempDamage, bestDamage;
	bestDamage = 0;
	for (int i1 = 0; i1 < loopList.flower.size(); i1++)
	{
		mCharacter->SetArtFlower(loopList.flower[i1]);
		for (int i2 = 0; i2 < loopList.feather.size(); i2++)
		{
			mCharacter->SetArtFeather(loopList.feather[i2]);
			for (int i3 = 0; i3 < loopList.clock.size(); i3++)
			{
				mCharacter->SetArtClock(loopList.clock[i3]);
				for (int i4 = 0; i4 < loopList.cup.size(); i4++)
				{
					mCharacter->SetArtCup(loopList.cup[i4]);
					for (int i5 = 0; i5 < loopList.crown.size(); i5++)
					{
						mCharacter->SetArtCrown(loopList.crown[i5]);
						
						// Initialize the character
						CALLOOPSTART = clock();
						mCharacter->Initialization();
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[0] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;
						
						// Calculate the damage
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = mCharacter->GetDamage();
						CALLOOPFINISH = clock();
						CALLOOPTIMELIST[1] += (double)(CALLOOPFINISH - CALLOOPSTART) / CLOCKS_PER_SEC;

						// Update the best damage and artifact combination if necessary
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


TH2D* Simulator::RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage, 
    bool seeLastArtifact, bool seeTimeConsumption, TString histName)
{
	SuperArtifactList artifactSuperList;

	// Histogram array to store results of each simulation run
    TH1D* N_Histogram[artifactNum];
    for (int i = 0; i < artifactNum; i++)
    {
        N_Histogram[i] = new TH1D(histName + Form(" %d-th trial", i+1), histName + Form(" %d-th trial", i+1), binNum, minDamage, maxDamage);
    }
	
	// Code Execution Time Evaluation
	double TIMELIST[4] = { 0. };
	double start, finish;
    for (int i = 0; i < 2; i++) CALLOOPTIMELIST[i] = 0.;
    // for (int i = 0; i < 5; i++) artInitTimeList[i] = 0.;

	mAppendableRate = {};
	mAppendableRate.reserve(artifactNum);
	mAppendableRate.resize(artifactNum);

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

		// Appendable Rate
		int numAppend = 0;

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
			bool whetherAppend = CheckWhetherAppendAndDelete(mCharacter, gennedArtifact, artifactSuperList);
			finish  = clock();
			TIMELIST[1] += (double)(finish - start) / CLOCKS_PER_SEC;

			if (whetherAppend)
			{				
				start = clock(); 
				double comparedDamage = CalLoopArtifact(gennedArtifact, artifactSuperList,
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

				numAppend++;
			}
			else delete gennedArtifact;

			N_Histogram[j]->Fill(bestDamage);

			mAppendableRate[j] += (double)numAppend;

			double beforePercent = (double)(i * artifactNum + j - 1)/(double)(simNum * artifactNum) * 100.;
			double percent = (double)(i * artifactNum + j)/(double)(simNum * artifactNum) * 100.;
			if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
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
            PrintStat(mCharacter->GetStat());
        }

		for (int j = 0; j < artifactSuperList.flower.size(); j++) delete artifactSuperList.flower[j];
		for (int j = 0; j < artifactSuperList.feather.size(); j++) delete artifactSuperList.feather[j];
		for (int j = 0; j < artifactSuperList.clock.size(); j++) delete artifactSuperList.clock[j];
		for (int j = 0; j < artifactSuperList.cup.size(); j++) delete artifactSuperList.cup[j];
		for (int j = 0; j < artifactSuperList.crown.size(); j++) delete artifactSuperList.crown[j];
	}

	for (int i = 0; i < artifactNum; i++)
	{
		mAppendableRate[i] /= (double)(i + 1);
		mAppendableRate[i] /= (double)simNum;
	}

    if (seeTimeConsumption)
    {
        std::cout << TIMELIST[0] << "s, " << TIMELIST[1] << "s, " << TIMELIST[2] << "s, " << TIMELIST[3] << "s" << std::endl;
        std::cout << CALLOOPTIMELIST[0] << "s, " << CALLOOPTIMELIST[1] << "s" << std::endl;
        // std::cout << artInitTimeList[0] << "s, " << artInitTimeList[1] << "s, " << artInitTimeList[2] << "s, "
        // << artInitTimeList[3] << "s, " << artInitTimeList[4] << "s" << std::endl;
    }

	// TH2D remake Part
	if (histName == "") histName = "Visual";
	TH2D* VisualHistogram = new TH2D(histName, histName, artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < binNum; j++)
		{
			VisualHistogram->SetBinContent(i + 1, j + 1, N_Histogram[i]->GetBinContent(j + 1));
		}
	}

    return VisualHistogram;
}