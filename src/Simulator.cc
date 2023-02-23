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
	

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�뢾占쎌몡占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돰占쎌삺�뜝�럥�룛占쎈쐻占쎈윪占쎈츧占쎈쐻占쎈윥筌뚭랬�삕筌뚮냵�삕�뙴�끋占쎌뼐�뜙占쎌굲�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮됵옙�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윲�뜝�럥�맶�뜝�럥�쑅�뜝�럡�떙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럥�돰占쎈쐻占쎈윥�젆遺븐땡占쎈튂�젆�씤逾녑칰�럥�룯�뜝�럥爾쎾뜝�럡�렊�뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮됵옙�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥�뜝�럥�떦�빊占썲뜝�럩逾�嶺뚮엪�삕占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮됵옙�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윲�뜝�럥�맶�뜝�럥�쑅�뜝�럡�떙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�뜫猷귨옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빢�삕占쎌맶�뜝�럥�쑅嶺뚮슢寃�占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅占쎌젂占쎄퐩占쎌맶�뜝�럥�쐾�뜝�룞�삕�뜝�룞�뵾占쎌뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮占썲뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼲彛쀯옙堉꿨퐲�꺈�삕�뙴�뵃�삕占쎄뎡占쎈눇�뙼蹂��굲�솾�봾�꺋占쎌굲�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉻�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욑옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�룞�삕�뜝�럩援뀐옙�쐻占쎈윥占쎈룿�뜝�럥�맶�뜝�럥�쑅占쎌젂�겫猷붾�뗥뜝�럩�뮦�뜝�럩援뀐옙�쐻占쎈윪占쎈늸�뜝�럥�맶�뜝�럥�쑅�뜝�럥萸썹솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�뜫猷귨옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빆�쐻占쎈윥占쎈ぁ占쎌녃域뱀꼶爾닷뜝�럡�렊占쎈쐻占쎈윞占쎈쑆�뜝�럥�룄占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵰占쎌몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥泥묕옙�쐻占쎈윥鸚룐벂�쐻占쎈윥�댚諛㏓쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮝�뜝�럥琉끻뜝�럥�맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅勇싲즾踰귨옙�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럩�꼥占쎈쐻占쎈짗占쎌굲占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�땱�떜利양땟�굢�삕占쎄뎡占쎈쐻占쎈윪筌띾쑚�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐┼�슢캉�뙴洹⑥삕�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�뢾占쎌몡�뜝�럥爰뤷뜝�럩援뀐옙�쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅占쎌젂�뜝�룞�삕占쎌맶�뜝�럥�쐾占쎈㎟占쎌뵢占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럥�돰占쎈쐻占쎈윥�젆遺븐땡占쎈튂�젆�돍�삕�뇡�쑚�쐻占쎈윞占쎈쑆占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�걨占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣鍮뽬キ�씛�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�룞�삕�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼲彛쀯옙堉뀐옙�굲�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈엶占쎈쐻占쎈윥�뜮�빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윪筌뤵룇�삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥釉붾���뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞�굜�껊쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥占쎄묽占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃 SimulationWorker�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욃뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧筌잙뎽�쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮猷욑옙�굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥�맱�뇯�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�댆臾덈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉻�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욑옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쎗�뜝�띂��롳옙占쏙옙占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�뜫猷귨옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빆�쐻占쎈윥占쎈ぁ癲ル슢移얏�⑤똾���占쎈쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥占쎈뭵占쎈쐻占쎈윞占쎈젇占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅占쎈쇀占쎈츐占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗傭��끉猷녶뜝�룞�삕�뿥轅붽뭔�뜝�뜦維�占쎈뮝力놂옙沃섓옙占쎌굲�뜝�럩紐∽옙�쐻占쎈윥�댆琉룸쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎈퉳�뜝�럥堉붼툣猷붽뎀占쎌굲�뜝�럡�렊占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮占썲뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼲彛쀯옙堉꿨퐲�꺈�삕�뙴�뵃�삕占쎄뎡占쎈눇�뙼蹂��굲�솾�봾�꺋占쎌굲�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�룞�삕�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥荑됧뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮占썲뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼍由경뉩�뫁�굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑋�뜝�럡�꺏占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럯占쎈쐻占쎈윥占쎄틯�뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧筌잙뎽�쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�솑占쏙옙�벀�걠占쎄뎡.
	// SimulationWorker占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤뢾異��뜝�룞�삕筌랃옙�뜝�럩援뀐옙�쐻占쎈윪�뜝�룞�삕�뜝�럩�맳�뛾�룇�삌�댚�렱�쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥�쑅勇싲즾維낉옙�굲�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� 2d-Histogram占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�뢾占쎌몡�뜝�럥爰뤷뜝�럩援뀐옙�쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪獄�釉앹삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럡肄ゅ뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�걨占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� simulatorVector[i]占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�룞�삕�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼇�븶占쎄덩占쎌굲�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�몱占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윪筌뤵룇�삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�룞�삕�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥궘占쏙옙�걠�뙴�돍�삕占쎄돈占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅勇싲즾紐억쭫�룊�삕野껓옙力놂옙�뜝�뜴�쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪獄��슃�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥夷ⓨ뜝�럥�꽣�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類ㅻ즸占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥�쑅�뜝�럥萸댐옙�쐻占쎈윪�뤃��먥뵾占쎌뒧占쎈뮚�뜝�럥�맋占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅勇싲즾踰귨옙�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럩�꼥占쎈쐻占쎈짗占쎌굲占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪占쎌쓦�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욃뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕앾옙�쐻占쎈윥筌뚮겭�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥�젆�꽒�쐻占쎈윞占쏙옙占쏙┼�슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�땱�떜利양땟�굢�삕占쎄뎡占쎈쐻占쎈윪筌띾쑚�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윥占쎈젳�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뤁�뜝�럥嫄쀥뜝�럩�젲�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮦�뜝�럥彛믣뜝�룞�삕占쎈폁占쎈쐷�뜝�럩援뀐옙�쐻占쎈윞占쎈젇占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욃뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧筌잙뎽�쐻占쎈쑌�뙴洹⑥삕占쏙옙�뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅嶺뚮슡�궡占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜝�럥�뀋占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�꺐�뇦猿딆굡�댚�엺�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩�읇�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮됵옙�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윲�뜝�럥�맶�뜝�럥�쑅�뜝�럡�떙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뛾占쏙옙�뒻占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
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

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵�뜝�럥����뜝�럩�뤈�뜝�럩援뀐옙�쐻占쎈윞占쎈젇占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆占쎌녃域뱀꼶猷긷뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類ㅻ즸占쎌굲�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ざ占쎈쐻占쎈윞占쎈�ㅿ옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윪筌뤵룇�삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎌넁�뜝�룞�삕占쎈��甕겸댙�삕繹먮벝�삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉�占쎈눀�겫�뼲�삕�뜝�룞�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽�뜝�럡����뜝�럡�뜐占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윞�눧�뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩�읇 AppendRate占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥�몞占쎈쇀域밟뫁�굲�뜝�룞�삕占쎈염占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜮�엺�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦끇�돫�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎄콞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅勇싲즾維낉옙�굲�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥堉℡뜝�럡���嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃怨ㅼ삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌넰占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윞占쎈뼂占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺧옙�뜝�럥�맶�뜝�럥�쑋占쎈쨨饔낃퀣�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈짗占쎌굲占쎈쐻占쎈쑞占쎄텛占쎈쐻占쎈쑕占쎌맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슪�맊�굢怨ㅼ삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩諭울옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윞占쎄틢占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윞�굜�쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥占쎄광占쎈쐻占쎈윞占쎈릊占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩紐욑옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윪筌뤵룇�삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥釉붾���뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞�굜�껊쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥�댆琉룸쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럡�뒋�뜝�럡�꽫�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�돲占쎈닑筌앸ŀ�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윞占쎄섐占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮝占쎈쑏筌믩끃�굲占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅勇싲즾踰귨옙�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럩�꼥占쎈쐻占쎈짗占쎌굲占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮됵옙�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윲�뜝�럥�맶�뜝�럥�쑅�뜝�럡�떙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�뜫猷귨옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빢�삕占쎌맶�뜝�럥�쑅嶺뚮슢寃�占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅占쎌젂占쎄퐩占쎌맶�뜝�럥�쐾�뜝�룞�삕�뜝�룞�뵾占쎌뒧占쎈뎐�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮占썲뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌앹뼲彛쀯옙堉꿨퐲�꺈�삕�뙴�뵃�삕占쎄뎡占쎈눇�뙼蹂��굲�솾�봾�꺋占쎌굲�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁쉩�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉐뜝�떥�궡留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛嶺뚯옓�렰占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뛾占쏙옙�뒻占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌몞�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
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