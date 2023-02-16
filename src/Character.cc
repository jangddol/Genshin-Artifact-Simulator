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
	// �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럡�떍占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럡�뒋�뜝�럩�뮍�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈퉻占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�뜝�럡愿� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�룞�삕�뜝�룞�삕占쎌맶�뜝�럥吏쀥뜝�럩援꿨뜝�룞�삕占쎈��筌��쓧留깍옙�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럡�떐占쎈쐻占쎈짗占쎌굲�뜝�럥占쏙옙�뵓怨쀬쪣占쎌굲�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌뤿럽�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹μ삌占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럵占쎈쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈즶占쎈쐻占쎈짗占쎌굲�뜝�럡�꽫�뜝�럩援뀐옙�쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋嶺뚮씮�뼇占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋노젘占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곎딆굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹μ삌占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럵占쎈쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈즶占쎈쐻占쎈짗占쎌굲�뜝�럡�꽫�뜝�럩援뀐옙�쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿룞�삕占쎌맶�뜝�럥�쑅�뜝�럥�늸占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� : 0 ~ 18 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留쏙옙�쐻占쎈윞占쎈뭼占쎈쐻占쎈윥�뜝�럩�뒻占쎌굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�꺐占쏙옙�뫀�뒧�뤃�깷�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈첋�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩諭뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮蹂⑹삕�굢琯�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫅�렡濚밸Ŧ�뵰占쎄뎡占쎈쐻占쎈윥占쎄괴�뜝�룞�삕占쎈��占쎈듋�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�솑占쏙옙�뫀踰졾뜝�룞�삕占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸블뀭占쎈쐻占쎈짗占쎌굲�뜝�럡肄у뜝�럥����뜝�럥�깱�뜝�럩援뀐옙�쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥愿섓옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈듃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젘占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵰鴉딆뮋�삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕쏉┼�슔�걞占쎄뎃�뜝�럩援뀐옙�쐻占쎈윞占쎈젇
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹μ삌占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럵占쎈쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈즶占쎈쐻占쎈짗占쎌굲�뜝�럡�꽫�뜝�럩援뀐옙�쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿룞�삕占쎌맶�뜝�럥�쑅�뜝�럥�늸占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨占쎈�섓옙�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁�뜝�룞�삕占쎈���뜝�룞�삕占쎈퉲繹먮씮�굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留쏙옙�쐻占쎈윞占쎈뭼占쎈쐻占쎈윥�뜝�럩�뒻占쎌굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�꺐占쏙옙�뫀�뒧�뤃�깷�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈첋�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩諭뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮蹂⑹삕�굢琯�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫅�렡濚밸Ŧ�뵰占쎄뎡占쎈쐻占쎈윥占쎄괴�뜝�룞�삕占쎈��占쎈듋�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�솑占쏙옙�뫀踰졾뜝�룞�삕占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸블뀭占쎈쐻占쎈짗占쎌굲�뜝�럡肄у뜝�럥����뜝�럥�깱�뜝�럩援뀐옙�쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥愿섓옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈듃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젘占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵰鴉딆뮋�삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕쏉┼�슔�걞占쎄뎃�뜝�럩援뀐옙�쐻占쎈윞占쎈젇
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

	// �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�댖怨뺤탣占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹섎쳴占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥�뤃�뼹�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�쓠占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾쑚�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� : 0 ~ 23, 27, 28 // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留쏙옙�쐻占쎈윞占쎈뭼占쎈쐻占쎈윥�뜝�럩�뒻占쎌굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�꺐占쏙옙�뫀�뒧�뤃�깷�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈첋�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩諭뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮蹂⑹삕�굢琯�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫅�렡濚밸Ŧ�뵰占쎄뎡占쎈쐻占쎈윥占쎄괴�뜝�룞�삕占쎈��占쎈듋�뜝�럥�젃�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�솑占쏙옙�뫀踰졾뜝�룞�삕占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸블뀭占쎈쐻占쎈짗占쎌굲�뜝�럡肄у뜝�럥����뜝�럥�깱�뜝�럩援뀐옙�쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥愿섓옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈듃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젘占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵰鴉딆뮋�삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕쏉┼�슔�걞占쎄뎃�뜝�럩援뀐옙�쐻占쎈윞占쎈젇
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
    
    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�댖怨뺤탣占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹섎쳴占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋嶺뚮씮�뼇占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋노젘占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곎딆굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� : 0 ~ 8, 10 ~ 18
    
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
    
    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�댖怨뺤탣占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹섎쳴占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿룞�삕占쎌맶�뜝�럥�쑅�뜝�럥�늸占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐벂�쐻占쎈짗占쎌굲占쎌넂�뜝�룞�삕�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� : 0 ~ 9

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
    // 占쏙옙�몿�뫒亦낆룊�삕�굢占� �뜝�럥�닡�뜝�럩�뭵�뜝�럥由� 占쎄껀�뜝�룞�삕占쎄맙 占쎈퉲�겫�룞�삕�뜝�룞�삕占쎈츎 ResonanceStat�뜝�럩紐드슖�댙�삕 �뜝�럥由썲뜝�럥堉�.
        // �뜝�럩逾졾뜝�럩占쏙옙�뜝�럥裕�, �윜諛몄굡繹먲옙 Stat�뜝�럩逾졾뜝�럩逾у뜝�럡�맋 �뜝�럩�젘�윜諭��닔占쎈턄 �뜝�럥�졅�뜝�럥留�.
        // Update�뤆�룊�삕 �뜝�럩沅롥뜝�럩�굥濾곌쑬梨띰옙遊븀뼨��먯삕 �뜝�럥由�嶺뚯쉻�삕嶺뚮엪�삕, �뜝�럥堉롦뤆�룄�궞�뇡占썹춯�쉻�삕 �뜝�럥瑜ュ뜝�럩踰�.
                                    // 230131
        // ResonanceStat �뜝�럥�뱺 �뜝�룞�삕�뜝�룞�삕�뇡占� Update Optimization 占쏙옙�뫁�닔占쎌젧�뜝�럥�뱺�뜝�럡�맋
        // 濚밸�λ룱占쎄맙占쏙옙�뫅�삕 �뇖�궡瑗백�얠뼃泥롥뜝占� �뜝�럡�뀬�뜝�럩�뮔�뜝�럥�뵹嶺뚯쉻�삕 �뜝�럥瑜ュ뜝�럥裕� �뇦猿볦삕 �뜝�럥瑜댐옙�닱筌뤾쑬�뱺
        // �뜝�럩逾� �뜝�럥留쇿뜝�럥�빢�뤆�룊�삕 嶺뚮씮鍮볟뜝�룞�삕嶺뚯쉻�삕. 
        // 0 ~ 18 嶺뚮ㅄ維�筌륅옙 �뜝�럡�뀬�뜝�럩�뮔�뜝�럥�뵹�뜝�럥裕� �뇦猿됲뜑占쎈굵 �뜝�럡�뀬�뜝�럩�뮔�뜝�럥�돵�뜝�럥�뒍�뜝�럥由썲뜝�럥堉�.
        // ArtSetStat�뜝�럩逾� 嶺뚮씮�돳占쎈츎 �뜝�럥苡� �뜝�럥由��뜝�럥堉�.

    double defaultDamage = GetDamage(); // �뜝�럩寃긷뜝�럩�궨 �뜝�럥裕욃뜝�럥�쓢�뜝�럩諭� �뼨轅명�▽빳�맮�삕�뇡�냲�삕占쎈펲.
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
            effectiveStats.emplace_back(i);
        }
    }

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃怨곸녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪獄��뼃�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌μ렱�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲 占쎈쐻占쎈윥占쎈듋�뜝�럥�뇢�뜝�럥�렑�뜝�럩留졾뜝�럥�맶�뜝�럥�쑅�뜝�럡�꺓占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼占쎈쐻�뜝占�.

    // Character�뜝�럥�맶�뜝�럥�쑅�뜝�럡愿댐옙�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 10占쎈쐻占쎈윥鸚롤껊쐻占쎈윥筌앸뱺�쐪筌먲퐢占쏙옙�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃占� 占쎈쐻占쎈윪占쎈�뉛옙�쐺獄�袁る쭒占쎈뼀占쎈뼗甕겹룇�삕�뤃�쉩�쐻占쎈짗占쎌굲占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥占쏙옙占썲뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�넭怨살삕�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈탶��⑤베彛졾뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃占�, 占쎈쐻占쎈윥鸚롤껊쐻占쎈윥筌앸ŀ�쐻占쎈윪�뤃占� Character占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃寃쇰쐻占쎈윥占쎈듋�뜝�럥�뇢�솻洹⑥삕�뜝�럡�렊 �뜝�럥�맶�뜝�럥�쐾占쎈쇊�뜝�뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾嶺뚮씮�땬占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�룞�삕占쎈섣占쎌굲 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪甕곌내�삕占쎌맶�뜝�럥�쐾占쎈쇊�뜝�뜴�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�걖�뜝�럥苑좑옙占썩뫅�맊�댚�뀘�쐻占쎈윞占쎈쭡占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럡�뜦�뜝�럥占쎌꼻�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦룂�굲 Artifact�뜝�럥�맶�뜝�럥�쑅�뜝�럡愿댐옙�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �슖�떝�돳�몭�띿삕占쎈뮡�뜝�럡�렊占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼占쎈쐻�뜝占�.
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

    for (int i = 0; i < endScore; i++) // for占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�솻洹섎룱占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅占쎈쐻占쎈윥占쎈뼟�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�솗占쎈쐻占쎈쑟占쎈뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥�뜝�럥�돟占쎌럸占쎈쐻占쎈짗占쎌굲�뜝�럥�꺙占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅嶺뚋뼛울옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�뜝�럡����뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� 45�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔筌욌굢�삕占쎌젦占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕앾옙�쐻占쎈윥筌뚮겭�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥�젆�꽒�쐻占쎈윞占쏙옙占쏙┼�슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥利�, 
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

            // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔夷뚳옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮녹삕�뜝�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩�쟼�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈뼓占쎈꽠��⑥궡�굲占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔夷뚳옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮녹삕�뜝�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윞�꽴臾뺤삕占쎌맶�뜝�럥�쑅�뜝�럥利뀐옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥彛��솾�꺂�뒧罹됧뜝�럡肄э옙�쐻占쎈윥塋딅ㅇ�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧筌ㅻ〕�삕占쎌벀占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럡肄욑옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윞占쎄틣占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈짗占쎌굲�뜝�럥占쏙옙�뜝�럥�뱥占쎈쐻占쎈윥占쎌쟽�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈ぁ占쎈쐻占쎈윥�눧袁��쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�몱占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� ((5 - �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋嶺뚮씮�뼇占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋노젘占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곎딆굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜾�녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윥�뜮猿볦삕占쎌맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩援�占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿룞�삕占쎌맶�뜝�럥�쑅�뜝�럥�늸占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�) �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈윥占쎈즽�뜝�럩援뀐옙�쐻占쎈윪筌띾냲�삕占쎌럸占쎈쐻占쎈쓠鴉딅뜉�삕�뤃琯�맪占쎌삖占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�럸占쎈쐻占쎈짗占쎌굲占쎌젂�뙼蹂��굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥愿섓옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈듃占쎈쐻占쎈윪占쎈�뉛옙�쐺獄쏆옓爾쎾뜝�럡�렊占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎌쟽 �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥鍮앾옙�꽞�눧誘り뎡�뜝�럥�맶�뜝�럥�몘力놂옙占쎄덩占쎌굲嶺뚮씭逾껓옙�굲�뜝�럥�넇占쎈쐻占쎈윥�젆議얜쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧筌잙뿰�삕占쎄뎡�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥彛��솾�꺂�뒧占쎈룜�뜝�럥彛믧땡�끉猷딉옙�굲占쎌녇占쎄틓占쎈뮛�뜝�럥裕쏉옙�쐻占쎈윞占쎈뼃�뜝�럥�맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�룞�삕�뜝�룞�삕占쎌맶�뜝�럥�쑅�뜝�럡臾썲뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎌쟽?)占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럡�뒋�뜝�럥�냷�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쐾�뜝�럡�꽢�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뎨占쎈쭠占쎌맶�뜝�럥�쑅占쎈뼀占쎈�섓옙�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젢�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎌넇占쎈쐻占쎈윪�굢酉대쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥鍮앾옙�꽞�눧誘り뎡�뜝�럥�맶�뜝�럥�몘力놂옙占쎄덩占쎌굲嶺뚮씭逾껓옙�굲�뜝�럥�넇占쎈쐻占쎈윥�젆議얜쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈첑�뜝�럥����뜝�럥猷뤷뜝�럩援뀐옙�쐻占쎈윞占쎈젇占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
                // If impossible,
                    // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿占쎈쐻占쎈윥�땻醫묒삕�뜝�뜦維곫쾮�댙�삕�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럡�떐占쎈쐻占쎈짗占쎌굲�뜝�럥占쏙옙�뜝�럡肄э옙�쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩�쟼�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈뼓占쎈꽠��⑥궡�굲占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔夷뚳옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮녹삕�뜝�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윞�꽴臾뺤삕占쎌맶�뜝�럥�쑅�뜝�럥利뀐옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥彛��솾�꺂�뒧罹됧뜝�럡肄э옙�쐻占쎈윥塋딅ㅇ�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧筌ㅻ〕�삕占쎌벀占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럡肄욑옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윞占쎄틣占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈짗占쎌굲�뜝�럥占쏙옙�뜝�럥�뱥占쎈쐻占쎈윥占쎌쟽�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈ぁ占쎈쐻占쎈윥�눧袁��쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�몱占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쐾�뜝�럡�꽢�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뎨占쎈쭠占쎌맶�뜝�럥�쑅占쎈뼀占쎈�섓옙�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젢占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎄콪占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�. (�뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類ｋ㎜占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽�뜝�럡����뜝�럥�뵰占쎄뎡占쎈쐻占쎈윪筌띿떣�뵾占쎌뒧占쎈룜�뜝�럥彛믣뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩占쎈뤅占쎈쐻占쎈뼢占쎄땀筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� 5�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮놅옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔鸚룔굩�삕占쎄뎡 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩諭뷴뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮щ┛占쎈묄占쎌굲�뜝�럡�렊�솾�꺂�뒧筌잙뿰�삕占쎄뎡占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�룞�삕占쎈폏占쎌졒�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�굝�샍占쎌맚�뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥夷⑨옙�쐻占쎈쑕占쎌맶�뜝�럥�쑋占쎈쨨�뜝占�)
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


double Character::GetScore_MonkeyMagic() const // TODO : �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類㏃삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥鍮앾옙�꽞�눧誘り뎡�뜝�럥�맶�뜝�럥�몘力놂옙占쎄덩占쎌굲嶺뚮씭逾껓옙�굲�뜝�럥�넇占쎈쐻占쎈윥�젆議얜쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈턂濚밸Ŧ夷わ옙�굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈쟿占쎈쐻占쎈윥占쎈젨占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占썬룂�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈열�뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占�
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

                // tempDamage�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔夷뚳옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮녹삕�뜝�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� top10Option�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇鍮�占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈첋占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥�쑋占쎈き占쎄땔占쎌굲�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥彛��솾�꺂�뒧罹됵옙�쇀域밟뫁�굲占쎈쨨�뜝占� minOption�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈윥占쎈즽�뜝�럩援뀐옙�쐻占쎈윪筌띾냲�삕占쎌럸占쎈쐻占쎈쓠鴉딅뜉�삕�뤃琯�맪占쎌삖占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎈뮋�뜝�럥堉붹뤆�룊�삕占쎈쐻占쎈윥�뙴�뮋�삕占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥愿섓옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈듃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈옅占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢履뉔뜮�냵�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉�뙴洹⑥삕�뤃占�
                if (tempRefDamage > top10Options[9].damage)
                {
                    for (int i = 0; i < 10; i++)// �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類ㅻ즸占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥�쑅�뜝�럥猷덌옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮蹂〓き占쎈쐣占쎌굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑋�뜝�럡�꺏占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럥泥묕옙�쐻占쎈윥鸚룐벂�쐻占쎈윥�뜝�뜫援⑼옙�굲嶺뚮씞援앾옙�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥筌욌뿥�쐻占쎈윥占쎈듃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쏙옙占썲뜝�럥�맶�뜝�럥�쐾占쎄슈�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�땱�떜利얕린洹⑥삕占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뀕筌띾맮�삕占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뵯占쎈탿�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅鶯ㅼ렮堉뱄옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�빋占쎈뼔夷뚳옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒩�뜮戮녹삕�뜝�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐몾履쏉옙寃�泳�占썲뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥彛��솾�꺂�뒧罹됵옙�쇀域밟뫁�굲占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑋�뜝�럥占쎈뿰�삕占쎌맶�뜝�럥�쐾�뜝�럥鍮잌뜝�럥�맶�뜝�럥�쑅�뜏類ㅻ묄占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슢�뒧占쎌맋�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥�댆�뜴�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�젴�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌젢�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎌넇占쎈쐻占쎈윪�굢酉대쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占�, �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮씮�쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥궘占쏙옙�걠占쎌졎�뵓怨뚮옱占쎌굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占싸살맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅鶯ㅼ렮�뙔占쎌굲�뜝�럥裕뗰옙�쐻占쎈윥�젆類앸쐻占쎈윥筌뚭퇊�쐻占쎈윥占쎄광占쎈쐻占쎈윥占쎈뎔�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑅�뜏類싲뙔占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎌쟽�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥�쑌�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋嶺뚮쪇占쏙옙占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌뱿�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
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