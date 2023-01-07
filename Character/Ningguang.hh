#ifndef NINGGUANG_HH
#define NINGGUANG_HH

#include "../Character.hh"

class Ningguang : public Character
{
public:
	Ningguang(Weapon* weapon, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown) 
	: Character(weapon, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();
		AddCharacterStat(15, 24.); // Geo Bonus

		SetCharacterBaseStat(0, 212.);
		SetCharacterBaseStat(1, 9787.);
		SetCharacterBaseStat(2, 573.);
	}
	~Ningguang() {}

	double GetDamage() { return GetDamage(this->GetStat()); }
	double GetDamage(Stat stat);
};

#endif