#ifndef STAT_HH
#define STAT_HH

class Stat
{
public:
	Stat() {};
	~Stat() {};

	void Initialization();

	void SetOption(int index, double amount) { mStat[index] = amount; }
	void AddOption(int index, double amount) { mStat[index] += amount; }
	double GetOption(int index) { return mStat[index]; }

	Stat GetSubOpt();
	void SetSubOpt(const double subOptArray[10]);

	void SetCriticalRate(double CR) { mStat[0] = CR; }
	double GetCriticalRate() { return mStat[0]; }
	void SetCriticalBonus(double CB) { mStat[1] = CB; }
	double GetCriticalBonus() { return mStat[1]; }
	void SetAttackPer(double AP) { mStat[2] = AP; }
	double GetAttackPer() { return mStat[2]; }
	void SetAttack(double attack) { mStat[3] = attack; }
	double GetAttack() { return mStat[3]; }
	void SetElementCharge(double EC) { mStat[4] = EC; }
	double GetElementCharge() { return mStat[4]; }
	void SetHPPer(double HPP) { mStat[5] = HPP; }
	double GetHPPer() { return mStat[5]; }
	void SetHP(double HP) { mStat[6] = HP; }
	double GetHP() { return mStat[6]; }
	void SetElementalMastery(double EM) { mStat[7] = EM; }
	double GetElementalMastery() { return mStat[7]; }
	void SetDefensePer(double DP) { mStat[8] = DP; }
	double GetDefensePer() { return mStat[8]; }
	void SetDefense(double defense) { mStat[9] = defense; }
	double GetDefense() { return mStat[9]; }
	void SetPiroBonus(double piroBonus) { mStat[10] = piroBonus; }
	double GetPiroBonus() { return mStat[10]; }
	void SetElectroBonus(double electroBonus) { mStat[11] = electroBonus; }
	double GetElectroBonus() { return mStat[11]; }
	void SetCryoBonus(double cryoBonus) { mStat[12] = cryoBonus; }
	double GetCryoBonus() { return mStat[12]; }
	void SetHydroBonus(double hydroBonus) { mStat[13] = hydroBonus; }
	double GetHydroBonus() { return mStat[13]; }
	void SetAnemoBonus(double anemoBonus) { mStat[14] = anemoBonus; }
	double GetAnemoBonus() { return mStat[14]; }
	void SetGeoBonus(double geoBonus) { mStat[15] = geoBonus; }
	double GetGeoBonus() { return mStat[15]; }
	void SetPhysicalBonus(double physicalBonus) { mStat[16] = physicalBonus; }
	double GetPhysicalBonus() { return mStat[16]; }
	void SetDendroBonus(double dendroBonus) { mStat[17] = dendroBonus; }
	double GetDendroBonus() { return mStat[17]; }
	void SetHealBonus(double healBonus) { mStat[18] = healBonus; }
	double GetHealBonus() { return mStat[18]; }
	void SetNormalAttackBonus(double normalAttackBonus) { mStat[19] = normalAttackBonus; }
	double GetNormalAttackBonus() { return mStat[19]; }
	void SetStrongAttackBonus(double strongAttackBonus) { mStat[20] = strongAttackBonus; }
	double GetStrongAttackBonus() { return mStat[20]; }
	void SetFlungeAttackBonus(double flungeAttackBonus) { mStat[21] = flungeAttackBonus; }
	double GetFlungeAttackBonus() { return mStat[21]; }
	void SetEBonus(double eBonus) { mStat[22] = eBonus; }
	double GetEBonus() { return mStat[22]; }
	void SetQBonus(double qBonus) { mStat[23] = qBonus; }
	double GetQBonus() { return mStat[23]; }
	void CalTotalAttack() { mStat[24] = mBaseStat[0] + mStat[2] + mStat[3]; } // BaseATK + AP + ATK
	double GetTotalAttack() { return mStat[24]; }
	void CalTotalHP() { mStat[25] = mBaseStat[1] + mStat[5] + mStat[6]; } // BaseHP + HPP + HP
	double GetTotalHP() { return mStat[25]; }
	void CalTotalDefense() { mStat[26] = mBaseStat[2] + mStat[8] + mStat[9]; } // BaseDF + DFP + DF
	double GetTotalDefense() { return mStat[26]; }
	void SetResistCut(double resistCut) { mStat[27] = resistCut; }
	double GetResistCut() { return mStat[27]; }
	void SetDefenseCut(double defenseCut) { mStat[28] = defenseCut; }
	double GetDefenseCut() { return mStat[28]; }
	void SetMonsterResist(double monsterResist) { mStat[29] = monsterResist; }
	double GetMonsterResist() { return mStat[29]; }
	void CalResistCoef();
	double GetResistCoef() { return mStat[30]; }
	void CalDefenseCoef();
	double GetDefenseCoef() { return mStat[31]; }
	void SetLevel(int level) { mStat[32] = (double)level; }
	double GetLevel() { return (int)mStat[32]; }
	void SetMonsterLevel(int monsterLevel) { mStat[33] = monsterLevel; }
	double GetMonsterLevel() { return mStat[33]; }
	void CalLevelCoef();
	double GetLevelCoef() { return mStat[34]; }

	double GetBaseAttack() { return mBaseStat[0]; }
	void SetBaseAttack(double baseATK) { mBaseStat[0] = baseATK; }
	double GetBaseHP() { return mBaseStat[1]; }
	void SetBaseHP(double baseHP) { mBaseStat[1] = baseHP; }
	double GetBaseDefense() { return mBaseStat[2]; }
	void SetBaseDefense(double baseDEF) { mBaseStat[2] = baseDEF; }

private:
	double mStat[35] = { 0. };
	// 0:ġȮ, 1:ġ��, 2:����, 3:����, 4:����, 5:HP��, 6:��HP, 7:����, 8:����, 9:����
	// 10:�ҿ���, 11:��������, 12:��������, 13:������, 14:�ٶ�����, 15:��������, 16:��������, 17:Ǯ����
	// 18:ġ�����ʽ�
	// 19:�Ϲݰ�������, 20:����������, 21:���ϰ�������, 22:E����, 23:Q����
	// 24:�Ѱ��ݷ�, 25:��HP, 26:�ѹ���
	// 27:������ 28:���, 29:���ͳ���, 30:�������, 31:���°��
	// 32:����, 33:���ͷ���, 34:�������

	double mBaseStat[3] = { 0. };
	// 0:���ݷ�, 1:HP, 2:����
};

#endif