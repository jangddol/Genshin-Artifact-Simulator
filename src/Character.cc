#include "../header/Character.hh"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <assert.h>


Character::Character(const Character *character)
{
    SetWeapon(character->CopyWeapon());
    SetArtFlower(character->CopyArtFlower());
    SetArtFeather(character->CopyArtFeather());
    SetArtClock(character->CopyArtClock());
    SetArtCup(character->CopyArtCup());
    SetArtCrown(character->CopyArtCrown());

    SetArtSetStat(character->CopyArtSetStat());
    SetResonanceStat(character->GetResonanceStat());
    
    mTargetEC = character->GetTargetEC();
    SetCharacterStat(character->GetCharacterStat());
    for (int i = 0; i < 19; i++)
    {
        mEffectionAmount[i] = character->GetEffection(i);
    }
    for (int i = 0; i < 46; i++)
    {
        mSavedFunction[i] = character->GetScoreFunction(i);
    }
    Update();
}


Character::Character(const Character& character)
{
    SetWeapon(character.CopyWeapon());
    SetArtFlower(character.CopyArtFlower());
    SetArtFeather(character.CopyArtFeather());
    SetArtClock(character.CopyArtClock());
    SetArtCup(character.CopyArtCup());
    SetArtCrown(character.CopyArtCrown());
    SetArtSetStat(character.CopyArtSetStat());
    SetResonanceStat(character.GetResonanceStat());
    
    mTargetEC = character.GetTargetEC();
    SetCharacterStat(character.GetCharacterStat());
    for (int i = 0; i < 19; i++)
    {
        mEffectionAmount[i] = character.GetEffection(i);
    }
    for (int i = 0; i < 46; i++)
    {
        mSavedFunction[i] = character.GetScoreFunction(i);
    }
    Update();
}

Character::Character(Character&& character)
{
    SetWeapon(character.CopyWeapon());
    SetArtFlower(character.CopyArtFlower());
    SetArtFeather(character.CopyArtFeather());
    SetArtClock(character.CopyArtClock());
    SetArtCup(character.CopyArtCup());
    SetArtCrown(character.CopyArtCrown());
    SetArtSetStat(character.GetArtSetStat());
    SetResonanceStat(character.GetResonanceStat());
    
    mTargetEC = character.GetTargetEC();
    SetCharacterStat(character.GetCharacterStat());
    for (int i = 0; i < 19; i++)
    {
        mEffectionAmount[i] = character.GetEffection(i);
    }
    for (int i = 0; i < 46; i++)
    {
        mSavedFunction[i] = character.GetScoreFunction(i);
    }
    Update();
}


Character& Character::operator = (const Character &character)
{
    if(&character == this)
    {
        return * this;
    }
    return * this;
}


Character& Character::operator = (Character &&character)
{
    if(&character == this)
    {
        return * this;
    }
    return * this;
}


Character* Character::Clone() const { return new Character(this); }


Character::~Character()
{
    mArtSetStat->DeleteCharacterPointer(this);
    mArtFlower->DeleteCharacterPointer(this);
    mArtFeather->DeleteCharacterPointer(this);
    mArtClock->DeleteCharacterPointer(this);
    mArtCup->DeleteCharacterPointer(this);
    mArtCrown->DeleteCharacterPointer(this);
}


void Character::DoFeedback(int& stat, double& amount) { stat = 0; amount = 0; }


void Character::Update(bool fastMode)
{
    // Process
        // CharacterStat + ResonanceStat
        // WeaponStat (Main + Sub + SubSub)
        // ArtSetStat
        // Artifact Main Stat
        // Artifact Sub Stat
        // Stat Update
        // Feedback
        // Stat Update Once Again

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
        if(fastMode) UpdateFromArtifactSubStatFast();
        else UpdateFromArtifactSubStat();
        mUpdateState = ARTIFACTSUBSTATUPDATED;
    }
    mStat = mStatAfterUpdateFromArtifactSubStat;
    mStat.Update();
    UpdateFromFeedback();
    mStat.Update();
}


void Character::UpdateFromCharacterResonance()
{
	// 筌�癒��봼占쎄숲 占쎌긿占쎈�� : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// ��⑤벉梨� : 0, 2, 5, 7, 10 ~ 17, 24
    mStatAfterUpdateFromCharacterResonance.AddOption(0, mResonanceStat.GetOption(0));
    mStatAfterUpdateFromCharacterResonance.AddOption(2, mResonanceStat.GetOption(2));
    mStatAfterUpdateFromCharacterResonance.AddOption(5, mResonanceStat.GetOption(5));
    mStatAfterUpdateFromCharacterResonance.AddOption(7, mResonanceStat.GetOption(7));
    mStatAfterUpdateFromCharacterResonance.AddOption(24, mResonanceStat.GetOption(27));
    for (int i = 10; i < 19; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mResonanceStat.GetOption(i));
    }
}


void Character::UpdateFromWeapon()
{
    Stat WeaponMainStat = mWeapon->GetMainStat();
    Stat WeaponSubStat = mWeapon->GetSubStat();
    Stat WeaponSubSubStat = mWeapon->GetSubSubStat();
    
    mStatAfterUpdateFromWeapon = mStatAfterUpdateFromCharacterResonance;

    // �눧�떯由� 雅뚯눘�긿 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// �눧�떯由� �겫占쏙옙�긿 : 0 ~ 18 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// �눧�떯由� �겫占썽겫占쏙옙�긿 : 0 ~ 26 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
    for (int i = 0; i < 27; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubSubStat.GetOption(i));
    }
}


void Character::UpdateFromArtSetStat()
{
    mStatAfterUpdateFromArtSetStat = mStatAfterUpdateFromWeapon;

	// 占쎄쉐占쎌���눧占� 占쎄쉭占쎈뱜 : 0 ~ 26 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
    for (int i = 0; i < 27; i++)
    {
        mStatAfterUpdateFromArtSetStat.AddOption(i, mArtSetStat->GetOption(i));
    }
}


void Character::UpdateFromArtifactMainStat()
{
    mStatAfterUpdateFromArtifactMainStat = mStatAfterUpdateFromArtSetStat;
    
    // 占쎄쉐占쎌���눧占� 雅뚯눘�긿 : 0 ~ 8, 10 ~ 18
    
    Stat FlowerMainStat = mArtFlower->GetMainStat();
    Stat FeatherMainStat = mArtFeather->GetMainStat();
    Stat ClockMainStat = mArtClock->GetMainStat();
    Stat CupMainStat = mArtCup->GetMainStat();
    Stat CrownMainStat = mArtCrown->GetMainStat();
    
    mStatAfterUpdateFromArtifactMainStat.AddOption(6, FlowerMainStat.GetOption(6));
    mStatAfterUpdateFromArtifactMainStat.AddOption(3, FeatherMainStat.GetOption(3));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtClock->GetMainType(), ClockMainStat.GetOption(mArtClock->GetMainType()));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtCup->GetMainType(), CupMainStat.GetOption(mArtCup->GetMainType()));
    mStatAfterUpdateFromArtifactMainStat.AddOption(mArtCrown->GetMainType(), CrownMainStat.GetOption(mArtCrown->GetMainType()));
}


void Character::UpdateFromArtifactSubStat()
{
    mStatAfterUpdateFromArtifactSubStat = mStatAfterUpdateFromArtifactMainStat;
    
    // 占쎄쉐占쎌���눧占� �겫占쏙옙�긿 : 0 ~ 9

    for (int i = 0; i < 10; i++)
    {
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtFlower->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtFeather->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtClock->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtCup->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtCrown->GetSubStatValue(i));
    }
}


void Character::UpdateFromArtifactSubStatFast()
{
    mStatAfterUpdateFromArtifactSubStat = mStatAfterUpdateFromArtifactMainStat;
    
    // 성유물 부옵 : 0 ~ 9

    for (int &stat: mEffectiveSubStats)
    {
        mStatAfterUpdateFromArtifactSubStat.AddOption(stat, mArtFlower->GetSubStatValue(stat));
        mStatAfterUpdateFromArtifactSubStat.AddOption(stat, mArtFeather->GetSubStatValue(stat));
        mStatAfterUpdateFromArtifactSubStat.AddOption(stat, mArtClock->GetSubStatValue(stat));
        mStatAfterUpdateFromArtifactSubStat.AddOption(stat, mArtCup->GetSubStatValue(stat));
        mStatAfterUpdateFromArtifactSubStat.AddOption(stat, mArtCrown->GetSubStatValue(stat));
    }
}


void Character::UpdateFromFeedback()
{
    int stat = -1;
    double amount = 0.;
    mWeapon->DoFeedback(this, stat, amount);
    mStat.AddOption(stat, amount);
    mArtSetStat->DoFeedback(this, stat, amount);
    mStat.AddOption(stat, amount);
    this->DoFeedback(stat, amount);
    mStat.AddOption(stat, amount);
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


double Character::GetDamageWithStat(const Stat& stat) const
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
    Character* tempCharacter = this->Clone();
    ArtSetStat* tempArtSetStat = tempCharacter->GetArtSetStat();
    // ��④쑴沅쏉옙肉� 占쎈툡占쎌뒄占쎈립 �겫占쏙옙�긿 �빊遺쏙옙占쏙옙�뮉 ResonanceStat占쎌몵嚥∽옙 占쎈립占쎈뼄.
        // 占쎌뵠占쎌��占쎈뮉, 域밸챶源� Stat占쎌뵠占쎌뵬占쎄퐣 占쎌젔域뱀눘�뵠 占쎈젶占쎈맙.
        // Update揶쏉옙 占쎌궎占쎌삋椰꾨챶�봺疫뀐옙 占쎈릭筌욑옙筌랃옙, 占쎈뼎揶쏄낱釉�筌욑옙 占쎈륫占쎌벉.
    // 230131
        // ResonanceStat 占쎈퓠 占쏙옙占쏙옙釉� Update Optimization ��⑥눘�젟占쎈퓠占쎄퐣
        // 繹먥돦�긿��⑨옙 燁살꼹逾얍첎占� 占쎄텢占쎌뒠占쎈┷筌욑옙 占쎈륫占쎈뮉 野껓옙 占쎈르�눧紐꾨퓠
        // 占쎌뵠 占쎈맙占쎈땾揶쏉옙 筌띿빓占쏙옙筌욑옙. 
        // 0 ~ 18 筌뤴뫀紐� 占쎄텢占쎌뒠占쎈┷占쎈뮉 野껉퍔�뱽 占쎄텢占쎌뒠占쎈퉸占쎈튊占쎈립占쎈뼄.
        // ArtSetStat占쎌뵠 筌띿쉶�뮉 占쎈쾹 占쎈릭占쎈뼄.

    mEffectiveSubStats = {};
    tempCharacter->Update();
    double defaultDamage = tempCharacter->GetDamage(); // 占쎌겱占쎌삺 占쎈뮞占쎈읃占쎌뱽 疫꿸퀡以됵옙釉놂옙�뼄.
    for (int i = 0; i < 10; i++)
    {
        tempArtSetStat->AddOption(i, 1.);
        tempCharacter->Update();
        mEffectionAmount[i] = tempCharacter->GetDamage() - defaultDamage;
        tempArtSetStat->AddOption(i, -1.);
        if(mEffectionAmount[i] > 0) mEffectiveSubStats.emplace_back(i);
    }
    for (int i = 10; i < 19; i++)
    {
        tempArtSetStat->AddOption(i, 1.);
        tempCharacter->Update();
        mEffectionAmount[i] = tempCharacter->GetDamage() - defaultDamage;
        tempArtSetStat->AddOption(i, -1.);
    }
    delete tempCharacter;
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


int FindNthLargestOption(std::array<double, 10> damArray, const int nth)
{
    if(nth == 1)
    {
        double tempDamage = damArray[0];
        int returnIndex = 0;
        for(int i=1; i < 10; i++)
        {
            if(damArray[i] > tempDamage)
            {
                tempDamage = damArray[i];
                returnIndex = i;
            }
        }
        return returnIndex;
    }
    
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
    std::array<MainOptionsAndDamage, 10> optimizationResult = OptimizeMainOption();
    std::array<int, 3> optimizedMainOption = optimizationResult[0].mainOptions;
    MakeScoreFunctionMainOptionFixed(optimizedMainOption[0], optimizedMainOption[1], optimizedMainOption[2]);
}


void Character::MakeScoreFunctionMainOptionFixed(int main3, int main4, int main5, int endScore)
{
    std::array<int, 10> mainOp = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    std::vector<int> effectiveStats = {};
    effectiveStats.reserve(10);
    for(int i = 0; i < 10; i++)
    {
        if (GetEffection(i) > 0)
        {
            effectiveStats.emplace_back(i);
        }
    }

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower占쎈퓠筌랃옙 占쎌읅占쎌뒠占쎈쭍 野껉퍔�뵠占쎈뼄.

    // Character�몴占� 10揶쏆뮆占쏙옙 癰귣벊沅쀯옙釉� 占쎈뼄占쎌벉占쎈퓠, 揶쏉옙 Character占쎈퓠野껓옙 �겫占쏙옙�긿占쎌뵠 占쎌읈�겫占� �뜮袁⑸선占쎌뿳占쎈뮉 Artifact�몴占� 餓ο옙占쎈뼄.
    ArtFlower emptyFlower = ArtFlower();
    ArtFeather emptyFeather = ArtFeather();
    ArtClock emptyClock = ArtClock();
    emptyClock.SetMainType(main3);
    ArtCup emptyCup = ArtCup();
    emptyCup.SetMainType(main4);
    ArtCrown emptyCrown = ArtCrown();
    emptyCrown.SetMainType(main5);

    tempCharacter = this->Clone();
    tempCharacter->SetArtFlower(&emptyFlower);
    tempCharacter->SetArtFeather(&emptyFeather);
    tempCharacter->SetArtClock(&emptyClock);
    tempCharacter->SetArtCup(&emptyCup);
    tempCharacter->SetArtCrown(&emptyCrown);

    Stat tempSubStat = Stat();

    tempCharacter->Update();
    mSavedFunction[0] = tempCharacter->GetDamage();

    for (int i = 0; i < endScore; i++) // for�눧紐꾩몵嚥∽옙 45占쎌돳占쎈짗占쎈툧, 
    {
        double difEC = mTargetEC - tempCharacter->GetStat().GetOption(4); // check the element charge is enough or not.
        bool whetherNotEnoughEC = difEC > 0;

        // If the element charge is not enouth, add element charge.
        if (whetherNotEnoughEC && ((i >= 20) || (5 - mainOp[4] > numArray[4])))
        {
            tempSubStat.AddOption(4, PLUSARRAY[4]);
            numArray[4] += 1;
        }
        else // If impossible,
        {
            // record how much damage will be if each option is added at damArray.
            for (const int &stat: effectiveStats) 
            {
                tempSubStatArray[stat] = tempSubStat;
                tempSubStatArray[stat].AddOption(stat, PLUSARRAY[stat]);
                tempCharacter->GetArtFlower()->SetSubStat(tempSubStatArray[stat]);
                tempCharacter->Update();
                damArray[stat] = tempCharacter->GetDamage();
            }

            // 揶쏉옙占쎌삢 占쎌젎占쎈땾揶쏉옙 占쎈꼥占쏙옙占� 占쎈뮞占쎄틛占쎈퓠 占쏙옙占쏙옙鍮먲옙苑� ((5 - 雅뚯눘�긿占쎈연�겫占�) 癰귣���뼄 占쎌읅野껓옙 筌�袁⑹뜳占쎈뮉揶쏉옙?)�몴占� 占쎌넇占쎌뵥占쎈릭��⑨옙 筌�袁⑹뒲占쎈뼄.
                // If impossible,
                    // 占쎈뼄占쎌벉 占쎌젎占쎈땾揶쏉옙 占쎈꼥占쏙옙占� 占쎈뮞占쎄틛占쎈퓠 占쏙옙占쏙옙鍮먲옙苑� 占쎌넇占쎌뵥占쎈립占쎈뼄. (筌ㅼ뮆占쏙옙 5占쎌돳 獄쏆꼶�궗)
            int jEnd = (i < 20) ? 5 : 2;
            for (int j = 1; j <= jEnd; j++)
            {
                int largeStat = FindNthLargestOption(damArray, j);
                if (5 - mainOp[largeStat] > numArray[largeStat])
                {
                    if ((i < 20) && (damArray[largeStat] == damArray[4]) && (5 - mainOp[4] > numArray[4])) largeStat = 4;
                    tempSubStat.AddOption(largeStat, PLUSARRAY[largeStat]);
                    numArray[largeStat] += 1;
                    break;
                }
            }
        }

        tempCharacter->GetArtFlower()->SetSubStat(tempSubStat);
        mSavedFunction[i + 1] = tempCharacter->GetDamage();
    }
}


double Character::GetScore() const
{
    double score = 0;
    double damage = GetDamage();

    if (damage <= mSavedFunction[0])
    {
        if (mSavedFunction[0] > mSavedFunction[1])
        {
            assert(false);
        }
        else if(mSavedFunction[0] == mSavedFunction[1])
        {
            score = 0;
        }
        else score = (damage - mSavedFunction[0]) / (mSavedFunction[1] - mSavedFunction[0]);
    }
    else
    {
        int left = 0;
        int right = 45;
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if (damage > mSavedFunction[mid]) left = mid + 1;
            else right = mid;
        }
        if (left == 0) score = 0;
        else if (left == 45) score = 45;
        else score = left + (damage - mSavedFunction[left - 1]) / (mSavedFunction[left] - mSavedFunction[left - 1]);
    }

    // Adjust the score based on the element charge
    double elementChargeDifference = mTargetEC - mStat.GetElementCharge();
    if (elementChargeDifference > 0)
    {
        score -= elementChargeDifference / PLUSARRAY[4];
    }

    return score;
}


double Character::GetScore_MonkeyMagic() const // TODO : 筌�袁⑹뜖占쎈튊占쎈맙
{
    return 0.;
}


std::array<MainOptionsAndDamage, 10> Character::OptimizeMainOption(int refScore) const
{
    std::vector<int> clockPossibleMain = ArtClock::GetPossibleMainOption();
    std::vector<int> cupPossibleMain = ArtCup::GetPossibleMainOption();
    std::vector<int> crownPossibleMain = ArtCrown::GetPossibleMainOption();

    for(auto clock: clockPossibleMain)
    {
        if(GetEffection(clock) <= 0)
        {
            clockPossibleMain.erase(std::remove(clockPossibleMain.begin(), clockPossibleMain.end(), clock), clockPossibleMain.end());
        }
    }
    for(auto cup: cupPossibleMain)
    {
        if(GetEffection(cup) <= 0)
        {
            cupPossibleMain.erase(std::remove(cupPossibleMain.begin(), cupPossibleMain.end(), cup), cupPossibleMain.end());
        }
    }
    for(auto crown: crownPossibleMain)
    {
        if(GetEffection(crown) <= 0)
        {
            crownPossibleMain.erase(std::remove(crownPossibleMain.begin(), crownPossibleMain.end(), crown), crownPossibleMain.end());
        }
    }

    Character* tempChar = this->Clone();
    tempChar->GetArtFlower()->SetSubStat(Stat());
    tempChar->GetArtFeather()->SetSubStat(Stat());
    tempChar->GetArtClock()->SetSubStat(Stat());
    tempChar->GetArtCup()->SetSubStat(Stat());
    tempChar->GetArtCrown()->SetSubStat(Stat());
    tempChar->Update();

    std::array<MainOptionsAndDamage, 10> top10Options;

    std::for_each(clockPossibleMain.begin(), clockPossibleMain.end(), [&](int clockOpt)
    {
		std::for_each(cupPossibleMain.begin(), cupPossibleMain.end(), [&](int cupOpt)
        {
			std::for_each(crownPossibleMain.begin(), crownPossibleMain.end(), [&](int crownOpt)
            {
                tempChar->MakeScoreFunctionMainOptionFixed(clockOpt, cupOpt, crownOpt, refScore);
                
				double tempRefDamage = tempChar->GetScoreFunction(refScore);

                // tempDamage揶쏉옙 top10Option占쎈퓠 占쎌뿳占쎈뮉 minOption癰귣���뼄 占쎄깻 野껋럩�뒭
                if (tempRefDamage > top10Options[9].damage) {
                    // Find the index i where tempRefDamage should be inserted
                    int left = 0;
                    int right = 9;
                    while (left < right) {
                        int mid = left + (right - left) / 2;
                        if (tempRefDamage > top10Options[mid].damage) {
                            right = mid;
                        }
                        else {
                            left = mid + 1;
                        }
                    }

                    // Insert the new option at index i
                    for (int j = 8; j >= left; j--) {
                        top10Options[j + 1] = top10Options[j];
                    }
                    top10Options[left].mainOptions = { clockOpt, cupOpt, crownOpt };
                    top10Options[left].damage = tempRefDamage;
                }
            });
        });
    });
    return top10Options;
}


WeaponList Character::GetWeaponName() const
{
    return mWeapon->GetName();
}


Weapon* Character::CopyWeapon() const
{
    return mWeapon->Clone();
}


void Character::SetArtSetStat(ArtSetStat* artSetStat)
{
    if (mArtSetStat) mArtSetStat->DeleteCharacterPointer(this);
    mArtSetStat = artSetStat;
    mArtSetStat->SaveCharacterPointer(this);
    ConfirmArtSetStatModified();
}


ArtSetStat* Character::CopyArtSetStat() const
{
    return mArtSetStat->Clone();
}


void Character::SetArtFlower(ArtFlower* artFlower)
{
    if (mIsManualMode)
    {
        mArtFlower = artFlower;
        ConfirmArtifactSubStatModified();
        return;
    }
    if (mArtFlower) mArtFlower->DeleteCharacterPointer(this);
    mArtFlower = artFlower;
    mArtFlower->SaveCharacterPointer(this);
    ConfirmArtifactSubStatModified();
}


void Character::SetArtFeather(ArtFeather* artFeather)
{
    if (mIsManualMode)
    {
        mArtFeather = artFeather;
        ConfirmArtifactSubStatModified();
        return;
    }
    if (mArtFeather) mArtFeather->DeleteCharacterPointer(this);
    mArtFeather = artFeather;
    mArtFeather->SaveCharacterPointer(this);
    ConfirmArtifactSubStatModified();
}


void Character::SetArtClock(ArtClock* artClock)
{
    if (mIsManualMode)
    {
        mArtClock = artClock;
        ConfirmArtifactMainStatModified();
        return;
    }
    if (mArtClock) mArtClock->DeleteCharacterPointer(this);
    mArtClock = artClock;
    mArtClock->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtCup(ArtCup* artCup)
{
    if (mIsManualMode)
    {
        mArtCup = artCup;
        ConfirmArtifactMainStatModified();
        return;
    }
    if (mArtCup) mArtCup->DeleteCharacterPointer(this);
    mArtCup = artCup;
    mArtCup->SaveCharacterPointer(this);
    ConfirmArtifactMainStatModified();
}


void Character::SetArtCrown(ArtCrown* artCrown)
{
    if (mIsManualMode)
    {
        mArtCrown = artCrown;
        ConfirmArtifactMainStatModified();
        return;
    }
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