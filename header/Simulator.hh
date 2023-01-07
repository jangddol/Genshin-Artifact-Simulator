#ifndef SIMULATOR_HH
#define SIMULATOR_HH


#include "Character.hh"
#include "TH2D.h"


class Simulator
{
public:
    Simulator() {}
    ~Simulator() {}

    TH2D* RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage,
        bool seeLastArtifact = false, bool seeTimeConsumption = false);

    Character* GetCharacter() { return mCharacter; }
    void       SetCharacter(Character* character) { mCharacter = character; }

    double GetAppendableRate() { return mAppendableRate; }

private:
    Character* mCharacter;
    double mAppendableRate;
};


#endif