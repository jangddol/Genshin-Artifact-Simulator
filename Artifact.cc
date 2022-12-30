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
	for (int i = 0; i < length; i++) // for���� �����鼭 ��Һ񱳸� �Ѵ�.
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
	int selectedInt = UseCummulatedWeight(mCummulatedWeight, 19);
	mMainType = selectedInt;
	FullMainOption(selectedInt); //////////// �ȹ����ִ°� �� �׷��׿�.
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
	int subCummulatedWeight[10] = { 0 };

	cout << "Start to generate CummulatedWeight" << endl;

	GenerateCummulatedWeight(subCummulatedWeight);
		// 1. ���οɼ��� Ȯ���ؼ� Ȯ��ǥ���� �ش� �κ��� 0���� �����.
			// 1-1. �̰� ������ cummulatedWeight�� �����.
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
		// 2. ó���� 3������ 4������ ����. -> 8�� or 9��
	
	cout << "Finish to decide whether 4 Option started or not" << endl;

	int startOptList[4] = { -1, -1, -1, -1 };
	GenerateStartOpt(startOptList, subCummulatedWeight);
		// 3. ó�� �ɼ� 4���� �������� �����Ѵ�. 4���� ��ġ�� �ʰ� �����Ѵ�.
	
	cout << "Finish to generate Start Options" << endl;

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. ���� 4���� �������� ���� 1ȸ ������ �������� 4ȸ �Ǵ� 5ȸ ������Ų��.
	
	cout << "Finish to generate Upgrade SubOptions" << endl;
}


void Artifact::Generation()
{
	cout << "Start to generate Artifact" << endl;
	
	GenerateMainOption(); // ���οɼ� : �������� �ٸ�

	cout << "Finish to generate MainOption" << endl;

	GenerateSubOption(); // �οɼ� : ��������, ���οɼǸ��� �ٸ�.

	cout << "Finish to generate SubOption" << endl;
}