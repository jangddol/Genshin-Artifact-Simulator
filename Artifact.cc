#include "Artifact.hh"
#include <algorithm>


void Artifact::FullMainOption(int mainType)
{
	mMainStat.SetOption(mainType, mMaxMainOptionList[mainType]);
}


int Artifact::UseCummulatedWeight(int cummulatedWeight[19])
{
	int tempInt = gRandom->Integer(cummulatedWeight[18]) + 1; // 1���� Ȯ��ǥ �ձ����� ���� int�� �����.
	int selectedInt = -1;
	for (int i = 0; i < 19; i++) // for���� �����鼭 ��Һ񱳸� �Ѵ�.
	{
		int ithWeight = cummulatedWeight[i];
		if (ithWeight <= tempInt)
		{
			selectedInt = i; // ���� int�� ������ ��� �ֿ����� �����Ѵ�.
			break;
		}
	}
	return selectedInt;
}


void Artifact::GenerateMainOption()
{
	int selectedInt = UseCummulatedWeight(mCummulatedWeight);
	mMainType = selectedInt;
	FullMainOption(selectedInt); //////////// �ȹ����ִ°� �� �׷��׿�.
}


int* Artifact::GenerateCummulatedWeight()
{
	int returnList[10];
	std::copy_n(std::begin(mSubOptProb), 10, std::begin(returnList));
	returnList[mMainType] = 0;
	for (int i = 1; i < 10; i++)
	{
		returnList[i] += mSubOptProb[i - 1];
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


int* Artifact::GenerateStartOpt(int cummulatedWeight[19])
{
	UseCummulatedWeight(cummulatedWeight);
	int returnList[4] = { -1, -1, -1, -1 };
	returnList[1] = UseCummulatedWeight(cummulatedWeight);

	for (int i = 2; i < 5; i++)
	{
		int temp = UseCummulatedWeight(cummulatedWeight);
		while (CheckIsThereIn(temp, returnList, 4))
		{
			temp = UseCummulatedWeight(cummulatedWeight);
		}
		returnList[i] = temp;
	}

	return returnList;
}


void Artifact::UpgradeSubOption(int startOptList[4], bool whether4OptStart)
{
	int numUpgrade = 4;
	if (whether4OptStart) numUpgrade = 5;

	// ���� 1ȸ��
	for (int i = 0; i < 4; i++)
	{
		int randomNum = gRandom->Integer(4);
		int optIndex = startOptList[i];
		int randomStat = mOptionArray[optIndex][randomNum];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// �������� numUpgrade��ŭ
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
	int* cummulatedWeight = GenerateCummulatedWeight();
		// 1. ���οɼ��� Ȯ���ؼ� Ȯ��ǥ���� �ش� �κ��� 0���� �����.
			// 1-1. �̰� ������ cummulatedWeight�� �����.
	bool whether4OptStart = Selected3or4OptStart();
		// 2. ó���� 3������ 4������ ����. -> 8�� or 9��
	int* startOptList = GenerateStartOpt(cummulatedWeight);
		// 3. ó�� �ɼ� 4���� �������� �����Ѵ�. 4���� ��ġ�� �ʰ� �����Ѵ�.
	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. ���� 4���� �������� ���� 1ȸ ������ �������� 4ȸ �Ǵ� 5ȸ ������Ų��.
}


void Artifact::Generation()
{
	GenerateMainOption(); // ���οɼ� : �������� �ٸ�
	GenerateSubOption(); // �οɼ� : ��������, ���οɼǸ��� �ٸ�.
}