#include "Artifact.hh"
#include <algorithm>


void Artifact::FullMainOption(int mainType)
{
	mMainStat.SetOption(mainType, mMaxMainOptionList[mainType]);
}


int Artifact::UseCummulatedWeight(int* cummulatedWeight, int length)
{
	// generate random integer from 0 to the sum of probability table
	int tempInt = gRandom->Integer(cummulatedWeight[length - 1]) + 1;  
	int selectedInt = -1;
	for (int i = 0; i < length; i++) // for문을 돌리면서 대소비교를 한다.
	{
		int ithWeight = cummulatedWeight[i];
		if (ithWeight <= tempInt)
		{
			selectedInt = i; // 랜덤 int를 가지고 어느 주옵인지 결정한다.
			break;
		}
	}
	return selectedInt;
}


void Artifact::GenerateMainOption()
{
	int selectedInt = UseCummulatedWeight(mCummulatedWeight, 19);
	mMainType = selectedInt;
	FullMainOption(selectedInt); //////////// 안묶여있는게 좀 그렇네요.
}


void Artifact::GenerateCummulatedWeight(int* oList)
{	
	std::copy_n(std::begin(mSubOptProb), 10, oList);
	oList[mMainType] = 0;
	for (int i = 1; i < 10; i++)
	{
		oList[i] += oList[i - 1];
	}
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


void Artifact::GenerateStartOpt(int* oList, int cummulatedWeight[10])
{
	oList[0] = UseCummulatedWeight(cummulatedWeight, 10);
	cout << "oList[0] : " << oList[0] << endl;
	for (int i = 1; i < 4; i++)
	{
		int temp = UseCummulatedWeight(cummulatedWeight, 10);
		cout << "temp : " << temp << endl;
		while (CheckIsThereIn(temp, oList, 4)) ///////////////////////////////// stuck in here
		{
			temp = UseCummulatedWeight(cummulatedWeight, 10);
		}
		oList[i] = temp;
	}
}


void Artifact::UpgradeSubOption(int startOptList[4], bool whether4OptStart)
{
	int numUpgrade = 4;
	if (whether4OptStart) numUpgrade = 5;

	// 각각 1회씩
	for (int i = 0; i < 4; i++)
	{
		int randomNum = gRandom->Integer(4);
		int optIndex = startOptList[i];
		int randomStat = mOptionArray[optIndex][randomNum];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// 랜덤으로 numUpgrade만큼
	for (int i = 0; i < numUpgrade; i++)
	{
		int randomNum1 = gRandom->Integer(4);
		int randomNum2 = gRandom->Integer(4);
		int randomIndex = startOptList[randomNum1];
		int randomStat = mOptionArray[randomIndex][randomNum2];
		mSubStat.AddOption(randomIndex, randomStat);
	}
}


void Artifact::GenerateSubOption()
{
	int subCummulatedWeight[10] = { 0 };

	cout << "Start to generate CummulatedWeight" << endl;

	GenerateCummulatedWeight(subCummulatedWeight);
		// 1. 메인옵션을 확인해서 확률표에서 해당 부분을 0으로 만든다.
			// 1-1. 이걸 가지고 cummulatedWeight을 만든다.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.
	
	cout << "Finish to generate CummulatedWeight" << endl;
	cout << "subCummulatedWeight : " << subCummulatedWeight[0] << ", "
									<< subCummulatedWeight[1] << ", "
									<< subCummulatedWeight[2] << ", "
									<< subCummulatedWeight[3] << ", "
									<< subCummulatedWeight[4] << ", "
									<< subCummulatedWeight[5] << ", "
									<< subCummulatedWeight[6] << ", "
									<< subCummulatedWeight[7] << ", "
									<< subCummulatedWeight[8] << ", "
									<< subCummulatedWeight[9] << endl;
	bool whether4OptStart = Selected3or4OptStart();
		// 2. 처음에 3개인지 4개인지 고른다. -> 8개 or 9개
	
	cout << "Finish to decide whether 4 Option started or not" << endl;

	int startOptList[4] = { -1, -1, -1, -1 };
	GenerateStartOpt(startOptList, subCummulatedWeight);
		// 3. 처음 옵션 4개가 무엇인지 결정한다. 4개를 겹치지 않게 생성한다.
	
	cout << "Finish to generate Start Options" << endl;

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. 기존 4개를 랜덤으로 각각 1회 고정에 랜덤으로 4회 또는 5회 증가시킨다.
	
	cout << "Finish to generate Upgrade SubOptions" << endl;
}


void Artifact::Generation()
{
	cout << "Start to generate Artifact" << endl;
	
	GenerateMainOption(); // 메인옵션 : 부위마다 다름

	cout << "Finish to generate MainOption" << endl;

	GenerateSubOption(); // 부옵션 : 부위마다, 메인옵션마다 다름.

	cout << "Finish to generate SubOption" << endl;
}