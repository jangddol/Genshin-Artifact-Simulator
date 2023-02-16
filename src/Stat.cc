#include "../header/Stat.hh"
#include <iostream>
#include <algorithm>


void PrintStat(const Stat& stat)
{
    for (int i = 0; i < 35; i++)
    {
        std::cout << STATSTRING[i] << " : " << stat.GetOption(i) << std::endl;
    }
    for (int i = 0; i < 3; i++)
    {
        std::cout << BASESTATSTRING[i] << " : " << stat.GetBaseOption(i) << std::endl;
    }
}


void Stat::SetZero()
{
	mStat.fill(0);
	mBaseStat.fill(0);
}


void Stat::Update()
{
	CalTotalAttack();
	CalTotalHP();
	CalTotalDefense();
	CalResistCoef();
	CalDefenseCoef();
	CalLevelCoef();
}


void Stat::CalResistCoef()
{
	double monsterResist = GetMonsterResist();
	double resistCut = GetResistCut();
	if (monsterResist - resistCut <= 75. && monsterResist - resistCut >= 0)
	{
		mStat[30] = 1. - (monsterResist - resistCut) / 100.;
	}
	else if (monsterResist - resistCut > 75)
	{
		mStat[30] = 1. / ((monsterResist - resistCut) / 25. + 1.);
	}
	else
	{
		mStat[30] = 1. - (monsterResist - resistCut) / 200.;
	}
}


void Stat::CalDefenseCoef()
{
	double charLv = GetLevel();
	double monsterLv = GetMonsterLevel();
	double defenseCut = GetDefenseCut();
	double result = (charLv + monsterLv + 200) / ((charLv + 100) + (monsterLv + 100) * (1 - defenseCut / 100));
	mStat[31] = result;
}


void Stat::CalLevelCoef()
{
	double charLv = GetLevel();
	double monsterLv = GetMonsterLevel();
	double result = (charLv + 100) / (charLv + monsterLv + 200);
	mStat[34] = result;
}