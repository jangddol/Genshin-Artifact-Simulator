#include "../header/Character.hh"
#include <algorithm>
#include <iostream>
#include <ctime>


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
        mEffectionArray[i] = character->GetEffection(i);
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
        mEffectionArray[i] = character.GetEffection(i);
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
        mEffectionArray[i] = character.GetEffection(i);
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


Character::~Character()
{
    mArtSetStat->DeleteCharacterPointer(this);
    mArtFlower->DeleteCharacterPointer(this);
    mArtFeather->DeleteCharacterPointer(this);
    mArtClock->DeleteCharacterPointer(this);
    mArtCup->DeleteCharacterPointer(this);
    mArtCrown->DeleteCharacterPointer(this);
}


void Character::Update()
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
        UpdateFromArtifactSubStat();
        mUpdateState = ARTIFACTSUBSTATUPDATED;
    }
    mStat = mStatAfterUpdateFromArtifactSubStat;
    mStat.Update();
    UpdateFromFeedback();
    mStat.Update();
}


void Character::UpdateFromCharacterResonance()
{
	// 캐릭터 옵션 : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 공명 : 0, 2, 5, 7, 10 ~ 17, 27
    mStatAfterUpdateFromCharacterResonance.AddOption(0, mResonanceStat.GetOption(0));
    mStatAfterUpdateFromCharacterResonance.AddOption(2, mResonanceStat.GetOption(2));
    mStatAfterUpdateFromCharacterResonance.AddOption(5, mResonanceStat.GetOption(5));
    mStatAfterUpdateFromCharacterResonance.AddOption(7, mResonanceStat.GetOption(7));
    mStatAfterUpdateFromCharacterResonance.AddOption(27, mResonanceStat.GetOption(27));
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

    // 무기 주옵 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 무기 부옵 : 0 ~ 18 // 완전히 배제된 것은 아님
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 무기 부부옵 : 0 ~ 23, 27, 28 // 완전히 배제된 것은 아님
    for (int i = 0; i < 24; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubSubStat.GetOption(i));
    }
    mStatAfterUpdateFromWeapon.AddOption(27, WeaponSubSubStat.GetOption(27));
    mStatAfterUpdateFromWeapon.AddOption(28, WeaponSubSubStat.GetOption(28));

}


void Character::UpdateFromArtSetStat()
{
    mStatAfterUpdateFromArtSetStat = mStatAfterUpdateFromWeapon;

	// 성유물 세트 : 0 ~ 23, 27, 28 // 완전히 배제된 것은 아님
    for (int i = 0; i < 24; i++)
    {
        mStatAfterUpdateFromArtSetStat.AddOption(i, mArtSetStat->GetOption(i));
    }
    mStatAfterUpdateFromArtSetStat.AddOption(27, mArtSetStat->GetOption(27));
    mStatAfterUpdateFromArtSetStat.AddOption(28, mArtSetStat->GetOption(28));
}


void Character::UpdateFromArtifactMainStat()
{
    mStatAfterUpdateFromArtifactMainStat = mStatAfterUpdateFromArtSetStat;
    
    // 성유물 주옵 : 0 ~ 8, 10 ~ 18
    
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
    
    // 성유물 부옵 : 0 ~ 9
    
    Stat FlowerSubStat = mArtFlower->GetSubStat();
    Stat FeatherSubStat = mArtFeather->GetSubStat();
    Stat ClockSubStat = mArtClock->GetSubStat();
    Stat CupSubStat = mArtCup->GetSubStat();
    Stat CrownSubStat = mArtCrown->GetSubStat();

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
    Character* tempCharacter;
    ArtSetStat* tempArtSetStat; // �뜝�룞�삕占쎈た占쎈쳮雅��굞猷딉옙�굲占쎄덩�뜝占� 占쎈쐻占쎈윥占쎈떋占쎈쐻占쎈윪占쎈�듸옙�쐻占쎈윥�뵳占� �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留� �뜝�럥�돯占쎄껀占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲�뜝�럥痢� ResonanceStat占쎈쐻占쎈윪筌뤿뱶�뒙占쎈뙔占쎌굲 占쎈쐻占쎈윥�뵳�뜴�쐻占쎈윥�젆占�.
                                        // 占쎈쐻占쎈윪��얠±�쐻占쎈윪�뜝�룞�삕占쎈쐻占쎈윥獒뺧옙, 占쎌쐺獄쏅챷援←뭐癒뀁삕 Stat占쎈쐻占쎈윪��얠±�쐻占쎈윪��앗껊쐻占쎈윞占쎈쭓 占쎈쐻占쎈윪占쎌젞占쎌쐺獄�占쏙옙�땾�뜝�럥�꼧 占쎈쐻占쎈윥占쎌죪占쎈쐻占쎈윥筌랃옙.
                                        // Update占쎈쨬占쎈즸占쎌굲 占쎈쐻占쎈윪亦낅‥�쐻占쎈윪占쎄데癲꾧퀗�뫊筌��씛�삕�걡釉�堉⑨옙占쎈Ŋ�굲 占쎈쐻占쎈윥�뵳占쏙┼�슣�돸占쎌굲癲ル슢�뿪占쎌굲, 占쎈쐻占쎈윥�젆濡�琉놅옙猷꾬옙沅욑옙�눀�뜝�뜾異�占쎌돸占쎌굲 占쎈쐻占쎈윥�몴�깷�쐻占쎈윪甕곤옙.
                                    // 230131
                                        // ResonanceStat 占쎈쐻占쎈윥占쎈군 占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲占쎈눀�뜝占� Update Optimization �뜝�룞�삕占쎈쳛占쎈땾�뜝�럩�젳占쎈쐻占쎈윥占쎈군占쎈쐻占쎈윞占쎈쭓
                                        // 嚥싲갭占싸삳１�뜝�럡留쇿뜝�룞�삕占쎈쳟占쎌굲 占쎈뇲占쎄땀�몭諛깍옙�뼚堉껓㎗濡λ쐻�뜝占� 占쎈쐻占쎈윞占쎈�э옙�쐻占쎈윪占쎈츛占쎈쐻占쎈윥占쎈돌癲ル슣�돸占쎌굲 占쎈쐻占쎈윥�몴�깷�쐻占쎈윥獒뺧옙 占쎈눇�뙼蹂��굲 占쎈쐻占쎈윥�몴�뙋�삕占쎈떛嶺뚮ㅎ�뫊占쎈군
                                        // 占쎈쐻占쎈윪��억옙 占쎈쐻占쎈윥筌띿눨�쐻占쎈윥占쎈묄占쎈쨬占쎈즸占쎌굲 癲ル슢�뵰�뜮蹂잙쐻占쎈짗占쎌굲癲ル슣�돸占쎌굲. 
                                        // 0 ~ 18 癲ル슢�뀈泳�占썹춯瑜낆삕 占쎈쐻占쎈윞占쎈�э옙�쐻占쎈윪占쎈츛占쎈쐻占쎈윥占쎈돌占쎈쐻占쎈윥獒뺧옙 占쎈눇�뙼�맪�쐭�뜝�럥援� 占쎈쐻占쎈윞占쎈�э옙�쐻占쎈윪占쎈츛占쎈쐻占쎈윥占쎈뤅占쎈쐻占쎈윥占쎈뭾占쎈쐻占쎈윥�뵳�뜴�쐻占쎈윥�젆占�.
                                        // ArtSetStat占쎈쐻占쎈윪��억옙 癲ル슢�뵰占쎈뤂�뜝�럥痢� 占쎈쐻占쎈윥�떋占� 占쎈쐻占쎈윥�뵳占쏙옙�쐻占쎈윥�젆占�.


    double defaultDamage = GetDamage(); // 占쎈쐻占쎈윪野껉막�쐻占쎈윪占쎄땍 占쎈쐻占쎈윥獒뺤쉩�쐻占쎈윥占쎌뱼占쎈쐻占쎈윪獄�占� 占쎈섀饔낅챸占썩뼺鍮놂옙留�占쎌굲占쎈눀占쎈꺋占쎌굲�뜝�럥�렡.
    for (int i = 0; i < 19; i++)
    {
        tempCharacter = this->Clone();
        tempArtSetStat = this->CopyArtSetStat();
        tempArtSetStat->AddOption(i, 1.);
        tempCharacter->SetArtSetStat(tempArtSetStat);
        tempCharacter->Update();
        mEffectionArray[i] = tempCharacter->GetDamage() - defaultDamage;
        delete tempArtSetStat;
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


// TODO: 占쎈쐻占쎈윪��얠±�쐻占쎈윪占쎌졆 StatExceptSubOpt占쎈쨬占쎈즸占쎌굲 占쎈쐻占쎈윪占쎄탾占쎈쐻占쎈윥筌욎�λ쐻占쎈윥�뵳占쏙┼�슣�돸占쎌굲 占쎈쐻占쎈윥�몴�깷�쐻占쎈윥獒뺣맕�쐻占쎈윥�젆占�. 占쎈쐻占쎈윥占쎈젇占쎈쐻占쎈윪��앗껊쐻占쎈윞占쎈쭓 占쎈쐻占쎈윞占쎈렰占쎈쐻占쎈윪�뜝�룞�삕�뜝�럥�떛占쎈쐻�뜝占� �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留숋옙�쐻占쎈윪獄�占� 癲꾧퀗�뫊�뜝�룞�삕�뛾占쏙옙�뵛占쎌굲�뜝�럩議롳옙�쐻占쎈윞占쎈쭓 占쎈쐻占쎈윥�뵳占쏙옙�쐻占쎈윥獒뺧옙 占쎈쎗占쎈젻泳�戮щ뼀占쎌뒻占쎌굲�뜝�럥援� 癲ル슓�룱�젆�떘�눀占쎄섶占쎌굲占쎈쑏�뜝占� 占쎈쐻占쎈윥�뵳�뜴�쐻占쎈윥�젆占�.
void Character::MakeScoreFunction()
{
    std::array<int, 10> mainOp = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower占쎈쐻占쎈윥占쎈군癲ル슢�뿪占쎌굲 占쎈쐻占쎈윪占쎌뱾占쎈쐻占쎈윪占쎈츛占쎈쐻占쎈윥壤쏉옙 占쎈눇�뙼�맪�쐭�뜝�럥�꼧占쎈쐻占쎈윥�젆占�.

    // Character�뜝�럥�걫占쎈쐻�뜝占� 10占쎈쨬占쎈즵獒뺣끀�쐻占쎈짗占쎌굲 占쎌녃域뱄퐢苡답벧�굞占쏙옙占쎌굲占쎈눀�뜝占� 占쎈쐻占쎈윥�젆袁��쐻占쎈윪甕곕맕�쐻占쎈윥占쎈군, 占쎈쨬占쎈즸占쎌굲 Character占쎈쐻占쎈윥占쎈군占쎈눇�뙼蹂��굲 �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留숋옙�쐻占쎈윪��억옙 占쎈쐻占쎈윪占쎌벁�뜝�럡猿�占쎈쐻�뜝占� �뜝�럥�몡�넭怨ｋ쳳占쎄퐨占쎈쐻占쎈윪�굢�뀘�쐻占쎈윥獒뺧옙 Artifact�뜝�럥�걫占쎈쐻�뜝占� 濚욌꼬�슦�굲占쎈쐻占쎈윥�젆占�.
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

    for (int i = 0; i < 45; i++) // for�뜝�럥�떛嶺뚮ㅎ�뫒筌뤿뱶�뒙占쎈뙔占쎌굲 45占쎈쐻占쎈윪占쎈쨧占쎈쐻占쎈윥筌욎�λ쐻占쎈윥占쎈떛, 
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

                // 占쎈쨬占쎈즸占쎌굲占쎈쐻占쎈윪占쎄텑 占쎈쐻占쎈윪占쎌젌占쎈쐻占쎈윥占쎈묄占쎈쨬占쎈즸占쎌굲 占쎈쐻占쎈윥�몭琯�쐻占쎈짗占쎌굲占쎈쐻�뜝占� 占쎈쐻占쎈윥獒뺤쉩�쐻占쎈윞占쎈뻺占쎈쐻占쎈윥占쎈군 占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲占쎈쑏�솒��곸굲占쎈빝�뜝占� ((5 - 占쎌뜏占쎌뒩占쎈땾�뜝�럡留숋옙�쐻占쎈윥占쎈염�뜝�럡猿�占쎈쐻�뜝占�) 占쎌녃域뱄퐦�삕�뜝�룞�삕�젆占� 占쎈쐻占쎈윪占쎌뱾占쎈눇�뙼蹂��굲 癲ル슪�삕�넭怨ｋ쳴占쎌몦占쎈쐻占쎈윥獒뺣맚琉놅옙猷딉옙�굲?)�뜝�럥�걫占쎈쐻�뜝占� 占쎈쐻占쎈윪占쎄섈占쎈쐻占쎈윪��앓듬쐻占쎈윥�뵳占썲뜝�룞�삕占쎈쳟占쎌굲 癲ル슪�삕�넭怨ｋ쳴占쎈�㏆옙�쐻占쎈윥�젆占�.
                    // If impossible,
                        // 占쎈쐻占쎈윥�젆袁��쐻占쎈윪甕곤옙 占쎈쐻占쎈윪占쎌젌占쎈쐻占쎈윥占쎈묄占쎈쨬占쎈즸占쎌굲 占쎈쐻占쎈윥�몭琯�쐻占쎈짗占쎌굲占쎈쐻�뜝占� 占쎈쐻占쎈윥獒뺤쉩�쐻占쎈윞占쎈뻺占쎈쐻占쎈윥占쎈군 占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲占쎈쑏�솒��곸굲占쎈빝�뜝占� 占쎈쐻占쎈윪占쎄섈占쎈쐻占쎈윪��앓듬쐻占쎈윥�뵳�뜴�쐻占쎈윥�젆占�. (癲ル슔�걠獒뺣끀�쐻占쎈짗占쎌굲 5占쎈쐻占쎈윪占쎈쨧 占쎈쎗占쎈즵�몭�씛�삕亦낉옙)
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
        // cout << "========== " << i << "-th ArtFlowerSubStat ==========" << endl;
        // PrintStat(tempCharacter->GetArtFlower()->GetSubStat());
        // cout << "========== " << i << "-th tempSubStat ==========" << endl;
        // PrintStat(tempSubStat);
        // tempCharacter->Update();
        // cout << "========== " << i << "-th tempCharacter mStat ==========" << endl;
        // PrintStat(tempCharacter->GetStat());
        mSavedFunction[i + 1] = tempCharacter->GetDamage();
    }
}


double Character::GetScore() const
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