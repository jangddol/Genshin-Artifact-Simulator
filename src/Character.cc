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
	// �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕�占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋�뜝�럡�꺏占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윞�꽴�슗�쐻占쎈윥占쎈땷占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩�걤 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥����뜝�룞�삕��⑤챶萸띰옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃占� : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲�뜝�럥�꺐占쏙옙�뫀踰좑옙�쟼占쎈쐻占쎈윥占쎌몺占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂잙쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�뜝�럥�뵰占쎄뎡�뜝�룞�삕占쎈폇占쏙옙�꼻�삕占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럩留싧뜝�럥�맶�뜝�럥�쑋�뜝�럥痢㎩뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾬옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂잙쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�뜝�럥�뵰占쎄뎡�뜝�룞�삕占쎈폇占쏙옙�꼻�삕占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 : 0 ~ 18 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾占쎄슈占쎈�울옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뵓怨뚮굫占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윞占쎈쑆�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�쓠�뜝�럥�맶�뜝�럥�쑅嶺뚯빓�뿥占쎌맶�뜝�럥吏쀥뜝�럩援꿨＄源띿삕占쎈ご��⑤㈇�렊�뜝�럥�맶�뜝�럥�쑋�댖怨뺣��占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥궘占쏙옙�걠�뙴�뵃�삕占쎄뎡 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈젒占쎈쐻占쎈윪筌띿쥜�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧筌ㅻ낑�삕占쎄뎡
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂잙쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�뜝�럥�뵰占쎄뎡�뜝�룞�삕占쎈폇占쏙옙�꼻�삕占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥援녑뜝�럥�늾占쎈쇊��⑤��異깍옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾占쎄슈占쎈�울옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뵓怨뚮굫占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윞占쎈쑆�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�쓠�뜝�럥�맶�뜝�럥�쑅嶺뚯빓�뿥占쎌맶�뜝�럥吏쀥뜝�럩援꿨＄源띿삕占쎈ご��⑤㈇�렊�뜝�럥�맶�뜝�럥�쑋�댖怨뺣��占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥궘占쏙옙�걠�뙴�뵃�삕占쎄뎡 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈젒占쎈쐻占쎈윪筌띿쥜�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧筌ㅻ낑�삕占쎄뎡
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

	// �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩議묈뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂⑹삕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅占쎈쇀占쎈굡占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�럸占쎈쐻占쎈쑕占쎌맶占쎈쐻�뜝占� : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾占쎄슈占쎈�울옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뵓怨뚮굫占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윞占쎈쑆�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�쓠�뜝�럥�맶�뜝�럥�쑅嶺뚯빓�뿥占쎌맶�뜝�럥吏쀥뜝�럩援꿨＄源띿삕占쎈ご��⑤㈇�렊�뜝�럥�맶�뜝�럥�쑋�댖怨뺣��占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥궘占쏙옙�걠�뙴�뵃�삕占쎄뎡 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈젒占쎈쐻占쎈윪筌띿쥜�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧筌ㅻ낑�삕占쎄뎡
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
    
    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩議묈뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂⑹삕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럩留싧뜝�럥�맶�뜝�럥�쑋�뜝�럥痢㎩뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾬옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 : 0 ~ 8, 10 ~ 18
    
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
    
    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅�뜝�럩議묈뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂⑹삕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 : 0 ~ 9

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
    Character* tempCharacter = this->Clone();
    ArtSetStat* tempArtSetStat = tempCharacter->GetArtSetStat();
    // 怨꾩궛�뿉 �븘�슂�븳 遺��샃 異붽���뒗 ResonanceStat�쑝濡� �븳�떎.
        // �씠�쑀�뒗, 洹몃깷 Stat�씠�씪�꽌 �젒洹쇱씠 �렪�븿.
        // Update媛� �삤�옒嫄몃━湲� �븯吏�留�, �떖媛곹븯吏� �븡�쓬.
                                    // 230131
        // ResonanceStat �뿉 ����븳 Update Optimization 怨쇱젙�뿉�꽌
        // 源≪샃怨� 移섑뵾媛� �궗�슜�릺吏� �븡�뒗 寃� �븣臾몄뿉
        // �씠 �븿�닔媛� 留앷��吏�. 
        // 0 ~ 18 紐⑤몢 �궗�슜�릺�뒗 寃껋쓣 �궗�슜�빐�빞�븳�떎.
        // ArtSetStat�씠 留욌뒗 �벏 �븯�떎.

    double defaultDamage = GetDamage(); // �쁽�옱 �뒪�럺�쓣 湲곕줉�븳�떎.
    for (int i = 0; i < 19; i++)
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
            effectiveStats.emplace_back(GetEffection(i));
        }
    }

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower占쎈쐻占쎈윥占쎈군癲ル슢�뿪占쎌굲 占쎈쐻占쎈윪占쎌뱾占쎈쐻占쎈윪占쎈츛占쎈쐻占쎈윥壤쏉옙 占쎈눇�뙼�맪�쐭�뜝�럥�꼧占쎈쐻占쎈윥�젆占�.

    // Character�뜝�럥�걫占쎈쐻�뜝占� 10占쎈쨬占쎈즵獒뺣끀�쐻占쎈짗占쎌굲 占쎌녃域뱄퐢苡답벧�굞占쏙옙占쎌굲占쎈눀�뜝占� 占쎈쐻占쎈윥�젆袁��쐻占쎈윪甕곕맕�쐻占쎈윥占쎈군, 占쎈쨬占쎈즸占쎌굲 Character占쎈쐻占쎈윥占쎈군占쎈눇�뙼蹂��굲 �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留숋옙�쐻占쎈윪��억옙 占쎈쐻占쎈윪占쎌벁�뜝�럡猿�占쎈쐻�뜝占� �뜝�럥�몡�넭怨ｋ쳳占쎄퐨占쎈쐻占쎈윪�굢�뀘�쐻占쎈윥獒뺧옙 Artifact�뜝�럥�걫占쎈쐻�뜝占� 濚욌꼬�슦�굲占쎈쐻占쎈윥�젆占�.
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

    for (int i = 0; i < endScore; i++) // for占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�젆蹂≪녇占쎄틓占쎈뮛�뜝�럥占쎈틶�쐻占쎈윥�댚占썹솾�꺂�뒧占쎈�뽫뛾占쏙옙�뵛占쎌굲�뜝�럥痢먨뜝�럥�맶�뜝�럥�쑅�뜝�럩�꼨�뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占� 45�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅勇싲짅�젘占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�룇, 
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

            // �뜝�럥�맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럥占쏙옙 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩�젆�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곻옙占쎌맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎄광�뜝�럥�룄占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ�걠占쎈／占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅嶺뚮쵎�읇占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�걖占쎈쐻占쎈윪占쎄샵占쎈쐻占쎈짗占쎌굲占쏙옙�뫀�늾占쎈젇�뜝�럥�맶�뜝�럥�쑅占쎈쑏占쎈묄占쎌굲�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� ((5 - �뜝�럥�맶�뜝�럥�쑋�뜝�럩留싧뜝�럥�맶�뜝�럥�쑋�뜝�럥痢㎩뜝�럥�맶�뜝�럥�쑅�뜝�럥臾꾬옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽癲ル슢�뵰占쎈빃�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�삓占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�) �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈돍�삕占쎌맳�뛾占썼쥈�굥彛잞옙�쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뛾占쏙옙堉껓옙�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎌녃域밟뫁�굲占쎈쐻占쎈윞占쎈젇 �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕�占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥�땻醫묒삕�뜝�뜦維낉옙留딉옙�뙕占쎈뼠占쎌맶�뜝�럥�쑋嶺뚮쪋�삕�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮㉡�맊壤쏉옙癲ル슢�뒧占쎄틣�뜝�럩援꿨뜝�럥����뜝�럥�깱�뜝�럩援뀐옙�쐻占쎈윞占쎈젇?)占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윞�꽴�뙋�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럡�꼸�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥由됧뜝�럥苡녑뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뙕占쎌뿺占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕�占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥�땻醫묒삕�뜝�뜦維낉옙留딉옙�뙕占쎈뼠占쎌맶�뜝�럥�쑅占쎈쐻占쎈쑌�뙴�돍�삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�.
                // If impossible,
                    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆占쎈꽠��⑥궡�굲占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�꺐占쏙옙�벀�걠占쎄뎡 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩�젆�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곻옙占쎌맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎄광�뜝�럥�룄占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ�걠占쎈／占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅嶺뚮쵎�읇占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�걖占쎈쐻占쎈윪占쎄샵占쎈쐻占쎈짗占쎌굲占쏙옙�뫀�늾占쎈젇�뜝�럥�맶�뜝�럥�쑅占쎈쑏占쎈묄占쎌굲�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럡�꼸�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥由됧뜝�럥苡녑뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�. (�뜝�럩�뀋�뜝�럡�땽�뜝�럥裕됵옙�쐻占쎈윞�굜�띿삕占쎌맚嶺뚮㉡�맊�뜝�룞�삕占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨�뜝占� 5�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅勇싲쨪�삕 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�쓠�뜝�럥�맶�뜝�럥�쑅嶺뚯빖踰앾옙�굲嶺뚮쪋�삕占쎈쐻占쎈윪��얠렱�쐻占쎈윪�뤃轅댁뜏�뜝�룞�삕�뤃占썲뜝�럩援�)
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


double Character::GetScore_MonkeyMagic() const // TODO : �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕�占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥�땻醫묒삕�뜝�뜦維낉옙留딉옙�뙕占쎈뼠占쎌맶�뜝�럥�쑋嶺뚮씞源뤄옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈㎟占쎈펱占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧占쎈역�뜝�럩援�
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

                // tempDamage�뜝�럥�맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� top10Option�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윞占쎈쑆占쎈쐻占쎈윥�뜝�럩瑗삼옙�굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ猷귨옙援� minOption�뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈돍�삕占쎌맳�뛾占썼쥈�굥彛잞옙�쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲슖�떜媛�占쎈룜�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈엶占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦룂�굲
                if (tempRefDamage > top10Options[9].damage)
                {
                    for (int i = 0; i < 10; i++)// �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥�뜝�럥�돇占쎌맶�뜝�럥�쑅嶺뚯빓瑗덌옙�맶�뜝�럥�쑅占쎌젂�뜝�룞�삕占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�럩�꼥占쎈쐻占쎈쑕占쎌맶�뜝�럥�몞占쎈쨨占쎈�わ옙留ゅ뜝�룞�삕占쎈짋占쎈눁占쎈쐻占쎈윥占쎈였�뜝�럡愿� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥鸚룐벂�쐻占쎈윪筌뤾막�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쐾�뜝�럥彛▼뜝�럥�맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ猷귨옙援� �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕앾옙�쐻占쎈윥筌뚮뜉�삕占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈뙀占쎈꺽占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�렠�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뙕占쎌뿺占쎌맶�뜝�럥�쑋占쎈쨨�뜝占�, �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅鶯ㅼ렮踰귨옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쐾�뜝�럥彛∽옙�뒙占쎈뼕占쎈뤂占쎈き占쎈씮�굲�뜝�럥裕∽옙�쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�.
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