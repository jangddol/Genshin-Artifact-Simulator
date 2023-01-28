#ifndef KOKOMI_HH
#define KOKOMI_HH


#include "../Character.hh"


class Kokomi : public Character
{
public:
	Kokomi(Weapon* weapon, ArtSetStat* artSetStat, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown) 
	: Character(weapon, artSetStat, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();

        SetTargetEC(160.);
        AddCharacterStat(0, -100.);
        AddCharacterStat(13, 28.8);
		AddCharacterStat(18, 25.); // Heal Bonus

		SetCharacterBaseStat(0, 234.);
		SetCharacterBaseStat(1, 13471.);
		SetCharacterBaseStat(2, 657.);
	}
	Kokomi(const Kokomi* other) : Character(other) { Update(); }
	Character* Clone() const override { return new Kokomi(this); }
	~Kokomi() override {}

	double GetDamageWithStat(const Stat& stat) const override;
};

#endif