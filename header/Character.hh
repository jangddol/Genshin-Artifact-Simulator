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
	std::array<int, 3> mainOptions{ {-1, -1, -1} };
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
	Character(  std::shared_ptr<Weapon> weapon,
				std::shared_ptr<ArtSetStat> artSetStat,
				std::shared_ptr<ArtFlower> flower,
				std::shared_ptr<ArtFeather> feather,
				std::shared_ptr<ArtClock> clock,
				std::shared_ptr<ArtCup> cup,
				std::shared_ptr<ArtCrown> crown)
	: mTargetEC{100.}, mWeapon{weapon}
	{ 
		SetArtifact(flower, feather, clock, cup, crown);
		SetArtSetStat(artSetStat);
		mEffectiveSubStats.reserve(10);
	}
	Character(const Character* character);
	Character(const Character& character);
	Character(Character&& character);
	Character& operator = (const Character &character);
	Character& operator = (Character &&character);
	virtual std::shared_ptr<Character> Clone_sharedptr() const;
	virtual ~Character();

	// Stat Update & UpdateState
	virtual void DoFeedback(int& stat, double& amount);
	void Update(bool fastMode=false);
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
	double GetEffection(int index) const { return mEffectionAmount[index]; }
	std::vector<int> GetEffectiveSubStats() const { return mEffectiveSubStats; }
	std::array<double, 19> GetScoreEffection() const;

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
	std::shared_ptr<Weapon> GetWeapon()               { return mWeapon; }
	WeaponList GetWeaponName() const;
	std::shared_ptr<Weapon> CopyWeapon() const;
	void    SetWeapon(const std::shared_ptr<Weapon>& weapon) { mWeapon = weapon; ConfirmWeaponStatModified(); }

	// ArtSetStat
	std::shared_ptr<ArtSetStat> GetArtSetStat() { return mArtSetStat; }
	std::shared_ptr<ArtSetStat> CopyArtSetStat() const;
	void        SetArtSetStat(const std::shared_ptr<ArtSetStat>& stat);

	// Artifact
	void        SetArtifact(const std::shared_ptr<ArtFlower>& flower,
							const std::shared_ptr<ArtFeather>& feather,
							const std::shared_ptr<ArtClock>& clock,
							const std::shared_ptr<ArtCup>& cup,
							const std::shared_ptr<ArtCrown>& crown);
	std::shared_ptr<ArtFlower>  GetArtFlower() { return mArtFlower; }
	std::shared_ptr<ArtFlower>  CopyArtFlower() const { return std::make_shared<ArtFlower>(mArtFlower.get()); }
	void        SetArtFlower(const std::shared_ptr<ArtFlower>& artFlower);
	std::shared_ptr<ArtFeather> GetArtFeather() { return mArtFeather; }
	std::shared_ptr<ArtFeather> CopyArtFeather() const { return std::make_shared<ArtFeather>(mArtFeather.get()); }
	void        SetArtFeather(const std::shared_ptr<ArtFeather>& artFeather);
	std::shared_ptr<ArtClock>   GetArtClock() { return mArtClock; }
	std::shared_ptr<ArtClock >  CopyArtClock() const { return std::make_shared<ArtClock>(mArtClock.get()); }
	void        SetArtClock(const std::shared_ptr<ArtClock>& artClock);
	std::shared_ptr<ArtCup>     GetArtCup() { return mArtCup; }
	std::shared_ptr<ArtCup>     CopyArtCup() const { return std::make_shared<ArtCup>(mArtCup.get()); }
	void        SetArtCup(const std::shared_ptr<ArtCup>& artCup);
	std::shared_ptr<ArtCrown>   GetArtCrown() { return mArtCrown; }
	std::shared_ptr<ArtCrown>   CopyArtCrown() const { return std::make_shared<ArtCrown>(mArtCrown.get()); }
	void        SetArtCrown(const std::shared_ptr<ArtCrown>& artCrown);

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
	
	CharList mCharacterName;

private:
	// Stat Update Process
	void UpdateFromCharacterResonance();
	void UpdateFromWeapon();
	void UpdateFromArtSetStat();
	void UpdateFromArtifactMainStat();
	void UpdateFromArtifactSubStat();
	void UpdateFromArtifactSubStatFast();
	void UpdateFromFeedback();

	int  mUpdateState = 0;
	Stat mStatAfterUpdateFromCharacterResonance; // never do initialization
	Stat mStatAfterUpdateFromWeapon; // never do initialization
	Stat mStatAfterUpdateFromArtSetStat; // never do initialization
	Stat mStatAfterUpdateFromArtifactMainStat; // never do initialization
	Stat mStatAfterUpdateFromArtifactSubStat; // never do initialization

	constexpr static int CHARACTERRESONANCEUPDATED = 1;
    constexpr static int WEAPONSTATUPDATED = 2;
    constexpr static int ARTSETSTATUPDATED = 3;
    constexpr static int ARTIFACTMAINSTATUPDATED = 4;
    constexpr static int ARTIFACTSUBSTATUPDATED = 5;

	bool        mIsManualMode = false;

	void        MakeScoreFunctionMainOptionFixed(int main3, int main4, int main5, int endScore=45);
	std::array<double, 46> mSavedFunction;
	std::array<double, 19> mEffectionAmount;
	std::vector<int> mEffectiveSubStats;

	Stat        mStat;
	Stat        mCharacterStat;
	double      mTargetEC;
	std::shared_ptr<Weapon>     mWeapon;
	std::shared_ptr<ArtFlower>  mArtFlower;
	std::shared_ptr<ArtFeather> mArtFeather;
	std::shared_ptr<ArtClock>   mArtClock;
	std::shared_ptr<ArtCup>     mArtCup;
	std::shared_ptr<ArtCrown>   mArtCrown;
	std::shared_ptr<ArtSetStat> mArtSetStat;
	Stat        mResonanceStat;
	// Skill mPSkill;
	// Skill mESkill;
	// Skill mQSkill;
};


#endif