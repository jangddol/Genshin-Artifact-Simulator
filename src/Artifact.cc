#include "../header/RandomInitialization.hh"
#include "../header/Artifact.hh"
#include <algorithm>
#include <iostream>


void PrintArtifact(Artifact artifact)
{
    std::cout << "Artifact Type      : " << artifact.GetType() << std::endl;
    std::cout << "Artifact Main Stat : " << STATSTRING[artifact.GetMainType()] << " = " << artifact.GetMainStat().GetOption(artifact.GetMainType()) << std::endl;
    std::cout << "Artifcat Sub Stat  : " << STATSTRING[0] << " = " << artifact.GetSubStat().GetOption(0) << std::endl;
    std::cout << "                   : " << STATSTRING[1] << " = " << artifact.GetSubStat().GetOption(1) << std::endl;
    std::cout << "                   : " << STATSTRING[2] << " = " << artifact.GetSubStat().GetOption(2) << std::endl;
    std::cout << "                   : " << STATSTRING[3] << " = " << artifact.GetSubStat().GetOption(3) << std::endl;
    std::cout << "                   : " << STATSTRING[4] << " = " << artifact.GetSubStat().GetOption(4) << std::endl;
    std::cout << "                   : " << STATSTRING[5] << " = " << artifact.GetSubStat().GetOption(5) << std::endl;
    std::cout << "                   : " << STATSTRING[6] << " = " << artifact.GetSubStat().GetOption(6) << std::endl;
    std::cout << "                   : " << STATSTRING[7] << " = " << artifact.GetSubStat().GetOption(7) << std::endl;
    std::cout << "                   : " << STATSTRING[8] << " = " << artifact.GetSubStat().GetOption(8) << std::endl;
    std::cout << "                   : " << STATSTRING[9] << " = " << artifact.GetSubStat().GetOption(9) << std::endl;
}


Artifact::Artifact(Artifact *artifact)
{
	mMainStat = artifact->GetMainStat();
	mSubStat = artifact->GetSubStat();
	mMainType = artifact->GetMainType();
}


int Artifact::UseCummulatedWeight(std::vector<int> cummulatedWeight)
{
	// generate random integer from 0 to the sum of probability table
	int length = cummulatedWeight.size();
	
	std::uniform_int_distribution<int> uniTemp(0, cummulatedWeight[length - 1] - 1); // Guaranteed unbiased
	int tempInt = uniTemp(rng) + 1;

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
	if (uni(rng) == 0) return true;
	else return false;
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

	// 각각 1회씩
	for (int i = 0; i < 4; i++)
	{
		int optIndex = startOptList[i];
		double randomStat = OPTIONARRAY[optIndex][uni4(rng)];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// 랜덤으로 numUpgrade만큼
	for (int i = 0; i < numUpgrade; i++)
	{
		int randomIndex = startOptList[uni4(rng)];
		double randomStat = OPTIONARRAY[randomIndex][uni4(rng)];
		mSubStat.AddOption(randomIndex, randomStat);
	}
}


void Artifact::GenerateSubOption()
{
	std::vector<int> subCummulatedWeight = GenerateCummulatedWeight();
		// 1. 메인옵션을 확인해서 확률표에서 해당 부분을 0으로 만든다.
			// 1-1. 이걸 가지고 cummulatedWeight을 만든다.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. 처음에 3개인지 4개인지 고른다. -> 8개 or 9개
	std::array<int, 4> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. 처음 옵션 4개가 무엇인지 결정한다. 4개를 겹치지 않게 생성한다.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. 기존 4개를 랜덤으로 각각 1회 고정에 랜덤으로 4회 또는 5회 증가시킨다.
}


void Artifact::Generation()
{
	mMainStat.SetZero();
	mSubStat.SetZero();
	GenerateMainOption(); // 메인옵션 : 부위마다 다름.
	GenerateSubOption(); // 부옵션 : 부위마다, 메인옵션마다 다름.
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


void Artifact::DeleteCharacterPointer(Character* character)
{
    int index = 0;
    int size = mCharactersUsingThis.size();
    for(int i = 0; i < size; i++)
    {
        if (mCharactersUsingThis[index] == character)
        {
            mCharactersUsingThis.erase(mCharactersUsingThis.begin() + index);
        }
        else index++;
    }
}


void Artifact::AlertModified()
{
	for(auto& character : mCharactersUsingThis)
    {
        character->ConfirmArtifactMainStatModified();
	}
}
