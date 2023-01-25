#ifndef NINGGUANG_HH
#define NINGGUANG_HH


#include "../Character.hh"


class Ningguang : public Character
{
public:
	Ningguang(Weapon* weapon, ArtSetStat* artSetStat, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown) 
	: Character(weapon, artSetStat, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();
		AddCharacterStat(15, 24.); // Geo Bonus

		SetCharacterBaseStat(0, 212.);
		SetCharacterBaseStat(1, 9787.);
		SetCharacterBaseStat(2, 573.);
	}
	Ningguang(Ningguang* other) : Character(other) {}
	Character* Clone() override { return new Ningguang(this); }
	~Ningguang() override {}

	double GetDamageWithStat(Stat stat) const override;
};

#endif