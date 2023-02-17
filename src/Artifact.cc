#include "../header/RandomInitialization.hh"
#include "../header/Artifact.hh"
#include "../header/Character.hh"
#include <algorithm>
#include <iostream>
#include <iomanip>  // for std::setw


void PrintArtifact(const Artifact* artifact)
{
    int type = artifact->GetType();
    int mainType = artifact->GetMainType();
    std::cout << "Type : " << ARTIFACTTYPESTRING[type] << std::endl;
    std::cout << "Main Option : " << STATSTRING[mainType] << " : " << std::setprecision(2) << std::fixed << artifact->GetMainStat().GetOption(mainType) << std::endl;
    std::cout << "SubStat" << std::endl;

	int category_column_widths = 0;
    for (int i = 0; i < 9; i++)
	{
        int category_width = STATSTRING[i].length();
        if (category_width > category_column_widths) category_column_widths = category_width;
    }
    int value_column_widths = 11;

    for (int i = 0; i < 5; i++)
	{
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << artifact->GetSubStat().GetOption(i);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i+5] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << artifact->GetSubStat().GetOption(i+5);
        std::cout << std::endl;
	}
}


Artifact::Artifact(const Artifact* artifact)
{
	mMainStat = artifact->GetMainStat();
	mSubStat = artifact->GetSubStat();
	mMainType = artifact->GetMainType();
}


int Artifact::UseCummulatedWeight(std::vector<int> cummulatedWeight)
{
	// generate random integer from 0 to the sum of probability table
	std::size_t length = cummulatedWeight.size();
	
	int tempInt;
	if (cummulatedWeight[length - 1] == 1) tempInt = 1;
	else tempInt = GetRdUni(cummulatedWeight[length - 1]) + 1;

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
	AlertModified();
}


void Artifact::GenerateMainOption()
{
	int selectedInt = UseCummulatedWeight(mCummulatedWeight);
	// if (selectedInt > 100) cout << "1" << endl;
	SetMainType(selectedInt);
}


std::vector<int> Artifact::GenerateCummulatedWeight()
{	
	std::vector<int> returnList(10);
	for (int i = 0; i < 10; i++)
	{
		returnList[i] = SUBOPTPROB[i];
	} 
	if ((mMainType >= 0) && (mMainType < 10) && (mMainType != 3) && (mMainType != 6)) returnList[mMainType] = 0;
	for (int i = 1; i < 10; i++)
	{
		returnList[i] += returnList[i - 1];
	}
	return returnList;
}


bool Artifact::Selected3or4OptStart()
{
	if (GetRdUni5() == 0) return true;
	else return false;
}


bool Artifact::IsUsingThis(const Character* character) const
{
	bool returnBool = false;
	int length = mCharactersUsingThis.size();
	for (int i = 0; i < length; i++)
	{
		if (character == mCharactersUsingThis[i])
		{
			returnBool = true;
			break;
		}
	}
	return returnBool;
}


bool CheckIsThereIn(int element, std::array<int, 4> list)
{
	bool returnBool = false;
	for (int i = 0; i < 4; i++)
	{
		if (element == list[i])
		{
			returnBool = true;
			break;
		}
	}
	return returnBool;
}


std::array<int, 4> Artifact::GenerateStartOpt(std::vector<int> cummulatedWeight)
{
	std::array<int, 4> returnList = { -1, -1, -1, -1 };
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


void Artifact::UpgradeSubOption(std::array<int, 4> startOptList, bool whether4OptStart)
{
	int numUpgrade = 4;
	if (whether4OptStart) numUpgrade = 5;

	// ���� 1ȸ��
	for (int i = 0; i < 4; i++)
	{
		int optIndex = startOptList[i];
		double randomStat = OPTIONARRAY[optIndex][GetRdUni4()];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// �������� numUpgrade��ŭ
	for (int i = 0; i < numUpgrade; i++)
	{
		int randomIndex = startOptList[GetRdUni4()];
		double randomStat = OPTIONARRAY[randomIndex][GetRdUni4()];
		mSubStat.AddOption(randomIndex, randomStat);
	}
}


void Artifact::GenerateSubOption()
{
	std::vector<int> subCummulatedWeight = GenerateCummulatedWeight();
		// 1. ���οɼ��� Ȯ���ؼ� Ȯ��ǥ���� �ش� �κ��� 0���� �����.
			// 1-1. �̰� ������ cummulatedWeight�� �����.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. ó���� 3������ 4������ ����. -> 8�� or 9��
	std::array<int, 4> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. ó�� �ɼ� 4���� �������� �����Ѵ�. 4���� ��ġ�� �ʰ� �����Ѵ�.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. ���� 4���� �������� ���� 1ȸ ������ �������� 4ȸ �Ǵ� 5ȸ ������Ų��.
}


void Artifact::Generation()
{
	mMainStat.SetZero();
	mSubStat.SetZero();
	GenerateMainOption(); // ���οɼ� : �������� �ٸ�.
	GenerateSubOption(); // �οɼ� : ��������, ���οɼǸ��� �ٸ�.
	AlertModified();
}


void Artifact::Generation(int mainType)
{
	mMainStat.SetZero();
	mSubStat.SetZero();
	SetMainType(mainType);
	GenerateSubOption();
	AlertModified();
}


void Artifact::SaveCharacterPointer(Character* character)
{
    mCharactersUsingThis.emplace_back(character);
}


void Artifact::DeleteCharacterPointer(const Character* character)
{
    int index = 0;
    int size = mCharactersUsingThis.size();
	if (size == 0) return;
	if (size == 1 && mCharactersUsingThis[0] == character) mCharactersUsingThis.erase(mCharactersUsingThis.begin());
	else mCharactersUsingThis.erase(remove(mCharactersUsingThis.begin(), mCharactersUsingThis.end(), character), mCharactersUsingThis.end());
}


void Artifact::AlertModified()
{
	for(auto& character : mCharactersUsingThis)
    {
        character->ConfirmArtifactMainStatModified();
	}
}
