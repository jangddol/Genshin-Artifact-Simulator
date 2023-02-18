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
#include "../PrintToCoordinates.hh"

#include <assert.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <string>
#include <stdarg.h>
#include "TH1D.h"
#include "TH2D.h"


bool DEBUGMODE = false;


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
	int temp = GetRdUni5();

	switch (temp)
	{
	case 0: return GenRandArtf_1(); 
	case 1: return GenRandArtf_2();
	case 2: return GenRandArtf_3();
	case 3: return GenRandArtf_4();
	case 4: return GenRandArtf_5();
	default:
		{
			assert(0);
			return new ArtFlower();
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


bool CheckWhetherElementIsIn(int element, int List[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (List[i] == element) return true;
    }
    return false;
}


bool CheckBetterSubOpt(Stat betterOpt, Stat worseOpt, int effectiveList[], int effListSize, Character* character)
{
	// CR
	if (CheckWhetherElementIsIn(0, effectiveList, effListSize))
	{
		if (betterOpt.GetOption(0) < worseOpt.GetOption(0)) return false;
	}
	// CB
	if (CheckWhetherElementIsIn(1, effectiveList, effListSize))
	{
		if (betterOpt.GetOption(1) < worseOpt.GetOption(1)) return false;
	}
	// ATK, AP
	if (CheckWhetherElementIsIn(2, effectiveList, effListSize))
	{
		double betterAP = betterOpt.GetOption(2);
		double betterATK = betterOpt.GetOption(3);
		double worseAP = worseOpt.GetOption(2);
		double worseATK = worseOpt.GetOption(3);
		double betterTotATKeffection = betterAP * effectiveList[2] + betterATK * effectiveList[3];
		double worseTotATKeffection = worseAP * effectiveList[2] + worseATK * effectiveList[3];
		if (betterTotATKeffection < worseTotATKeffection) return false;
	}
	// EC
	if (character->GetTargetEC() > 100 + character->GetWeapon()->GetSubStat().GetElementCharge())
	{
		if (betterOpt.GetOption(4) < worseOpt.GetOption(4)) return false;
	}
	// HP, HPP
	if (CheckWhetherElementIsIn(5, effectiveList, effListSize))
	{
		double betterHPP = betterOpt.GetOption(5);
		double betterHP = betterOpt.GetOption(6);
		double worseHPP = worseOpt.GetOption(5);
		double worseHP = worseOpt.GetOption(6);
		double betterTotHPeffection = betterHPP * effectiveList[2] + betterHP * effectiveList[3];
		double worseTotHPeffection = worseHPP * effectiveList[2] + worseHP * effectiveList[3];
		if (betterTotHPeffection < worseTotHPeffection) return false;
	}
	// EM
	if (CheckWhetherElementIsIn(7, effectiveList, effListSize))
	{
		if (betterOpt.GetOption(7) < worseOpt.GetOption(7)) return false;
	}
	// DEF, DP
	if (CheckWhetherElementIsIn(8, effectiveList, effListSize))
	{
		double betterDP = betterOpt.GetOption(2);
		double betterDEF = betterOpt.GetOption(3);
		double worseDP = worseOpt.GetOption(2);
		double worseDEF = worseOpt.GetOption(3);
		double betterTotDEFeffection = betterDP * effectiveList[2] + betterDEF * effectiveList[3];
		double worseTotDEFeffection = worseDP * effectiveList[2] + worseDEF * effectiveList[3];
		if (betterTotDEFeffection < worseTotDEFeffection) return false;
	}
	return true;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtFlower*> flowerList)
{
	std::vector<Artifact*> returnVector(flowerList.size());
	for (std::size_t i = 0; i < flowerList.size() ; i++) returnVector[i] = (Artifact*)flowerList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtFeather*> featherList)
{
	std::vector<Artifact*> returnVector(featherList.size());
	for (std::size_t i = 0; i < featherList.size() ; i++) returnVector[i] = (Artifact*)featherList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtClock*> clockList)
{
	std::vector<Artifact*> returnVector(clockList.size());
	for (std::size_t i = 0; i < clockList.size() ; i++) returnVector[i] = (Artifact*)clockList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtCup*> cupList)
{
	std::vector<Artifact*> returnVector(cupList.size());
	for (std::size_t i = 0; i < cupList.size() ; i++) returnVector[i] = (Artifact*)cupList[i];
	return returnVector;
}


std::vector<Artifact*> Convert2VecArtifactPointer(std::vector<ArtCrown*> crownList)
{
	std::vector<Artifact*> returnVector(crownList.size());
	for (std::size_t i = 0; i < crownList.size() ; i++) returnVector[i] = (Artifact*)crownList[i];
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
    for (std::size_t i = 0; i < selectedList.size(); i++)
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

	if      (numType == 1) ArtifactSuperList.flower.emplace_back((ArtFlower*)gennedArtifact);
	else if (numType == 2) ArtifactSuperList.feather.emplace_back((ArtFeather*)gennedArtifact);
	else if (numType == 3) ArtifactSuperList.clock.emplace_back((ArtClock*)gennedArtifact);
	else if (numType == 4) ArtifactSuperList.cup.emplace_back((ArtCup*)gennedArtifact);
	else if (numType == 5) ArtifactSuperList.crown.emplace_back((ArtCrown*)gennedArtifact);
	else
	{
		std::cout << "Error : gennedArtifact has wrong Type (@ AppendArtifactList)" << std::endl; 
	}
}


double Simulator::CalLoopArtifact_Damage(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](ArtCrown* crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](ArtCup* cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](ArtClock* clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](ArtFeather* feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](ArtFlower* flower) {
                        mCharacter->SetArtFlower(flower);
						
						// Initialize the character
						CALLOOPSTART = std::chrono::system_clock::now();
						mCharacter->Update(true);
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[0] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();
						
						// Calculate the damage
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = mCharacter->GetDamage();
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[1] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();

						// Update the best damage and artifact combination if necessary
						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
							bestTryArtifacts.flower = flower;
							bestTryArtifacts.feather = feather;
							bestTryArtifacts.clock = clock;
							bestTryArtifacts.cup = cup;
							bestTryArtifacts.crown = crown;
						}
                    });
                });
            });
        });
    });
	
	return bestDamage;
}















// std::pair<int, double> GetScoreForEachArtifact(const std::array<double, 19>& scoreEffection, const std::vector<ArtFlower*>& artVector)
// {
// 	double bestScore = 0;
// 	int bestIndex = -1;
// 	for(int i = 0; i < artVector.size(); i++)
// 	{
// 		double tempScore = 0;
// 		const ArtFlower* art = artVector[i];
// 		tempScore += art->GetMainStat().GetOption(art->GetMainType()) * scoreEffection[art->GetMainType()];
// 		Stat tempSubStat = art->GetSubStat();
// 		for (int j = 0; j < 10; j++)
// 		{
// 			tempScore += tempSubStat.GetOption(j) * scoreEffection[j];
// 		}
// 		if (tempScore >= bestScore)
// 		{
// 			bestScore = tempScore;
// 			bestIndex = i;
// 		}
// 	}
// 	return {bestIndex, bestScore};
// }


// std::pair<int, double> GetScoreForEachArtifact(const std::array<double, 19>& scoreEffection, const std::vector<ArtFeather*>& artVector)
// {
// 	double bestScore = 0;
// 	int bestIndex = -1;
// 	for(int i = 0; i < artVector.size(); i++)
// 	{
// 		double tempScore = 0;
// 		const ArtFeather* art = artVector[i];
// 		tempScore += art->GetMainStat().GetOption(art->GetMainType()) * scoreEffection[art->GetMainType()];
// 		Stat tempSubStat = art->GetSubStat();
// 		for (int j = 0; j < 10; j++)
// 		{
// 			tempScore += tempSubStat.GetOption(j) * scoreEffection[j];
// 		}
// 		if (tempScore >= bestScore)
// 		{
// 			bestScore = tempScore;
// 			bestIndex = i;
// 		}
// 	}
// 	return {bestIndex, bestScore};
// }


// std::pair<int, double> GetScoreForEachArtifact(const std::array<double, 19>& scoreEffection, const std::vector<ArtClock*>& artVector)
// {
// 	double bestScore = 0;
// 	int bestIndex = -1;
// 	for(int i = 0; i < artVector.size(); i++)
// 	{
// 		double tempScore = 0;
// 		const ArtClock* art = artVector[i];
// 		tempScore += art->GetMainStat().GetOption(art->GetMainType()) * scoreEffection[art->GetMainType()];
// 		Stat tempSubStat = art->GetSubStat();
// 		for (int j = 0; j < 10; j++)
// 		{
// 			tempScore += tempSubStat.GetOption(j) * scoreEffection[j];
// 		}
// 		if (tempScore >= bestScore)
// 		{
// 			bestScore = tempScore;
// 			bestIndex = i;
// 		}
// 	}
// 	return {bestIndex, bestScore};
// }


// std::pair<int, double> GetScoreForEachArtifact(const std::array<double, 19>& scoreEffection, const std::vector<ArtCup*>& artVector)
// {
// 	double bestScore = 0;
// 	int bestIndex = -1;
// 	for(int i = 0; i < artVector.size(); i++)
// 	{
// 		double tempScore = 0;
// 		const ArtCup* art = artVector[i];
// 		tempScore += art->GetMainStat().GetOption(art->GetMainType()) * scoreEffection[art->GetMainType()];
// 		Stat tempSubStat = art->GetSubStat();
// 		for (int j = 0; j < 10; j++)
// 		{
// 			tempScore += tempSubStat.GetOption(j) * scoreEffection[j];
// 		}
// 		if (tempScore >= bestScore)
// 		{
// 			bestScore = tempScore;
// 			bestIndex = i;
// 		}
// 	}
// 	return {bestIndex, bestScore};
// }


// std::pair<int, double> GetScoreForEachArtifact(const std::array<double, 19>& scoreEffection, const std::vector<ArtCrown*>& artVector)
// {
// 	double bestScore = 0;
// 	int bestIndex = -1;
// 	for(int i = 0; i < artVector.size(); i++)
// 	{
// 		double tempScore = 0;
// 		const ArtCrown* art = artVector[i];
// 		tempScore += art->GetMainStat().GetOption(art->GetMainType()) * scoreEffection[art->GetMainType()];
// 		Stat tempSubStat = art->GetSubStat();
// 		for (int j = 0; j < 10; j++)
// 		{
// 			tempScore += tempSubStat.GetOption(j) * scoreEffection[j];
// 		}
// 		if (tempScore >= bestScore)
// 		{
// 			bestScore = tempScore;
// 			bestIndex = i;
// 		}
// 	}
// 	return {bestIndex, bestScore};
// }


// double Simulator::CalLoopArtifact_jangddolScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
// {
// 	if ((loopList.flower.size() == 0) ||
// 		(loopList.feather.size() == 0) ||
// 		(loopList.clock.size() == 0) ||
// 		(loopList.cup.size() == 0) ||
// 		(loopList.crown.size() == 0)) return 0.;
	
// 	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

// 	Character* tempCharacter = mCharacter->Clone();

// 	tempCharacter->Update();
// 	std::array<double, 19> firstScoreEffection = tempCharacter->GetScoreEffection();
// 	std::pair<int, double> firstBestScore_Flower = GetScoreForEachArtifact(firstScoreEffection, loopList.flower); // descending sorted
// 	std::pair<int, double> firstBestScore_Feather = GetScoreForEachArtifact(firstScoreEffection, loopList.feather);
// 	std::pair<int, double> firstBestScore_Clock = GetScoreForEachArtifact(firstScoreEffection, loopList.clock);
// 	std::pair<int, double> firstBestScore_Cup = GetScoreForEachArtifact(firstScoreEffection, loopList.cup);
// 	std::pair<int, double> firstBestScore_Crown = GetScoreForEachArtifact(firstScoreEffection, loopList.crown);

// 	delete tempCharacter->GetArtFlower();
// 	delete tempCharacter->GetArtFeather();
// 	delete tempCharacter->GetArtClock();
// 	delete tempCharacter->GetArtCup();
// 	delete tempCharacter->GetArtCrown();
// 	tempCharacter->SetArtifact(loopList.flower[firstBestScore_Flower.first],
// 								loopList.feather[firstBestScore_Feather.first],
// 								loopList.clock[firstBestScore_Clock.first],
// 								loopList.cup[firstBestScore_Cup.first],
// 								loopList.crown[firstBestScore_Crown.first]);
	
// 	tempCharacter->Update();
// 	std::array<double, 19> secondScoreEffection = tempCharacter->GetScoreEffection();
// 	std::pair<int, double> secondBestScore_Flower = GetScoreForEachArtifact(secondScoreEffection, loopList.flower); // descending sorted
// 	std::pair<int, double> secondBestScore_Feather = GetScoreForEachArtifact(secondScoreEffection, loopList.feather);
// 	std::pair<int, double> secondBestScore_Clock = GetScoreForEachArtifact(secondScoreEffection, loopList.clock);
// 	std::pair<int, double> secondBestScore_Cup = GetScoreForEachArtifact(secondScoreEffection, loopList.cup);
// 	std::pair<int, double> secondBestScore_Crown = GetScoreForEachArtifact(secondScoreEffection, loopList.crown);

// 	if( (firstBestScore_Flower.first == secondBestScore_Flower.first) && 
// 		(firstBestScore_Feather.first == secondBestScore_Feather.first) && 
// 		(firstBestScore_Clock.first == secondBestScore_Clock.first) && 
// 		(firstBestScore_Cup.first == secondBestScore_Cup.first) && 
// 		(firstBestScore_Crown.first == secondBestScore_Crown.first))
// 	{
// 		tempCharacter->SetArtifact(loopList.flower[secondBestScore_Flower.first],
// 									loopList.feather[secondBestScore_Feather.first],
// 									loopList.clock[secondBestScore_Clock.first],
// 									loopList.cup[secondBestScore_Cup.first],
// 									loopList.crown[secondBestScore_Crown.first]);
		
// 		// Initialize the character
// 		CALLOOPSTART = std::chrono::system_clock::now();
// 		tempCharacter->Update(true);
// 		CALLOOPFINISH = std::chrono::system_clock::now();
// 		mCalLoopTimeList[0] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();
		
// 		// Calculate the damage
// 		CALLOOPSTART = CALLOOPFINISH;
// 		double bestDamage = tempCharacter->GetScore();
// 		CALLOOPFINISH = std::chrono::system_clock::now();
// 		mCalLoopTimeList[1] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();

// 		bestTryArtifacts.flower = loopList.flower[firstBestScore_Flower.first];
// 		bestTryArtifacts.feather = loopList.feather[firstBestScore_Feather.first];
// 		bestTryArtifacts.clock = loopList.clock[firstBestScore_Clock.first];
// 		bestTryArtifacts.cup = loopList.cup[firstBestScore_Cup.first];
// 		bestTryArtifacts.crown = loopList.crown[firstBestScore_Crown.first];

// 		delete tempCharacter->GetWeapon();
// 		delete tempCharacter->GetArtSetStat();
// 		delete tempCharacter;
// 		return bestDamage;
// 	}
// 	else
// 	{
// 		std::array<double, 19> thirdScoreEffection = { 0. };
// 		for (int i = 0; i < 19; i++)
// 		{
// 			thirdScoreEffection[i] = (firstScoreEffection[i] + secondScoreEffection[i]) * 0.5;
// 		}
// 		std::pair<int, double> thirdBestScore_Flower = GetScoreForEachArtifact(thirdScoreEffection, loopList.flower); // descending sorted
// 		std::pair<int, double> thirdBestScore_Feather = GetScoreForEachArtifact(thirdScoreEffection, loopList.feather);
// 		std::pair<int, double> thirdBestScore_Clock = GetScoreForEachArtifact(thirdScoreEffection, loopList.clock);
// 		std::pair<int, double> thirdBestScore_Cup = GetScoreForEachArtifact(thirdScoreEffection, loopList.cup);
// 		std::pair<int, double> thirdBestScore_Crown = GetScoreForEachArtifact(thirdScoreEffection, loopList.crown);
		
// 		tempCharacter->SetArtifact(loopList.flower[thirdBestScore_Flower.first],
// 									loopList.feather[thirdBestScore_Feather.first],
// 									loopList.clock[thirdBestScore_Clock.first],
// 									loopList.cup[thirdBestScore_Cup.first],
// 									loopList.crown[thirdBestScore_Crown.first]);
		
// 		// Initialize the character
// 		CALLOOPSTART = std::chrono::system_clock::now();
// 		tempCharacter->Update(true);
// 		CALLOOPFINISH = std::chrono::system_clock::now();
// 		mCalLoopTimeList[0] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();
		
// 		// Calculate the damage
// 		CALLOOPSTART = CALLOOPFINISH;
// 		double bestDamage = tempCharacter->GetScore();
// 		CALLOOPFINISH = std::chrono::system_clock::now();
// 		mCalLoopTimeList[1] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();

// 		bestTryArtifacts.flower = loopList.flower[thirdBestScore_Flower.first];
// 		bestTryArtifacts.feather = loopList.feather[thirdBestScore_Feather.first];
// 		bestTryArtifacts.clock = loopList.clock[thirdBestScore_Clock.first];
// 		bestTryArtifacts.cup = loopList.cup[thirdBestScore_Cup.first];
// 		bestTryArtifacts.crown = loopList.crown[thirdBestScore_Crown.first];

// 		delete tempCharacter->GetWeapon();
// 		delete tempCharacter->GetArtSetStat();
// 		delete tempCharacter;
// 		return bestDamage;
// 	}
// }





















double Simulator::CalLoopArtifact_jangddolScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](ArtCrown* crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](ArtCup* cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](ArtClock* clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](ArtFeather* feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](ArtFlower* flower) {
                        mCharacter->SetArtFlower(flower);
						
						// Initialize the character
						CALLOOPSTART = std::chrono::system_clock::now();
						mCharacter->Update(true);
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[0] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();
						
						// Calculate the damage
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = mCharacter->GetScore();
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[1] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();

						// Update the best damage and artifact combination if necessary
						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
							bestTryArtifacts.flower = flower;
							bestTryArtifacts.feather = feather;
							bestTryArtifacts.clock = clock;
							bestTryArtifacts.cup = cup;
							bestTryArtifacts.crown = crown;
						}
                    });
                });
            });
        });
    });
	
	return bestDamage;
}


double Simulator::CalLoopArtifact_MonkeyMagicScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](ArtCrown* crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](ArtCup* cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](ArtClock* clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](ArtFeather* feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](ArtFlower* flower) {
                        mCharacter->SetArtFlower(flower);
						
						// Initialize the character
						CALLOOPSTART = std::chrono::system_clock::now();
						mCharacter->Update(true);
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[0] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();
						
						// Calculate the damage
						CALLOOPSTART = CALLOOPFINISH;
						tempDamage = mCharacter->GetScore_MonkeyMagic();
						CALLOOPFINISH = std::chrono::system_clock::now();
						mCalLoopTimeList[1] += std::chrono::duration<double>(CALLOOPFINISH- CALLOOPSTART).count();

						// Update the best damage and artifact combination if necessary
						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
							bestTryArtifacts.flower = flower;
							bestTryArtifacts.feather = feather;
							bestTryArtifacts.clock = clock;
							bestTryArtifacts.cup = cup;
							bestTryArtifacts.crown = crown;
						}
                    });
                });
            });
        });
    });

	return bestDamage;
}


double Simulator::CalLoopArtifact(Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList,
	ArtifactBundle& bestTryArtifacts)
{
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

	bool beforeManualMode = mCharacter->GetManualMode();
	mCharacter->SetManualMode(true);

	double bestDamage;
	switch (mScoreIndex)
    {
        case DAMAGE:
			bestDamage = CalLoopArtifact_Damage(loopList, bestTryArtifacts);
            break;
        case JANGDDOL:
			bestDamage = CalLoopArtifact_jangddolScore(loopList, bestTryArtifacts);
            break;
        case MONKEYMAGIC:
			bestDamage = CalLoopArtifact_MonkeyMagicScore(loopList, bestTryArtifacts);
            break;
        default:
			assert(0);
    }

	mCharacter->SetManualMode(beforeManualMode);

	return bestDamage;
}


void PrintArtifact(const Artifact* artifact);
void Simulator::PrintLastArtifacts(int trialNum, double bestDamage, const ArtifactBundle& bestArtifacts) const
{
	if (mSeeLastArtifact)
	{
		std::cout << trialNum << "-th result =============================================" << std::endl;
		std::cout << "bestDamage : " << bestDamage << std::endl;
		std::cout << "============== Last Artifacts ===============" << std::endl;
		PrintArtifact(bestArtifacts.flower);
		PrintArtifact(bestArtifacts.feather);
		PrintArtifact(bestArtifacts.clock);
		PrintArtifact(bestArtifacts.cup);
		PrintArtifact(bestArtifacts.crown);
		std::cout << "============== Character Stat ===============" << std::endl;
		PrintStat(mCharacter->GetStat());
	}
}


void Simulator::PrintTimeConsumption() const
{
	if (mSeeTimeConsumption && !mWorkerMode)
    {
        std::cout << mTimeList[0] << "s, " << mTimeList[1] << "s, " << mTimeList[2] << "s, " << mTimeList[3] << "s" << std::endl;
        std::cout << mCalLoopTimeList[0] << "s, " << mCalLoopTimeList[1] << "s" << std::endl;
    }
}


void Simulator::PrintProgress(int trial, int nowArtNum, int simNum, int artifactNum) const
{
	double beforePercent = (double)(trial * artifactNum + nowArtNum - 1)/(double)(simNum * artifactNum) * 100.;
	double percent = (double)(trial * artifactNum + nowArtNum)/(double)(simNum * artifactNum) * 100.;
	if (!mWorkerMode)
	{
		if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
	}
	else
	{
		if (((int)beforePercent != (int)percent) && !DEBUGMODE)
		{
			printToCoordinates(2, 1 + FIRSTCOLUMNWIDTH + mWorkerID * COLUMNWIDTH, "|%d %%", (int)percent);
		}
	}

}


TH2D* Simulator::RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage,
								TString histName)
{
	SuperArtifactList artifactSuperList;

	// Histogram array to store results of each simulation run
    if (histName == "") histName = "SIMULATOR_RUNSIMULATION_RESULT";
	TH2D* VisualHistogram;

	int newArtNum = (artifactNum % mBundleNum == 0) ? artifactNum : ((artifactNum / mBundleNum + 1) * mBundleNum + 1);
	int newArtBinNum = (artifactNum % mBundleNum == 0) ? (artifactNum / mBundleNum) : (newArtNum / mBundleNum + 1);

	switch (mScoreIndex)
    {
        case DAMAGE:
			VisualHistogram = new TH2D(histName, "", newArtBinNum, 0, newArtNum, binNum, minDamage, maxDamage);
            break;
        case JANGDDOL:
			VisualHistogram = new TH2D(histName, "", newArtBinNum, 0, newArtNum, binNum, MINSCORE, MAXSCORE);
            break;
        case MONKEYMAGIC:
			VisualHistogram = new TH2D(histName, "", newArtBinNum, 0, newArtNum, binNum, MINSCORE, MAXSCORE);
            break;
        default:
			assert(0);
    }

	// Code Execution Time Evaluation
	for (int i = 0; i < 4; i++) mTimeList[i] = 0.;
	for (int i = 0; i < 2; i++) mCalLoopTimeList[i] = 0.;
	std::chrono::system_clock::time_point start, finish;

	mAppendableRate = {};
	mAppendableRate.reserve(newArtNum);
	mAppendableRate.resize(newArtNum);

	mCharacter->MakeEffectionArray();
	mCharacter->MakeScoreFunction();

	// Simulation Part
	for (int i = 0; i < simNum; i++)
	{
		// trial initialization
		double bestDamage = 0;
		artifactSuperList.Clear();
		ArtifactBundle bestArtifacts;
		int numAppend = 0; // for Appendable Rate

		for (int j = 0; j < artifactNum; j++)
		{
			ArtifactBundle bestTryArtifacts;
			
			start = std::chrono::system_clock::now();
			Artifact* gennedArtifact = GenerateRandomArtifact();
			finish  = std::chrono::system_clock::now();
			mTimeList[0] += std::chrono::duration<double>(finish - start).count();

			start = finish; 
			bool whetherAppend = CheckWhetherAppendAndDelete(mCharacter, gennedArtifact, artifactSuperList);
			finish  = std::chrono::system_clock::now();
			mTimeList[1] += std::chrono::duration<double>(finish - start).count();

			if (whetherAppend)
			{				

				if (j % mBundleNum == 0)
				{
				start = std::chrono::system_clock::now(); 
				double comparedDamage = CalLoopArtifact(gennedArtifact, artifactSuperList, bestTryArtifacts);
					finish = std::chrono::system_clock::now();
				mTimeList[2] += std::chrono::duration<double>(finish - start).count();

				if (comparedDamage >= bestDamage)
				{
					bestDamage = comparedDamage;
					bestArtifacts = bestTryArtifacts;
				}
				}

				numAppend++;
			}
			else delete gennedArtifact;

			if (j % mBundleNum == 0)
			{
			VisualHistogram->Fill(j + 0.5, bestDamage);
			}

			mAppendableRate[j] += (double)numAppend;

			PrintProgress(i, j, simNum, artifactNum);
		}

		PrintLastArtifacts(i, bestDamage, bestArtifacts);

		artifactSuperList.DeleteAll();
	}

	for (int i = 0; i < artifactNum; i++)
	{
		mAppendableRate[i] /= (double)(i + 1);
		mAppendableRate[i] /= (double)simNum;
	}

	PrintTimeConsumption();

	if (DEBUGMODE) std::cout << "VisualHistogram->GetBinContent(artifactNum, (int)(0.7*binNum))" << VisualHistogram->GetBinContent(artifactNum, (int)(0.7*binNum)) << std::endl;

    return VisualHistogram;
}


void Simulator::SimulationWorker(int workerID, int simNum, int artifactNum, int binNum, double minDamage, double maxDamage)
{
	mCharacter->MakeEffectionArray();
	// simulation number
	simNum = simNum / mNumThread;
	
	SetWorkerID(workerID);

	TString histName = Form("SIMULATOR_RUNSIMULATION_RESULT-%d", workerID);

	if (DEBUGMODE) std::cout << "histName : " << histName << std::endl;

	TH2D* VisualHistogram = RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage, histName);
	// printToCoordinates(13 + workerID, 1, "mTimeList Pointer : %p", mTimeList);
	if (DEBUGMODE) std::cout << "Histogram Generation Done." << std::endl;
	// printToCoordinates(13 + workerID, 21, "mTimeList Pointer : %p", mTimeList);
	mSimulationResult = VisualHistogram;
}


TH2D* Simulator::RunSimulationMultiThreads(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage)
{
	#ifdef _MSC_VER
		system("cls");
	#else
		if (!DEBUGMODE) system("clear");
	#endif
	
	std::vector<TH2D*> HistogramArray = {};

	// Copy the number of numThread Characters from character.
		// The Copy Constructor of Character is made.
			// In the copy constructor of character, the copy one of ARtifact and Weapon is used.
				// The Copy Constructor of Artifact is made.
			// The Unit Test Code of these Copy Constructor is needed.
	std::vector<Character*> characterVector;
	characterVector.reserve(mNumThread);
	for (int i = 0; i < mNumThread; i++)
	{
		characterVector.emplace_back(mCharacter->Clone());
	}
	// These Characters is to be the memeber of Simulators (number of them = numThread)
	std::vector<Simulator> simulatorVector;
	simulatorVector.reserve(mNumThread);
	for (int i = 0; i < mNumThread; i++)
	{
		simulatorVector.emplace_back(Simulator());
		simulatorVector[i].SetCharacter(characterVector[i]);
		simulatorVector[i].SetWorkerMode(true);
		simulatorVector[i].SetNumThread(mNumThread);
		simulatorVector[i].SetSeeLastArtifact(mSeeLastArtifact);
		simulatorVector[i].SetSeeTimeConsumption(mSeeTimeConsumption);
		simulatorVector[i].SetScoreIndexMode(mScoreIndex);
		simulatorVector[i].SetBundleNum(mBundleNum);
	}

	if (DEBUGMODE)
	{
		for (int i = 0; i < mNumThread; i++)
		{
			std::cout << "Character Pointer" << characterVector[i] << std::endl;
			std::cout << "Simulaotr Pointer" << &(simulatorVector[i]) << std::endl;
		}
	}

	// terminal clear for Info Print out
	
	if (!DEBUGMODE)
	{
		printToCoordinates(1, 1, "ThreadNum");
		printToCoordinates(2, 1, "Progress");
		printToCoordinates(3, 1, "Time Consumption");
		printToCoordinates(4, 1, "  Worker");
		printToCoordinates(5, 1, "    Generation");
		printToCoordinates(6, 1, "    CheckAppend");
		printToCoordinates(7, 1, "    CalLoop");
		printToCoordinates(8, 1, "    Append");
		printToCoordinates(9, 1, "  CalLoop");
		printToCoordinates(10, 1, "    Update");
		printToCoordinates(11, 1, "    Damage Calc.");
		for (int i = 0; i < mNumThread; i++)
		{
			printToCoordinates(1, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|# %d", i + 1);
		}
	}
	

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈짂嶺뚯쉶媛딉옙�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅勇싲즾踰귨옙�맶�뜝�럥�쑋嶺뚮ㅎ留됵옙�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�땱�떜쨌�뙼�봿留띰옙�쐻占쎈윥占쎌몗癲ル슣鍮볩옙肉ε뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜝�뜫援⑼옙�굲�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅占쎌젂�뜝�뜴�쐻占쎈윥�뜝�럥��뺧옙�굲占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥�떋�깷�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅勇싲즾維낉옙�굲�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈즵占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥夷③뇦猿뗫닑占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲 SimulationWorker�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쐾占쎄슈占쎈솇占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌뱺占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣鍮뽬キ釉앹삕占쎈쭦癲ル슢�돘占쎈쭒濾곌쑴鍮놅옙�맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥鸚뤄옙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥����뜝�럥�뿞�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩議�.
	// SimulationWorker占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉�뙴洹⑥삕�뤃占� 2d-Histogram占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈읁�뜝�럥�맶�뜝�럥�몧�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� simulatorVector[i]占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥吏쀥뜝�럥彛잞옙�쐻占쎈윪�뤃轅⑤쐻占쎈윪占쎌읆占쎈쐻占쎈윥占쎄굡占쎈쐻占쎈윥�뜝�룞�삕�솾�꺂�뒧占쎈�뽳옙琉껃뜝�뜴�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪筌띿떑�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌γ렔�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌ｋ뵃�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�돲占쎄텊�뜝�럥踰⑨옙�굲�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
	std::vector<std::thread> threads;
	for (int i = 0; i < mNumThread; i++)
	{
		threads.emplace_back(std::thread(&Simulator::SimulationWorker, &(simulatorVector[i]),
										i, simNum, artifactNum, binNum, minDamage, maxDamage));
	}
	for (auto& thread : threads) {
		thread.join();
	}
	
	// Info Print out.
	
	if (!DEBUGMODE) 
	{
		for (int i = 0; i < mNumThread; i++)
		{
			printToCoordinates(5, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(0));
			printToCoordinates(6, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(1));
			printToCoordinates(7, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(2));
			printToCoordinates(8, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(3));
			printToCoordinates(10, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(0));
			printToCoordinates(11, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(1));
		}
	}

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿룞�삕占쎌맶�뜝�럥�쑅�뜝�럥堉볡솾�꺂�뒧占쏙옙占썸납占썲뜝�뜴�쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪鈺곕쵆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�κ텣�뜝�룞�삕占쎄콬占쎈쇀占쎈탟占쎌굲�뜝�럡�렊 AppendRate占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮝占쎈쑏�뜝�뜴�쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌굯占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈쓠�뜝�룞�삕占쎄껀占쎈폁占쎄샴癲ル슪�삕嶺뚮　維�占쎈㎍占쎈쐻占쎈윥占쎈첋�뜝�럩�쟼占쎈눀占쎄턁占쎈ぁ占쎈쐻占쎈윥�뜝�럩瑗삼옙�굲�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷놅옙占쏙옙�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈괘�뜝�럥�맶�뜝�럥�쑅�뜝�룞�삕�뜝�룞�삕占쎌맶�뜝�럥�쑋�뜝�럥痢쏉옙�쐻占쎈윥占쎌넇占쎈쐻占쎈윥筌앸ŀ�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
	std::vector<double> tempVector(artifactNum);
	for (int i = 0; i < mNumThread; i++)
	{
		std::vector<double> ithAppendableRate = simulatorVector[i].GetAppendableRate();
		for (int j = 0; j < artifactNum; j++)
		{
			tempVector[j] += (double)ithAppendableRate[j] / (double)mNumThread;
		}
	}
	mAppendableRate = tempVector;
	
	if (DEBUGMODE)
	{
		std::cout << "tempVector : size = " << tempVector.size() << std::endl;
		std::cout << "tempVector : first element = " << tempVector[0] << std::endl;
		for (int i = 0; i < mNumThread; i++)
		std::cout << Form("simulatorVector[%d].GetSimulationResult() : ", i) << simulatorVector[i].GetSimulationResult() << std::endl;
	}

	if (DEBUGMODE) std::cout << "10" << std::endl;
	// Sum over of all histograms
	for (int i = 0; i < mNumThread; i++)
	{
		HistogramArray.emplace_back(simulatorVector[i].GetSimulationResult());
	}

	if (DEBUGMODE) std::cout << "11" << std::endl;

	TH2D* VisualHistogram;

	int newArtNum = (artifactNum % mBundleNum == 0) ? artifactNum : ((artifactNum / mBundleNum + 1) * mBundleNum + 1);
	int newArtBinNum = (artifactNum % mBundleNum == 0) ? (artifactNum / mBundleNum) : (newArtNum / mBundleNum + 1);

	switch (mScoreIndex)
    {
        case DAMAGE:
			VisualHistogram = new TH2D("VisualHistogram", "", newArtBinNum, 0, newArtNum, binNum, minDamage, maxDamage);
            break;
        case JANGDDOL:
			VisualHistogram = new TH2D("VisualHistogram", "", newArtBinNum, 0, newArtNum, binNum, MINSCORE, MAXSCORE);
            break;
        case MONKEYMAGIC:
			VisualHistogram = new TH2D("VisualHistogram", "", newArtBinNum, 0, newArtNum, binNum, MINSCORE, MAXSCORE);
            break;
        default:
			assert(0);
    }

	for (int i = 0; i < mNumThread; i++)
	{
		VisualHistogram->Add(HistogramArray[i]);
	}

	if (DEBUGMODE) std::cout << "12" << std::endl;

	printToCoordinates(12, 1, "");
	return VisualHistogram;
}