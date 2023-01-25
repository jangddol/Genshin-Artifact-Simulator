#ifndef EMBLEMOFSEVEREDFATE_HH
#define EMBLEMOFSEVEREDFATE_HH


#include "../ArtSetStat.hh"


class EmblemOfSeveredFate : public ArtSetStat
{
public:
    EmblemOfSeveredFate() : ArtSetStat()
    {
        SetZero();
        SetOption(4, 20.); // EC
    }
    ArtSetStat* Clone() const override { return new EmblemOfSeveredFate(*this); }
    ~EmblemOfSeveredFate() override {}

    void DoFeedback(Character* character) const override
    {
        double EC = character->GetStat().GetElementCharge();
        double qBonus = std::min(EC * 0.25, 75.);
        character->AddFeedbackedStat(23, qBonus);
    }
};

#endif