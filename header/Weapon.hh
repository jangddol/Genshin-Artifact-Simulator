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
	Weapon(const Weapon* weapon)
	{ 
		mWeaponName = weapon->GetName();
		mMainStat = weapon->GetMainStat();
		mSubStat = weapon->GetSubStat();
		mSubSubStat = weapon->GetSubSubStat();
	}
	virtual Weapon* Clone() const { return new Weapon(this); }
	virtual ~Weapon() {}

	virtual void DoFeedback (Character* character) const {}

	Stat GetMainStat() const { return mMainStat; }
	Stat GetSubStat() const { return mSubStat; }
	Stat GetSubSubStat() const { return mSubSubStat; }

	std::string GetName() const { return mWeaponName; }
	
protected:
	std::string mWeaponName;
	Stat mMainStat;
	Stat mSubStat;
	Stat mSubSubStat;
};

#endif