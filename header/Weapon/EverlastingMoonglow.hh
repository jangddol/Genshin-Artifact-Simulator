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
    std::shared_ptr<Weapon> Clone_sharedptr() const override { return std::dynamic_pointer_cast<Weapon>(std::make_shared<EverlastingMoonglow>(this)); }
	~EverlastingMoonglow() override {}
};

#endif