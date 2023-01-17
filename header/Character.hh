#ifndef CHARACTER_HH
#define CHARACTER_HH

#include "Stat.hh"
#include "Weapon.hh"
#include "Artifact.hh"
#include "ArtSetStat.hh"
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


class Weapon;
class ArtSetStat;


class Character
{
public:
	Character(Weapon* weapon, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown)
	{ 
		mWeapon = weapon;
		SetArtifact(flower, feather, clock, cup, crown);
		mTargetEC = 100.;
	}
	Character(Weapon* weapon)
	{
		ArtFlower* flower = new ArtFlower();
		ArtFeather* feather = new ArtFeather();
		ArtClock* clock = new ArtClock();
		ArtCup* cup = new ArtCup();
		ArtCrown* crown = new ArtCrown();
		mWeapon = weapon;
		SetArtifact(flower, feather, clock, cup, crown);
		mTargetEC = 100.;
	}
	Character(Character* character);
	virtual Character* Clone() { return new Character(this); }
	~Character() {}

	virtual void DoFeedback() {}
	void Update();
	void ArtifactInitialization();
	void InitializationExceptArtifact();

	virtual double GetDamage() { return GetDamage(mStat); }
	virtual double GetDamage(Stat stat);
	void MakeEffectionArray();
	void MakeScoreFunction();
	double GetScoreFunction(int index) { return mSavedFunction[index]; }
	double GetScore();
	double GetEffection(int index) { return mEffectionArray[index]; }

	Weapon* GetWeapon()               { return mWeapon; }
	void    SetWeapon(Weapon* weapon) { mWeapon = weapon; bPossibleExceptArtifact = false; }

	Stat        GetStat()                       { return mStat; }
	void        SetStat(Stat stat)              { mStat = stat; bPossibleExceptArtifact = false; }
	Stat        GetCharacterStat()              { return mCharacterStat; }
	Stat        GetFeedbackedStat()             { return mFeedbackedStat; }
	void        SetFeedbackedStat(Stat stat)    { mFeedbackedStat = stat; }
	void        AddFeedbackedStat(int index, double amount) { mFeedbackedStat.AddOption(index, amount); }
	ArtSetStat* GetArtSetStat()                 { return mArtSetStat; }
	void        SetArtSetStat(ArtSetStat* stat) { mArtSetStat = stat; bPossibleExceptArtifact = false; }
	Stat        GetResonanceStat()              { return mResonanceStat; }
	void        SetResonanceStat(Stat stat)     { mResonanceStat = stat; bPossibleExceptArtifact = false; }

	double GetTargetEC() { return mTargetEC; }
	void   SetTargetEC(double targetEC) { mTargetEC = targetEC; }
	
	void SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown);
	ArtFlower*  GetArtFlower()							{ return mArtFlower; }
	void        SetArtFlower(ArtFlower* artFlower)		{ mArtFlower = artFlower; }
	ArtFeather* GetArtFeather()							{ return mArtFeather; }
	void        SetArtFeather(ArtFeather* artFeather)	{ mArtFeather = artFeather; }
	ArtClock*   GetArtClock()							{ return mArtClock; }
	void        SetArtClock(ArtClock* artClock)			{ mArtClock = artClock; }
	ArtCup*     GetArtCup()								{ return mArtCup; }
	void        SetArtCup(ArtCup* artCup)				{ mArtCup = artCup; }
	ArtCrown*   GetArtCrown()							{ return mArtCrown; }
	void        SetArtCrown(ArtCrown* artCrown)			{ mArtCrown = artCrown; }
	// Skill GetPSkill() { return mPSkill; }
	// void  SetPSkill(Skill pSkill) { mPSkill = pSkill; }
	// Skill GetESkill() { return mESkill; }
	// void  SetESkill(Skill eSkill) { mESkill = eSkill; }
	// Skill GetQSkill() { return mQSkill; }
	// void  SetQSkill(Skill qSkill) { mQSkill = qSkill; }

protected:
	void SetBasicCharacterStat();
	void SetCharacterStat(int index, double amount)     { mCharacterStat.SetOption(index, amount); }
	void AddCharacterStat(int index, double amount)     { mCharacterStat.AddOption(index, amount); }
	void SetCharacterBaseStat(int index, double amount) { mCharacterStat.SetBaseOption(index, amount); }

private:
	Stat        GenerateStatExceptSubOpt();

	Stat        mStatExceptArtifact;
	bool        bPossibleExceptArtifact = false;

	double      mSavedFunction[46];
	double      mEffectionArray[19];

	Stat        mStat;
	Stat        mCharacterStat;
	Stat        mFeedbackedStat;
	double      mTargetEC;
	Weapon*     mWeapon;
	ArtFlower*  mArtFlower;
	ArtFeather* mArtFeather;
	ArtClock*   mArtClock;
	ArtCup*     mArtCup;
	ArtCrown*   mArtCrown;
	ArtSetStat* mArtSetStat;
	Stat        mResonanceStat;
	// Skill mPSkill;
	// Skill mESkill;
	// Skill mQSkill;

};

#endif