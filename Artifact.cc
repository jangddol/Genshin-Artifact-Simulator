#include "Artifact.hh"
#include <algorithm>


void Artifact::FullMainOption(int mainType)
{
	mMainStat.SetOption(mainType, mMaxMainOptionList[mainType]);
}


int Artifact::UseCummulatedWeight(vector<int> cummulatedWeight)
{
	// generate random integer from 0 to the sum of probability table
	int length = cummulatedWeight.size();
	int tempInt = gRandom->Integer(cummulatedWeight[length - 1]) + 1;

	int selectedInt = 0;
	int beforeElement = 0;
	int nowElement = 0;
	for (int i = 0; i < length; i++) // for문을 돌리면서 대소비교를 한다.
	{
		nowElement = cummulatedWeight[i];
		if (tempInt > beforeElement && tempInt <= nowElement)
		{
			selectedInt = i; // 랜덤 int를 가지고 어느 주옵인지 결정한다.
			break;
		}
		beforeElement = nowElement;
	}
	return selectedInt;
}


void Artifact::GenerateMainOption()
{
	int selectedInt = UseCummulatedWeight(mCummulatedWeight);
	mMainType = selectedInt;
	FullMainOption(selectedInt); //////////// 안묶여있는게 좀 그렇네요.
}


vector<int> Artifact::GenerateCummulatedWeight()
{	
	vector<int> returnList = mSubOptProb;
	if ((mMainType >= 0) && (mMainType < 10)) returnList[mMainType] = 0;
	for (int i = 1; i < 10; i++)
	{
		returnList[i] += returnList[i - 1];
	}
	return returnList;
}


bool Artifact::Selected3or4OptStart()
{
	if (gRandom->Integer(5) == 0) return true;
	else return false;
}


bool CheckIsThereIn(int element, int* list, int length)
{
	bool returnBool = false;
	for (int i = 0; i < length; i++)
	{
		if (element == list[i])
		{
			returnBool = true;
			break;
		}
	}
	return returnBool;
}


bool CheckIsThereIn(int element, vector<int> list)
{
	bool returnBool = false;
	int length = list.size();
	for (int i = 0; i < length; i++)
	{
		if (element == list[i])
		{
			returnBool = true;
			break;
		}
	}
	return returnBool;
}


vector<int> Artifact::GenerateStartOpt(vector<int> cummulatedWeight)
{
	vector<int> returnList(4);
	returnList[0] = UseCummulatedWeight(cummulatedWeight);
	for (int i = 1; i < 4; i++)
	{
		int temp = UseCummulatedWeight(cummulatedWeight);
		while (CheckIsThereIn(temp, returnList))
		{
			temp = UseCummulatedWeight(cummulatedWeight);
		}
		returnList[i] = temp;
	}
	return returnList;
}


void Artifact::UpgradeSubOption(vector<int> startOptList, bool whether4OptStart)
{
	int numUpgrade = 4;
	if (whether4OptStart) numUpgrade = 5;

	// 각각 1회씩
	for (int i = 0; i < 4; i++)
	{
		int randomNum = gRandom->Integer(4);
		int optIndex = startOptList[i];
		double randomStat = mOptionArray[optIndex][randomNum];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// 랜덤으로 numUpgrade만큼
	for (int i = 0; i < numUpgrade; i++)
	{
		int randomNum1 = gRandom->Integer(4);
		int randomNum2 = gRandom->Integer(4);
		int randomIndex = startOptList[randomNum1];
		double randomStat = mOptionArray[randomIndex][randomNum2];
		mSubStat.AddOption(randomIndex, randomStat);
	}
}


void Artifact::GenerateSubOption()
{
	vector<int> subCummulatedWeight = GenerateCummulatedWeight();
		// 1. 메인옵션을 확인해서 확률표에서 해당 부분을 0으로 만든다.
			// 1-1. 이걸 가지고 cummulatedWeight을 만든다.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. 처음에 3개인지 4개인지 고른다. -> 8개 or 9개

	vector<int> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. 처음 옵션 4개가 무엇인지 결정한다. 4개를 겹치지 않게 생성한다.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. 기존 4개를 랜덤으로 각각 1회 고정에 랜덤으로 4회 또는 5회 증가시킨다.
}


void Artifact::Generation()
{
	GenerateMainOption(); // 메인옵션 : 부위마다 다름.
	GenerateSubOption(); // 부옵션 : 부위마다, 메인옵션마다 다름.
}