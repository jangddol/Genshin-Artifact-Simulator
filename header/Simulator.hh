#ifndef SIMULATOR_HH
#define SIMULATOR_HH


#include "Character.hh"
#include "TH2D.h"
#include "TString.h"
#include <vector>


struct SuperArtifactList
{
	std::vector<ArtFlower*> flower;
	std::vector<ArtFeather*> feather;
	std::vector<ArtClock*> clock;
	std::vector<ArtCup*> cup;
	std::vector<ArtCrown*> crown;
};


class Simulator
{
public:
    Simulator() {}
    ~Simulator() {}

    TH2D* RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage,
        bool seeLastArtifact = false, bool seeTimeConsumption = false, TString histName = "");
    
    double CalLoopArtifact(Artifact* gennedArtifact, SuperArtifactList ArtifactSuperList,
        ArtFlower* &oFlower, ArtFeather* &oFeather, ArtClock* &oClock, ArtCup* &oCup, ArtCrown* &oCrown);

    Character* GetCharacter() { return mCharacter; }
    void       SetCharacter(Character* character) { mCharacter = character; }

    std::vector<double> GetAppendableRate() { return mAppendableRate; }

private:
    Character* mCharacter;
    std::vector<double> mAppendableRate;
    double CALLOOPTIMELIST[2] = { 0. };
};


#endif