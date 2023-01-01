#include "Character.hh"
#include <algorithm>
#include <iostream>
#include <ctime>


void Character::Initialization()
{
    if (!bPossibleExceptArtifact)
    {
        InitializationExceptArtifact();
    }
    ArtifactInitialization();
}


double ARTINITSTART, ARTINITFINISH;
double ARTINITTIMELIST[5] = { 0. };

void Character::ArtifactInitialization()
{
    ARTINITSTART = clock();
    
    Stat FlowerMainStat = fArtFlower->GetMainStat();
    Stat FlowerSubStat = fArtFlower->GetSubStat();
    Stat FeatherMainStat = fArtFeather->GetMainStat();
    Stat FeatherSubStat = fArtFeather->GetSubStat();
    Stat ClockMainStat = fArtClock->GetMainStat();
    Stat ClockSubStat = fArtClock->GetSubStat();
    Stat CupMainStat = fArtCup->GetMainStat();
    Stat CupSubStat = fArtCup->GetSubStat();
    Stat CrownMainStat = fArtCrown->GetMainStat();
    Stat CrownSubStat = fArtCrown->GetSubStat();

    ARTINITFINISH = clock();
	ARTINITTIMELIST[0] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    ARTINITSTART = ARTINITFINISH;

    mStat = mStatExceptArtifact;

    ARTINITFINISH = clock();
	ARTINITTIMELIST[1] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    ARTINITSTART = ARTINITFINISH;

    mStat.AddOption(6, FlowerMainStat.GetOption(6));
    mStat.AddOption(3, FeatherMainStat.GetOption(3));
    mStat.AddOption(fArtClock->GetMainType(), ClockMainStat.GetOption(fArtClock->GetMainType()));
    mStat.AddOption(fArtCup->GetMainType(), CupMainStat.GetOption(fArtCup->GetMainType()));
    mStat.AddOption(fArtCrown->GetMainType(), CrownMainStat.GetOption(fArtCrown->GetMainType()));

    ARTINITFINISH = clock();
	ARTINITTIMELIST[2] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    ARTINITSTART = ARTINITFINISH;

    for (int i = 0; i < 10; i++)
    {
        mStat.AddOption(i, FlowerSubStat.GetOption(i));
        mStat.AddOption(i, FeatherSubStat.GetOption(i));
        mStat.AddOption(i, ClockSubStat.GetOption(i));
        mStat.AddOption(i, CupSubStat.GetOption(i));
        mStat.AddOption(i, CrownSubStat.GetOption(i));
    }

    ARTINITFINISH = clock();
	ARTINITTIMELIST[3] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
    ARTINITSTART = ARTINITFINISH;

    mStat.Initialization();

    ARTINITFINISH = clock();
	ARTINITTIMELIST[4] += (double)(ARTINITFINISH - ARTINITSTART) / CLOCKS_PER_SEC;
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
    cout << "not called" << endl;
    
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

    for (int j = 0; j < 10; j++)
    {
        tempStatArray[j].AddOption(j, PLUSARRAY[j]);
        tempStatArray[j].Initialization();
        mEffectionArray[j] = GetDamage(tempStatArray[j]) - startDamage;
    }
    for (int j = 10; j < 19; j++)
    {
        tempStatArray[j].AddOption(j, 10.);
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
    Stat FlowerMainStat = fArtFlower->GetMainStat();
    Stat FeatherMainStat = fArtFeather->GetMainStat();
    Stat ClockMainStat = fArtClock->GetMainStat();
    Stat CupMainStat = fArtCup->GetMainStat();
    Stat CrownMainStat = fArtCrown->GetMainStat();

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


struct Point{
    double x;
    int y;
};


bool cmp(const Point &p1, const Point &p2){
    if(p1.x < p2.x){
        return true;
    }
    else{
        return false;
    }
}


int FindNthLargestOption(double damArray[], int nth)
{
    Point tempList[10];
    for (int i = 0; i < 10; i++)
    {
        tempList[i].x = damArray[i];
        tempList[i].y = i;
    }
    std::sort(tempList, tempList+10, cmp);
    return tempList[10 - nth].y;
}


void Character::MakeScoreFunction()
{
    int mainOp[10] = { 0 }; // 메인옵션에 무엇무엇이 있는지 확인한다. (mainOp[10])
    if (fArtClock->GetMainType() < 10) mainOp[fArtClock->GetMainType()] = 1;
    if (fArtCup->GetMainType() < 10) mainOp[fArtCup->GetMainType()] = 1;
    if (fArtCrown->GetMainType() < 10) mainOp[fArtCrown->GetMainType()] = 1;

    int numArray[10] = { 0 }; // 각 옵션이 몇번 들어갔는지 기록한 어레이를 만든다. (numArray[10])
    double damArray[10] = { 0 }; // 각 옵션이 추가되었을 때의 데미지를 기록할 어레이를 만든다. (damArray[10])

    Stat tempStat = GenerateStatExceptSubOpt(); // 성유물이 초기화된 새로운 스탯을 생성한다.
    cout << "tempStat generated" << endl;
    double startDamage = GetDamage(tempStat); // 현재 스펙을 기록한다.

    Stat tempStatArray[10] = { tempStat, tempStat, tempStat, tempStat, tempStat,
                               tempStat, tempStat, tempStat, tempStat, tempStat };

    mSavedFunction[0] = startDamage;

    for (int i = 0; i < 45; i++) // for문으로 45회동안, 
    {
        cout << "part1" << endl;

        double beforeDamage = GetDamage(tempStat); // 현재 스탯에 대한 데미지를 기록하고,
        double difEC = mTargetEC - tempStat.GetOption(4); // 현재 원충이 원충요구수치보다 낮은지 체크한다.
        bool whetherNotEnoughEC = (difEC > 0);

        cout << i + 1 << "-th difEC & whetherNotEnoughEC : " << difEC << ", " << whetherNotEnoughEC << endl;
        cout << "part2" << endl;

        if (i < 20)
        {
            // 원충이 들어가야 하면 원충을 넣는다.
            if (whetherNotEnoughEC && (5 - mainOp[4] > numArray[4]))
            {
                cout << "part2-1" << endl;
                tempStat.AddOption(4, PLUSARRAY[4]);
                numArray[4] += 1;
            }
            else // 그렇지 않다면
            {
                // 10개의 부옵에 대해서 하나씩 추가하면 점수가 어떻게 되는지 확인한다.
                cout << "part2-2" << endl;
                for (int j = 0; j < 10; j++) 
                {
                    tempStatArray[j] = tempStat;
                    tempStatArray[j].AddOption(j, PLUSARRAY[j]);
                    tempStatArray[j].Initialization();
                    damArray[j] = GetDamage(tempStatArray[j]);
                }

                // 가장 점수가 높은 스탯에 대해서 ((5 - 주옵여부) 보다 적게 채웠는가?)를 확인하고 채운다.
                    // 불가능한 경우
                        // 다음 점수가 높은 스탯에 대해서 확인한다. (최대 5회 반복)
                cout << "part2-3" << endl;
                for (int j = 1; j <= 5; j++)
                {
                    cout << "part2-3-1" << endl;
                    int largeStat = FindNthLargestOption(damArray, j);
                    cout << "part2-3-2" << endl;
                    if (5 - mainOp[largeStat] > numArray[largeStat])
                    {
                        cout << "part2-3-2-1" << endl;
                        if ((damArray[largeStat] == damArray[4]) && (5 - mainOp[4] > numArray[4])) largeStat = 4;
                        tempStat.AddOption(largeStat, PLUSARRAY[largeStat]);
                        cout << i + 1 << "-th Added Stat : " << STATSTRING[largeStat] << endl;
                        cout << "part2-3-2-2" << endl;
                        numArray[largeStat] += 1;
                        cout << "part2-3-2-3" << endl;
                        break;
                    }
                    cout << "part2-3-3" << endl;
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
                        cout << i << "-th Added Stat : " << STATSTRING[largeStat] << endl;
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }

        cout << "part3" << endl;

        tempStat.Initialization();
        mSavedFunction[i + 1] = GetDamage(tempStat);
    }
}


double Character::GetScore()
{
    double spec = GetDamage();
    if (mSavedFunction[0] == spec) return 0.;

    double score;
    for (int i = 0; i < 45; i++)
    {
        if ((mSavedFunction[i] < spec) && (spec <= mSavedFunction[i + 1]))
        {
            score = i + (spec - mSavedFunction[i]) / (mSavedFunction[i + 1] - mSavedFunction[i]);
            if (mTargetEC - mStat.GetElementCharge() > 0)
            {
                score -= (mTargetEC - mStat.GetElementCharge()) / 6.4800001680851;
            }
            break;
        }
    }
	return score;
}


void Character::SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, 
							ArtCup* cup, ArtCrown* crown)
{
	fArtFlower = flower;
	fArtFeather = feather;
	fArtClock = clock;
	fArtCup = cup;
	fArtCrown = crown;
}