#include "top.hh"
#include "PrintToCoordinates.hh"
#include "TRandom.h"
#include "TStyle.h"


using namespace std;


void testRaiden()
{
    gStyle->SetOptStat(false);
	gRandom->SetSeed(0);

	EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->SetSubStat(Stat());
    artinit2->SetSubStat(Stat());
    artinit3->SetSubStat(Stat());
    artinit3->SetMainType(4);
    artinit4->SetSubStat(Stat());
    artinit4->SetMainType(2);
    artinit5->SetSubStat(Stat());
    artinit5->SetMainType(1);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

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
    PrintArtifact(artinit1);
    PrintArtifact(artinit2);
    PrintArtifact(artinit3);
    PrintArtifact(artinit4);
    PrintArtifact(artinit5);
    cout << "========== Character Stat ==========" << endl;
    PrintStat(simChar->GetStat());
    cout << "========== Character Damage ==========" << endl;
    cout << simChar->GetDamage() << endl;

    cout << "========== Clone Test ==========" << endl;
    cout << "========== Cloned Character Stat ==========" << endl;
    PrintStat(simChar->Clone()->GetStat());
    cout << "========== Cloned Character Damage ==========" << endl;
    cout << simChar->Clone()->GetDamage() << endl;

    std::array<MainOptionsAndDamage, 10> top10Options = simChar->OptimizeMainOption();
    cout << "========== Artifact Main Option Optimization Test ==========" << endl;
    cout << " 1st : " << top10Options[0].damage << " : " << STATSTRING[top10Options[0].mainOptions[0]] << ", " << STATSTRING[top10Options[0].mainOptions[1]] << ", " << STATSTRING[top10Options[0].mainOptions[2]] << endl;
    cout << " 2nd : " << top10Options[1].damage << " : " << STATSTRING[top10Options[1].mainOptions[0]] << ", " << STATSTRING[top10Options[1].mainOptions[1]] << ", " << STATSTRING[top10Options[1].mainOptions[2]] << endl;
    cout << " 3rd : " << top10Options[2].damage << " : " << STATSTRING[top10Options[2].mainOptions[0]] << ", " << STATSTRING[top10Options[2].mainOptions[1]] << ", " << STATSTRING[top10Options[2].mainOptions[2]] << endl;
    cout << " 4th : " << top10Options[3].damage << " : " << STATSTRING[top10Options[3].mainOptions[0]] << ", " << STATSTRING[top10Options[3].mainOptions[1]] << ", " << STATSTRING[top10Options[3].mainOptions[2]] << endl;
    cout << " 5th : " << top10Options[4].damage << " : " << STATSTRING[top10Options[4].mainOptions[0]] << ", " << STATSTRING[top10Options[4].mainOptions[1]] << ", " << STATSTRING[top10Options[4].mainOptions[2]] << endl;
    cout << " 6th : " << top10Options[5].damage << " : " << STATSTRING[top10Options[5].mainOptions[0]] << ", " << STATSTRING[top10Options[5].mainOptions[1]] << ", " << STATSTRING[top10Options[5].mainOptions[2]] << endl;
    cout << " 7th : " << top10Options[6].damage << " : " << STATSTRING[top10Options[6].mainOptions[0]] << ", " << STATSTRING[top10Options[6].mainOptions[1]] << ", " << STATSTRING[top10Options[6].mainOptions[2]] << endl;
    cout << " 8th : " << top10Options[7].damage << " : " << STATSTRING[top10Options[7].mainOptions[0]] << ", " << STATSTRING[top10Options[7].mainOptions[1]] << ", " << STATSTRING[top10Options[7].mainOptions[2]] << endl;
    cout << " 9th : " << top10Options[8].damage << " : " << STATSTRING[top10Options[8].mainOptions[0]] << ", " << STATSTRING[top10Options[8].mainOptions[1]] << ", " << STATSTRING[top10Options[8].mainOptions[2]] << endl;
    cout << "10th : " << top10Options[9].damage << " : " << STATSTRING[top10Options[9].mainOptions[0]] << ", " << STATSTRING[top10Options[9].mainOptions[1]] << ", " << STATSTRING[top10Options[9].mainOptions[2]] << endl;


    cout << "========== Score Function Test (artifact regeneration)==========" << endl;
    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit4->Generation();
    artinit5->Generation();
    simChar->Update();
    cout << "========== Artifact Stat ==========" << endl;
    // Damage with random artifact
    PrintArtifact(artinit1);
    PrintArtifact(artinit2);
    PrintArtifact(artinit3);
    PrintArtifact(artinit4);
    PrintArtifact(artinit5);
    cout << "========== Character Stat ==========" << endl;
    PrintStat(simChar->GetStat());
    cout << "========== Character Damage ==========" << endl;
    cout << simChar->GetDamage() << endl;
    cout << "score  : " << simChar->GetScore() << endl;
}