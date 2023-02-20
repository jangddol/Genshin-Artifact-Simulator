#include "top.hh"


using namespace std;


void testCharacter()
{
    std::shared_ptr<MemoryOfDust> weapon = std::make_shared<MemoryOfDust>();

    std::shared_ptr<ArtFlower> flower = std::make_shared<ArtFlower>();
    std::shared_ptr<ArtFeather> feather = std::make_shared<ArtFeather>();
    std::shared_ptr<ArtClock> clock = std::make_shared<ArtClock>();
    std::shared_ptr<ArtCup> cup = std::make_shared<ArtCup>();
    std::shared_ptr<ArtCrown> crown = std::make_shared<ArtCrown>();

    ArtSetStat artSetStat = ArtSetStat();
    artSetStat.SetZero();
    artSetStat.SetOption(2, 18);
    artSetStat.SetOption(23, 20);

    std::shared_ptr<Ningguang> character = std::make_shared<Ningguang>(weapon, artSetStat, flower, feather, clock, cup, crown);

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
    double damage = character->GetDamage();
    cout << "Damage : " << damage << endl;

    PrintArtifact(flower.get());
    PrintArtifact(feather.get());
    PrintArtifact(clock.get());
    PrintArtifact(cup.get());
    PrintArtifact(crown.get());
}