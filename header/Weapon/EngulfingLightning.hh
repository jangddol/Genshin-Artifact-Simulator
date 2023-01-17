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
        mSubStat.SetHPPer(49.6);
        mSubSubStat.SetHealBonus(10.);

        mWeaponName = "Engulfing Lightning";
    }
	~EngulfingLightning() {}

    void DoFeedback(Character* character)
    {
        double extraEC = character->GetStat().GetElementCharge() - 100.;
        double feedbackAP = std::min((extraEC * ECCoef), 80.);
        character->AddFeedbackedStat(2, feedbackAP);
    }

private:
    double ECCoef = 0.28;
};

#endif