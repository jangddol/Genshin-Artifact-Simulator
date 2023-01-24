#ifndef WEAPON_HH
#define WEAPON_HH


#include "Stat.hh"
#include "Character.hh"
#include <string>


class Character;


class Weapon
{
public:
	Weapon() {}
	Weapon(Weapon* weapon)
	{ 
		mWeaponName = weapon->GetName();
		mMainStat = weapon->GetMainStat();
		mSubStat = weapon->GetSubStat();
		mSubSubStat = weapon->GetSubSubStat();
	}
	virtual Weapon* Clone() { return new Weapon(this); }
	virtual ~Weapon() {}

	virtual void DoFeedback(Character* character) {}

	Stat GetMainStat() { return mMainStat; }
	Stat GetSubStat() { return mSubStat; }
	Stat GetSubSubStat() { return mSubSubStat; }

	string GetName() { return mWeaponName; }
	
protected:
	string mWeaponName;
	Stat mMainStat;
	Stat mSubStat;
	Stat mSubSubStat;
};

#endif