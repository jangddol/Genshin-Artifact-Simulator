#include "../header/RandomInitialization.hh"
#include "../header/Artifact.hh"
#include <algorithm>
#include <iostream>


void PrintArtifact(const Artifact* artifact)
{
    std::cout << "Artifact Type      : " << artifact->GetType() << std::endl;
    std::cout << "Artifact Main Stat : " << STATSTRING[artifact->GetMainType()] << " = " << artifact->GetMainStat().GetOption(artifact->GetMainType()) << std::endl;
    std::cout << "Artifcat Sub Stat  : " << STATSTRING[0] << " = " << artifact->GetSubStat().GetOption(0) << std::endl;
    std::cout << "                   : " << STATSTRING[1] << " = " << artifact->GetSubStat().GetOption(1) << std::endl;
    std::cout << "                   : " << STATSTRING[2] << " = " << artifact->GetSubStat().GetOption(2) << std::endl;
    std::cout << "                   : " << STATSTRING[3] << " = " << artifact->GetSubStat().GetOption(3) << std::endl;
    std::cout << "                   : " << STATSTRING[4] << " = " << artifact->GetSubStat().GetOption(4) << std::endl;
    std::cout << "                   : " << STATSTRING[5] << " = " << artifact->GetSubStat().GetOption(5) << std::endl;
    std::cout << "                   : " << STATSTRING[6] << " = " << artifact->GetSubStat().GetOption(6) << std::endl;
    std::cout << "                   : " << STATSTRING[7] << " = " << artifact->GetSubStat().GetOption(7) << std::endl;
    std::cout << "                   : " << STATSTRING[8] << " = " << artifact->GetSubStat().GetOption(8) << std::endl;
    std::cout << "                   : " << STATSTRING[9] << " = " << artifact->GetSubStat().GetOption(9) << std::endl;
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
	int length = cummulatedWeight.size();
	
	int tempInt = GetRdUni(cummulatedWeight[length - 1] - 1) + 1;

	int selectedInt = 0;
	int beforeElement = 0;
	int nowElement = 0;
	for (int i = 0; i < length; i++) // for�눧紐꾩뱽 占쎈즼�뵳�됥늺占쎄퐣 占쏙옙占쏙옙�꺖�뜮袁㏉꺍�몴占� 占쎈립占쎈뼄.
	{
		nowElement = cummulatedWeight[i];
		if (tempInt > beforeElement && tempInt <= nowElement)
		{
			selectedInt = i; // 占쎌삏占쎈쑁 int�몴占� 揶쏉옙筌욑옙��⑨옙 占쎈선占쎈뮄 雅뚯눘�긿占쎌뵥筌욑옙 野껉퀣�젟占쎈립占쎈뼄.
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

	// 각각 1회씩
	for (int i = 0; i < 4; i++)
	{
		int optIndex = startOptList[i];
		double randomStat = OPTIONARRAY[optIndex][GetRdUni4()];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// 랜덤으로 numUpgrade만큼
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
		// 1. 筌롫뗄�뵥占쎌긿占쎈�∽옙�뱽 占쎌넇占쎌뵥占쎈퉸占쎄퐣 占쎌넇�몴醫뤿ご占쎈퓠占쎄퐣 占쎈퉸占쎈뼣 �겫占썽겫袁⑹뱽 0占쎌몵嚥∽옙 筌띾슢諭븝옙�뼄.
			// 1-1. 占쎌뵠椰꾬옙 揶쏉옙筌욑옙��⑨옙 cummulatedWeight占쎌뱽 筌띾슢諭븝옙�뼄.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. 筌ｌ꼷�벉占쎈퓠 3揶쏆뮇�뵥筌욑옙 4揶쏆뮇�뵥筌욑옙 ��⑥쥓�뀲占쎈뼄. -> 8揶쏉옙 or 9揶쏉옙
	std::array<int, 4> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. 筌ｌ꼷�벉 占쎌긿占쎈�� 4揶쏆뮄占쏙옙 �눧�똻毓울옙�뵥筌욑옙 野껉퀣�젟占쎈립占쎈뼄. 4揶쏆뮆占쏙옙 野껊���뒄筌욑옙 占쎈륫野껓옙 占쎄문占쎄쉐占쎈립占쎈뼄.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. 疫꿸퀣��� 4揶쏆뮆占쏙옙 占쎌삏占쎈쑁占쎌몵嚥∽옙 揶쏄낫而� 1占쎌돳 ��⑥쥙�젟占쎈퓠 占쎌삏占쎈쑁占쎌몵嚥∽옙 4占쎌돳 占쎌굢占쎈뮉 5占쎌돳 筌앹빓占쏙옙占쎈뻻占쎄텚占쎈뼄.
}


void Artifact::Generation()
{
	mMainStat.SetZero();
	mSubStat.SetZero();
	GenerateMainOption(); // 筌롫뗄�뵥占쎌긿占쎈�� : �겫占쏙옙�맄筌띾뜄�뼄 占쎈뼄�뵳占�.
	GenerateSubOption(); // �겫占쏙옙�긿占쎈�� : �겫占쏙옙�맄筌띾뜄�뼄, 筌롫뗄�뵥占쎌긿占쎈�∽쭕�뜄�뼄 占쎈뼄�뵳占�.
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
