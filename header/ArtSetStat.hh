#ifndef ARTSETSTAT_HH
#define ARTSETSTAT_HH


#include "Character.hh"


class Character;


class ArtSetStat
{
public:
    ArtSetStat() {}
    ArtSetStat(const ArtSetStat* artSetStat) { mStat = artSetStat->GetStat(); }
    virtual ArtSetStat* Clone() const { return new ArtSetStat(*this); }
    virtual ~ArtSetStat() {}

    virtual void DoFeedback(Character* character) const {}

    void   SaveCharacterPointer(const Character* character);
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