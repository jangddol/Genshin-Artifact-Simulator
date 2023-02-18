#ifndef WEAPON_HH
#define WEAPON_HH


#include "Stat.hh"
#include "Character.hh"
#include <string>


class Character;


enum WeaponList
{
	ENGULFINGLIGHTNING,
	EVERLASTINGMOONGLOW,
	MEMORYOFDUST,
	SOLARPEARL
};


class Weapon
{
public:
	Weapon() {}
	Weapon(const Weapon* weapon)
	{ 
		mWeaponName = weapon->GetName();
		mMainStat = weapon->GetMainStat();
		mSubStat = weapon->GetSubStat();
		mSubSubStat = weapon->GetSubSubStat();
	}
	virtual std::shared_ptr<Weapon> Clone_sharedptr() const { return std::make_shared<Weapon>(this); }
	virtual ~Weapon() {}

	virtual void DoFeedback(const Character* character, int& stat, double& amount) const { stat = 0; amount = 0; }

	Stat GetMainStat() const { return mMainStat; }
	Stat GetSubStat() const { return mSubStat; }
	Stat GetSubSubStat() const { return mSubSubStat; }

	WeaponList GetName() const { return mWeaponName; }
	
protected:
	WeaponList mWeaponName;
	Stat mMainStat;
	Stat mSubStat;
	Stat mSubSubStat;
};


#endif