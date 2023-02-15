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
enum WeaponList;
class Artifact;
class ArtFlower;
class ArtFeather;
class ArtClock;
class ArtCup;
class ArtCrown;
class ArtSetStat;


struct MainOptionsAndDamage
{
    std::array<int, 3> mainOptions = {-1, -1, -1};
    double damage = 0;
};


enum CharList
{
	NONE,
	KOKOMI,
	NINGGUANG,
	RAIDEN
};


class Character
{
public:
	Character(Weapon* weapon, ArtSetStat* artSetStat, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown)
	: mTargetEC{100.}, mWeapon{weapon}
	{ 
		SetArtifact(flower, feather, clock, cup, crown);
		SetArtSetStat(artSetStat);
	}
	Character(const Character* character);
	Character(const Character& character);
	Character(Character&& character);
	Character& operator = (const Character &character);
	Character& operator = (Character &&character);
	virtual Character* Clone() const;
	virtual ~Character();

	// Stat Update & UpdateState
	virtual void DoFeedback();
	void Update();
	void ConfirmResonanceStatModified();
	void ConfirmWeaponStatModified();
	void ConfirmArtSetStatModified();
	void ConfirmArtifactMainStatModified();
	void ConfirmArtifactSubStatModified();
	int  GetUpdateState() const { return mUpdateState; }

	// Set Manual Mode
	void SetManualMode(bool isManualMode) { mIsManualMode = isManualMode; }
	bool GetManualMode()                  { return mIsManualMode; }

	// Damage and EffectionArray
	double GetDamage() const { return this->GetDamageWithStat(mStat); }
	virtual double GetDamageWithStat(const Stat& stat) const;

	// Score (algorithm by jangddol)
	void MakeEffectionArray();
	void MakeScoreFunction();
	double GetScoreFunction(int index) const { return mSavedFunction[index]; }
	double GetScore() const;
	double GetEffection(int index) const { return mEffectionArray[index]; }

	// Score (algorithm by MonkeyMagic)
	double GetScore_MonkeyMagic() const;

	// Artifact MainOption Optimization
	std::array<MainOptionsAndDamage, 10> OptimizeMainOption(int refScore = 28) const;

	// Stat
	Stat GetStat() const    { return mStat; }
	void SetStat(const Stat& stat) { mStat = stat; mUpdateState = ARTIFACTSUBSTATUPDATED; }

	// Character Stat
	Stat GetCharacterStat() const { return mCharacterStat; }

	// Resonance Stat
	Stat GetResonanceStat() const    { return mResonanceStat; }
	void SetResonanceStat(const Stat& stat) { mResonanceStat = stat; ConfirmResonanceStatModified(); }

	// Weapon
	Weapon* GetWeapon()               { return mWeapon; }
	WeaponList GetWeaponName() const;
	Weapon* CopyWeapon() const;
	void    SetWeapon(Weapon* weapon) { mWeapon = weapon; ConfirmWeaponStatModified(); }

	// ArtSetStat
	ArtSetStat* GetArtSetStat() { return mArtSetStat; }
	ArtSetStat* CopyArtSetStat() const;
	void        SetArtSetStat(ArtSetStat* stat);

	// Artifact
	void        SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown);
	ArtFlower*  GetArtFlower() { return mArtFlower; }
	ArtFlower*  CopyArtFlower() const { return new ArtFlower(mArtFlower); }
	void        SetArtFlower(ArtFlower* artFlower);
	ArtFeather* GetArtFeather() { return mArtFeather; }
	ArtFeather* CopyArtFeather() const { return new ArtFeather(mArtFeather); }
	void        SetArtFeather(ArtFeather* artFeather);
	ArtClock*   GetArtClock() { return mArtClock; }
	ArtClock *  CopyArtClock() const { return new ArtClock(mArtClock); }
	void        SetArtClock(ArtClock* artClock);
	ArtCup*     GetArtCup() { return mArtCup; }
	ArtCup*     CopyArtCup() const { return new ArtCup(mArtCup); }
	void        SetArtCup(ArtCup* artCup);
	ArtCrown*   GetArtCrown() { return mArtCrown; }
	ArtCrown*   CopyArtCrown() const { return new ArtCrown(mArtCrown); }
	void        SetArtCrown(ArtCrown* artCrown);

	// Feedbacked Stat
	Stat GetFeedbackedStat() const       { return mFeedbackedStat; }
	void SetFeedbackedStat(const Stat& stat)    { mFeedbackedStat = stat; }
	void AddFeedbackedStat(int index, double amount) { mFeedbackedStat.AddOption(index, amount); }

	// TargetEC
	double GetTargetEC() const          { return mTargetEC; }
	void   SetTargetEC(double targetEC) { mTargetEC = targetEC; }
	
	// Skill GetPSkill() { return mPSkill; }
	// void  SetPSkill(Skill pSkill) { mPSkill = pSkill; }
	// Skill GetESkill() { return mESkill; }
	// void  SetESkill(Skill eSkill) { mESkill = eSkill; }
	// Skill GetQSkill() { return mQSkill; }
	// void  SetQSkill(Skill qSkill) { mQSkill = qSkill; }

	Stat GetStatAfterUpdateFromCharacterResonance() const { return mStatAfterUpdateFromCharacterResonance; }
	Stat GetStatAfterUpdateFromWeapon() const { return mStatAfterUpdateFromWeapon; }
	Stat GetStatAfterUpdateFromArtSetStat() const { return mStatAfterUpdateFromArtSetStat; }
	Stat GetStatAfterUpdateFromArtifactMainStat() const { return mStatAfterUpdateFromArtifactMainStat; }
	Stat GetStatAfterUpdateFromArtifactSubStat() const { return mStatAfterUpdateFromArtifactSubStat; }

protected:
	void SetBasicCharacterStat();
	void SetCharacterStat(int index, double amount)     { mCharacterStat.SetOption(index, amount); mUpdateState = 0; }
	void SetCharacterStat(const Stat& stat)             { mCharacterStat = stat; mUpdateState = 0; }
	void AddCharacterStat(int index, double amount)     { mCharacterStat.AddOption(index, amount); mUpdateState = 0; }
	void SetCharacterBaseStat(int index, double amount) { mCharacterStat.SetBaseOption(index, amount); }
	
	static CharList mCharacterName;

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

	bool        mIsManualMode = false;

	void        MakeScoreFunctionMainOptionFixed(int main3, int main4, int main5, int endScore=45);
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