#ifndef SIMULATOR_HH
#define SIMULATOR_HH


#include "../top.hh"


class Simulator
{
public:
    Simulator() {}
    ~Simulator() {}

    TH2D* RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage
    bool seeLastArtifact, bool seeTimeConsumption);

    Character* GetCharacter() { return mCharacter; }
    void       SetCharacter(Character* character) { mCharacter = character; }

private:
    Character* mCharacter;
    
}


#endif