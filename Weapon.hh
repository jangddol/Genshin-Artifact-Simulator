#ifndef WEAPON_HH
#define WEAPON_HH


#include "Stat.hh"
#include <string>

class Weapon
{
public:
	Weapon() {}
	~Weapon() {}

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