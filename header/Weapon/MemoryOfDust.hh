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
    std::shared_ptr<Weapon> Clone_sharedptr() const override { return std::dynamic_pointer_cast<Weapon>(std::make_shared<MemoryOfDust>(this)); }
	~MemoryOfDust() override {}
};

#endif