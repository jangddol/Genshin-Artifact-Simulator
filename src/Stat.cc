#include "../header/Stat.hh"
#include <iostream>
#include <algorithm>
#include <iomanip>  // for std::setw


void PrintStat(const Stat& stat) {
    int category_column_widths = 0;
    for (std::size_t i = 0; i < 35; i++) {
        int category_width = STATSTRING[i].length();
        if (category_width > category_column_widths) {
            category_column_widths = category_width;
        }
    }
    int value_column_widths = 11;

    for (int i = 0; i < 6; i++) {
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i + 10] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i + 10);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i + 19] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i + 19);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i + 29] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i + 29);
        std::cout << std::endl;
    }
    for (int i = 6; i < 9; i++) {
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i + 10] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i + 10);
        std::cout << std::left << std::setw(category_column_widths) << STATSTRING[i + 19] << " : ";
        std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(i + 19);
        std::cout << std::endl;
    }
    std::cout << std::left << std::setw(category_column_widths) << STATSTRING[9] << " : ";
    std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(9);
    std::cout << std::left << std::setw(category_column_widths) << "" << "   ";
    std::cout << std::left << std::setw(value_column_widths) << "";
    std::cout << std::left << std::setw(category_column_widths) << STATSTRING[28] << " : ";
    std::cout << std::left << std::setw(value_column_widths) << std::setprecision(2) << std::fixed << stat.GetOption(28);
    std::cout << std::endl;
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