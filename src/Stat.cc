#include "../header/Stat.hh"
#include <iostream>


void Stat::SetZero()
{
	for (int i = 0; i < 35 ; i++)
	{
		mStat[i] = 0.;
	}
	for (int i = 0; i < 3 ; i++)
	{
		mBaseStat[i] = 0.;
	}
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
	double result;
	if (monsterResist - resistCut > 75)
	{
		result = 1. / ((monsterResist - resistCut) / 25. + 1.);
	}
	else if (monsterResist - resistCut < 0)
	{
		result = 1. - (monsterResist - resistCut) / 200.;
	}
	else
	{
		result = 1. - (monsterResist - resistCut) / 100.;
	}
	mStat[30] = result;
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