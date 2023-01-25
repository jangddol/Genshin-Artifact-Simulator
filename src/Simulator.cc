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
	int temp = uni5(rng);

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


double Simulator::CalLoopArtifact(Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList,
	ArtifactBundle& bestTryArtifacts)
{
	std::chrono::system_clock::time_point CALLOOPSTART, CALLOOPFINISH;
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


	std::for_each(loopList.flower.begin(), loopList.flower.end(), [&](ArtFlower* flower) {
        mCharacter->SetArtFlower(flower);
		std::for_each(loopList.feather.begin(), loopList.feather.end(), [&](ArtFeather* feather) {
            mCharacter->SetArtFeather(feather);
			std::for_each(loopList.clock.begin(), loopList.clock.end(), [&](ArtClock* clock) {
                mCharacter->SetArtClock(clock);
				std::for_each(loopList.cup.begin(), loopList.cup.end(), [&](ArtCup* cup) {
                    mCharacter->SetArtCup(cup);
					std::for_each(loopList.crown.begin(), loopList.crown.end(), [&](ArtCrown* crown) {
                        mCharacter->SetArtCrown(crown);
						
						// Initialize the character
						CALLOOPSTART = std::chrono::system_clock::now();
						mCharacter->Update();
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


void Simulator::PrintLastArtifacts(int trialNum, double bestDamage, ArtifactBundle bestArtifacts)
{
	if (mSeeLastArtifact)
	{
		std::cout << trialNum << "-th result =============================================" << std::endl;
		std::cout << "bestDamage : " << bestDamage << std::endl;
		std::cout << "============== Last Artifacts ===============" << std::endl;
		PrintArtifact(*(bestArtifacts.flower));
		PrintArtifact(*(bestArtifacts.feather));
		PrintArtifact(*(bestArtifacts.clock));
		PrintArtifact(*(bestArtifacts.cup));
		PrintArtifact(*(bestArtifacts.crown));
		std::cout << "============== Character Stat ===============" << std::endl;
		PrintStat(mCharacter->GetStat());
	}
}


void Simulator::PrintTimeConsumption()
{
	if (mSeeTimeConsumption && !mWorkerMode)
    {
        std::cout << mTimeList[0] << "s, " << mTimeList[1] << "s, " << mTimeList[2] << "s, " << mTimeList[3] << "s" << std::endl;
        std::cout << mCalLoopTimeList[0] << "s, " << mCalLoopTimeList[1] << "s" << std::endl;
    }
}


void Simulator::PrintProgress(int trial, int nowArtNum, int simNum, int artifactNum)
{
	double beforePercent = (double)(trial * artifactNum + nowArtNum - 1)/(double)(simNum * artifactNum) * 100.;
	double percent = (double)(trial * artifactNum + nowArtNum)/(double)(simNum * artifactNum) * 100.;
	if (!mWorkerMode)
	{
		if ((int)beforePercent != (int)percent) std::cout << (int)percent << "% end" << std::endl;
	}
	else
	{
		int firstColumnWidth = 18;
		int columnWidth = 9;
		if (((int)beforePercent != (int)percent) && !DEBUGMODE)
		{
			printToCoordinates(2, 1 + firstColumnWidth + mWorkerID * columnWidth, "|%d %%", (int)percent);
		}
	}

}


TH2D* Simulator::RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage,
								TString histName)
{
	SuperArtifactList artifactSuperList;

	// Histogram array to store results of each simulation run
    if (histName == "") histName = "SIMULATOR_RUNSIMULATION_RESULT";
	TH2D* VisualHistogram = new TH2D(histName, "", artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);

	// Code Execution Time Evaluation
	for (int i = 0; i < 4; i++) mTimeList[i] = 0.;
	for (int i = 0; i < 2; i++) mCalLoopTimeList[i] = 0.;
	std::chrono::system_clock::time_point start, finish;

	mAppendableRate = {};
	mAppendableRate.reserve(artifactNum);
	mAppendableRate.resize(artifactNum);

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
				start = std::chrono::system_clock::now(); 
				double comparedDamage = CalLoopArtifact(gennedArtifact, artifactSuperList, bestTryArtifacts);
				finish  = std::chrono::system_clock::now();
				mTimeList[2] += std::chrono::duration<double>(finish - start).count();

				if (comparedDamage >= bestDamage)
				{
					bestDamage = comparedDamage;
					bestArtifacts = bestTryArtifacts;
				}
				
				start = finish; 
				AppendArtifactList(gennedArtifact, artifactSuperList);
				finish  = std::chrono::system_clock::now();
				mTimeList[3] += std::chrono::duration<double>(finish - start).count();

				numAppend++;
			}
			else delete gennedArtifact;

			VisualHistogram->Fill(j + 0.5, bestDamage);
			// N_Histogram[j]->Fill(bestDamage);

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

	if (DEBUGMODE) cout << "VisualHistogram->GetBinContent(artifactNum, (int)(0.7*binNum))" << VisualHistogram->GetBinContent(artifactNum, (int)(0.7*binNum)) << endl;

    return VisualHistogram;
}


void Simulator::SimulationWorker(int workerID, int simNum, int artifactNum, int binNum, double minDamage, double maxDamage)
{
	mCharacter->MakeEffectionArray();
	// simulation number
	simNum = simNum / mNumThread;
	
	SetWorkerID(workerID);

	TString histName = Form("SIMULATOR_RUNSIMULATION_RESULT-%d", workerID);

	if (DEBUGMODE) cout << "histName : " << histName << endl;

	TH2D* VisualHistogram = RunSimulation(simNum, artifactNum, binNum, minDamage, maxDamage, histName);
	// printToCoordinates(13 + workerID, 1, "mTimeList Pointer : %p", mTimeList);
	if (DEBUGMODE) cout << "Histogram Generation Done." << endl;
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
	}

	if (DEBUGMODE)
	{
		for (int i = 0; i < mNumThread; i++)
		{
			cout << "Character Pointer" << characterVector[i] << endl;
			cout << "Simulaotr Pointer" << &(simulatorVector[i]) << endl;
		}
	}

	// terminal clear for Info Print out
	
	int firstColumnWidth = 18;
	int columnWidth = 9;
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
			printToCoordinates(1, 1 + firstColumnWidth + i * columnWidth, "|# %d", i + 1);
		}
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
	
	if (!DEBUGMODE) 
	{
		for (int i = 0; i < mNumThread; i++)
		{
			printToCoordinates(5, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetTimeList(0));
			printToCoordinates(6, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetTimeList(1));
			printToCoordinates(7, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetTimeList(2));
			printToCoordinates(8, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetTimeList(3));
			printToCoordinates(10, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(0));
			printToCoordinates(11, 1 + firstColumnWidth + i * columnWidth, "|%.1f s", simulatorVector[i].GetCalLoopTimeList(1));
		}
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
	
	if (DEBUGMODE)
	{
		cout << "tempVector : size = " << tempVector.size() << endl;
		cout << "tempVector : first element = " << tempVector[0] << endl;
		for (int i = 0; i < mNumThread; i++)
		cout << Form("simulatorVector[%d].GetSimulationResult() : ", i) << simulatorVector[i].GetSimulationResult() << endl;
	}

	if (DEBUGMODE) cout << "10" << endl;
	// Sum over of all histograms
	for (int i = 0; i < mNumThread; i++)
	{
		HistogramArray.emplace_back(simulatorVector[i].GetSimulationResult());
	}

	if (DEBUGMODE) cout << "11" << endl;

	TH2D* VisualHistogram = new TH2D("VisualHistogram", "", artifactNum, 0, artifactNum, binNum, minDamage, maxDamage);
	for (int i = 0; i < mNumThread; i++)
	{
		VisualHistogram->Add(HistogramArray[i]);
	}

	if (DEBUGMODE) cout << "12" << endl;

	printToCoordinates(12, 1, "");
	return VisualHistogram;
}