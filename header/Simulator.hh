#ifndef SIMULATOR_HH
#define SIMULATOR_HH


#include "Character.hh"
#include "TH2D.h"
#include "TString.h"
#include <vector>
#include <thread>


struct ArtifactBundle
{
    ArtFlower* flower;
    ArtFeather* feather;
    ArtClock* clock;
    ArtCup* cup;
    ArtCrown* crown;
};


class SuperArtifactList
{
public:
    SuperArtifactList() { Clear(); }
    ~SuperArtifactList() {}

    void Clear() { flower = {}; feather = {}; clock = {}; cup = {}; crown = {};}
    void DeleteAll()
    {
        for (int j = 0; j < flower.size(); j++) delete flower[j];
		for (int j = 0; j < feather.size(); j++) delete feather[j];
		for (int j = 0; j < clock.size(); j++) delete clock[j];
		for (int j = 0; j < cup.size(); j++) delete cup[j];
		for (int j = 0; j < crown.size(); j++) delete crown[j];
    }

	std::vector<ArtFlower*> flower;
	std::vector<ArtFeather*> feather;
	std::vector<ArtClock*> clock;
	std::vector<ArtCup*> cup;
	std::vector<ArtCrown*> crown;
};


enum ScoreIndex
{
    DAMAGE,
    JANGDDOL,
    MONKEYMAGIC
};


class Simulator
{
public:
    Simulator() { mNumThread = std::thread::hardware_concurrency(); }
    virtual ~Simulator() {}

    TH2D* RunSimulation(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage, TString histName = "");

    TH2D* RunSimulationMultiThreads(int simNum, int artifactNum, int binNum, double minDamage, double maxDamage);
    TH2D* GetSimulationResult() { return mSimulationResult; }

    Character* GetCharacter() { return mCharacter; }
    void       SetCharacter(Character* character) { mCharacter = character; }
    int        GetNumThread() const { return mNumThread; }
    void       SetNumThread(int numThread) { mNumThread = numThread; }
    
    void SetWorkerMode(bool workerMode) { mWorkerMode = workerMode; if (workerMode) mSeeLastArtifact = false; }
    void SetSeeTimeConsumption(bool seeTimeConsumption) { mSeeTimeConsumption = seeTimeConsumption; }
    void SetSeeLastArtifact(bool seeLastArtifact) { mSeeLastArtifact = seeLastArtifact; }
    void SetScoreIndexMode(ScoreIndex scoreIndex) { mScoreIndex = scoreIndex; }

    std::vector<double> GetAppendableRate() const { return mAppendableRate; }
    double GetCalLoopTimeList(int index) const { return mCalLoopTimeList[index]; }
    double GetTimeList(int index) const { return mTimeList[index]; }

private:
    ScoreIndex mScoreIndex = DAMAGE;
    double CalLoopArtifact_Damage(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact_jangddolScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact_MonkeyMagicScore(SuperArtifactList& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact(Artifact* gennedArtifact, SuperArtifactList& ArtifactSuperList,
                            ArtifactBundle& bestTryArtifacts);

    void PrintLastArtifacts(int trialNum, double bestDamage, const ArtifactBundle& bestArtifacts) const;
    void PrintTimeConsumption() const;
    void PrintProgress(int trial, int nowArtNum, int simNum, int artifactNum) const;

    Character* mCharacter;
    std::vector<double> mAppendableRate;

    double mCalLoopTimeList[2] = { 0. };
    double mTimeList[4] = { 0. };

    void SimulationWorker(int workerID, int simNum, int artifactNum, int binNum, double minDamage, double maxDamage);
    
    int mNumThread;

    bool mSeeLastArtifact = false;
    bool mSeeTimeConsumption = false;
    bool mWorkerMode = false;

    void SetWorkerID(int workerID) { mWorkerID = workerID; }
    int mWorkerID = 1;

    TH2D* mSimulationResult;
};


#endif