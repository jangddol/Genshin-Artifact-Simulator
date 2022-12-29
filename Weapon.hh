#ifndef WEAPON_HH
#define WEAPON_HH


#include "Stat.hh"


class Weapon
{
public:
	Weapon() {}
	~Weapon() {}

	Stat GetMainStat() { return mMainStat; }
	Stat GetSubStat() { return mSubStat; }
	Stat GetSubSubStat() { return mSubSubStat; }

private:
	Stat mMainStat;
	Stat mSubStat;
	Stat mSubSubStat;
};

#endif