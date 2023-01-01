#ifndef NINGGUANG_HH
#define NINGGUANG_HH

#include "../Character.hh"

class Ningguang : public Character
{
public:
	Ningguang(Weapon weapon) : Character(weapon)
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

private:

};

#endif