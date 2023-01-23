#ifndef EMBLEMOFSEVEREDFATE_HH
#define EMBLEMOFSEVEREDFATE_HH


#include "../ArtSetStat.hh"


class EmblemOfSeveredFate : public ArtSetStat
{
public:
    EmblemOfSeveredFate()
    {
        SetZero();
        SetElementCharge(20.);
    }
    ~EmblemOfSeveredFate() override {}

    void DoFeedback(Character* character) override
    {
        double EC = character->GetStat().GetElementCharge();
        double qBonus = std::min(EC * 0.25, 75.);
        character->AddFeedbackedStat(23, qBonus);
    }
};

#endif