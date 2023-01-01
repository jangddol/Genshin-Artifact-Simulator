#include "RandomInitialization.hh"
#include "Artifact.hh"
#include <algorithm>
#include <iostream>


void PrintArtifact(Artifact artifact)
{
    cout << "Artifact Type      : " << artifact.GetType() << endl;
    cout << "Artifact Main Stat : " << STATSTRING[artifact.GetMainType()] << " = " << artifact.GetMainStat().GetOption(artifact.GetMainType()) << endl;
    cout << "Artifcat Sub Stat  : " << STATSTRING[0] << " = " << artifact.GetSubStat().GetOption(0) << endl;
    cout << "                   : " << STATSTRING[1] << " = " << artifact.GetSubStat().GetOption(1) << endl;
    cout << "                   : " << STATSTRING[2] << " = " << artifact.GetSubStat().GetOption(2) << endl;
    cout << "                   : " << STATSTRING[3] << " = " << artifact.GetSubStat().GetOption(3) << endl;
    cout << "                   : " << STATSTRING[4] << " = " << artifact.GetSubStat().GetOption(4) << endl;
    cout << "                   : " << STATSTRING[5] << " = " << artifact.GetSubStat().GetOption(5) << endl;
    cout << "                   : " << STATSTRING[6] << " = " << artifact.GetSubStat().GetOption(6) << endl;
    cout << "                   : " << STATSTRING[7] << " = " << artifact.GetSubStat().GetOption(7) << endl;
    cout << "                   : " << STATSTRING[8] << " = " << artifact.GetSubStat().GetOption(8) << endl;
    cout << "                   : " << STATSTRING[9] << " = " << artifact.GetSubStat().GetOption(9) << endl;
}


void Artifact::FullMainOption(int mainType)
{
	mMainStat.SetOption(mainType, MAXMAINOPTIONLIST[mainType]);
}


int Artifact::UseCummulatedWeight(vector<int> cummulatedWeight)
{
	// generate random integer from 0 to the sum of probability table
	int length = cummulatedWeight.size();
	int tempInt = gRandom->Integer(cummulatedWeight[length - 1]) + 1;

	int selectedInt = 0;
	int beforeElement = 0;
	int nowElement = 0;
	for (int i = 0; i < length; i++) // for���� �����鼭 ��Һ񱳸� �Ѵ�.
	{
		nowElement = cummulatedWeight[i];
		if (tempInt > beforeElement && tempInt <= nowElement)
		{
			selectedInt = i; // ���� int�� ������ ��� �ֿ����� �����Ѵ�.
			break;
		}
		beforeElement = nowElement;
	}
	return selectedInt;
}


void Artifact::SetMainType(int mainType)
{
	mMainStat.SetZero();
	mMainStat.SetOption(mainType, MAXMAINOPTIONLIST[mainType]);
	mMainType = mainType;
}


void Artifact::GenerateMainOption()
{
	int selectedInt = UseCummulatedWeight(mCummulatedWeight);
	mMainType = selectedInt;
	FullMainOption(selectedInt); //////////// �ȹ����ִ°� �� �׷��׿�.
}


vector<int> Artifact::GenerateCummulatedWeight()
{	
	vector<int> returnList(10);
	for (int i = 0; i < 10; i++)
	{
		returnList[i] = SUBOPTPROB[i];
	} 
	if ((mMainType >= 0) && (mMainType < 10)) returnList[mMainType] = 0;
	for (int i = 1; i < 10; i++)
	{
		returnList[i] += returnList[i - 1];
	}
	return returnList;
}


bool Artifact::Selected3or4OptStart()
{
	if (uni(rng) == 0) return true;
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

	// ���� 1ȸ��
	for (int i = 0; i < 4; i++)
	{
		int optIndex = startOptList[i];
		double randomStat = OPTIONARRAY[optIndex][uni4(rng)];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// �������� numUpgrade��ŭ
	for (int i = 0; i < numUpgrade; i++)
	{
		int randomIndex = startOptList[uni4(rng)];
		double randomStat = OPTIONARRAY[randomIndex][uni4(rng)];
		mSubStat.AddOption(randomIndex, randomStat);
	}
}


void Artifact::GenerateSubOption()
{
	vector<int> subCummulatedWeight = GenerateCummulatedWeight();
		// 1. ���οɼ��� Ȯ���ؼ� Ȯ��ǥ���� �ش� �κ��� 0���� �����.
			// 1-1. �̰� ������ cummulatedWeight�� �����.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. ó���� 3������ 4������ ����. -> 8�� or 9��

	vector<int> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. ó�� �ɼ� 4���� �������� �����Ѵ�. 4���� ��ġ�� �ʰ� �����Ѵ�.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. ���� 4���� �������� ���� 1ȸ ������ �������� 4ȸ �Ǵ� 5ȸ ������Ų��.
}


void Artifact::Generation()
{
	GenerateMainOption(); // ���οɼ� : �������� �ٸ�.
	GenerateSubOption(); // �οɼ� : ��������, ���οɼǸ��� �ٸ�.
}


void Artifact::Generation(int mainType)
{
	SetMainType(mainType);
	GenerateSubOption();
}