#ifndef SIMULATOR_HH
#define SIMULATOR_HH


#include "Character.hh"
#include "TH2D.h"
#include "TString.h"
#include <vector>
#include <thread>


struct ArtifactBundle
{
    std::shared_ptr<ArtFlower> flower;
    std::shared_ptr<ArtFeather> feather;
    std::shared_ptr<ArtClock> clock;
    std::shared_ptr<ArtCup> cup;
    std::shared_ptr<ArtCrown> crown;
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

    int  GetBundleNum() { return mBundleNum; }
    void SetBundleNum(int bundleNum) { mBundleNum = bundleNum; }
    std::shared_ptr<Character> GetCharacter() { return mCharacter; }
    void SetCharacter(std::shared_ptr<Character> character) { mCharacter = character; }
    int  GetNumThread() const { return mNumThread; }
    void SetNumThread(int numThread) { mNumThread = numThread; }
    
    void SetWorkerMode(bool workerMode) { mWorkerMode = workerMode; if (workerMode) mSeeLastArtifact = false; }
    void SetSeeTimeConsumption(bool seeTimeConsumption) { mSeeTimeConsumption = seeTimeConsumption; }
    void SetSeeLastArtifact(bool seeLastArtifact) { mSeeLastArtifact = seeLastArtifact; }
    void SetScoreIndexMode(ScoreIndex scoreIndex) { mScoreIndex = scoreIndex; }

    std::vector<double> GetAppendableRate() const { return mAppendableRate; }
    double GetCalLoopTimeList(int index) const { return mCalLoopTimeList[index]; }
    double GetTimeList(int index) const { return mTimeList[index]; }

private:
    ScoreIndex mScoreIndex = DAMAGE;
    double CalLoopArtifact_Damage(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact_jangddolScore(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact_MonkeyMagicScore(std::array<std::vector<std::shared_ptr<Artifact>>, 5>& loopList, ArtifactBundle& bestTryArtifacts);
    double CalLoopArtifact(std::shared_ptr<Artifact> gennedArtifact, std::array<std::vector<std::shared_ptr<Artifact>>, 5>& ArtifactSuperList,
                            ArtifactBundle& bestTryArtifacts);

    void PrintLastArtifacts(int trialNum, double bestDamage, const ArtifactBundle& bestArtifacts) const;
    void PrintTimeConsumption() const;
    void PrintProgress(int trial, int nowArtNum, int simNum, int artifactNum) const;

    int mBundleNum = 1;

    std::shared_ptr<Character> mCharacter;
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

    static constexpr double MINSCORE = 0.;
    static constexpr double MAXSCORE = 45.;
};


#endif