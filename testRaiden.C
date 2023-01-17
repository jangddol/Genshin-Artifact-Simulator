#include "top.hh"
#include "PrintToCoordinates.cc"


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
    artinit4->Generation();
    artinit5->Generation();
	
    Raiden* simChar = new Raiden(weapon, artinit1, artinit2, artinit3, artinit4, artinit5);

    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();
    simChar->SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    simChar->Update();
	simChar->MakeEffectionArray();
    simChar->MakeScoreFunction();

    // WeaponStat
    PrintStat(weapon->GetMainStat());
    PrintStat(weapon->GetSubStat());
    PrintStat(weapon->GetSubSubStat());
    // ArtSetStat
    PrintStat(*artSetStat);
    // ResonanceStat
    PrintStat(resonanceStat);
    // EffectionArray
    for (int i = 0; i < 19; i++)
    {
        cout << "EffectionArray : " << STATSTRING[i] << " : " << simChar->GetEffection(i) << endl;
    }
    // ScoreFunction
    for (int i = 0; i < 46; i++)
    {
        cout << i << "-th Score : " << simChar->GetScoreFunction(i) << endl;
    }
    // Damage with random artifact
    PrintArtifact(artinit1);
    PrintArtifact(artinit2);
    PrintArtifact(artinit3);
    PrintArtifact(artinit4);
    PrintArtifact(artinit5);
    PrintStat(simChar->GetStat());

}