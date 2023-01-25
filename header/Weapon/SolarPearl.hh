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

        mWeaponName = "Solar Pearl";
    }
    SolarPearl(const Weapon* weapon) : Weapon(weapon) {}
    Weapon* Clone() const override { return new SolarPearl(this); }
	~SolarPearl() override {}
};

#endif