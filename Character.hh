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

class Character
{
public:
	Character(Weapon weapon) { mWeapon = weapon; }
	~Character() {}

	void Initialization();

	virtual double GetDamage() { return GetDamage(mStat); }
	virtual double GetDamage(Stat stat);
	void MakeScoreFunction();
	double GetScore();
	double GetEffection(int index) { return mEffectionArray[index]; }

	Stat GetStat() { return mStat; }
	void SetStat(Stat stat) { mStat = stat; }
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
	void SetCharacterStat(int index, double amount) { mCharacterStat.SetOption(index, amount); }

private:
	Stat GenerateStatExceptArtifact();

	double mSavedFunction[46];
	double mEffectionArray[10];

	Stat mStat;
	Stat mCharacterStat;
	double mTargetEC;
	Weapon mWeapon;
	ArtFlower  fArtFlower;
	ArtFeather fArtFeather;
	ArtClock   fArtClock;
	ArtCup     fArtCup;
	ArtCrown   fArtCrown;
	// Skill mPSkill;
	// Skill mESkill;
	// Skill mQSkill;

};

#endif