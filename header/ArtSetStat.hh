#ifndef ARTSETSTAT_HH
#define ARTSETSTAT_HH


#include "Character.hh"


class Character;


class ArtSetStat
{
public:
    ArtSetStat() {}
    virtual ~ArtSetStat() {}

    virtual void DoFeedback(Character* character) {}

    void   SaveCharacterPointer(Character* character);
    void   DeleteCharacterPointer(Character* chraracter);

	void   SetZero() {mStat.SetZero(); AlertModified(); }
	void   Initialization() { mStat.Initialization(); AlertModified(); }

    double GetOption(int index) { return mStat.GetOption(index); }
	void   SetOption(int index, double amount) { mStat.SetOption(index, amount); AlertModified(); }
	void   AddOption(int index, double amount) { mStat.AddOption(index, amount); AlertModified(); }

private:
    void AlertModified();

    std::vector<Character*> mCharactersUsingThis = {};

    Stat mStat; // composition stat->artsetstat
};


#endif