#ifndef STAT_HH
#define STAT_HH


#include <array>
#include <iostream>
#include <vector>
#include <string>

static std::array<std::string, 3> BASESTATSTRING = {"Base ATK", "Base HP", "Base DEF"};
static std::array<std::string, 35> STATSTRING
	= {"CR", "CB", "ATK%", "ATK", "EC", 
		"HP%", "HP", "EM", "DEF%", "DEF",
		"Pyro", "Elec", "Cryo", "Hydro", "Anymo",
		"Geo", "Phys", "Dendro", "Heal", "NA Bonus", 
		"CA Bonus", "FA Bonus", "E Skill Bonus", "Q Skill Bonus", "Resist Cut",
		"DEF Cut", "DEF ignore", "Total ATK", "Total HP", "Total DEF",
		"Monster Resist", "Lv", "Monster Lv", "Resist Coef", "LvDef Coef"};


class Stat
{
public:
	Stat() {};
	virtual ~Stat() {};

	void SetZero();
	void Update();

	void SetOption(int index, double amount) { mStat[index] = amount; }
	void AddOption(int index, double amount) { mStat[index] += amount; }
	double GetOption(int index) const { return mStat[index]; }

	void   SetCriticalRate(double CR) { mStat[0] = CR; }
	double GetCriticalRate() const { return mStat[0]; }
	void   SetCriticalBonus(double CB) { mStat[1] = CB; }
	double GetCriticalBonus() const { return mStat[1]; }
	void   SetAttackPer(double AP) { mStat[2] = AP; }
	double GetAttackPer() const { return mStat[2]; }
	void   SetAttack(double attack) { mStat[3] = attack; }
	double GetAttack() const { return mStat[3]; }
	void   SetElementCharge(double EC) { mStat[4] = EC; }
	double GetElementCharge() const { return mStat[4]; }
	void   SetHPPer(double HPP) { mStat[5] = HPP; }
	double GetHPPer() const { return mStat[5]; }
	void   SetHP(double HP) { mStat[6] = HP; }
	double GetHP() const { return mStat[6]; }
	void   SetElementalMastery(double EM) { mStat[7] = EM; }
	double GetElementalMastery() const { return mStat[7]; }
	void   SetDefensePer(double DP) { mStat[8] = DP; }
	double GetDefensePer() const { return mStat[8]; }
	void   SetDefense(double defense) { mStat[9] = defense; }
	double GetDefense() const { return mStat[9]; }
	void   SetPiroBonus(double piroBonus) { mStat[10] = piroBonus; }
	double GetPiroBonus() const { return mStat[10]; }
	void   SetElectroBonus(double electroBonus) { mStat[11] = electroBonus; }
	double GetElectroBonus() const { return mStat[11]; }
	void   SetCryoBonus(double cryoBonus) { mStat[12] = cryoBonus; }
	double GetCryoBonus() const { return mStat[12]; }
	void   SetHydroBonus(double hydroBonus) { mStat[13] = hydroBonus; }
	double GetHydroBonus() const { return mStat[13]; }
	void   SetAnemoBonus(double anemoBonus) { mStat[14] = anemoBonus; }
	double GetAnemoBonus() const { return mStat[14]; }
	void   SetGeoBonus(double geoBonus) { mStat[15] = geoBonus; }
	double GetGeoBonus() const { return mStat[15]; }
	void   SetPhysicalBonus(double physicalBonus) { mStat[16] = physicalBonus; }
	double GetPhysicalBonus() const { return mStat[16]; }
	void   SetDendroBonus(double dendroBonus) { mStat[17] = dendroBonus; }
	double GetDendroBonus() const { return mStat[17]; }
	void   SetHealBonus(double healBonus) { mStat[18] = healBonus; }
	double GetHealBonus() const { return mStat[18]; }
	void   SetNormalAttackBonus(double normalAttackBonus) { mStat[19] = normalAttackBonus; }
	double GetNormalAttackBonus() const { return mStat[19]; }
	void   SetChargedAttackBonus(double chargedAttackBonus) { mStat[20] = chargedAttackBonus; }
	double GetChargedAttackBonus() const { return mStat[20]; }
	void   SetFlungeAttackBonus(double flungeAttackBonus) { mStat[21] = flungeAttackBonus; }
	double GetFlungeAttackBonus() const { return mStat[21]; }
	void   SetEBonus(double eBonus) { mStat[22] = eBonus; }
	double GetEBonus() const { return mStat[22]; }
	void   SetQBonus(double qBonus) { mStat[23] = qBonus; }
	double GetQBonus() const { return mStat[23]; }
	void   SetResistCut(double resistCut) { mStat[24] = resistCut; }
	double GetResistCut() const { return mStat[24]; }
	void   SetDefenseCut(double defenseCut) { mStat[25] = defenseCut; }
	double GetDefenseCut() const { return mStat[25]; }
	void   SetDefenseIgnore(double defenseIgnore) { mStat[26] = defenseIgnore; }
	double GetDefenseIgnore() const { return mStat[26]; }
	void   CalTotalAttack() { mStat[27] = mBaseStat[0] * (1. + mStat[2] * 0.01) + mStat[3]; } // BaseATK * (1 + AP / 100) + ATK
	double GetTotalAttack() const { return mStat[27]; }
	void   CalTotalHP() { mStat[28] = mBaseStat[1] * (1. + mStat[5] * 0.01) + mStat[6]; } // BaseHP * (1 + HPP / 100) + HP
	double GetTotalHP() const { return mStat[28]; }
	void   CalTotalDefense() { mStat[29] = mBaseStat[2] * (1. + mStat[8] * 0.01) + mStat[9]; } // BaseDF * (1 + DFP / 100) + DF
	double GetTotalDefense() const { return mStat[29]; }
	void   SetMonsterResist(double monsterResist) { mStat[30] = monsterResist; }
	double GetMonsterResist() const { return mStat[30]; }
	void   SetLevel(int level) { mStat[31] = (double)level; }
	double GetLevel() const { return (int)mStat[31]; }
	void   SetMonsterLevel(int monsterLevel) { mStat[32] = monsterLevel; }
	double GetMonsterLevel() const { return mStat[32]; }
	void   CalResistCoef();
	double GetResistCoef() const { return mStat[33]; }
	void   CalLvDefCoef();
	double GetLvDefCoef() const { return mStat[34]; }

	double GetBaseOption(int index) const { return mBaseStat[index]; }
	void   SetBaseOption(int index, double amount) { mBaseStat[index] = amount; }
	double GetBaseAttack() const { return mBaseStat[0]; }
	void   SetBaseAttack(double baseATK) { mBaseStat[0] = baseATK; }
	double GetBaseHP() const { return mBaseStat[1]; }
	void   SetBaseHP(double baseHP) { mBaseStat[1] = baseHP; }
	double GetBaseDefense() const { return mBaseStat[2]; }
	void   SetBaseDefense(double baseDEF) { mBaseStat[2] = baseDEF; }

private:
	std::array<double, 35> mStat = { 0. };
	// 0:ġȮ, 1:ġ��, 2:����, 3:����, 4:����, 5:HP��, 6:��HP, 7:����, 8:����, 9:����
	// 10:�ҿ���, 11:��������, 12:��������, 13:������, 14:�ٶ�����, 15:��������, 16:��������, 17:Ǯ����
	// 18:ġ�����ʽ�
	// 19:�Ϲݰ�������, 20:����������, 21:���ϰ�������, 22:E����, 23:Q����
	// 24:������, 25:���, 26:�湫
	// 27:�Ѱ��ݷ�, 28:��HP, 29:�ѹ���
	// 30:���ͳ���, 31:����, 32:���ͷ���, 33:�������, 34:�������°��

	// ������ �ο� : 0 ~ 9
	// ������ �ֿ� : 0 ~ 8, 10 ~ 18
	// ������ ��Ʈ : 0 ~ 26 // ������ ������ ���� �ƴ�
	// ���� �κο� : 0 ~ 26 // ������ ������ ���� �ƴ�
	// ���� �ο� : 0 ~ 18 // ������ ������ ���� �ƴ�
	// ���� �ֿ� : b0
	// ���� : 0, 2, 5, 7, 10 ~ 17, 24
	// ĳ���� �ɼ� : 0 ~ 34, b0 ~ b2

	std::array<double, 3> mBaseStat = { 0. };
	// 0:���ݷ�, 1:HP, 2:����
};


void PrintStat(const Stat& stat);


#endif