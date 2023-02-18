#ifndef SOLARPEARL_HH
#define SOLARPEARL_HH


#include "../Weapon.hh"


class SolarPearl : public Weapon
{
public:
	SolarPearl()
    {
        mMainStat.SetZero();
        mSubStat.SetZero();
        mSubSubStat.SetZero();

        mMainStat.SetBaseOption(0, 510.);
        mSubStat.SetOption(0, 27.6);
        mSubSubStat.SetOption(19, 40.);
        mSubSubStat.SetOption(22, 40.);
        mSubSubStat.SetOption(23, 40.);

        mWeaponName = SOLARPEARL;
    }
    SolarPearl(const Weapon* weapon) : Weapon(weapon) {}
    std::shared_ptr<Weapon> Clone_sharedptr() const override { return std::dynamic_pointer_cast<Weapon>(std::make_shared<SolarPearl>(this)); }
	~SolarPearl() override {}
};

#endif