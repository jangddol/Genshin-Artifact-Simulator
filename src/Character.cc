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


void Character::DoFeedback() {}


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
	// 癲ル슪�삕占쎌넂�뜝�룞�삕�걡�뇯�쐻占쎈윞占쎈뙃 占쎈쐻占쎈윪疫뀁슃�쐻占쎈윥�뜝�룞�삕 : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// �뜝�룞�삕占쎈��甕곕맮�럷�뜝占� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // �뜝�럥�떛�뜝�럥堉э옙逾녑뜝占� 占쎌뜏占쎌뒩占쎈땾�뜝�럡留� : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// �뜝�럥�떛�뜝�럥堉э옙逾녑뜝占� �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留� : 0 ~ 18 // 占쎈쐻占쎈윪占쎄괜占쎈쐻占쎈윪占쎌벁占쎈쐻占쎈윪�굢占� 占쎈쎗占쎈즲占쏙옙節륁삕占쎌졆占쎈쐻占쎈윥壤쏉옙 占쎈눇�뙼�맪�쐭占쎈쐻占쎈짗占쎌굲 占쎈쐻占쎈윥占쎈떋占쎈쐻占쎈윥筌묕옙
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// �뜝�럥�떛�뜝�럥堉э옙逾녑뜝占� �뜝�럡猿�占쎈쐻占쎈쓡野껁깷�쐻占쎈짗占쎌굲�뜝�럡留� : 0 ~ 23, 27, 28 // 占쎈쐻占쎈윪占쎄괜占쎈쐻占쎈윪占쎌벁占쎈쐻占쎈윪�굢占� 占쎈쎗占쎈즲占쏙옙節륁삕占쎌졆占쎈쐻占쎈윥壤쏉옙 占쎈눇�뙼�맪�쐭占쎈쐻占쎈짗占쎌굲 占쎈쐻占쎈윥占쎈떋占쎈쐻占쎈윥筌묕옙
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

	// 占쎈쐻占쎈윞占쎈렰占쎈쐻占쎈윪�뜝�룞�삕�뜝�럥�떛占쎈쐻�뜝占� 占쎈쐻占쎈윞占쎈룵占쎈쐻占쎈윥獄�占� : 0 ~ 23, 27, 28 // 占쎈쐻占쎈윪占쎄괜占쎈쐻占쎈윪占쎌벁占쎈쐻占쎈윪�굢占� 占쎈쎗占쎈즲占쏙옙節륁삕占쎌졆占쎈쐻占쎈윥壤쏉옙 占쎈눇�뙼�맪�쐭占쎈쐻占쎈짗占쎌굲 占쎈쐻占쎈윥占쎈떋占쎈쐻占쎈윥筌묕옙
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
    
    // 占쎈쐻占쎈윞占쎈렰占쎈쐻占쎈윪�뜝�룞�삕�뜝�럥�떛占쎈쐻�뜝占� 占쎌뜏占쎌뒩占쎈땾�뜝�럡留� : 0 ~ 8, 10 ~ 18
    
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
    
    // 占쎈쐻占쎈윞占쎈렰占쎈쐻占쎈윪�뜝�룞�삕�뜝�럥�떛占쎈쐻�뜝占� �뜝�럡猿�占쎈쐻占쎈짗占쎌굲�뜝�럡留� : 0 ~ 9
    
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
    ArtSetStat* tempArtSetStat; // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윞�뤃�떣�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�솗占쎈쐻占쎈짗占쎌굲�뜝�럥�맚�뜝�럥�맶�뜝�럥吏쀥뜝�럩援꿨뜝�럥夷ⓨ뜝�럩�룲�뜝�럩占쏙옙占쎈쐻占쎈윥占쎄묽占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾�뜝�럥�젃占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈첋�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뜝�룞�삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥�쑅�뵓怨쀫묄占쎌굲�뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�젛占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留쏙옙�쐻占쎈윥占쎈눁�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썹솾�꺂�뒧占쎈역�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쇀占쎈섣占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�돲占쎌뒭占쎄뎡 ResonanceStat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빉�녇占쎄틓占쎈뮛�뜝�럥占쎈쉥�쎗�뜝�룞�삕占쎈탿�뜝�럩援뀐옙�쐻占쎈윥筌λĿ�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪占쎄섀占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�젛�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
                                        // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈짂嶺뚯쉶媛딉옙�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉�뙴洹⑥삕�뤃占�, 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥占쎈젇占쎈쐻占쎈윥占쎈읁占쎈쐻占쎈윥筌앸떦異��뜝�뜾�쇀占쏙옙蹂┙울옙�쐻占쎈윥�뜝�룞�삕�뜝�럥�렅占쎌넂占쎄덱占쎄샵占쎈쐻占쎈짗占쎌굲占쏙옙�뫀�늾占쎈젇 Stat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈짂嶺뚯쉶媛딉옙�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥�뵳�띿삕占쎌뇢�뜝�럥�뒌嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌죦占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥占쎈젇占쎈쐻占쎈윥占쎈읁�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈쑏占쎄퐩占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쐾�뜝�럡�떓 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎌뜏占쎌뒗�뤃�빢�삕占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占�.
                                        // Update占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�땱�떜쨌�뙼�봿留띰옙�쐻占쎈윥占쎌몗癲ル슣鍮뽳옙占썲뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎈쐪占쎈쐻占쎈짗占쎌굲占쎈쨨占쎈묄占쎌굲占쎈쐻占쎈뼢占쎌뒭占쎄뎡占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥占쎈ぅ占쎈쐻占쎈윪占쎈�뗰옙�꽠��ⓑ살┯占쎈쐻占쎈윪�뜝�룞�삕�뜝�럩援뀐쫲�냲�삕�뜝�럥�돱占쎈퉲�뜝�룞�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎄틯�뜝�럥�맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쐾占쎄슈占쎌뒱占쎌굲�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�몧�뜝�럩�읆占쎈쐻占쎈윥�댚�엺�쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩逾쀥뜝�럥�맶�뜝�럥�쐾�뜝�럥�젃 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�젛占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윥占쎈짏�뜝�럥�맶�뜝�럥�쑋�뜝�럥痢㎩뜝�럥�맶�뜝�럥�쑅嶺뚮슢�쐣占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�굛��ｏ옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅�뜝�럥�뿴占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占�, 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥�뜝�럩�뮋�뜝�럥�솕�뜝�럩援꿰솾�꺂�뒧占쎈뮛�뜝�럡�떍占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥占쏙옙占쏙옙�꽠��⑤∥遊욘뤃轅댁뜏�뜝�룞�삕�뤃�쉻�삕占쎈쨮占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈쐻占쎈짗占쎌굲�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걮占쎈쐻占쎈윥占쎈룾�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윥鸚뤄옙占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럡愿닷뜝�럥�맶�뜝�럥�쐾占쎈㎟占쎌뵢占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎄틦�뜝�룞�삕占쎈��占쎄콬�뜝�럡�렊.
                                    // 230131
                                        // ResonanceStat 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� Update Optimization �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�솗�뜝�럥�졒�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥�눧袁ъ삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�댖怨뚮옱占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥夷③뇦猿뗫닑占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗傭��끉猷딉옙�굲
                                        // 占쎈쐻占쎈윪占쎈츐占쎈쐻占쎈윥�젆遺밸쨬占쎈즸占쎌굲占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥�젆�꽒�쐻占쎈윞占쎈�룡セ�뿫遊억ℓ蹂⑹삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썹솾�꺂�뒧占쎈뎔�뜝�럥�뼔占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�솗�뜝�럩肉뷴뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈��占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럡愿묈뜝�럥�럸�뜝�럥利꿨뜝�럥琉곻옙�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥吏귨옙�쐻占쎈윪占쎌읆�뜝�럥�뇢�솻洹λ퉾占쎌뱺�뜝�럩�뮋�뜝�럩�걢�뜝�럡���嶺뚮씭�몱占쎌맶�뜝�럥�쑅�뜝�럥�럪占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥堉⒴뜝�럩逾겼뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル쉵�눖�졒�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥筌앹뇯�쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빢�삕占쎌맶�뜝�럥�쑅嶺뚮슢�쐣占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럡愿닷뜝�럥�맶�뜝�럥�쐾占쎈㎟占쎌뵢占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉�뙴洹⑥삕�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈괘�뜝�럥�맶�뜝�럥�쑅�뜝�럥�늸�뜝�럩�뀇�윜諛잙쳛占쎄뎡�뜝�럥�맶�뜝�럥�쐾�뜝�럥�젃 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럡愿닷뜝�럥�맶�뜝�럥�쑅�뜝�럥爰묈뜝�럥�맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈즵�뜝�럩�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥�뜝�럥�떢占쎌맶�뜝�럥�쑅占쎈뙕�걹占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥夷⑨옙�쐻�뜝占�
                                        // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎄퐫占쎈쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�㏃녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윥占쎈폇�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�떛�넭怨살삕�뜝�럩留띰옙�쐻占쎈윥占쎌몗�땱�떜쨌�뙼�봿留띰옙�쐻占쎈윥占쎌몗癲ル슣鍮뽳옙占썲뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占� 占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅�뜝�럥�쑜�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걖�뜝�럩�뀇�윜誘쇱눘援�占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슣�돵�뜝�떥�궡留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷③쪛�굛��ｏ옙��뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤빢�삕占쎌맶�뜝�럥�쑅嶺뚮슢�쐣占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷⑨옙�쐻�뜝占�. 
                                        // 0 ~ 18 占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅占쎈쐻占쎈윥占쎌럷�뜝�럡�뀤�뜝�럥�맶�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎈し�뜝�럥�돯占쎈쐻占쎈짗占쎌굲嶺뚮ㅏ�솇占쎌굲占쎈쨨占쎌돸占쎌굲占쎈쨨�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥堉⒴뜝�럩逾겼뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル쉵�눖�졒�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥筌앹뇯�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉�뙴洹⑥삕�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈괘�뜝�럥�맶�뜝�럥�쑅�뜝�럥�늸�뜝�럥�맶�뜝�럥�쑅�뜝�럥�젔�뜝�럥�맶�뜝�럥�쑋嶺뚮씮짹占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥鸚룐뫅�삕占쎌맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌띿뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�맶�뜝�럥堉⒴뜝�럩逾겼뜝�럡�렊�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル쉵�눖�졒�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슢�뒧野껓옙�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�렅�뜝�럥�렠�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�젛�뜝�럥�맶�뜝�럥�쑅�뜝�럥�걞�뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.
                                        // ArtSetStat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빢�삕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎄퐫占쎈쐻占쎈윪�뤃占� 占쎈쐻占쎈윪占쎈�뗰옙�쐻占쎈윞占쎈빟占쎈쐻占쎈윥獒뺤룊�삕占쎌맶�뜝�럥�쑅�뜝�럥�쑜占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슢�뒦占쎌삺�뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�돲占쎌뒭占쎄뎡 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅占쎌젂�뜝�뜴�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑅�뜝�럥�젛占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁���삕占쎌맶�뜝�럥�쑋�뜝�럩�쓥占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占�.


    double defaultDamage = GetDamage(); // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈듋�뜝�럥�뇢�뜝�럥�렍鶯ㅼ룆理놅옙�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥�몴占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈쭦癲ル슢캉占쎄콬�뜝�럥竊뤷뜝�럥�맶�뜝�럥�쑋嶺뚮씭�뵛占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁�λ쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쎗�뜝�룞�삕占쎈닍�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈윫占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪筌륁빆�쐻占쎈윥占쎈읁�뜝�럥�맶�뜝�럥�몧�뜝�럩留뜹뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈쓠�뜝�룞�삕占쎄껀占쎈폁占쎄샴癲ル슪�삕嶺뚮　維�占쎈㎍占쎈쐻占쎈윥占쎈첋�뜝�럩�쟼占쎈눀占쎄턁占쎈ぁ占쎈쐻占쎈윥�댆�뿥�쐻占쎈윪�뤃��먥뵾占쎌뒧占쎈역�뜝�럩援뀐옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�럥夷③쪛�겦維�占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈짗占쎌굲占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥����뜝�럥�뿞�뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥夷ⓨ뜝�룞�삕�솒占쏙옙援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럩議�.
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


void Character::MakeScoreFunctionMainOptionFixed(int main3, int main4, int main5)
{
    std::array<int, 10> mainOp = { 0 }; // It will be checked which main option is activated.
    if (mArtClock->GetMainType() < 10) mainOp[mArtClock->GetMainType()] = 1;
    if (mArtCup->GetMainType() < 10) mainOp[mArtCup->GetMainType()] = 1;
    if (mArtCrown->GetMainType() < 10) mainOp[mArtCrown->GetMainType()] = 1;

    std::array<int, 10> numArray = { 0 }; // It will be recorded in this array how many times each option is added.
    std::array<double, 10> damArray = { 0. }; // It will be recorded in this array how much damage will be if each option is added.

    Character* tempCharacter;
    std::array<Stat, 10> tempSubStatArray; // Flower占쎈퓠筌랃옙 占쎌읅占쎌뒠占쎈쭍 野껉퍔�뵠占쎈뼄.

    // Character�몴占� 10揶쏆뮆占쏙옙 癰귣벊沅쀯옙釉� 占쎈뼄占쎌벉占쎈퓠, 揶쏉옙 Character占쎈퓠野껓옙 �겫占쏙옙�긿占쎌뵠 占쎌읈�겫占� �뜮袁⑸선占쎌뿳占쎈뮉 Artifact�몴占� 餓ο옙占쎈뼄.
    ArtFlower* emptyFlower = new ArtFlower();
    ArtFeather* emptyFeather = new ArtFeather();
    ArtClock* emptyClock = new ArtClock();
    emptyClock->SetMainType(main3);
    ArtCup* emptyCup = new ArtCup();
    emptyCup->SetMainType(main4);
    ArtCrown* emptyCrown = new ArtCrown();
    emptyCrown->SetMainType(main5);

    tempCharacter = this->Clone();
    tempCharacter->SetArtFlower(new ArtFlower(emptyFlower));
    tempCharacter->SetArtFeather(new ArtFeather(emptyFeather));
    tempCharacter->SetArtClock(new ArtClock(emptyClock));
    tempCharacter->SetArtCup(new ArtCup(emptyCup));
    tempCharacter->SetArtCrown(new ArtCrown(emptyCrown));
    
    Stat tempSubStat = Stat();

    tempCharacter->Update();
    mSavedFunction[0] = tempCharacter->GetDamage();

    for (int i = 0; i < 45; i++) // for�눧紐꾩몵嚥∽옙 45占쎌돳占쎈짗占쎈툧, 
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

                // 揶쏉옙占쎌삢 占쎌젎占쎈땾揶쏉옙 占쎈꼥占쏙옙占� 占쎈뮞占쎄틛占쎈퓠 占쏙옙占쏙옙鍮먲옙苑� ((5 - 雅뚯눘�긿占쎈연�겫占�) 癰귣���뼄 占쎌읅野껓옙 筌�袁⑹뜳占쎈뮉揶쏉옙?)�몴占� 占쎌넇占쎌뵥占쎈릭��⑨옙 筌�袁⑹뒲占쎈뼄.
                    // If impossible,
                        // 占쎈뼄占쎌벉 占쎌젎占쎈땾揶쏉옙 占쎈꼥占쏙옙占� 占쎈뮞占쎄틛占쎈퓠 占쏙옙占쏙옙鍮먲옙苑� 占쎌넇占쎌뵥占쎈립占쎈뼄. (筌ㅼ뮆占쏙옙 5占쎌돳 獄쏆꼶�궗)
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



std::string  Character::GetWeaponName() const
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