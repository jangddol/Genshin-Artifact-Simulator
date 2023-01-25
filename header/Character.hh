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
	Character(Weapon* weapon, ArtSetStat* artSetStat, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown)
	{ 
		mWeapon = weapon;
		SetArtifact(flower, feather, clock, cup, crown);
		mTargetEC = 100.;
		SetArtSetStat(artSetStat);
	}
	Character(Character* character);
	Character(Character& character);
	Character(Character&& character);
	Character& operator = (Character &character);
	Character& operator = (Character &&character);
	virtual Character* Clone() { return new Character(this); }
	virtual ~Character();

	// Stat Update & UpdateState
	virtual void DoFeedback() {}
	void Update();
	void ConfirmResonanceStatModified();
	void ConfirmWeaponStatModified();
	void ConfirmArtSetStatModified();
	void ConfirmArtifactMainStatModified();
	void ConfirmArtifactSubStatModified();
	int  GetUpdateState() { return mUpdateState; }

	// Damage and EffectionArray, Score Function
	double GetDamage() { return this->GetDamageWithStat(mStat); }
	virtual double GetDamageWithStat(Stat stat);
	void MakeEffectionArray();
	void MakeScoreFunction();
	double GetScoreFunction(int index) { return mSavedFunction[index]; }
	double GetScore();
	double GetEffection(int index) { return mEffectionArray[index]; }

	// Stat
	Stat GetStat()          { return mStat; }
	void SetStat(Stat stat) { mStat = stat; mUpdateState = ARTIFACTSUBSTATUPDATED; }

	// Character Stat
	Stat GetCharacterStat() { return mCharacterStat; }

	// Resonance Stat
	Stat GetResonanceStat()          { return mResonanceStat; }
	void SetResonanceStat(Stat stat) { mResonanceStat = stat; ConfirmResonanceStatModified(); }

	// Weapon
	Weapon* GetWeapon()               { return mWeapon; }
	void    SetWeapon(Weapon* weapon) { mWeapon = weapon; ConfirmWeaponStatModified(); }

	// ArtSetStat
	ArtSetStat* GetArtSetStat() { return mArtSetStat; }
	void        SetArtSetStat(ArtSetStat* stat);

	// Artifact
	void        SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown);
	ArtFlower*  GetArtFlower() { return mArtFlower; }
	void        SetArtFlower(ArtFlower* artFlower);
	ArtFeather* GetArtFeather() { return mArtFeather; }
	void        SetArtFeather(ArtFeather* artFeather);
	ArtClock*   GetArtClock() { return mArtClock; }
	void        SetArtClock(ArtClock* artClock);
	ArtCup*     GetArtCup() { return mArtCup; }
	void        SetArtCup(ArtCup* artCup);
	ArtCrown*   GetArtCrown() { return mArtCrown; }
	void        SetArtCrown(ArtCrown* artCrown);

	// Feedbacked Stat
	Stat GetFeedbackedStat()             { return mFeedbackedStat; }
	void SetFeedbackedStat(Stat stat)    { mFeedbackedStat = stat; }
	void AddFeedbackedStat(int index, double amount) { mFeedbackedStat.AddOption(index, amount); }

	// TargetEC
	double GetTargetEC()                { return mTargetEC; }
	void   SetTargetEC(double targetEC) { mTargetEC = targetEC; }
	
	// Skill GetPSkill() { return mPSkill; }
	// void  SetPSkill(Skill pSkill) { mPSkill = pSkill; }
	// Skill GetESkill() { return mESkill; }
	// void  SetESkill(Skill eSkill) { mESkill = eSkill; }
	// Skill GetQSkill() { return mQSkill; }
	// void  SetQSkill(Skill qSkill) { mQSkill = qSkill; }

	Stat GetStatAfterUpdateFromCharacterResonance() { return mStatAfterUpdateFromCharacterResonance; }
	Stat GetStatAfterUpdateFromWeapon() { return mStatAfterUpdateFromWeapon; }
	Stat GetStatAfterUpdateFromArtSetStat() { return mStatAfterUpdateFromArtSetStat; }
	Stat GetStatAfterUpdateFromArtifactMainStat() { return mStatAfterUpdateFromArtifactMainStat; }
	Stat GetStatAfterUpdateFromArtifactSubStat() { return mStatAfterUpdateFromArtifactSubStat; }

protected:
	void SetBasicCharacterStat();
	void SetCharacterStat(int index, double amount)     { mCharacterStat.SetOption(index, amount); }
	void AddCharacterStat(int index, double amount)     { mCharacterStat.AddOption(index, amount); }
	void SetCharacterBaseStat(int index, double amount) { mCharacterStat.SetBaseOption(index, amount); }

private:
	// Stat Update Process
	void UpdateFromCharacterResonance();
	void UpdateFromWeapon();
	void UpdateFromArtSetStat();
	void UpdateFromArtifactMainStat();
	void UpdateFromArtifactSubStat();
	void UpdateFromFeedback();
	Stat mStatAfterUpdateFromCharacterResonance; // never do initialization
	Stat mStatAfterUpdateFromWeapon; // never do initialization
	Stat mStatAfterUpdateFromArtSetStat; // never do initialization
	Stat mStatAfterUpdateFromArtifactMainStat; // never do initialization
	Stat mStatAfterUpdateFromArtifactSubStat; // never do initialization
	int  mUpdateState = 0;

	constexpr static int CHARACTERRESONANCEUPDATED = 1;
    constexpr static int WEAPONSTATUPDATED = 2;
    constexpr static int ARTSETSTATUPDATED = 3;
    constexpr static int ARTIFACTMAINSTATUPDATED = 4;
    constexpr static int ARTIFACTSUBSTATUPDATED = 5;

	double      mSavedFunction[46];
	double      mEffectionArray[19];

	Stat        mStat;
	Stat        mCharacterStat;
	Stat        mFeedbackedStat;
	double      mTargetEC;
	Weapon*     mWeapon = nullptr;
	ArtFlower*  mArtFlower = nullptr;
	ArtFeather* mArtFeather = nullptr;
	ArtClock*   mArtClock = nullptr;
	ArtCup*     mArtCup = nullptr;
	ArtCrown*   mArtCrown = nullptr;
	ArtSetStat* mArtSetStat = nullptr;
	Stat        mResonanceStat;
	// Skill mPSkill;
	// Skill mESkill;
	// Skill mQSkill;
};

#endif