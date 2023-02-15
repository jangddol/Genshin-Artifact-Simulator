#ifndef EVERLASTINGMOONGLOW_HH
#define EVERLASTINGMOONGLOW_HH


#include "../Weapon.hh"


class EverlastingMoonglow : public Weapon
{
public:
	EverlastingMoonglow()
    {
        mMainStat.SetZero();
        mSubStat.SetZero();
        mSubSubStat.SetZero();

        mMainStat.SetBaseOption(0, 608.);
        mSubStat.SetHPPer(49.6);
        mSubSubStat.SetHealBonus(10.);

        mWeaponName = EVERLASTINGMOONGLOW;
    }
    EverlastingMoonglow(const Weapon* weapon) : Weapon(weapon) {}
    Weapon* Clone() const override { return new EverlastingMoonglow(this); }
	~EverlastingMoonglow() override {}
};

#endif