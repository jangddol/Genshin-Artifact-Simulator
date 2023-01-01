#include "top.hh"


void testCharacter()
{
    MemoryOfDust weapon = MemoryOfDust();
    Ningguang character = Ningguang(weapon);

    Stat artSetStat = Stat();
    artSetStat.SetZero();
    artSetStat.SetAttackPer(18);
    artSetStat.SetQBonus(20);
    character.SetArtSetStat(artSetStat);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetResistCut(20.);
    resonanceStat.SetGeoBonus(15.);
    character.SetResonanceStat(resonanceStat);

    ArtFlower flower = ArtFlower();
    ArtFeather feather = ArtFeather();
    ArtClock clock = ArtClock();
    ArtCup cup = ArtCup();
    ArtCrown crown = ArtCrown();

    flower.SetMainType(6);
    feather.SetMainType(3);
    clock.SetMainType(2);
    cup.SetMainType(15);
    crown.SetMainType(0);

    character.SetArtifact(flower, feather, clock, cup, crown);

    character.Initialization();
    character.MakeEffectionArray();

    for (int i = 0; i < 10; i++)
    {
        cout << i << "  Effection : " << character.GetEffection(i) << endl;
    }

    character.MakeScoreFunction();

    for (int i = 0; i <= 45; i++)
    {
        cout << i << "  score : " << character.GetScoreFunction(i) << endl;
    }

    Stat charStat = character.GetStat();
    cout << "========== Character Total Stat ==========" << endl;
    PrintStat(charStat);
    double damage = character.GetDamage();
    cout << "Damage : " << damage << endl;
}