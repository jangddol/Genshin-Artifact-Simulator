#ifndef NINGGUANG_HH
#define NINGGUANG_HH

#include "../Character.hh"

class Ningguang : public Character
{
public:
	Ningguang(Weapon weapon) : Character(weapon)
	{
		for (int i = 0; i < 35; i++)
		{
			SetCharacterStat(i, 0.);
		}
		SetCharacterStat(6, 9787.);
		SetCharacterStat(3, 212.);
		SetCharacterStat(9, 573.);
		SetCharacterStat(0, 5.);
		SetCharacterStat(1, 50.);
		SetCharacterStat(15, 24.);
		SetCharacterStat(32, 90.);
		SetCharacterStat(33, 90.);
	}
	~Ningguang() {}

	double GetDamage() { return GetDamage(this->GetStat()); }
	double GetDamage(Stat stat);

private:

};

#endif