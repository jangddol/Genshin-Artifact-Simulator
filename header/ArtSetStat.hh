#ifndef ARTSETSTAT_HH
#define ARTSETSTAT_HH


#include "Character.hh"


class Character;


class ArtSetStat : public Stat
{
public:
    virtual void DoFeedback(Character* character) {}
};


#endif