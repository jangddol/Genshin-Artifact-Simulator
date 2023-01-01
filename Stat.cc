#include "Stat.hh"
#include <iostream>


void PrintStat(Stat stat)
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
	for (int i = 0; i < 35 ; i++)
	{
		mStat[i] = 0.;
	}
	for (int i = 0; i < 3 ; i++)
	{
		mBaseStat[i] = 0.;
	}
}


void Stat::Initialization()
{
	CalTotalAttack();
	CalTotalHP();
	CalTotalDefense();
	CalResistCoef();
	CalDefenseCoef();
	CalLevelCoef();
}


void Stat::SetSubOpt(const double subOptArray[10])
{
	for (int i = 0; i < 10; i++)
	{
		mStat[i] = subOptArray[i];
	}
}


Stat Stat::GetSubOpt()
{
	Stat tempStat = Stat();
	tempStat.SetCriticalRate(mStat[0]);
	tempStat.SetCriticalBonus(mStat[1]);
	tempStat.SetAttackPer(mStat[2]);
	tempStat.SetAttack(mStat[3]);
	tempStat.SetElementCharge(mStat[4]);
	tempStat.SetHPPer(mStat[5]);
	tempStat.SetHP(mStat[6]);
	tempStat.SetElementalMastery(mStat[7]);
	tempStat.SetDefensePer(mStat[8]);
	tempStat.SetDefense(mStat[9]);
	return tempStat;
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