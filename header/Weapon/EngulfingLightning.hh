#ifndef ENGULFINGLIGHTNING_HH
#define ENGULFINGLIGHTNING_HH


#include "../Weapon.hh"
#include <algorithm>


class EngulfingLightning : public Weapon
{
public:
	EngulfingLightning()
    {
        mMainStat.SetZero();
        mSubStat.SetZero();
        mSubSubStat.SetZero();

        mMainStat.SetBaseOption(0, 608.);
        mSubStat.SetElementCharge(85.1); // 55.1 + 30

        mWeaponName = ENGULFINGLIGHTNING;
    }
    EngulfingLightning(const Weapon* weapon) : Weapon(weapon) {}
    Weapon* Clone() const override { return new EngulfingLightning(this); }
	~EngulfingLightning() override {}

    void DoFeedback(const Character* character, int& stat, double& amount) const override
    {
        double extraEC = character->GetStat().GetElementCharge() - 100.;
        amount = std::min((extraEC * ECCoef), 80.);
        stat = 2;
    }

private:
    double ECCoef = 0.28;
};

#endif