#ifndef MEMORYOFDUST_HH
#define MEMORYOFDUST_HH


#include "../Weapon.hh"


class MemoryOfDust : public Weapon
{
public:
	MemoryOfDust()
    {
        mMainStat.SetZero();
        mSubStat.SetZero();
        mSubSubStat.SetZero();

        mMainStat.SetBaseOption(0, 608.);
        mSubStat.SetOption(2, 49.6);
        mSubSubStat.SetOption(2, 40.);

        mWeaponName = MEMORYOFDUST;
    }
    MemoryOfDust(const Weapon* weapon) : Weapon(weapon) {}
    Weapon* Clone() const override { return new MemoryOfDust(this); }
	~MemoryOfDust() override {}
};

#endif