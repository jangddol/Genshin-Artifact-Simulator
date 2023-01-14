#ifndef RAIDEN_HH
#define RAIDEN_HH


#include "../Character.hh"


class Raiden : public Character
{
public:
	Raiden(Weapon* weapon, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown) 
	: Character(weapon, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();

        SetTargetEC(250.);
        AddCharacterStat(4, 32.);
        AddCharacterStat(23, 0.29 * 90.); // Q Bonus (E Skill Lv = 8)

		SetCharacterBaseStat(0, 337.);
		SetCharacterBaseStat(1, 12907.);
		SetCharacterBaseStat(2, 789.);
	}
	Raiden(Raiden* other) : Character(other) {}
	Character* Clone() { return new Raiden(this); }
	~Raiden() {}

    void DoFeedback() { this->AddFeedbackedStat(11, this->GetStat().GetElementCharge()); }
	double GetDamage() { return GetDamage(this->GetStat()); }
	double GetDamage(Stat stat);
};

#endif