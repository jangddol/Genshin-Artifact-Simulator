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
	// 罹먮┃�꽣 �샃�뀡 : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 怨듬챸 : 0, 2, 5, 7, 10 ~ 17, 27
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

    // 臾닿린 二쇱샃 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 臾닿린 遺��샃 : 0 ~ 18 // �셿�쟾�엳 諛곗젣�맂 寃껋�� �븘�떂
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 臾닿린 遺�遺��샃 : 0 ~ 23, 27, 28 // �셿�쟾�엳 諛곗젣�맂 寃껋�� �븘�떂
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

	// �꽦�쑀臾� �꽭�듃 : 0 ~ 23, 27, 28 // �셿�쟾�엳 諛곗젣�맂 寃껋�� �븘�떂
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
    
    // �꽦�쑀臾� 二쇱샃 : 0 ~ 8, 10 ~ 18
    
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
    
    // �꽦�쑀臾� 遺��샃 : 0 ~ 9
    
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
    ArtSetStat* tempArtSetStat; // 占쎈쐻占쎈짗占쎌굲�뜝�럥�걼�뜝�럥爾��썒占쏙옙援욅뙴�뵃�삕占쎄뎡�뜝�럡�뜦占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼀�뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈벝�삕占쎌맶�뜝�럥�쑅占쎈뎨�뜝占� 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌랃옙 占쎈쐻占쎈윥占쎈룾�뜝�럡猿��뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥筌ο옙 ResonanceStat�뜝�럥�맶�뜝�럥�쑋嶺뚮ㅏ諭띰옙�뮋�뜝�럥�솕�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈뎨占쎈쑕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
                                        // �뜝�럥�맶�뜝�럥�쑋占쏙옙�뼚짹占쎌맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲�뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕, �뜝�럩�맳�뛾�룆梨룡뤃�넀萸먪솒��곸굲 Stat�뜝�럥�맶�뜝�럥�쑋占쏙옙�뼚짹占쎌맶�뜝�럥�쑋占쏙옙�븮猿딆맶�뜝�럥�쐾�뜝�럥彛� �뜝�럥�맶�뜝�럥�쑋�뜝�럩�젦�뜝�럩�맳�뛾占썲뜝�룞�삕占쎈빢占쎈쐻占쎈윥占쎄샹 �뜝�럥�맶�뜝�럥�쑅�뜝�럩二ゅ뜝�럥�맶�뜝�럥�쑅嶺뚮엪�삕.
                                        // Update�뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑋雅��굝��ο옙�맶�뜝�럥�쑋�뜝�럡�뜲�솾袁㏉�쀯옙維딁춯占쏙옙�뵛占쎌굲占쎄괌�뇡占썲젆�뫅�삕�뜝�럥흮占쎄뎡 �뜝�럥�맶�뜝�럥�쑅占쎈뎨�뜝�룞�뵾占쎌뒩占쎈뤈�뜝�럩援꿰솾�꺂�뒧占쎈역�뜝�럩援�, �뜝�럥�맶�뜝�럥�쑅占쎌젂嚥∽옙筌뚮냵�삕�뙴袁ъ삕亦낆쉻�삕占쎈��占쎈쐻占쎈쑟�빊占썲뜝�럩�뤈�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈ご占쎄뭍占쎌맶�뜝�럥�쑋�뵓怨ㅼ삕.
                                    // 230131
                                        // ResonanceStat �뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥���占쎈쐻�뜝占� Update Optimization 占쎈쐻占쎈짗占쎌굲�뜝�럥爾쎾뜝�럥�빢占쎈쐻占쎈윪占쎌졂�뜝�럥�맶�뜝�럥�쑅�뜝�럥援겼뜝�럥�맶�뜝�럥�쐾�뜝�럥彛�
                                        // �슖�떜媛��뜝�떥�궠竊묕옙�쐻占쎈윞筌띿눨�쐻占쎈짗占쎌굲�뜝�럥爾잌뜝�럩援� �뜝�럥�눚�뜝�럡���占쎈き獄쏄퉵�삕占쎈폏�젆猿볝럸嚥♂살맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쐾�뜝�럥占싼띿삕占쎌맶�뜝�럥�쑋�뜝�럥痢쎾뜝�럥�맶�뜝�럥�쑅�뜝�럥�룎�솾�꺂�뒩占쎈뤈�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈ご占쎄뭍占쎌맶�뜝�럥�쑅�뜏類㏃삕 �뜝�럥�늾占쎈쇊癰귨옙占쎄뎡 �뜝�럥�맶�뜝�럥�쑅占쎈ご占쎈솇占쎌굲�뜝�럥�뼓癲ル슢�뀕占쎈쳥�뜝�럥援�
                                        // �뜝�럥�맶�뜝�럥�쑋占쏙옙�뼲�삕 �뜝�럥�맶�뜝�럥�쑅嶺뚮씮�닲占쎌맶�뜝�럥�쑅�뜝�럥臾꾢뜝�럥夷у뜝�럥利멨뜝�럩援� �솾�꺂�뒧占쎈뎔占쎈쑏癰귥옓�맶�뜝�럥吏쀥뜝�럩援꿰솾�꺂�뒩占쎈뤈�뜝�럩援�. 
                                        // 0 ~ 18 �솾�꺂�뒧占쎈�덃납占썲뜝�뜾異��몴�굞�굲 �뜝�럥�맶�뜝�럥�쐾�뜝�럥占싼띿삕占쎌맶�뜝�럥�쑋�뜝�럥痢쎾뜝�럥�맶�뜝�럥�쑅�뜝�럥�룎�뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕 �뜝�럥�늾占쎈쇊占쎈㎦占쎌맠占쎈쐻占쎈윥�뤃占� �뜝�럥�맶�뜝�럥�쐾�뜝�럥占싼띿삕占쎌맶�뜝�럥�쑋�뜝�럥痢쎾뜝�럥�맶�뜝�럥�쑅�뜝�럥琉끻뜝�럥�맶�뜝�럥�쑅�뜝�럥萸얍뜝�럥�맶�뜝�럥�쑅占쎈뎨占쎈쑕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
                                        // ArtSetStat�뜝�럥�맶�뜝�럥�쑋占쏙옙�뼲�삕 �솾�꺂�뒧占쎈뎔�뜝�럥琉귨옙�쐻占쎈윥筌ο옙 �뜝�럥�맶�뜝�럥�쑅占쎈뼀�뜝占� �뜝�럥�맶�뜝�럥�쑅占쎈뎨�뜝�룞�삕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.


    double defaultDamage = GetDamage(); // �뜝�럥�맶�뜝�럥�쑋�뇦猿됰쭑占쎌맶�뜝�럥�쑋�뜝�럡�븤 �뜝�럥�맶�뜝�럥�쑅�뜏類ㅼ돥占쎌맶�뜝�럥�쑅�뜝�럩諭쇔뜝�럥�맶�뜝�럥�쑋�뛾占썲뜝占� �뜝�럥���耀붾굝梨멨뜝�뜦堉븅뜮�냲�삕筌랃옙�뜝�럩援꿨뜝�럥����뜝�럥爰뗥뜝�럩援뀐옙�쐻占쎈윥占쎈젩.
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


// TODO: �뜝�럥�맶�뜝�럥�쑋占쏙옙�뼚짹占쎌맶�뜝�럥�쑋�뜝�럩議� StatExceptSubOpt�뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑋�뜝�럡�꺘�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑅占쎈뎨�뜝�룞�뵾占쎌뒩占쎈뤈�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈ご占쎄뭍占쎌맶�뜝�럥�쑅�뜏類ｋ쭠占쎌맶�뜝�럥�쑅占쎌젂�뜝占�. �뜝�럥�맶�뜝�럥�쑅�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑋占쏙옙�븮猿딆맶�뜝�럥�쐾�뜝�럥彛� �뜝�럥�맶�뜝�럥�쐾�뜝�럥�젾�뜝�럥�맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈뼓�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌띿닂�삕占쎌맶�뜝�럥�쑋�뛾占썲뜝占� �솾袁㏉�쀯옙維딉옙�쐻占쎈짗占쎌굲占쎈쎗�뜝�룞�삕占쎈탿�뜝�럩援뀐옙�쐻占쎈윪鈺곕〕�삕占쎌맶�뜝�럥�쐾�뜝�럥彛� �뜝�럥�맶�뜝�럥�쑅占쎈뎨�뜝�룞�삕占쎌맶�뜝�럥�쑅�뜏類㏃삕 �뜝�럥�럸�뜝�럥�졎力놂옙筌믍됰���뜝�럩�뮲�뜝�럩援뀐옙�쐻占쎈윥�뤃占� �솾�꺂�뒖占쎈１占쎌젂占쎈뼐占쎈���뜝�럡�꽫�뜝�럩援꿨뜝�럥�몡占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅占쎈뎨占쎈쑕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
void Character::MakeScoreFunction()
{
    std::array<int, 10> mainOp = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower�뜝�럥�맶�뜝�럥�쑅�뜝�럥援곁솾�꺂�뒧占쎈역�뜝�럩援� �뜝�럥�맶�뜝�럥�쑋�뜝�럩諭얍뜝�럥�맶�뜝�럥�쑋�뜝�럥痢쎾뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 �뜝�럥�늾占쎈쇊占쎈㎦占쎌맠占쎈쐻占쎈윥占쎄샹�뜝�럥�맶�뜝�럥�쑅占쎌젂�뜝占�.

    // Character占쎈쐻占쎈윥占쎄괴�뜝�럥�맶占쎈쐻�뜝占� 10�뜝�럥夷у뜝�럥利든뜏類ｋ��占쎌맶�뜝�럥吏쀥뜝�럩援� �뜝�럩�뀇�윜諭꾪맊�떋�떟踰㏆옙援욃뜝�룞�삕�뜝�럩援꿨뜝�럥���占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅占쎌젂熬곻옙占쎌맶�뜝�럥�쑋�뵓怨뺣쭠占쎌맶�뜝�럥�쑅�뜝�럥援�, �뜝�럥夷у뜝�럥利멨뜝�럩援� Character�뜝�럥�맶�뜝�럥�쑅�뜝�럥援겼뜝�럥�늾占쎈쇊癰귨옙占쎄뎡 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌띿닂�삕占쎌맶�뜝�럥�쑋占쏙옙�뼲�삕 �뜝�럥�맶�뜝�럥�쑋�뜝�럩踰곻옙�쐻占쎈윞�뙼占썲뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈ぁ占쎈꽠��⑨퐢爾녑뜝�럡�맖�뜝�럥�맶�뜝�럥�쑋占쎄덩占쎈�섓옙�맶�뜝�럥�쑅�뜏類㏃삕 Artifact占쎈쐻占쎈윥占쎄괴�뜝�럥�맶占쎈쐻�뜝占� 嚥싳쉶瑗э옙�뒭占쎄뎡�뜝�럥�맶�뜝�럥�쑅占쎌젂�뜝占�.
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

    for (int i = 0; i < 45; i++) // for占쎈쐻占쎈윥占쎈뼓癲ル슢�뀕占쎈쳮嶺뚮ㅏ諭띰옙�뮋�뜝�럥�솕�뜝�럩援� 45�뜝�럥�맶�뜝�럥�쑋�뜝�럥夷㎩뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑅�뜝�럥�뼓, 
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

                // �뜝�럥夷у뜝�럥利멨뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋�뜝�럡�뀘 �뜝�럥�맶�뜝�럥�쑋�뜝�럩�젋�뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾢뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈き�맦占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜏類ㅼ돥占쎌맶�뜝�럥�쐾�뜝�럥六뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�몡占쎌넂占쏙옙怨멸뎡�뜝�럥鍮앾옙�쐻�뜝占� ((5 - �뜝�럩�쐪�뜝�럩�뮝�뜝�럥�빢占쎈쐻占쎈윞筌띿닂�삕占쎌맶�뜝�럥�쑅�뜝�럥�뿼占쎈쐻占쎈윞�뙼占썲뜝�럥�맶占쎈쐻�뜝占�) �뜝�럩�뀇�윜諭꾪맔占쎌굲占쎈쐻占쎈짗占쎌굲占쎌젂�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럩諭얍뜝�럥�늾占쎈쇊癰귨옙占쎄뎡 �솾�꺂�뒱占쎌굲占쎈꽠��⑨퐢爾닷뜝�럩紐��뜝�럥�맶�뜝�럥�쑅�뜏類ｋ쭦筌뚮냵�삕�뙴�뵃�삕占쎄뎡?)占쎈쐻占쎈윥占쎄괴�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럡�꼫�뜝�럥�맶�뜝�럥�쑋占쏙옙�븪�벉�맶�뜝�럥�쑅占쎈뎨�뜝�뜴�쐻占쎈짗占쎌굲�뜝�럥爾잌뜝�럩援� �솾�꺂�뒱占쎌굲占쎈꽠��⑨퐢爾닷뜝�럥占썬룇�삕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
                    // If impossible,
                        // �뜝�럥�맶�뜝�럥�쑅占쎌젂熬곻옙占쎌맶�뜝�럥�쑋�뵓怨ㅼ삕 �뜝�럥�맶�뜝�럥�쑋�뜝�럩�젋�뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾢뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈き�맦占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜏類ㅼ돥占쎌맶�뜝�럥�쐾�뜝�럥六뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�몡占쎌넂占쏙옙怨멸뎡�뜝�럥鍮앾옙�쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럡�꼫�뜝�럥�맶�뜝�럥�쑋占쏙옙�븪�벉�맶�뜝�럥�쑅占쎈뎨占쎈쑕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�. (�솾�꺂�뒗占쎄콬�뜏類ｋ��占쎌맶�뜝�럥吏쀥뜝�럩援� 5�뜝�럥�맶�뜝�럥�쑋�뜝�럥夷� �뜝�럥�럸�뜝�럥利듸옙紐�占쎌뵛占쎌굲雅��굢�삕)
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