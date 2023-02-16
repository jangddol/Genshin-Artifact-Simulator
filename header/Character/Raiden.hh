#ifndef RAIDEN_HH
#define RAIDEN_HH


#include "../Character.hh"


class Raiden : public Character
{
public:
	Raiden(Weapon* weapon, ArtSetStat* artSetStat, ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown) 
	: Character(weapon, artSetStat, flower, feather, clock, cup, crown)
	{
		SetBasicCharacterStat();

        SetTargetEC(250.);
        AddCharacterStat(4, 32.);
        AddCharacterStat(23, 0.29 * 90.); // Q Bonus (E Skill Lv = 8)

		SetCharacterBaseStat(0, 337.);
		SetCharacterBaseStat(1, 12907.);
		SetCharacterBaseStat(2, 789.);

		mCharacterName = RAIDEN;
	}
	Raiden(const Raiden* other) : Character(other) { Update(); }
	Raiden(const Raiden& other) : Character(other) { Update(); }
	Raiden(Raiden&& other) : Character(other) { Update(); }
	Raiden& operator = (const Raiden& other) { return * this; }
	Raiden& operator = (Raiden&& other) { return * this; }
	virtual Character* Clone() const override;
	virtual ~Raiden() override;

	virtual double GetDamageWithStat(const Stat& stat) const override;

    virtual void DoFeedback(int& stat, double& amount) override;

	double GetTotalPartyEnergy() const { return mTotalPartyEnergy; }
	void   SetTotalPartyEnergy(double totalPartyEnergy) { mTotalPartyEnergy = totalPartyEnergy; }

private:
	double mTotalPartyEnergy = 220.;
};

#endif