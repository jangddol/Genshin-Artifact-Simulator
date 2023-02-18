#ifndef NINGGUANG_HH
#define NINGGUANG_HH


#include "../Character.hh"


class Ningguang : public Character
{
public:
	Ningguang(std::shared_ptr<Weapon> weapon, std::shared_ptr<ArtSetStat> artSetStat, std::shared_ptr<ArtFlower> flower, std::shared_ptr<ArtFeather> feather, std::shared_ptr<ArtClock> clock, std::shared_ptr<ArtCup> cup, std::shared_ptr<ArtCrown> crown) 
	: Character(weapon, artSetStat, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();
		AddCharacterStat(15, 24.); // Geo Bonus

		SetCharacterBaseStat(0, 212.);
		SetCharacterBaseStat(1, 9787.);
		SetCharacterBaseStat(2, 573.);

		mCharacterName = NINGGUANG;
	}
	Ningguang(const Ningguang* other) : Character(other) { Update(); }
	std::shared_ptr<Character> Clone_sharedptr() const override { return std::dynamic_pointer_cast<Character>(std::make_shared<Ningguang>(this)); }
	~Ningguang() override {}

	double GetDamageWithStat(const Stat& stat) const override;
};

#endif