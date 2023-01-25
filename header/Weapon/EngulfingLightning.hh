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
        mSubStat.SetElementCharge(55.1);

        mWeaponName = "Engulfing Lightning";
    }
    EngulfingLightning(Weapon* weapon) : Weapon(weapon) {}
    Weapon* Clone() override { return new EngulfingLightning(this); }
	~EngulfingLightning() override {}

    void DoFeedback(Character* character) override
    {
        double extraEC = character->GetStat().GetElementCharge() - 100.;
        double feedbackAP = std::min((extraEC * ECCoef), 80.);
        character->AddFeedbackedStat(2, feedbackAP);
    }

private:
    double ECCoef = 0.28;
};

#endif