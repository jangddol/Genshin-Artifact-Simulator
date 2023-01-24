#include "top.hh"


void testCharacter()
{
    MemoryOfDust* weapon = new MemoryOfDust();

    ArtFlower* flower = new ArtFlower();
    ArtFeather* feather = new ArtFeather();
    ArtClock* clock = new ArtClock();
    ArtCup* cup = new ArtCup();
    ArtCrown* crown = new ArtCrown();

    Ningguang* character = new Ningguang(weapon, flower, feather, clock, cup, crown);

    Stat artSetStat = Stat();
    artSetStat.SetZero();
    artSetStat.SetAttackPer(18);
    artSetStat.SetQBonus(20);
    character->SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetResistCut(20.);
    resonanceStat.SetGeoBonus(15.);
    character->SetResonanceStat(resonanceStat);

    flower->SetMainType(6);
    feather->SetMainType(3);
    clock->SetMainType(2);
    cup->SetMainType(15);
    crown->SetMainType(0);

    character->SetArtifact(flower, feather, clock, cup, crown);

    character->Update();
    character->MakeEffectionArray();

    for (int i = 0; i < 19; i++)
    {
        cout << i << "  Effection : " << character->GetEffection(i) << endl;
    }

    character->MakeScoreFunction();

    for (int i = 0; i <= 45; i++)
    {
        cout << i << "  score : " << character->GetScoreFunction(i) << endl;
    }

    Stat charStat = character->GetStat();
    cout << "========== Character Total Stat ==========" << endl;
    PrintStat(charStat);
    double damage = character->GetDamageWithStat();
    cout << "Damage : " << damage << endl;

    PrintArtifact(*flower);
    PrintArtifact(*feather);
    PrintArtifact(*clock);
    PrintArtifact(*cup);
    PrintArtifact(*crown);
}