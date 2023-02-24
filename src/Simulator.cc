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
	returnArtf->Generation();
	return returnArtf;
}


bool CheckEffectiveOption(const std::shared_ptr<Character>& character, int index)
{
	if (character->GetEffection(index) > 1.e-5) return true;
	else return false;
}


bool CheckBetterSubOpt(const Stat& betterOpt, const Stat& worseOpt, std::shared_ptr<Character>& character)
{
	std::array<double, 19> effectiveAmount;
	for (int i = 0; i < 19; i++) effectiveAmount[i] = character->GetEffection(i);

	// CR
	if (CheckEffectiveOption(character, 0))
	{
		if (betterOpt.GetOption(0) < worseOpt.GetOption(0)) return false;
	}
	// CB
	if (CheckEffectiveOption(character, 1))
	{
		if (betterOpt.GetOption(1) < worseOpt.GetOption(1)) return false;
	}
	// ATK, AP
	if (CheckEffectiveOption(character, 2))
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
	if (CheckEffectiveOption(character, 5))
	{
		double betterHPP = betterOpt.GetOption(5);
		double betterHP = betterOpt.GetOption(6);
		double worseHPP = worseOpt.GetOption(5);
		double worseHP = worseOpt.GetOption(6);
		double betterTotHPeffection = betterHPP * effectiveAmount[5] + betterHP * effectiveAmount[6];
		double worseTotHPeffection = worseHPP * effectiveAmount[5] + worseHP * effectiveAmount[6];
		if (betterTotHPeffection < worseTotHPeffection) return false;
	}
	// EM
	if (CheckEffectiveOption(character, 7))
	{
		if (betterOpt.GetOption(7) < worseOpt.GetOption(7)) return false;
	}
	// DEF, DP
	if (CheckEffectiveOption(character, 8))
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


void EraseSuperArtifactList(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& ArtifactSuperList, int numType, int index)
{
	ArtifactSuperList[numType - 1].erase(ArtifactSuperList[numType - 1].begin() + index);
}


bool CheckWhetherAppendAndDelete(std::shared_ptr<Character> character, std::shared_ptr<Artifact> gennedArtifact, std::array<std::vector<std::shared_ptr<Artifact>>, 5>& ArtifactSuperList)
{
    // Return false if the effective option check fails and the main type is not 3 or 6
    if (!CheckEffectiveOption(character, gennedArtifact->GetMainType()) && (gennedArtifact->GetMainType() != 3 && gennedArtifact->GetMainType() != 6))
    {
        return false;
    }

    // Get the list of artifacts for the given type
    std::vector<std::shared_ptr<Artifact>> selectedList = ArtifactSuperList[gennedArtifact->GetType() - 1];

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


void AppendArtifactList(std::shared_ptr<Artifact> gennedArtifact, std::array<std::vector<std::shared_ptr<Artifact>>, 5>& ArtifactSuperList)
{
	int numType = gennedArtifact->GetType();
	ArtifactSuperList[numType - 1].emplace_back(gennedArtifact);
}


double Simulator::CalLoopArtifact_Damage(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList[4].begin(), loopList[4].end(), [&](std::shared_ptr<Artifact> crown) {
        mCharacter->SetArtCrown(std::dynamic_pointer_cast<ArtCrown>(crown));
		std::for_each(loopList[3].begin(), loopList[3].end(), [&](std::shared_ptr<Artifact> cup) {
            mCharacter->SetArtCup(std::dynamic_pointer_cast<ArtCup>(cup));
			std::for_each(loopList[2].begin(), loopList[2].end(), [&](std::shared_ptr<Artifact> clock) {
                mCharacter->SetArtClock(std::dynamic_pointer_cast<ArtClock>(clock));
				std::for_each(loopList[1].begin(), loopList[1].end(), [&](std::shared_ptr<Artifact> feather) {
                    mCharacter->SetArtFeather(std::dynamic_pointer_cast<ArtFeather>(feather));
					std::for_each(loopList[0].begin(), loopList[0].end(), [&](std::shared_ptr<Artifact> flower) {
                        mCharacter->SetArtFlower(std::dynamic_pointer_cast<ArtFlower>(flower));
						
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
							bestTryArtifacts.flower = std::dynamic_pointer_cast<ArtFlower>(flower);
							bestTryArtifacts.feather = std::dynamic_pointer_cast<ArtFeather>(feather);
							bestTryArtifacts.clock = std::dynamic_pointer_cast<ArtClock>(clock);
							bestTryArtifacts.cup = std::dynamic_pointer_cast<ArtCup>(cup);
							bestTryArtifacts.crown = std::dynamic_pointer_cast<ArtCrown>(crown);
						}
                    });
                });
            });
        });
    });
	
	return bestDamage;
}


double Simulator::CalLoopArtifact_jangddolScore(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList[4].begin(), loopList[4].end(), [&](std::shared_ptr<Artifact> crown) {
        mCharacter->SetArtCrown(std::dynamic_pointer_cast<ArtCrown>(crown));
		std::for_each(loopList[3].begin(), loopList[3].end(), [&](std::shared_ptr<Artifact> cup) {
            mCharacter->SetArtCup(std::dynamic_pointer_cast<ArtCup>(cup));
			std::for_each(loopList[2].begin(), loopList[2].end(), [&](std::shared_ptr<Artifact> clock) {
                mCharacter->SetArtClock(std::dynamic_pointer_cast<ArtClock>(clock));
				std::for_each(loopList[1].begin(), loopList[1].end(), [&](std::shared_ptr<Artifact> feather) {
                    mCharacter->SetArtFeather(std::dynamic_pointer_cast<ArtFeather>(feather));
					std::for_each(loopList[0].begin(), loopList[0].end(), [&](std::shared_ptr<Artifact> flower) {
                        mCharacter->SetArtFlower(std::dynamic_pointer_cast<ArtFlower>(flower));
						
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
							bestTryArtifacts.flower = std::dynamic_pointer_cast<ArtFlower>(flower);
							bestTryArtifacts.feather = std::dynamic_pointer_cast<ArtFeather>(feather);
							bestTryArtifacts.clock = std::dynamic_pointer_cast<ArtClock>(clock);
							bestTryArtifacts.cup = std::dynamic_pointer_cast<ArtCup>(cup);
							bestTryArtifacts.crown = std::dynamic_pointer_cast<ArtCrown>(crown);
						}
                    });
                });
            });
        });
    });
	
	return bestDamage;
}


double Simulator::CalLoopArtifact_MonkeyMagicScore(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;

	double tempDamage;
	double bestDamage = 0;

	std::for_each(loopList[4].begin(), loopList[4].end(), [&](std::shared_ptr<Artifact> crown) {
        mCharacter->SetArtCrown(std::dynamic_pointer_cast<ArtCrown>(crown));
		std::for_each(loopList[3].begin(), loopList[3].end(), [&](std::shared_ptr<Artifact> cup) {
            mCharacter->SetArtCup(std::dynamic_pointer_cast<ArtCup>(cup));
			std::for_each(loopList[2].begin(), loopList[2].end(), [&](std::shared_ptr<Artifact> clock) {
                mCharacter->SetArtClock(std::dynamic_pointer_cast<ArtClock>(clock));
				std::for_each(loopList[1].begin(), loopList[1].end(), [&](std::shared_ptr<Artifact> feather) {
                    mCharacter->SetArtFeather(std::dynamic_pointer_cast<ArtFeather>(feather));
					std::for_each(loopList[0].begin(), loopList[0].end(), [&](std::shared_ptr<Artifact> flower) {
                        mCharacter->SetArtFlower(std::dynamic_pointer_cast<ArtFlower>(flower));
						
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
							bestTryArtifacts.flower = std::dynamic_pointer_cast<ArtFlower>(flower);
							bestTryArtifacts.feather = std::dynamic_pointer_cast<ArtFeather>(feather);
							bestTryArtifacts.clock = std::dynamic_pointer_cast<ArtClock>(clock);
							bestTryArtifacts.cup = std::dynamic_pointer_cast<ArtCup>(cup);
							bestTryArtifacts.crown = std::dynamic_pointer_cast<ArtCrown>(crown);
						}
                    });
                });
            });
        });
    });

	return bestDamage;
}


double Simulator::CalLoopArtifact(std::shared_ptr<Artifact> gennedArtifact, std::array<std::vector<std::shared_ptr<Artifact>>, 5>& ArtifactSuperList,
	ArtifactBundle& bestTryArtifacts)
{
	// Set the generated artifact as the only artifact in the loop list for its type
    std::array<std::vector<std::shared_ptr<Artifact>>, 5> loopList = ArtifactSuperList;
	loopList[gennedArtifact->GetType() - 1] = { gennedArtifact };

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
	std::array<std::vector<std::shared_ptr<Artifact>>, 5> artifactSuperList;

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
		artifactSuperList.fill({});
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

				start = finish;
				AppendArtifactList(gennedArtifact, artifactSuperList);
				finish = std::chrono::system_clock::now();
				mTimeList[3] += std::chrono::duration<double>(finish - start).count();

				numAppend++;
			}

			if (j % mBundleNum == 0)
			{
				VisualHistogram->Fill(j + 0.5, bestDamage);
			}

			mAppendableRate[j] += (double)numAppend;

			PrintProgress(i, j, simNum, artifactNum);
		}

		if ((artifactSuperList[0].size() == 0) ||
		(artifactSuperList[1].size() == 0) ||
		(artifactSuperList[2].size() == 0) ||
		(artifactSuperList[3].size() == 0) ||
		(artifactSuperList[4].size() == 0))
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
	

	// 이들은 각각 쓰레드 안에서 SimulationWorker를 발동한다.
	// SimulationWorker는 2d-Histogram을 simulatorVector[i]에 남기고 죽는다.
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

	// 생성된 AppendRate도 여기로 넘겨준다.
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