#include "top.hh"
#include <vector>
#include <algorithm>


using namespace std;


Artifact GenRandArtf_1()
{
	ArtFlower returnArtf = ArtFlower();
	returnArtf.Generation();
	return (Artifact)returnArtf;
}


Artifact GenRandArtf_2()
{
	ArtFeather returnArtf = ArtFeather();
	returnArtf.Generation();
	return (Artifact)returnArtf;
}


Artifact GenRandArtf_3()
{
	ArtClock returnArtf = ArtClock();
	returnArtf.Generation();
	return (Artifact)returnArtf;
}


Artifact GenRandArtf_4()
{
	ArtCup returnArtf = ArtCup();
	returnArtf.Generation();
	return (Artifact)returnArtf;
}


Artifact GenRandArtf_5()
{
	ArtCrown returnArtf = ArtCrown();
	returnArtf.Generation();
	return (Artifact)returnArtf;
}


Artifact GenerateRandomArtifact()
{
	// 20% 확률
	int temp = gRandom->Integer(5);

	switch (temp)
	{
	case 0: return GenRandArtf_1(); // 깃털 랜덤 생성
	case 1: return GenRandArtf_2(); // 이하 동문
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

	// 해당 주옵션이 데미지에 영향을 주는지 체크
	if (CheckEffectiveOption(character, mainType) == false) return false; // 안주면 패스

	// 해당 부옵션에서 데미지에 영향을 주는 애들만 모은 다음에
		// 기존 리스트에 이보다 더 좋은 성유물이 있는지 체크
			// 있으면 패스
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
	int numType = gennedArtifact.GetType();
	int indexList[4] = { 0 };
	int CaseList[5][4] = { { 2, 3, 4, 5 },
							{ 1, 3, 4, 5 },
							{ 1, 2, 4, 5 },
							{ 1, 2, 3, 5 },
							{ 1, 2, 3, 4 } };
	std::copy_n(std::begin(CaseList[numType]), 4, std::begin(indexList));

	int len1 = ArtifactSuperList[indexList[1]].size();
	int len2 = ArtifactSuperList[indexList[2]].size();
	int len3 = ArtifactSuperList[indexList[3]].size();
	int len4 = ArtifactSuperList[indexList[4]].size();

	for (int i1 = 0; i1 < len1; i1++)
	{
		for (int i2 = 0; i2 < len2; i2++)
		{
			for (int i3 = 0; i3 < len3; i3++)
			{
				for (int i4 = 0; i4 < len4; i4++)
				{
					/////////////////////////// 내용 필요
				}
			}
		}
	}

}


void AppendArtifactList(Artifact gennedArtifact, vector<vector<Artifact>> ArtifactSuperList)
{
	int numType = gennedArtifact.GetType();
	int index = numType - 1;
	ArtifactSuperList[index].push_back(gennedArtifact);
}


void Simulator()
{
	gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);

	Character* simChar = new Ningguang();

	// 각 부위별 리스트 1 ~ 5
	vector<Artifact> ArtifactList1 = {};
	vector<Artifact> ArtifactList2 = {};
	vector<Artifact> ArtifactList3 = {};
	vector<Artifact> ArtifactList4 = {};
	vector<Artifact> ArtifactList5 = {};
	vector<vector<Artifact>> ArtifactSuperList 
		= { ArtifactList1, ArtifactList2, ArtifactList3, ArtifactList4, ArtifactList5 };

	// simulation 횟수
	int simNum = 100;
	
	// 성유물 캔 갯수
	constexpr int artifactNum = 10; // 한달 ~ 300개

	// N회-histogram
	TH1D* N_Histogram[artifactNum];
	for (int i = 0; i < artifactNum; i++)
	{
		N_Histogram[i] = new TH1D(Form("%s-th trial", i+1), "", artifactNum, 0, artifactNum);
	}

	for (int i = 0; i < simNum; i++)
	{
		// 해당 번째의 시뮬레이션에서의 bestScore.
		double bestScore = 0;

		for (int j = 0; j < artifactNum; j++)
		{
			// 랜덤 성유물 생성 : 부위 별 20%
			Artifact gennedArtifact = GenerateRandomArtifact();

			// 성유물 타입에 맞춰서 추가할 것인지 확인
			bool whetherAppend = CheckWhetherAppend(simChar, gennedArtifact, ArtifactSuperList);
			if (whetherAppend)
			{
				// 나머지 4부위와의 비교 시작
				double comparedScore = CalLoopArtifact(simChar, gennedArtifact, ArtifactSuperList);
				
				// 해당 최고 점수 업데이트
				if (comparedScore >= bestScore)
				{
					bestScore = comparedScore; 
				}

				// 타입에 맞게 리스트에 추가
				AppendArtifactList(gennedArtifact, ArtifactSuperList);
			}
			
			// N회-histogram 에 추가
			N_Histogram[i]->Fill(bestScore);
		}	
	}



	/////////////// N개의 1D histogram을 2D로 변환해야함
	/////////////// 그리고 평균 및 통계처리를 해서 1D Histogram으로 바꿔야함.
	/////////////// 아마 평균과 표준편차의 변화를 보는 것이 유의미 할 것.


	

	TCanvas* can1 = new TCanvas("canvas", "canvas", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

}