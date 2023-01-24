#include "../header/Character.hh"
#include <algorithm>
#include <iostream>
#include <ctime>


void PrintStat(Stat stat);
Character::Character(Character *character)
{
    SetWeapon(character->GetWeapon()->Clone());
    SetArtFlower(new ArtFlower(character->GetArtFlower()));
    SetArtFeather(new ArtFeather(character->GetArtFeather()));
    SetArtClock(new ArtClock(character->GetArtClock()));
    SetArtCup(new ArtCup(character->GetArtCup()));
    SetArtCrown(new ArtCrown(character->GetArtCrown()));

    SetArtSetStat(character->GetArtSetStat());
    SetResonanceStat(character->GetResonanceStat());
    mTargetEC = character->GetTargetEC();
    mCharacterStat = character->GetCharacterStat(); // TODO: SetCharacterStat을 분화해서 통째로 넣는거랑 아닌걸아 둘다 만들기
    mUpdateState = 0; // TODO: 위에꺼 완료되면 이거 없애기
    for (int i = 0; i < 19; i++)
    {
        mEffectionArray[i] = character->GetEffection(i);
    }
    for (int i = 0; i < 46; i++)
    {
        mSavedFunction[i] = character->GetScoreFunction(i);
    }
    Update();
}


void Character::Update()
{
    // Process
        // CharacterStat + ResonanceStat
        // WeaponStat (Main + Sub + SubSub)
        // ArtSetStat
        // Artifact Main Stat
        // Artifact Sub Stat
        // Stat Initialization
        // Feedback
        // Stat Initialization Once Again

    if (mUpdateState < CHARACTERRESONANCEUPDATED)
    {
        UpdateFromCharacterResonance();
        mUpdateState = CHARACTERRESONANCEUPDATED;
    }
    if (mUpdateState < WEAPONSTATUPDATED)
    {
        UpdateFromWeapon();
        mUpdateState = WEAPONSTATUPDATED;
    }
    if (mUpdateState < ARTSETSTATUPDATED)
    {
        UpdateFromArtSetStat();
        mUpdateState = ARTSETSTATUPDATED;
    }
    if (mUpdateState < ARTIFACTMAINSTATUPDATED)
    {
        UpdateFromArtifactMainStat();
        mUpdateState = ARTIFACTMAINSTATUPDATED;
    }
    if (mUpdateState < ARTIFACTSUBSTATUPDATED)
    {
        UpdateFromArtifactSubStat();
        mUpdateState = ARTIFACTSUBSTATUPDATED;
    }
    mStat = mStatAfterUpdateFromArtifactSubStat;
    mStat.Initialization();
    UpdateFromFeedback();
    mStat.Initialization();
}


void Character::UpdateFromCharacterResonance()
{
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mResonanceStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }
}


void Character::UpdateFromWeapon()
{
    Stat WeaponMainStat = mWeapon->GetMainStat();
    Stat WeaponSubStat = mWeapon->GetSubStat();
    Stat WeaponSubSubStat = mWeapon->GetSubSubStat();

    mStatAfterUpdateFromWeapon = mStatAfterUpdateFromCharacterResonance;
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponMainStat.GetOption(i));
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubSubStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromWeapon.SetBaseOption(i, mStatAfterUpdateFromWeapon.GetBaseOption(i) + WeaponMainStat.GetBaseOption(i));
    }
}


void Character::UpdateFromArtSetStat()
{
    mStatAfterUpdateFromArtSetStat = mStatAfterUpdateFromWeapon;
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromArtSetStat.AddOption(i, mArtSetStat->GetOption(i));
    }
	mStatAfterUpdateFromArtSetStat;
}


void Character::UpdateFromArtifactMainStat()
{
    Stat FlowerMainStat = mArtFlower->GetMainStat();
    Stat FeatherMainStat = mArtFeather->GetMainStat();
    Stat ClockMainStat = mArtClock->GetMainStat();
    Stat CupMainStat = mArtCup->GetMainStat();
    Stat CrownMainStat = mArtCrown->GetMainStat();
    
    mStatAfterUpdateFromArtifactMainStat = mStatAfterUpdateFromArtSetStat;
    mStatAfterUpdateFromArtifactMainStat.AddOption(6, FlowerMainStat.GetOption(6));
    mStatAfterUpdateFromArtifactMainStat.AddOption(3, FeatherMainStat.GetOption(3));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtClock->GetMainType(), ClockMainStat.GetOption(mArtClock->GetMainType()));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtCup->GetMainType(), CupMainStat.GetOption(mArtCup->GetMainType()));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtCrown->GetMainType(), CrownMainStat.GetOption(mArtCrown->GetMainType()));
}


void Character::UpdateFromArtifactSubStat()
{
    Stat FlowerSubStat = mArtFlower->GetSubStat();
    Stat FeatherSubStat = mArtFeather->GetSubStat();
    Stat ClockSubStat = mArtClock->GetSubStat();
    Stat CupSubStat = mArtCup->GetSubStat();
    Stat CrownSubStat = mArtCrown->GetSubStat();

    mStatAfterUpdateFromArtifactSubStat = mStatAfterUpdateFromArtifactMainStat;
    for (int i = 0; i < 10; i++)
    {
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, FlowerSubStat.GetOption(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, FeatherSubStat.GetOption(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, ClockSubStat.GetOption(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, CupSubStat.GetOption(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, CrownSubStat.GetOption(i));
    }
}

void Character::UpdateFromFeedback()
{
    mFeedbackedStat.SetZero();
    mWeapon->DoFeedback(this);
    mArtSetStat->DoFeedback(this);
    this->DoFeedback();
    for (int i = 0; i < 35; i++) mStat.AddOption(i, mFeedbackedStat.GetOption(i));
}


void Character::ConfirmResonanceStatModified()
{
    if (mUpdateState >= CHARACTERRESONANCEUPDATED)
    {
        mUpdateState = CHARACTERRESONANCEUPDATED - 1;
    }
}


void Character::ConfirmWeaponStatModified()
{
    if (mUpdateState >= WEAPONSTATUPDATED)
    {
        mUpdateState = WEAPONSTATUPDATED - 1;
    }
}


void Character::ConfirmArtSetStatModified()
{
    if (mUpdateState >= ARTSETSTATUPDATED)
    {
        mUpdateState = ARTSETSTATUPDATED - 1;
    }
}


void Character::ConfirmArtifactMainStatModified()
{
    if (mUpdateState >= ARTIFACTMAINSTATUPDATED)
    {
        mUpdateState = ARTIFACTMAINSTATUPDATED - 1;
    }
}


void Character::ConfirmArtifactSubStatModified()
{
    if (mUpdateState >= ARTIFACTSUBSTATUPDATED)
    {
        mUpdateState = ARTIFACTSUBSTATUPDATED - 1;
    }
}


double Character::GetDamageWithStat(Stat stat)
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
    Character* tempCharacter;
    Stat tempResonanceStat; // 계산에 필요한 부옵 추가는 ResonanceStat으로 한다.
                                        // 이유는, 그냥 Stat이라서 접근이 편함.
                                        // Update가 오래걸리긴 하지만, 심각하진 않음.
    double defaultDamage = GetDamage(); // 현재 스펙을 기록한다.
    for (int i = 0; i < 19; i++)
    {
        tempCharacter = this->Clone();
        tempResonanceStat = mResonanceStat;
        tempResonanceStat.AddOption(i, 1.);
        tempCharacter->SetResonanceStat(tempResonanceStat);
        tempCharacter->Update();
        mEffectionArray[i] = tempCharacter->GetDamage() - defaultDamage; // TODO: Raiden에서 음수가 나옴.
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


int FindNthLargestOption(std::array<double, 10> damArray, int nth)
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


// TODO: 이제 StatExceptSubOpt가 작동하지 않는다. 따라서 성유물 부옵을 건드려서 하는 방법을 찾아야 한다.
void PrintStat(Stat stat);
void Character::MakeScoreFunction()
{
    std::array<int, 10> mainOp = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower에만 적용될 것이다.

    // Character를 10개를 복사한 다음에, 각 Character에게 부옵이 전부 비어있는 Artifact를 준다.
    ArtFlower* emptyFlower = new ArtFlower(this->GetArtFlower());
    emptyFlower->SetSubStat(Stat());
    ArtFeather* emptyFeather = new ArtFeather(this->GetArtFeather());
    emptyFeather->SetSubStat(Stat());
    ArtClock* emptyClock = new ArtClock(this->GetArtClock());
    emptyClock->SetSubStat(Stat());
    ArtCup* emptyCup = new ArtCup(this->GetArtCup());
    emptyCup->SetSubStat(Stat());
    ArtCrown* emptyCrown = new ArtCrown(this->GetArtCrown());
    emptyCrown->SetSubStat(Stat());

    tempCharacter = this->Clone();
    tempCharacter->SetArtFlower(new ArtFlower(emptyFlower));
    tempCharacter->SetArtFeather(new ArtFeather(emptyFeather));
    tempCharacter->SetArtClock(new ArtClock(emptyClock));
    tempCharacter->SetArtCup(new ArtCup(emptyCup));
    tempCharacter->SetArtCrown(new ArtCrown(emptyCrown));
    
    Stat tempSubStat = Stat();

    tempCharacter->Update();
    mSavedFunction[0] = tempCharacter->GetDamage();

    for (int i = 0; i < 45; i++) // for문으로 45회동안, 
    {
        double difEC = mTargetEC - tempCharacter->GetStat().GetOption(4); // check the element charge is enough or not.
        bool whetherNotEnoughEC = difEC > 0;

        if (i < 20)
        {
            // If the element charge is not enouth, add element charge.
            if (whetherNotEnoughEC && (5 - mainOp[4] > numArray[4]))
            {
                tempSubStat.AddOption(4, PLUSARRAY[4]);
                numArray[4] += 1;
            }
            else // If impossible,
            {
                // record how much damage will be if each option is added at damArray.
                for (int j = 0; j < 10; j++) 
                {
                    tempSubStatArray[j] = tempSubStat;
                    tempSubStatArray[j].AddOption(j, PLUSARRAY[j]);
                    tempCharacter->GetArtFlower()->SetSubStat(tempSubStatArray[j]);
                    tempCharacter->Update();
                    damArray[j] = tempCharacter->GetDamage();
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
                        tempSubStat.AddOption(largeStat, PLUSARRAY[largeStat]);
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
                tempSubStat.AddOption(4, PLUSARRAY[4]);
                numArray[4] += 1;
            }
            else
            {
                for (int j = 0; j < 10; j++)
                {
                    tempSubStatArray[j] = tempSubStat;
                    tempSubStatArray[j].AddOption(j, PLUSARRAY[j]);
                    tempCharacter->GetArtFlower()->SetSubStat(tempSubStatArray[j]);
                    tempCharacter->Update();
                    damArray[j] = tempCharacter->GetDamage();
                }

                for (int j = 1; j <= 2; j++)
                {
                    int largeStat = FindNthLargestOption(damArray, j);
                    if (30 - mainOp[largeStat] != numArray[largeStat])
                    {
                        tempSubStat.AddOption(largeStat, PLUSARRAY[largeStat]);
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }

        tempCharacter->GetArtFlower()->SetSubStat(tempSubStat);
        cout << "========== " << i << "-th ArtFlowerSubStat ==========" << endl;
        PrintStat(tempCharacter->GetArtFlower()->GetSubStat());
        cout << "========== " << i << "-th tempSubStat ==========" << endl;
        PrintStat(tempSubStat);
        tempCharacter->Update();
        cout << "========== " << i << "-th tempCharacter mStat ==========" << endl;
        PrintStat(tempCharacter->GetStat());
        mSavedFunction[i + 1] = tempCharacter->GetDamage();
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
        score -= elementChargeDifference / PLUSARRAY[4];
    }

    return score;
}


void Character::SetArtSetStat(ArtSetStat* artSetStat)
{
    if (mArtSetStat) mArtSetStat->DeleteCharacterPointer(this);
    mArtSetStat = artSetStat;
    mArtSetStat->SaveCharacterPointer(this);
    ConfirmArtSetStatModified();
}


void Character::SetArtFlower(ArtFlower* artFlower)
{
    if (mArtFlower) mArtFlower->DeleteCharacterPointer(this);
    mArtFlower = artFlower;
    mArtFlower->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtFeather(ArtFeather* artFeather)
{
    if (mArtFeather) mArtFeather->DeleteCharacterPointer(this);
    mArtFeather = artFeather;
    mArtFeather->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtClock(ArtClock* artClock)
{
    if (mArtClock) mArtClock->DeleteCharacterPointer(this);
    mArtClock = artClock;
    mArtClock->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtCup(ArtCup* artCup)
{
    if (mArtCup) mArtCup->DeleteCharacterPointer(this);
    mArtCup = artCup;
    mArtCup->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtCrown(ArtCrown* artCrown)
{
    if (mArtCrown) mArtCrown->DeleteCharacterPointer(this);
    mArtCrown = artCrown;
    mArtCrown->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtifact(ArtFlower* flower, ArtFeather* feather, ArtClock* clock, ArtCup* cup, ArtCrown* crown)
{
	SetArtFlower(flower);
    SetArtFeather(feather);
    SetArtClock(clock);
    SetArtCup(cup);
    SetArtCrown(crown);
}