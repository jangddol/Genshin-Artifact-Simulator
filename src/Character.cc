#include "../header/Character.hh"
#include <algorithm>
#include <iostream>
#include <ctime>


Character::Character(Character *character)
{
    SetWeapon(new Weapon(character->GetWeapon()));
    SetArtFlower(new ArtFlower(character->GetArtFlower()));
    SetArtFeather(new ArtFeather(character->GetArtFeather()));
    SetArtClock(new ArtClock(character->GetArtClock()));
    SetArtCup(new ArtCup(character->GetArtCup()));
    SetArtCrown(new ArtCrown(character->GetArtCrown()));
    
    mArtSetStat = character->GetArtSetStat();
    mResonanceStat = character->GetResonanceStat();
    mTargetEC = character->GetTargetEC();
    mCharacterStat = character->GetCharacterStat();
    for (int i = 0; i < 19; i++)
    {
        mEffectionArray[i] = character->GetEffection(i);
    }
    for (int i = 0; i < 46; i++)
    {
        mSavedFunction[i] = character->GetScoreFunction(i);
    }
    Initialization();
}


void Character::Initialization()
{
    if (!bPossibleExceptArtifact)
    {
        InitializationExceptArtifact();
    }
    ArtifactInitialization();
}



void Character::ArtifactInitialization()
{
    // double ARTINITSTART, ARTINITFINISH;
    // ARTINITSTART = clock();
    
    Stat FlowerMainStat = mArtFlower->GetMainStat();
    Stat FlowerSubStat = mArtFlower->GetSubStat();
    Stat FeatherMainStat = mArtFeather->GetMainStat();
    Stat FeatherSubStat = mArtFeather->GetSubStat();
    Stat ClockMainStat = mArtClock->GetMainStat();
    Stat ClockSubStat = mArtClock->GetSubStat();
    Stat CupMainStat = mArtCup->GetMainStat();
    Stat CupSubStat = mArtCup->GetSubStat();
    Stat CrownMainStat = mArtCrown->GetMainStat();
    Stat CrownSubStat = mArtCrown->GetSubStat();

    // ARTINITFINISH = clock();
	// artInitTimeList[0] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    // ARTINITSTART = ARTINITFINISH;

    mStat = mStatExceptArtifact;

    // ARTINITFINISH = clock();
	// artInitTimeList[1] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    // ARTINITSTART = ARTINITFINISH;

    mStat.AddOption(6, FlowerMainStat.GetOption(6));
    mStat.AddOption(3, FeatherMainStat.GetOption(3));
    mStat.AddOption(mArtClock->GetMainType(), ClockMainStat.GetOption(mArtClock->GetMainType()));
    mStat.AddOption(mArtCup->GetMainType(), CupMainStat.GetOption(mArtCup->GetMainType()));
    mStat.AddOption(mArtCrown->GetMainType(), CrownMainStat.GetOption(mArtCrown->GetMainType()));

    // ARTINITFINISH = clock();
	// artInitTimeList[2] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    // ARTINITSTART = ARTINITFINISH;

    for (int i = 0; i < 10; i++)
    {
        mStat.AddOption(i, FlowerSubStat.GetOption(i));
        mStat.AddOption(i, FeatherSubStat.GetOption(i));
        mStat.AddOption(i, ClockSubStat.GetOption(i));
        mStat.AddOption(i, CupSubStat.GetOption(i));
        mStat.AddOption(i, CrownSubStat.GetOption(i));
    }

    // ARTINITFINISH = clock();
	// artInitTimeList[3] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    // ARTINITSTART = ARTINITFINISH;

    mStat.InitializationFast();

    // ARTINITFINISH = clock();
	// artInitTimeList[4] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
}


void Character::InitializationExceptArtifact()
{
    // 캐릭터 기초 스탯, 무기, 성유물의 효과를 모두 합산해서 mStat으로 넘기는 작업
    
    Stat WeaponMainStat = mWeapon->GetMainStat();
    Stat WeaponSubStat = mWeapon->GetSubStat();
    Stat WeaponSubSubStat = mWeapon->GetSubSubStat();

    mStatExceptArtifact.SetZero();

    for (int i = 0; i < 35; i++)
    {
        mStatExceptArtifact.AddOption(i, mCharacterStat.GetOption(i));
        mStatExceptArtifact.AddOption(i, WeaponMainStat.GetOption(i));
        mStatExceptArtifact.AddOption(i, WeaponSubStat.GetOption(i));
        mStatExceptArtifact.AddOption(i, WeaponSubSubStat.GetOption(i));
        mStatExceptArtifact.AddOption(i, mArtSetStat.GetOption(i));
        mStatExceptArtifact.AddOption(i, mResonanceStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatExceptArtifact.SetBaseOption(i, mCharacterStat.GetBaseOption(i) + WeaponMainStat.GetBaseOption(i));
    }

    mStatExceptArtifact.Initialization();
    bPossibleExceptArtifact = true;
}


double Character::GetDamage(Stat stat)
{
    double AP = stat.GetAttackPer();
    double ATK = stat.GetAttack();
    double BaseATK = stat.GetBaseAttack();
    double CR = stat.GetCriticalRate();
    if (CR > 100) CR = 100.;
    if (CR < 0) CR = 0.;
    double CB = stat.GetCriticalBonus();

    return (BaseATK * (1 + AP / 100.) + ATK) * (1 + CR * CB / 10000.);
}


void Character::MakeEffectionArray()
{
    Stat tempStat = GenerateStatExceptSubOpt(); // 성유물이 초기화된 새로운 스탯을 생성한다.
    Stat tempStatArray[19] = { tempStat, tempStat, tempStat, tempStat, tempStat,
                                tempStat, tempStat, tempStat, tempStat, tempStat,
                                tempStat, tempStat, tempStat, tempStat, tempStat,
                                tempStat, tempStat, tempStat, tempStat};

    double startDamage = GetDamage(tempStat); // 현재 스펙을 기록한다.

    for (int j = 0; j < 19; j++)
    {
        tempStatArray[j].AddOption(j, 1.);
        tempStatArray[j].Initialization();
        mEffectionArray[j] = GetDamage(tempStatArray[j]) - startDamage;
    }
}


void Character::SetBasicCharacterStat()
{
    mCharacterStat.SetZero();	
	mCharacterStat.SetCriticalRate(5.);
	mCharacterStat.SetCriticalBonus(50.);
    mCharacterStat.SetElementCharge(100.);
    mCharacterStat.SetLevel(90.);
    mCharacterStat.SetMonsterLevel(100.);
    mCharacterStat.SetMonsterResist(10.);
}


Stat Character::GenerateStatExceptSubOpt()
{    
    Stat returnStat;
    
    Stat WeaponMainStat = mWeapon->GetMainStat();
    Stat WeaponSubStat = mWeapon->GetSubStat();
    Stat WeaponSubSubStat = mWeapon->GetSubSubStat();
    Stat FlowerMainStat = mArtFlower->GetMainStat();
    Stat FeatherMainStat = mArtFeather->GetMainStat();
    Stat ClockMainStat = mArtClock->GetMainStat();
    Stat CupMainStat = mArtCup->GetMainStat();
    Stat CrownMainStat = mArtCrown->GetMainStat();

    for (int i = 0; i < 35; i++)
    {
        returnStat.SetOption(i, 0.);
        returnStat.AddOption(i, mCharacterStat.GetOption(i));
        returnStat.AddOption(i, WeaponMainStat.GetOption(i));
        returnStat.AddOption(i, WeaponSubStat.GetOption(i));
        returnStat.AddOption(i, WeaponSubSubStat.GetOption(i));
        returnStat.AddOption(i, FlowerMainStat.GetOption(i));
        returnStat.AddOption(i, FeatherMainStat.GetOption(i));
        returnStat.AddOption(i, ClockMainStat.GetOption(i));
        returnStat.AddOption(i, CupMainStat.GetOption(i));
        returnStat.AddOption(i, CrownMainStat.GetOption(i));
        returnStat.AddOption(i, mArtSetStat.GetOption(i));
        returnStat.AddOption(i, mResonanceStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        returnStat.SetBaseOption(i, mCharacterStat.GetBaseOption(i) + WeaponMainStat.GetBaseOption(i));
    }
    returnStat.Initialization();

    return returnStat;
}


int FindNthLargestOption(double damArray[], int nth)
{
    std::vector<std::pair<double, int>> tempList;
    tempList.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        tempList.emplace_back(damArray[i], i);
    }
    std::sort(tempList.begin(), tempList.end(), [](const auto& a, const auto& b){
        return a.first > b.first;
    });
    return tempList[nth - 1].second;
}


void Character::MakeScoreFunction()
{
    int mainOp[10] = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    int numArray[10] = { 0 }; // It will be recorded in this array how many times each option is added.
    double damArray[10] = { 0 }; // It will be recorded in this array how much damage will be if each option is added.

    Stat tempStat = GenerateStatExceptSubOpt(); // 성유물이 초기화된 새로운 스탯을 생성한다.
    double startDamage = GetDamage(tempStat); // 현재 스펙을 기록한다.

    Stat tempStatArray[10] = { tempStat, tempStat, tempStat, tempStat, tempStat,
                               tempStat, tempStat, tempStat, tempStat, tempStat };

    mSavedFunction[0] = startDamage;

    for (int i = 0; i < 45; i++) // for문으로 45회동안, 
    {
        double beforeDamage = GetDamage(tempStat); // record the damage.
        double difEC = mTargetEC - tempStat.GetOption(4); // check the element charge is enough or not.
        bool whetherNotEnoughEC = (difEC > 0);

        if (i < 20)
        {
            // If the element charge is not enouth, add element charge.
            if (whetherNotEnoughEC && (5 - mainOp[4] > numArray[4]))
            {
                tempStat.AddOption(4, PLUSARRAY[4]);
                numArray[4] += 1;
            }
            else // If impossible,
            {
                // record how much damage will be if each option is added at damArray.
                for (int j = 0; j < 10; j++) 
                {
                    tempStatArray[j] = tempStat;
                    tempStatArray[j].AddOption(j, PLUSARRAY[j]);
                    tempStatArray[j].Initialization();
                    damArray[j] = GetDamage(tempStatArray[j]);
                }

                // 가장 점수가 높은 스탯에 대해서 ((5 - 주옵여부) 보다 적게 채웠는가?)를 확인하고 채운다.
                    // If impossible,
                        // 다음 점수가 높은 스탯에 대해서 확인한다. (최대 5회 반복)
                for (int j = 1; j <= 5; j++)
                {
                    int largeStat = FindNthLargestOption(damArray, j);
                    if (5 - mainOp[largeStat] > numArray[largeStat])
                    {
                        if ((damArray[largeStat] == damArray[4]) && (5 - mainOp[4] > numArray[4])) largeStat = 4;
                        tempStat.AddOption(largeStat, PLUSARRAY[largeStat]);
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }
        else
        {
            if (whetherNotEnoughEC)
            {
                tempStat.AddOption(4, PLUSARRAY[4]);
                numArray[4] += 1;
            }
            else
            {
                for (int j = 0; j < 10; j++)
                {
                    tempStatArray[j] = tempStat;
                    tempStatArray[j].AddOption(j, PLUSARRAY[j]);
                    tempStatArray[j].Initialization();
                    damArray[j] = GetDamage(tempStatArray[j]);
                }

                for (int j = 1; j <= 2; j++)
                {
                    int largeStat = FindNthLargestOption(damArray, j);
                    if (30 - mainOp[largeStat] != numArray[largeStat])
                    {
                        tempStat.AddOption(largeStat, PLUSARRAY[largeStat]);
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }
        tempStat.Initialization();
        mSavedFunction[i + 1] = GetDamage(tempStat);
    }
}


double Character::GetScore()
{
    double score = 0;
    double damage = GetDamage();

    // Return 0 if the saved function is equal to the damage
    if (mSavedFunction[0] == damage)
    {
        return 0;
    }

    // Find the score by iterating through the saved function array
    for (int i = 0; i < 45; i++)
    {
        if ((mSavedFunction[i] < damage) && (damage <= mSavedFunction[i + 1]))
        {
            score = i + (damage - mSavedFunction[i]) / (mSavedFunction[i + 1] - mSavedFunction[i]);
            break;
        }
    }

    // Adjust the score based on the element charge
    double elementChargeDifference = mTargetEC - mStat.GetElementCharge();
    if (elementChargeDifference > 0)
    {
        score -= elementChargeDifference / 6.4800001680851;
    }

    return score;
}


void Character::SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, 
							ArtCup* cup, ArtCrown* crown)
{
	mArtFlower = flower;
	mArtFeather = feather;
	mArtClock = clock;
	mArtCup = cup;
	mArtCrown = crown;
}