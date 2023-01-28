#ifndef STAT_HH
#define STAT_HH


#include <array>
#include <vector>
#include <string>


std::array<std::string, 3> BASESTATSTRING = {"Base ATK", "Base HP", "Base DEF"};
std::array<std::string, 35> STATSTRING
	= {"CR", "CB", "ATK%%", "ATK", "EC", 
		"HP%%", "HP", "EM", "DEF%%", "DEF",
		"Pyro", "Elec", "Cryo", "Hydro", "Anymo",
		"Geo", "Phys", "Dendro", "Heal", "Normal Attack Bonus", 
		"Strong Attack Bonus", "Dodge Attack Bonus", "E Skill Bonus", "Q Skill Bonus", "Total ATK",
		"Total HP", "Total DEF", "Resist Cut", "DEF Cut", "Monster Resist",
		"Resist Coef", "DEF Coef", "Lv", "Monster Lv", "Lv Coef"};


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
	void   SetStrongAttackBonus(double strongAttackBonus) { mStat[20] = strongAttackBonus; }
	double GetStrongAttackBonus() const { return mStat[20]; }
	void   SetFlungeAttackBonus(double flungeAttackBonus) { mStat[21] = flungeAttackBonus; }
	double GetFlungeAttackBonus() const { return mStat[21]; }
	void   SetEBonus(double eBonus) { mStat[22] = eBonus; }
	double GetEBonus() const { return mStat[22]; }
	void   SetQBonus(double qBonus) { mStat[23] = qBonus; }
	double GetQBonus() const { return mStat[23]; }
	void   CalTotalAttack() { mStat[24] = mBaseStat[0] * (1. + mStat[2] / 100.) + mStat[3]; } // BaseATK * (1 + AP / 100) + ATK
	double GetTotalAttack() const { return mStat[24]; }
	void   CalTotalHP() { mStat[25] = mBaseStat[1] * (1. + mStat[5] / 100.) + mStat[6]; } // BaseHP * (1 + HPP / 100) + HP
	double GetTotalHP() const { return mStat[25]; }
	void   CalTotalDefense() { mStat[26] = mBaseStat[2] * (1. + mStat[8] / 100.) + mStat[9]; } // BaseDF * (1 + DFP / 100) + DF
	double GetTotalDefense() const { return mStat[26]; }
	void   SetResistCut(double resistCut) { mStat[27] = resistCut; }
	double GetResistCut() const { return mStat[27]; }
	void   SetDefenseCut(double defenseCut) { mStat[28] = defenseCut; }
	double GetDefenseCut() const { return mStat[28]; }
	void   SetMonsterResist(double monsterResist) { mStat[29] = monsterResist; }
	double GetMonsterResist() const { return mStat[29]; }
	void   CalResistCoef();
	double GetResistCoef() const { return mStat[30]; }
	void   CalDefenseCoef();
	double GetDefenseCoef() const { return mStat[31]; }
	void   SetLevel(int level) { mStat[32] = (double)level; }
	double GetLevel() const { return (int)mStat[32]; }
	void   SetMonsterLevel(int monsterLevel) { mStat[33] = monsterLevel; }
	double GetMonsterLevel() const { return mStat[33]; }
	void   CalLevelCoef();
	double GetLevelCoef() const { return mStat[34]; }

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
	// 0:치확, 1:치피, 2:공퍼, 3:깡공, 4:원충, 5:HP퍼, 6:깡HP, 7:원마, 8:방퍼, 9:깡방
	// 10:불원소, 11:번개원소, 12:얼음원소, 13:물원소, 14:바람원소, 15:바위원소, 16:물리피해, 17:풀피해
	// 18:치유보너스
	// 19:일반공격피해, 20:강공격피해, 21:낙하공격피해, 22:E피해, 23:Q피해
	// 24:총공격력, 25:총HP, 26:총방어력
	// 27:내성깎 28:방깎, 29:몬스터내성, 30:내성계수, 31:방어력계수
	// 32:레벨, 33:몬스터레벨, 34:레벨계수

	// 성유물 부옵 : 0 ~ 9
	// 성유물 주옵 : 0 ~ 8, 10 ~ 18
	// 성유물 세트 : 0 ~ 23, 27, 28 // 완전히 배제된 것은 아님
	// 무기 부부옵 : 0 ~ 23, 27, 28 // 완전히 배제된 것은 아님
	// 무기 부옵 : 0 ~ 18 // 완전히 배제된 것은 아님
	// 무기 주옵 : b0
	// 공명 : 0, 2, 5, 7, 10 ~ 17, 27
	// 캐릭터 옵션 : 0 ~ 34, b0 ~ b2

	std::array<double, 3> mBaseStat = { 0. };
	// 0:공격력, 1:HP, 2:방어력
};

#endif