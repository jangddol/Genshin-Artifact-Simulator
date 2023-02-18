#ifndef KOKOMI_HH
#define KOKOMI_HH


#include "../Character.hh"


class Kokomi : public Character
{
public:
	Kokomi(std::shared_ptr<Weapon> weapon, std::shared_ptr<ArtSetStat> artSetStat, std::shared_ptr<ArtFlower> flower, std::shared_ptr<ArtFeather> feather, std::shared_ptr<ArtClock> clock, std::shared_ptr<ArtCup> cup, std::shared_ptr<ArtCrown> crown) 
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

		mCharacterName = KOKOMI;
	}
	Kokomi(const Kokomi* other) : Character(other) { Update(); }
	std::shared_ptr<Character> Clone_sharedptr() const override { return std::dynamic_pointer_cast<Character>(std::make_shared<Kokomi>(this)); }
	~Kokomi() override {}

	double GetDamageWithStat(const Stat& stat) const override;
};

#endif