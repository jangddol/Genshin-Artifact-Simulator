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
#include <memory>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <string>
#include <stdarg.h>
#include "TH1D.h"
#include "TH2D.h"


std::shared_ptr<Artifact> GenerateRandomArtifact()
{
	int temp = GetRdUni5();
	std::shared_ptr<Artifact> returnArtf;
	switch (temp)
	{
	case 0: returnArtf = std::dynamic_pointer_cast<Artifact>(std::make_shared<ArtFlower>()); break;
	case 1: returnArtf = std::dynamic_pointer_cast<Artifact>(std::make_shared<ArtFeather>()); break;
	case 2: returnArtf = std::dynamic_pointer_cast<Artifact>(std::make_shared<ArtClock>()); break;
	case 3: returnArtf = std::dynamic_pointer_cast<Artifact>(std::make_shared<ArtCup>()); break;
	case 4: returnArtf = std::dynamic_pointer_cast<Artifact>(std::make_shared<ArtCrown>()); break;
	default: assert(0);
		}
	return returnArtf;
}


bool CheckEffectiveOption(const std::shared_ptr<Character>& character, int index)
{
	if (character->GetEffection(index) > 1.e-5) return true;
	else return false;
}


bool CheckWhetherElementIsIn(int element, std::vector<int> List)
{
    for (int& inList: List)
	{
        if (inList == element) return true;
		}
    return false;
}


bool CheckWhetherElementIsIn(int element, std::array<double, 19> List)
{
    for (int i = 0; i < 19; i++)
    {
        if (List[i] == element) return true;
    }
    return false;
}


bool CheckBetterSubOpt(Stat betterOpt, Stat worseOpt, std::shared_ptr<Character>& character)
{
	std::vector<int> effectiveSubStats = character->GetEffectiveSubStats();
	std::array<double, 19> effectiveAmount;
	for (int i = 0; i < 19; i++) effectiveAmount[i] = character->GetEffection(i);

	// CR
	if (CheckWhetherElementIsIn(0, effectiveSubStats))
	{
		if (betterOpt.GetOption(0) < worseOpt.GetOption(0)) return false;
	}
	// CB
	if (CheckWhetherElementIsIn(1, effectiveSubStats))
	{
		if (betterOpt.GetOption(1) < worseOpt.GetOption(1)) return false;
	}
	// ATK, AP
	if (CheckWhetherElementIsIn(2, effectiveSubStats))
	{
		double betterAP = betterOpt.GetOption(2);
		double betterATK = betterOpt.GetOption(3);
		double worseAP = worseOpt.GetOption(2);
		double worseATK = worseOpt.GetOption(3);
		double betterTotATKeffection = betterAP * effectiveAmount[2] + betterATK * effectiveAmount[3];
		double worseTotATKeffection = worseAP * effectiveAmount[2] + worseATK * effectiveAmount[3];
		if (betterTotATKeffection < worseTotATKeffection) return false;
	}
	// EC
	if (character->GetTargetEC() > 100 + character->GetStatAfterUpdateFromArtSetStat().GetElementCharge())
	{
		if (betterOpt.GetOption(4) < worseOpt.GetOption(4)) return false;
	}
	// HP, HPP
	if (CheckWhetherElementIsIn(5, effectiveSubStats))
	{
		double betterHPP = betterOpt.GetOption(5);
		double betterHP = betterOpt.GetOption(6);
		double worseHPP = worseOpt.GetOption(5);
		double worseHP = worseOpt.GetOption(6);
		double betterTotHPeffection = betterHPP * effectiveAmount[2] + betterHP * effectiveAmount[3];
		double worseTotHPeffection = worseHPP * effectiveAmount[2] + worseHP * effectiveAmount[3];
		if (betterTotHPeffection < worseTotHPeffection) return false;
	}
	// EM
	if (CheckWhetherElementIsIn(7, effectiveSubStats))
	{
		if (betterOpt.GetOption(7) < worseOpt.GetOption(7)) return false;
	}
	// DEF, DP
	if (CheckWhetherElementIsIn(8, effectiveSubStats))
	{
		double betterDP = betterOpt.GetOption(8);
		double betterDEF = betterOpt.GetOption(9);
		double worseDP = worseOpt.GetOption(8);
		double worseDEF = worseOpt.GetOption(9);
		double betterTotDEFeffection = betterDP * effectiveAmount[8] + betterDEF * effectiveAmount[8];
		double worseTotDEFeffection = worseDP * effectiveAmount[9] + worseDEF * effectiveAmount[9];
		if (betterTotDEFeffection < worseTotDEFeffection) return false;
	}
	return true;
}

template <typename T>
std::vector<std::shared_ptr<Artifact>> Convert2VecArtifactPointer(std::vector<std::shared_ptr<T>> TList)
{
	std::vector<std::shared_ptr<Artifact>> returnVector(TList.size());
	for (std::size_t i = 0; i < TList.size() ; i++) returnVector[i] = std::dynamic_pointer_cast<Artifact>(TList[i]);
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


bool CheckWhetherAppendAndDelete(std::shared_ptr<Character> character, std::shared_ptr<Artifact> gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
    // Return false if the effective option check fails and the main type is not 3 or 6
    if (!CheckEffectiveOption(character, gennedArtifact->GetMainType()) && (gennedArtifact->GetMainType() != 3 && gennedArtifact->GetMainType() != 6))
    {
        return false;
    }

    // Get the list of artifacts for the given type
    std::vector<std::shared_ptr<Artifact>> selectedList;
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

    // Check whether the generated artifact should be appended to or deleted from the list
    bool whetherAppend = true;
    Stat gennedSubOpt = gennedArtifact->GetSubStat();
    for (int i = 0; i < selectedList.size(); i++)
    {
        if (selectedList[i]->GetMainType() == gennedArtifact->GetMainType())
        {
            Stat tempSubOpt = selectedList[i]->GetSubStat();
            if (CheckBetterSubOpt(tempSubOpt, gennedSubOpt, character))
            {
                // Existing artifact is better, do not append the generated artifact
                whetherAppend = false;
            }
            else if (CheckBetterSubOpt(gennedSubOpt, tempSubOpt, character))
            {
                // Generated artifact is better, delete the existing artifact
                EraseSuperArtifactList(ArtifactSuperList, gennedArtifact->GetType(), i);
                selectedList.erase(selectedList.begin() + i);
                i--;
            }
        }
    }
	

    return whetherAppend;
}


void AppendArtifactList(std::shared_ptr<Artifact> gennedArtifact, SuperArtifactList& ArtifactSuperList)
{
	int numType = gennedArtifact->GetType();

	if      (numType == 1) ArtifactSuperList.flower.emplace_back(std::dynamic_pointer_cast<ArtFlower>(gennedArtifact));
	else if (numType == 2) ArtifactSuperList.feather.emplace_back(std::dynamic_pointer_cast<ArtFeather>(gennedArtifact));
	else if (numType == 3) ArtifactSuperList.clock.emplace_back(std::dynamic_pointer_cast<ArtClock>(gennedArtifact));
	else if (numType == 4) ArtifactSuperList.cup.emplace_back(std::dynamic_pointer_cast<ArtCup>(gennedArtifact));
	else if (numType == 5) ArtifactSuperList.crown.emplace_back(std::dynamic_pointer_cast<ArtCrown>(gennedArtifact));
	else
	{
		assert(false && "Error : gennedArtifact has wrong Type (@ AppendArtifactList)");
	}
}


double Simulator::CalLoopArtifact_Damage(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](std::shared_ptr<ArtCrown> crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](std::shared_ptr<ArtCup> cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](std::shared_ptr<ArtClock> clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](std::shared_ptr<ArtFeather> feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](std::shared_ptr<ArtFlower> flower) {
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


double Simulator::CalLoopArtifact_jangddolScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](std::shared_ptr<ArtCrown> crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](std::shared_ptr<ArtCup> cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](std::shared_ptr<ArtClock> clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](std::shared_ptr<ArtFeather> feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](std::shared_ptr<ArtFlower> flower) {
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

	std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](std::shared_ptr<ArtCrown> crown) {
        mCharacter->SetArtCrown(crown);
		std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](std::shared_ptr<ArtCup> cup) {
            mCharacter->SetArtCup(cup);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](std::shared_ptr<ArtClock> clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](std::shared_ptr<ArtFeather> feather) {
                    mCharacter->SetArtFeather(feather);
					std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](std::shared_ptr<ArtFlower> flower) {
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


double Simulator::CalLoopArtifact(std::shared_ptr<Artifact> gennedArtifact, SuperArtifactList& ArtifactSuperList,
	ArtifactBundle& bestTryArtifacts)
{
	// Set the generated artifact as the only artifact in the loop list for its type
    SuperArtifactList loopList = ArtifactSuperList;
    switch (gennedArtifact->GetType())
    {
        case 1:
            loopList.flower = { std::dynamic_pointer_cast<ArtFlower>(gennedArtifact) };
            break;
        case 2:
            loopList.feather = { std::dynamic_pointer_cast<ArtFeather>(gennedArtifact) };
            break;
        case 3:
            loopList.clock = { std::dynamic_pointer_cast<ArtClock>(gennedArtifact) };
            break;
        case 4:
            loopList.cup = { std::dynamic_pointer_cast<ArtCup>(gennedArtifact) };
            break;
        case 5:
            loopList.crown = { std::dynamic_pointer_cast<ArtCrown>(gennedArtifact) };
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
		PrintArtifact(bestArtifacts.flower.get());
		PrintArtifact(bestArtifacts.feather.get());
		PrintArtifact(bestArtifacts.clock.get());
		PrintArtifact(bestArtifacts.cup.get());
		PrintArtifact(bestArtifacts.crown.get());
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
	double beforePercent = (double)(trial * artifactNum + nowArtNum - 1) / (double)(simNum * artifactNum) * 100.;
	double percent = (double)(trial * artifactNum + nowArtNum) / (double)(simNum * artifactNum) * 100.;
	if (!mWorkerMode)
	{
		if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
	}
	else
	{
		if ((int)beforePercent != (int)percent)
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

	mCharacter->Update();
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
			std::shared_ptr<Artifact> gennedArtifact = GenerateRandomArtifact();
			finish = std::chrono::system_clock::now();
			mTimeList[0] += std::chrono::duration<double>(finish - start).count();

			start = finish; 
			bool whetherAppend = CheckWhetherAppendAndDelete(mCharacter, gennedArtifact, artifactSuperList);
			finish = std::chrono::system_clock::now();
			mTimeList[1] += std::chrono::duration<double>(finish - start).count();

			if (whetherAppend)
			{				
				start = finish;
				AppendArtifactList(gennedArtifact, artifactSuperList);
				finish = std::chrono::system_clock::now();
				mTimeList[3] += std::chrono::duration<double>(finish - start).count();

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

			if (j % mBundleNum == 0)
			{
			VisualHistogram->Fill(j + 0.5, bestDamage);
			}

			mAppendableRate[j] += (double)numAppend;

			PrintProgress(i, j, simNum, artifactNum);
		}

		if ((artifactSuperList.flower.size() == 0) ||
		(artifactSuperList.feather.size() == 0) ||
		(artifactSuperList.clock.size() == 0) ||
		(artifactSuperList.cup.size() == 0) ||
		(artifactSuperList.crown.size() == 0))
		{
			mCharacter->SetArtifact(bestArtifacts.flower, bestArtifacts.feather, bestArtifacts.clock, bestArtifacts.cup, bestArtifacts.crown);
			mCharacter->Update();
		PrintLastArtifacts(i, bestDamage, bestArtifacts);
		}
	}

	for (int i = 0; i < artifactNum; i++)
	{
		mAppendableRate[i] /= (double)(i + 1);
		mAppendableRate[i] /= (double)simNum;
	}

	PrintTimeConsumption();

    return VisualHistogram;
}


void Simulator::SimulationWorker(int workerID, int simNum, int artifactNum, int binNum, double minDamage, double maxDamage)
{
	mCharacter->MakeEffectionArray();
	// simulation number
	simNum = simNum / mNumThread;
	if (workerID <= simNum % mNumThread) simNum++;
	
	SetWorkerID(workerID);

	TString histName = Form("SIMULATOR_RUNSIMULATION_RESULT-%d", workerID);
	TH2D* VisualHistogram = RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage, histName);
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
	std::vector<std::shared_ptr<Character>> characterVector;
	characterVector.reserve(mNumThread);
	for (int i = 0; i < mNumThread; i++)
	{
		characterVector.emplace_back(mCharacter->Clone_sharedptr());
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

	// terminal clear for Info Print out
	
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
	

	// �뜝�럥�맶�뜝�럥�쑋占쏙옙�뼚짹占쎌맶�뜝�럥�쑅占쎈쨨占쎌몱占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶占쎈쐻�뜝占� �뜝�럥夷у뜝�럥利꿨뜝�럡�뀪�뜝�룞�삕占쎌뒱占쎌굲 �뜝�럥�맶�뜝�럥�쑅占쎈뼀�뀎����맶�뜝�럥�쑋�뜝�럩�벂�뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼓�뜝�럥�맶�뜝�럥�쑅�뜝�럥援겼뜝�럥�맶�뜝�럥�쐾�뜝�럥彛� SimulationWorker占쎈쐻占쎈윥占쎄괴�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�럸�뜝�럥利든뜏類ｋ걝占쎌���뜝�럩�뤈�뜝�럩援꿨뜝�럥����뜝�럥爰뗥뜝�럩援뀐옙�쐻占쎈윥占쎈젩.
	// SimulationWorker�뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕 2d-Histogram�뜝�럥�맶�뜝�럥�쑋�뛾占썲뜝占� simulatorVector[i]�뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥�쐾�뜝�럥占쏙퐦�삕�젆�몼�맀筌뤿굢�삕占쏙옙占쏙옙�굲占쎈쐻占쎈짗占쎌굲 �뜝�럩�쐪�뜝�럩�뮝�뜝�럩�몗占쎈쐻占쎈윥筌λ〕�삕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
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
	
		for (int i = 0; i < mNumThread; i++)
		{
			printToCoordinates(5, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(0));
			printToCoordinates(6, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(1));
			printToCoordinates(7, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(2));
			printToCoordinates(8, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetTimeList(3));
			printToCoordinates(10, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(0));
			printToCoordinates(11, 1 + FIRSTCOLUMNWIDTH + i * COLUMNWIDTH, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(1));
	}

	// �뜝�럥�맶�뜝�럥�쐾占쎈닱筌롡뫀�맶�뜝�럥�쐾�뜝�럥�젾�뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 AppendRate�뜝�럥�맶�뜝�럥�쑅嶺뚯빢�삕 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뿼�뜝�럥���耀붾굝梨멨뜝�뜦堉븅뜮�뀘�쐻�뜝占� �뜝�럥�맶�뜝�럥�쐾�뜝�럥彛녶뜝�럥�늾占쎈뙀占쎌뒠�댚�룊�삕占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑅占쎌젂�뜝占�.
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

	// Sum over of all histograms
	for (int i = 0; i < mNumThread; i++)
	{
		HistogramArray.emplace_back(simulatorVector[i].GetSimulationResult());
	}

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

	printToCoordinates(12, 1, "");
	return VisualHistogram;
}