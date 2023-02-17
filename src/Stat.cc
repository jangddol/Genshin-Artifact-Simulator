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
	CalLvDefCoef();
}


inline void Stat::CalResistCoef()
{
	double monsterResist = GetMonsterResist();
	double resistCut = GetResistCut();
	double remainResist = monsterResist - resistCut;
	if (remainResist <= 75. && remainResist >= 0)
	{
		mStat[33] = 1. - remainResist * 0.01;
	}
	else if (remainResist > 75)
	{
		mStat[33] = 1. / (remainResist * 0.04 + 1.);
	}
	else
	{
		mStat[33] = 1. - remainResist * 0.005;
	}
}


inline void Stat::CalLvDefCoef()
{
	double charLvPlus100 = GetLevel() + 100;
	double monsterLv = GetMonsterLevel();
	double defenseCut = GetDefenseCut();
	double defenseIgnore = GetDefenseIgnore();
	mStat[34] = charLvPlus100 / (charLvPlus100 + (monsterLv + 100.) * (1 - defenseCut * 0.01) * (1 - defenseIgnore * 0.01));
}