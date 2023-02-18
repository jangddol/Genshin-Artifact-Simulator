#ifndef ARTSETSTAT_HH
#define ARTSETSTAT_HH


#include "Stat.hh"
#include <vector>
#include <memory>
// #include "Character.hh" : since of cylic reference : goes to ArtSetStat.cc


class Character;


class ArtSetStat
{
public:
    ArtSetStat() {}
    ArtSetStat(const ArtSetStat* artSetStat) { mStat = artSetStat->GetStat(); }
    virtual std::shared_ptr<ArtSetStat> Clone_sharedptr() const { return std::make_shared<ArtSetStat>(this); }
    virtual ~ArtSetStat() {}

    virtual void DoFeedback(const Character* character, int& stat, double& amount) const { stat = 0; amount = 0; }

    void   SaveCharacterPointer(Character* character);
    void   DeleteCharacterPointer(const Character* chraracter);

	void   SetZero() {mStat.SetZero(); AlertModified(); }
	void   Update() { mStat.Update(); AlertModified(); }

    Stat   GetStat() const { return mStat; }

    double GetOption(int index) const { return mStat.GetOption(index); }
	void   SetOption(int index, double amount) { mStat.SetOption(index, amount); AlertModified(); }
	void   AddOption(int index, double amount) { mStat.AddOption(index, amount); AlertModified(); }

    bool   IsUsingThis(const Character* character) const;

private:
    void AlertModified() const;

    std::vector<Character*> mCharactersUsingThis = {};

    Stat mStat; // composition stat->artsetstat
};


#endif