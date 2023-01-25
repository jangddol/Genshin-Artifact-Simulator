#include "top.hh"
#include "PrintToCoordinates.hh"


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

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
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
    

}