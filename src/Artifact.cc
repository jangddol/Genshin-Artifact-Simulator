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
	
	int tempInt;
	if (cummulatedWeight[length - 1] == 1) tempInt = 1;
	else tempInt = GetRdUni(cummulatedWeight[length - 1]) + 1;

	int selectedInt = 0;
	int beforeElement = 0;
	int nowElement = 0;
	for (int i = 0; i < length; i++) // for占쎈닱筌뤾쑴諭� �뜝�럥利쇽옙逾놂옙�맓�듆�뜝�럡�맋 �뜝�룞�삕�뜝�룞�삕占쎄틬占쎈쑏熬곥룊爰랃옙紐닷뜝占� �뜝�럥由썲뜝�럥堉�.
	{
		nowElement = cummulatedWeight[i];
		if (tempInt > beforeElement && tempInt <= nowElement)
		{
			selectedInt = i; // �뜝�럩�굩�뜝�럥�몓 int占쎈ご�뜝占� �뤆�룊�삕嶺뚯쉻�삕占쏙옙�뫅�삕 �뜝�럥�꽑�뜝�럥裕� �썒�슣�닔占쎄맙�뜝�럩逾η춯�쉻�삕 �뇦猿됲�ｏ옙�젧�뜝�럥由썲뜝�럥堉�.
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

	// 媛곴컖 1�쉶�뵫
	for (int i = 0; i < 4; i++)
	{
		int optIndex = startOptList[i];
		double randomStat = OPTIONARRAY[optIndex][GetRdUni4()];
		mSubStat.AddOption(optIndex, randomStat);
	}

	// �옖�뜡�쑝濡� numUpgrade留뚰겮
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
		// 1. 嶺뚮∥�뾼占쎈데�뜝�럩湲욕뜝�럥占썩댙�삕占쎈굵 �뜝�럩�꼪�뜝�럩逾ε뜝�럥�돵�뜝�럡�맋 �뜝�럩�꼪占쎈ご�넫琉욍걫�뜝�럥�뱺�뜝�럡�맋 �뜝�럥�돵�뜝�럥堉� 占쎄껀�뜝�띂寃ヨ쥈�뫗諭� 0�뜝�럩紐드슖�댙�삕 嶺뚮씭�뒧獄�釉앹삕占쎈펲.
			// 1-1. �뜝�럩逾졿ㅀ袁ъ삕 �뤆�룊�삕嶺뚯쉻�삕占쏙옙�뫅�삕 cummulatedWeight�뜝�럩諭� 嶺뚮씭�뒧獄�釉앹삕占쎈펲.
				// This cummulatedWeight is for subOption
				// Therefore the length of cummulatedWeight is 10.

	bool whether4OptStart = Selected3or4OptStart();
		// 2. 嶺뚳퐣瑗뤄옙踰됧뜝�럥�뱺 3�뤆�룇裕뉛옙逾η춯�쉻�삕 4�뤆�룇裕뉛옙逾η춯�쉻�삕 占쏙옙�뫁伊볩옙��꿨뜝�럥堉�. -> 8�뤆�룊�삕 or 9�뤆�룊�삕
	std::array<int, 4> startOptList = GenerateStartOpt(subCummulatedWeight);
		// 3. 嶺뚳퐣瑗뤄옙踰� �뜝�럩湲욕뜝�럥占쏙옙 4�뤆�룇裕꾢뜝�룞�삕 占쎈닱占쎈샍驪볦슱�삕占쎈데嶺뚯쉻�삕 �뇦猿됲�ｏ옙�젧�뜝�럥由썲뜝�럥堉�. 4�뤆�룇裕녶뜝�룞�삕 �뇦猿딉옙占쏙옙�뭵嶺뚯쉻�삕 �뜝�럥瑜ラ뇦猿볦삕 �뜝�럡臾멨뜝�럡�뎽�뜝�럥由썲뜝�럥堉�.

	UpgradeSubOption(startOptList, whether4OptStart);
		// 4. �뼨轅명�ｏ옙占쏙옙 4�뤆�룇裕녶뜝�룞�삕 �뜝�럩�굩�뜝�럥�몓�뜝�럩紐드슖�댙�삕 �뤆�룄�궖��뚳옙 1�뜝�럩�뤂 占쏙옙�뫁伊숋옙�젧�뜝�럥�뱺 �뜝�럩�굩�뜝�럥�몓�뜝�럩紐드슖�댙�삕 4�뜝�럩�뤂 �뜝�럩援℡뜝�럥裕� 5�뜝�럩�뤂 嶺뚯빘鍮볟뜝�룞�삕�뜝�럥六삣뜝�럡�뀣�뜝�럥堉�.
}


void Artifact::Generation()
{
	mMainStat.SetZero();
	mSubStat.SetZero();
	GenerateMainOption(); // 嶺뚮∥�뾼占쎈데�뜝�럩湲욕뜝�럥占쏙옙 : 占쎄껀�뜝�룞�삕占쎈쭊嶺뚮씭�쐞占쎈펲 �뜝�럥堉꾬옙逾녑뜝占�.
	GenerateSubOption(); // 占쎄껀�뜝�룞�삕占쎄맙�뜝�럥占쏙옙 : 占쎄껀�뜝�룞�삕占쎈쭊嶺뚮씭�쐞占쎈펲, 嶺뚮∥�뾼占쎈데�뜝�럩湲욕뜝�럥占썩댙彛뺧옙�쐞占쎈펲 �뜝�럥堉꾬옙逾녑뜝占�.
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
