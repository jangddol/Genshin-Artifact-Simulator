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
	// �솾�꺂�뒱占쎌굲�뜝�럩�꼥占쎈쐻占쎈짗占쎌굲占쎄괌占쎈눓占쎌맶�뜝�럥�쐾�뜝�럥�셾 �뜝�럥�맶�뜝�럥�쑋�뼨��곸뒅占쎌맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲 : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 占쎈쐻占쎈짗占쎌굲�뜝�럥占쏙옙�뵓怨뺣㎜占쎈윿占쎈쐻�뜝占� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // 占쎈쐻占쎈윥占쎈뼓占쎈쐻占쎈윥�젆�띿삕��얜뀘�쐻�뜝占� �뜝�럩�쐪�뜝�럩�뮝�뜝�럥�빢占쎈쐻占쎈윞筌랃옙 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 占쎈쐻占쎈윥占쎈뼓占쎈쐻占쎈윥�젆�띿삕��얜뀘�쐻�뜝占� 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌랃옙 : 0 ~ 18 // �뜝�럥�맶�뜝�럥�쑋�뜝�럡愿쒎뜝�럥�맶�뜝�럥�쑋�뜝�럩踰곩뜝�럥�맶�뜝�럥�쑋占쎄덩�뜝占� �뜝�럥�럸�뜝�럥利꿨뜝�룞�삕影�瑜곸굲�뜝�럩議녶뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 �뜝�럥�늾占쎈쇊占쎈㎦占쎌맠�뜝�럥�맶�뜝�럥吏쀥뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼀�뜝�럥�맶�뜝�럥�쑅嶺뚮쵓�삕
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 占쎈쐻占쎈윥占쎈뼓占쎈쐻占쎈윥�젆�띿삕��얜뀘�쐻�뜝占� 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥�뱻�뇦猿곴뭍占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌랃옙 : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑋�뜝�럡愿쒎뜝�럥�맶�뜝�럥�쑋�뜝�럩踰곩뜝�럥�맶�뜝�럥�쑋占쎄덩�뜝占� �뜝�럥�럸�뜝�럥利꿨뜝�룞�삕影�瑜곸굲�뜝�럩議녶뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 �뜝�럥�늾占쎈쇊占쎈㎦占쎌맠�뜝�럥�맶�뜝�럥吏쀥뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼀�뜝�럥�맶�뜝�럥�쑅嶺뚮쵓�삕
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

	// �뜝�럥�맶�뜝�럥�쐾�뜝�럥�젾�뜝�럥�맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈뼓�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쐾�뜝�럥猷드뜝�럥�맶�뜝�럥�쑅�뛾占썲뜝占� : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑋�뜝�럡愿쒎뜝�럥�맶�뜝�럥�쑋�뜝�럩踰곩뜝�럥�맶�뜝�럥�쑋占쎄덩�뜝占� �뜝�럥�럸�뜝�럥利꿨뜝�룞�삕影�瑜곸굲�뜝�럩議녶뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 �뜝�럥�늾占쎈쇊占쎈㎦占쎌맠�뜝�럥�맶�뜝�럥吏쀥뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼀�뜝�럥�맶�뜝�럥�쑅嶺뚮쵓�삕
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
    
    // �뜝�럥�맶�뜝�럥�쐾�뜝�럥�젾�뜝�럥�맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈뼓�뜝�럥�맶占쎈쐻�뜝占� �뜝�럩�쐪�뜝�럩�뮝�뜝�럥�빢占쎈쐻占쎈윞筌랃옙 : 0 ~ 8, 10 ~ 18
    
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
    
    // �뜝�럥�맶�뜝�럥�쐾�뜝�럥�젾�뜝�럥�맶�뜝�럥�쑋占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈윥占쎈뼓�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윞�뙼占썲뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윞筌랃옙 : 0 ~ 9

    for (int i = 0; i < 10; i++)
    {
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtFlower->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtFeather->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtClock->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtCup->GetSubStatValue(i));
        mStatAfterUpdateFromArtifactSubStat.AddOption(i, mArtCrown->GetSubStatValue(i));
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
            effectiveStats.emplace_back(GetEffection(i));
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

    for (int i = 0; i < endScore; i++) // for占쎈쐻占쎈윥占쎈뼓癲ル슢�뀕占쎈쳮嶺뚮ㅏ諭띰옙�뮋�뜝�럥�솕�뜝�럩援� 45�뜝�럥�맶�뜝�럥�쑋�뜝�럥夷㎩뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑅�뜝�럥�뼓, 
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

            // �뜝�럥夷у뜝�럥利멨뜝�럩援꿨뜝�럥�맶�뜝�럥�쑋�뜝�럡�뀘 �뜝�럥�맶�뜝�럥�쑋�뜝�럩�젋�뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾢뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈き�맦占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜏類ㅼ돥占쎌맶�뜝�럥�쐾�뜝�럥六뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�몡占쎌넂占쏙옙怨멸뎡�뜝�럥鍮앾옙�쐻�뜝占� ((5 - �뜝�럩�쐪�뜝�럩�뮝�뜝�럥�빢占쎈쐻占쎈윞筌띿닂�삕占쎌맶�뜝�럥�쑅�뜝�럥�뿼占쎈쐻占쎈윞�뙼占썲뜝�럥�맶占쎈쐻�뜝占�) �뜝�럩�뀇�윜諭꾪맔占쎌굲占쎈쐻占쎈짗占쎌굲占쎌젂�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럩諭얍뜝�럥�늾占쎈쇊癰귨옙占쎄뎡 �솾�꺂�뒱占쎌굲占쎈꽠��⑨퐢爾닷뜝�럩紐��뜝�럥�맶�뜝�럥�쑅�뜏類ｋ쭦筌뚮냵�삕�뙴�뵃�삕占쎄뎡?)占쎈쐻占쎈윥占쎄괴�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럡�꼫�뜝�럥�맶�뜝�럥�쑋占쏙옙�븪�벉�맶�뜝�럥�쑅占쎈뎨�뜝�뜴�쐻占쎈짗占쎌굲�뜝�럥爾잌뜝�럩援� �솾�꺂�뒱占쎌굲占쎈꽠��⑨퐢爾닷뜝�럥占썬룇�삕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�.
                // If impossible,
                    // �뜝�럥�맶�뜝�럥�쑅占쎌젂熬곻옙占쎌맶�뜝�럥�쑋�뵓怨ㅼ삕 �뜝�럥�맶�뜝�럥�쑋�뜝�럩�젋�뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾢뜝�럥夷у뜝�럥利멨뜝�럩援� �뜝�럥�맶�뜝�럥�쑅占쎈き�맦占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜏類ㅼ돥占쎌맶�뜝�럥�쐾�뜝�럥六뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥�몡占쎌넂占쏙옙怨멸뎡�뜝�럥鍮앾옙�쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럡�꼫�뜝�럥�맶�뜝�럥�쑋占쏙옙�븪�벉�맶�뜝�럥�쑅占쎈뎨占쎈쑕占쎌맶�뜝�럥�쑅占쎌젂�뜝占�. (�솾�꺂�뒗占쎄콬�뜏類ｋ��占쎌맶�뜝�럥吏쀥뜝�럩援� 5�뜝�럥�맶�뜝�럥�쑋�뜝�럥夷� �뜝�럥�럸�뜝�럥利듸옙紐�占쎌뵛占쎌굲雅��굢�삕)
            int jEnd = 5 ? i < 20 : 2;
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

    if (damage <= mSavedFunction[0])
    {
        if (mSavedFunction[0] >= mSavedFunction[1])
        {
            assert(false);
        }
        score = (damage - mSavedFunction[0]) / (mSavedFunction[1] - mSavedFunction[0]);
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


double Character::GetScore_MonkeyMagic() const // TODO : �솾�꺂�뒱占쎌굲占쎈꽠��⑨퐢爾닷뜝�럩留ュ뜝�럥�맶�뜝�럥�쑅�뜝�럥萸얍뜝�럥�맶�뜝�럥�쑅嶺뚮엪�삕
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
            remove(clockPossibleMain.begin(), clockPossibleMain.end(), clock);
        }
    }
    for(auto cup: cupPossibleMain)
    {
        if(GetEffection(cup) <= 0)
        {
            remove(cupPossibleMain.begin(), cupPossibleMain.end(), cup);
        }
    }
    for(auto crown: crownPossibleMain)
    {
        if(GetEffection(crown) <= 0)
        {
            remove(crownPossibleMain.begin(), crownPossibleMain.end(), crown);
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

                // tempDamage�뜝�럥夷у뜝�럥利멨뜝�럩援� top10Option�뜝�럥�맶�뜝�럥�쑅�뜝�럥援� �뜝�럥�맶�뜝�럥�쑋占쎄덩占쎈�섓옙�맶�뜝�럥�쑅�뜏類㏃삕 minOption�뜝�럩�뀇�윜諭꾪맔占쎌굲占쎈쐻占쎈짗占쎌굲占쎌젂�뜝占� �뜝�럥�맶�뜝�럥�쐾濚밸㉡�삕 �뜝�럥�늾占쎈쇊占쎈엮占쎌몝占쎈쐻占쎈윥獒뺧옙
                if (tempRefDamage > top10Options[9].damage)
                {
                    for (int i = 0; i < 10; i++)// �솾�꺂�뒧占쎈�뽩뜝�럥利겼뜝�럥堉��뜝�룞�삕�솒占썲뜝�뜫援⒴퐲��㏓쇊占쎈열�걫 �뜝�럥�맶�뜝�럥�쑅占쎈쨨占쎌몱占쎌맶�뜝�럥�쑅�뜝�럡�맖�뜝�럥夷у뜝�럥利멨뜝�럩援꿨뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕 �솾�꺂�뒩占쎈뤈�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥爰듿뜝�럥�맶�뜝�럥�쑅�뜝�럥�뼃�뜝�럥�맶�뜝�럥�쑅占쎈뎨�뜝�뜴�쐻占쎈짗占쎌굲�뜝�럥爾잌뜝�럩援�, �뜝�럥�맶�뜝�럥�쐾�뜝�럥彛ⓨ뜝�럥�맶�뜝�럥�쑅�뜝�럡�맖嚥싳쉶瑗э옙�뒭占쎄뎡�뜝�럥�맶�뜝�럥�쑅占쎌젂�뜝占�.
                    {
                        if (tempRefDamage > top10Options[i].damage)
                        {
                            for (int j = 8; j > i; j--)
                            {
                                top10Options[j + 1] = top10Options[j];
                            }
                            // push to backside if damage is less than tempRefDamage
                            top10Options[i].mainOptions = { clockOpt, cupOpt, crownOpt };
                            top10Options[i].damage = tempRefDamage;
                            break;
                        }
                    }
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