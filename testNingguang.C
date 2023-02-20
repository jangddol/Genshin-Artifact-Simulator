#include "top.hh"
#include "TStyle.h"
#include "TRandom.h"


using namespace std;


void testNingguang()
{
    gStyle->SetOptStat(false);
	gRandom->SetSeed(0);

	std::shared_ptr<MemoryOfDust> weapon = std::make_shared<MemoryOfDust>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();
    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();
    std::shared_ptr<Ningguang> simChar = std::make_shared<Ningguang>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    simChar->Update();
	simChar->MakeEffectionArray();
    simChar->MakeScoreFunction();

    // Stat AfterUpdateFromCharcterResonance
    cout << "========== AfterUpdateFromCharcterResonance ==========" << endl;
    PrintStat(simChar->GetStatAfterUpdateFromCharacterResonance());
    // Stat AfterUpdateFromWeapon
    cout << "========== AfterUpdateFromWeapon ==========" << endl;
    PrintStat(simChar->GetStatAfterUpdateFromWeapon());
    // Stat AfterUpdateFromArtSetStat
    cout << "========== AfterUpdateFromArtSetStat ==========" << endl;
    PrintStat(simChar->GetStatAfterUpdateFromArtSetStat());
    // Stat AfterUpdateFromArtifactMainStat
    cout << "========== AfterUpdateFromArtifactMainStat ==========" << endl;
    PrintStat(simChar->GetStatAfterUpdateFromArtifactMainStat());
    // Stat AfterUpdateFromArtifactSubStat
    cout << "========== AfterUpdateFromArtifactSubStat ==========" << endl;
    PrintStat(simChar->GetStatAfterUpdateFromArtifactSubStat());

    // EffectionArray
    cout << "========== Effection Array ==========" << endl;
    for (int i = 0; i < 19; i++)
    {
        cout << "EffectionArray : " << STATSTRING[i] << " : " << simChar->GetEffection(i) << endl;
    }
    // ScoreFunction
    cout << "========== Score Function ==========" << endl;
    for (int i = 0; i < 46; i++)
    {
        cout << i << "-th Score : " << simChar->GetScoreFunction(i) << endl;
    }
    cout << "========== Artifact Stat ==========" << endl;
    // Damage with random artifact
    PrintArtifact(artinit1.get());
    PrintArtifact(artinit2.get());
    PrintArtifact(artinit3.get());
    PrintArtifact(artinit4.get());
    PrintArtifact(artinit5.get());
    cout << "========== Character Stat ==========" << endl;
    PrintStat(simChar->GetStat());
    cout << "========== Character Damage ==========" << endl;
    cout << simChar->GetDamage() << endl;
    

}