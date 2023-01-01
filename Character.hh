#ifndef CHARACTER_HH
#define CHARACTER_HH

#include "Stat.hh"
#include "Weapon.hh"
#include "Artifact.hh"
#include "Artifact/ArtFlower.hh"
#include "Artifact/ArtFeather.hh"
#include "Artifact/ArtClock.hh"
#include "Artifact/ArtCup.hh"
#include "Artifact/ArtCrown.hh"
#include "Skill.hh"


constexpr double PLUSARRAY[10] = { 3.88999991118908, 7.76999965310097, 5.82999996840954,
                             19.4500007629395, 6.4800001680851,  5.82999996840954,
                             298.75,           23.3099994659424, 7.28999972343445,
                             23.1499996185303 };


class Character
{
public:
	Character(Weapon weapon)
	{ 
		mWeapon = weapon;
		mTargetEC = 100.;
	}
	~Character() {}

	void Initialization();
	void ArtifactInitialization();
	void InitializationExceptArtifact();

	virtual double GetDamage() { return GetDamage(mStat); }
	virtual double GetDamage(Stat stat);
	void MakeEffectionArray();
	void MakeScoreFunction();
	double GetScoreFunction(int index) { return mSavedFunction[index]; }
	double GetScore();
	double GetEffection(int index) { return mEffectionArray[index]; }

	Weapon GetWeapon()              { return mWeapon; }
	void   SetWeapon(Weapon weapon) { mWeapon = weapon; bPossibleExceptArtifact = false; }

	Stat GetStat()                   { return mStat; }
	void SetStat(Stat stat)          { mStat = stat; bPossibleExceptArtifact = false; }
	Stat GetArtSetStat()             { return mArtSetStat; }
	void SetArtSetStat(Stat stat)    { mArtSetStat = stat; bPossibleExceptArtifact = false; }
	Stat GetResonanceStat()          { return mResonanceStat; }
	void SetResonanceStat(Stat stat) { mResonanceStat = stat; bPossibleExceptArtifact = false; }

	void SetArtifact(ArtFlower flower, ArtFeather feather, ArtClock clock, ArtCup cup, ArtCrown crown);
	Artifact GetArtFlower()							{ return fArtFlower; }
	void     SetArtFlower(ArtFlower artFlower)		{ fArtFlower = artFlower; }
	Artifact GetArtFeather()						{ return fArtFeather; }
	void     SetArtFeather(ArtFeather artFeather)	{ fArtFeather = artFeather; }
	Artifact GetArtClock()							{ return fArtClock; }
	void     SetArtClock(ArtClock artClock)			{ fArtClock = artClock; }
	Artifact GetArtCup()							{ return fArtCup; }
	void     SetArtCup(ArtCup artCup)				{ fArtCup = artCup; }
	Artifact GetArtCrown()							{ return fArtCrown; }
	void     SetArtCrown(ArtCrown artCrown)			{ fArtCrown = artCrown; }
	// Skill GetPSkill() { return mPSkill; }
	// void  SetPSkill(Skill pSkill) { mPSkill = pSkill; }
	// Skill GetESkill() { return mESkill; }
	// void  SetESkill(Skill eSkill) { mESkill = eSkill; }
	// Skill GetQSkill() { return mQSkill; }
	// void  SetQSkill(Skill qSkill) { mQSkill = qSkill; }

protected:
	void SetBasicCharacterStat();
	void SetCharacterStat(int index, double amount) { mCharacterStat.SetOption(index, amount); }
	void AddCharacterStat(int index, double amount) { mCharacterStat.AddOption(index, amount); }
	void SetCharacterBaseStat(int index, double amount) { mCharacterStat.SetBaseOption(index, amount); }

private:
	Stat GenerateStatExceptSubOpt();
	Stat mStatExceptArtifact;
	bool bPossibleExceptArtifact = false;

	double mSavedFunction[46];
	double mEffectionArray[19];

	Stat mStat;
	Stat mCharacterStat;
	double mTargetEC;
	Weapon mWeapon;
	ArtFlower  fArtFlower;
	ArtFeather fArtFeather;
	ArtClock   fArtClock;
	ArtCup     fArtCup;
	ArtCrown   fArtCrown;
	Stat mArtSetStat;
	Stat mResonanceStat;
	// Skill mPSkill;
	// Skill mESkill;
	// Skill mQSkill;

};

#endif