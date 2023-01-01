#include "top.hh"
#include <cstdio>
#include <vector>
#include <algorithm>


using namespace std;


Artifact GenRandArtf_1()
{
	ArtFlower tempArtf = ArtFlower();
	tempArtf.Generation();
	Artifact returnArtf = *(Artifact*)&tempArtf;
	return returnArtf;
}


Artifact GenRandArtf_2()
{
	ArtFeather tempArtf = ArtFeather();
	tempArtf.Generation();
	Artifact returnArtf = *(Artifact*)&tempArtf;
	return returnArtf;
}


Artifact GenRandArtf_3()
{
	ArtClock tempArtf = ArtClock();
	tempArtf.Generation();
	Artifact returnArtf = *(Artifact*)&tempArtf;
	return returnArtf;
}


Artifact GenRandArtf_4()
{
	ArtCup tempArtf = ArtCup();
	tempArtf.Generation();
	Artifact returnArtf = *(Artifact*)&tempArtf;
	return returnArtf;
}


Artifact GenRandArtf_5()
{
	ArtCrown tempArtf = ArtCrown();
	tempArtf.Generation();
	Artifact returnArtf = *(Artifact*)&tempArtf;
	return returnArtf;
}


Artifact GenerateRandomArtifact()
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
			return Artifact();
			cout << "Error : gRandom occur some errors at GenerateRandomArtifact" << endl; 
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


bool CheckWhetherAppend(Character* character, Artifact gennedArtifact, vector<vector<Artifact>> ArtifactSuperList)
{
	int numType = gennedArtifact.GetType();
	vector<Artifact> SelectedList = ArtifactSuperList[numType-1];
	int mainType = gennedArtifact.GetMainType();

	if (CheckEffectiveOption(character, mainType) == false && (mainType != 3 && mainType != 6)) return false; 

	int effectiveList[10] = { 0 };
	int effListSize;
	MakeEffectiveOptionList(effectiveList, effListSize, character);

	Stat gennedSubOpt = gennedArtifact.GetSubStat();
	bool returnBool = true;
	for (int i = 0; i < SelectedList.size(); i++)
	{
		Stat tempSubOpt = SelectedList[i].GetSubStat();
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


double CalLoopArtifact(Character* character, Artifact gennedArtifact, vector<vector<Artifact>> ArtifactSuperList)
{
	vector<vector<Artifact>> loopList = ArtifactSuperList;
	loopList[gennedArtifact.GetType() - 1] = { gennedArtifact };

	double tempDamage, bestDamage;
	bestDamage = 0;
	for (int i1 = 0; i1 < loopList[0].size(); i1++)
	{
		for (int i2 = 0; i2 < loopList[1].size(); i2++)
		{
			for (int i3 = 0; i3 < loopList[2].size(); i3++)
			{
				for (int i4 = 0; i4 < loopList[3].size(); i4++)
				{
					for (int i5 = 0; i5 < loopList[4].size(); i5++)
					{
						character->SetArtifact( *(ArtFlower*)&loopList[0][i1],
												*(ArtFeather*)&loopList[1][i2],
												*(ArtClock*)&loopList[2][i3],
												*(ArtCup*)&loopList[3][i4],
												*(ArtCrown*)&loopList[4][i5]);
						character->Initialization();
						tempDamage = character->GetDamage();
						if (tempDamage > bestDamage)
						{
							bestDamage = tempDamage;
							// oCombination = { loopList[0][i1], loopList[1][i2], loopList[2][i3], loopList[3][i4], loopList[4][i5] };
						}
					}
				}
			}
		}
	}
	return bestDamage;
}


vector<vector<Artifact>> AppendArtifactList(Artifact gennedArtifact, vector<vector<Artifact>> ArtifactSuperList)
{
	int numType = gennedArtifact.GetType();
	int index = numType - 1;
	ArtifactSuperList[index].push_back(gennedArtifact);
	return ArtifactSuperList;
}


void Simulator()
{
	// gStyle->SetOptStat(kFALSE);
	// gRandom->SetSeed(0);

	SolarPearl weapon = SolarPearl();
	cout << "weapon generated" << endl;

	Character* simChar = new Ningguang(weapon);
	cout << "character generated" << endl;
	
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
	vector<Artifact> ArtifactList1 = {};
	vector<Artifact> ArtifactList2 = {};
	vector<Artifact> ArtifactList3 = {};
	vector<Artifact> ArtifactList4 = {};
	vector<Artifact> ArtifactList5 = {};
	vector<vector<Artifact>> ArtifactSuperList 
		= { ArtifactList1, ArtifactList2, ArtifactList3, ArtifactList4, ArtifactList5 };

	// simulation number
	int simNum = 100;
	
	// the number of artifacts to get
	constexpr int artifactNum = 100; // 1 month ~ 300 artifacts
	
	// maxDamage, binNum
	int binNum = 50;
	double maxDamage = 200000.;

	// Nth-histogram
	TH1D* N_Histogram[artifactNum];
	for (int i = 0; i < artifactNum; i++)
	{
		N_Histogram[i] = new TH1D(Form("%d-th trial", i+1), "", binNum, 0, maxDamage);
	}


	// Simulation Part
	for (int i = 0; i < simNum; i++)
	{
		double bestDamage = 0;
		ArtifactSuperList = { ArtifactList1, ArtifactList2, ArtifactList3, ArtifactList4, ArtifactList5 };
		// cout << "==========" << i << "-th simulation start" << "==========" << endl;

		for (int j = 0; j < artifactNum; j++)
		{
			Artifact gennedArtifact = GenerateRandomArtifact();
			
			// cout << "	Artifact Generation End " << endl;
			// cout << "		Artifact MainOption : " << STATSTRING[gennedArtifact.GetMainType()] << " = " << gennedArtifact.GetMainStat().GetOption(gennedArtifact.GetMainType()) << endl;
			// cout << " 		Artifact SubOption  : " << gennedArtifact.GetSubStat().GetOption(0) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(1) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(2) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(3) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(4) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(5) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(6) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(7) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(8) << ", "
			// 										<< gennedArtifact.GetSubStat().GetOption(9) << endl;
			bool whetherAppend = CheckWhetherAppend(simChar, gennedArtifact, ArtifactSuperList);

			// cout << "	Checking Whether Append End : " << whetherAppend << endl;

			if (whetherAppend)
			{				
				double comparedDamage = CalLoopArtifact(simChar, gennedArtifact, ArtifactSuperList);
				
				// cout << "	Artifact Loop Calculation done : comparedDamage = " << comparedDamage << endl;

				if (comparedDamage >= bestDamage)
				{
					bestDamage = comparedDamage; 
				}

				ArtifactSuperList = AppendArtifactList(gennedArtifact, ArtifactSuperList);
			}

			// cout << "	" << i << "-th bestDamage : " << bestDamage << endl;

			N_Histogram[j]->Fill(bestDamage);

			double beforePercent = (double)(i * artifactNum + j - 1)/(double)(simNum * artifactNum) * 100.;
			double percent = (double)(i * artifactNum + j)/(double)(simNum * artifactNum) * 100.;
			if ((int)beforePercent != (int)percent) cout << (int)percent << "% end" << endl;
		}	
	}

	/*
	// Plot Part
	TH2D* VisualHistogram = new TH2D("Visual", "", artifactNum, 0, artifactNum, binNum, 0, maxDamage);
	for (int i = 0; i < artifactNum; i++)
	{
		for (int j = 0; j < artifactNum; j++)
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
	*/
}