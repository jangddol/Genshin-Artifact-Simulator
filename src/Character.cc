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

	// ��⑤벉梨� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // �눧�떯由� 雅뚯눘�긿 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// �눧�떯由� �겫占쏙옙�긿 : 0 ~ 18 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// �눧�떯由� �겫占썽겫占쏙옙�긿 : 0 ~ 23, 27, 28 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
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

	// 占쎄쉐占쎌���눧占� 占쎄쉭占쎈뱜 : 0 ~ 23, 27, 28 // 占쎌끏占쎌읈占쎌뿳 獄쏄퀣�젫占쎈쭆 野껉퍔占쏙옙 占쎈툡占쎈뻷
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
    ArtSetStat* tempArtSetStat; // �뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎄굴占쎈쐻占쎈윥�댚占쏙옙�뜏�뜝�룞�삕�뤃�쉫�쇀占쎈탟占쎌굲�뜝�럡�렊占쎈쐻占쎈윞占쎈쑆�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈��占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윥�뜝�럥踰앾옙�굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럥�렓占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쐾占쎈쇊�뜝�뜴�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쐾嶺뚮엪�삕 �뜝�럥�맶�뜝�럥�쑅�뜝�럥猷억옙�쐻占쎈윞�뙼占쏙옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅嶺뚋우삕 ResonanceStat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝癲ル슢�뀖獄��씛�삕占쎈츐占쎈쐻占쎈윥占쎌넅占쎈쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�렓�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럩�쟼占쎈쐻�뜝占�.
                                        // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�룞�삕占쎈폏吏밧뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦룂�굲, 占쎈쐻占쎈윪占쎈㎡占쎈쎗占쎈즴筌�猷〓쨨占쎈���맱癒れ넂占쏙옙怨멸뎡 Stat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�룞�삕占쎈폏吏밧뜝�럩留띰옙�쐻占쎈윥占쎌몝�뜝�룞�삕占쎈리�뙼�봿留띰옙�쐻占쎈윥占쎌맽占쎈쐻占쎈윥壤쏉옙 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪占쎌젲占쎈쐻占쎈윪占쎈㎡占쎈쎗�뜝�뜴�쐻占쎈짗占쎌굲�뜝�럥鍮℡뜝�럥�맶�뜝�럥�쑅�뜝�럡�꺓 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윪雅뚣굝�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슢�뿪占쎌굲.
                                        // Update占쎈쐻占쎈윥鸚롤껊쐻占쎈윥筌앸ŀ�쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�썒占쏙옙援앾옙占싸우삕占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윞占쎈쑓占쎌녇熬곥룊占쎌���삕泳��봺異��뜝�룞�삕占쎈탿�뜝�럩援꿨뜝�럡愿뚳옙�눀�뜝�뜴�젂占쎈쳟占쎌굲占쎈쐻占쎈윥�씙�뜝�럡�렊 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�렓占쎈쐻占쎈짗占쎈돗�뜝�럩�뮝�뜝�럥琉덌옙�쐻占쎈윪�뤃轅곗녇占쎄틓占쎈뮛�뜝�럥�뿭占쎈쐻占쎈윪�뤃占�, 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럩�쟼�슖�댙�삕嶺뚮슢�꺏占쎌굲占쎈쇀熬곎딆굲雅��굞�돸占쎌굲�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�몷占쎈퉲�뜝�뜴�쐻占쎈윪占쎈쨮占쎈쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�걫�뜝�럡萸띶뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈탶��ⓦ끉�굲.
                                    // 230131
                                        // ResonanceStat 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅⑤쐻占쎈윥占쏙옙占썲뜝�럥�맶占쎈쐻�뜝占� Update Optimization �뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥�댚�렱�쐻占쎈윥占쎈묄�뜝�럥�맶�뜝�럥�쑋�뜝�럩議귨옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뤃寃쇰쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥壤쏉옙
                                        // 占쎌뒙占쎈뼔揶쏉옙占쎈쐻占쎈뼢占쎄텭塋딅쵓�삕占쎌맶�뜝�럥�쐾嶺뚮씮�닲占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥�댚�엺�쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈닖占쎈쐻占쎈윞占쏙옙占썲뜝�럥�걤�뛾�룄�돲占쎌굲�뜝�럥�룒占쎌젂�뙼蹂앸읁�슖�셽�궡留뜹뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜝�떬�씮�굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌μ렱�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈즼占쎌녇占쎄틓占쎈뮝�뜝�럥琉덌옙�쐻占쎈윪�뤃占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�걫�뜝�럡萸띶뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐪筌먦룂�굲 占쎈쐻占쎈윥占쎈듋�뜝�럥�뇢�솻洹⑥삕�뜝�럡�렊 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�걫�뜝�럥�냷�뜝�럩援뀐옙�쐻占쎈윥占쎈폆�솾�꺂�뒧占쎈�뺝뜝�럥爾ο옙�쐻占쎈윥�뤃占�
                                        // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�룞�삕占쎈섣占쎌굲 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗癲ル슢�뵰占쎈떜�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥�눧袁��쐻占쎈윥鸚롤껊쐻占쎈윥筌앸ŀ�쐻占쎈윪�뤃占� 占쎌녇占쎄틓占쎈뮛�뜝�럥�럵�뜝�럥�몡�솻洹μ삌占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃轅곗녇占쎄틓占쎈뮝�뜝�럥琉덌옙�쐻占쎈윪�뤃占�. 
                                        // 0 ~ 18 占쎌녇占쎄틓占쎈뮛�뜝�럥占쎈뛽�궔�뜝�뜴�쐻占쎈쑟�빊占쏙옙紐댐옙援욑옙援� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜝�떬�씮�굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌μ렱�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎈즼占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦룂�굲 占쎈쐻占쎈윥占쎈듋�뜝�럥�뇢�뜝�럥�렑�뜝�럩留졾뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽占쎈쐻占쎈윥�뜝�떬�씮�굲�뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윥筌μ렱�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥筌뚮겭�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�맱�뼃�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�렓�뜝�럥�몧�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럩�쟼占쎈쐻�뜝占�.
                                        // ArtSetStat占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝�뜝�룞�삕占쎈섣占쎌굲 占쎌녇占쎄틓占쎈뮛�뜝�럥�럵占쎈쐻占쎈윥筌뚭랬�삕占쎌맶�뜝�럥�쑅嶺뚋우삕 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥堉�占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥�렓占쎈쐻占쎈짗占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗�뜝�럩�쟼占쎈쐻�뜝占�.


    double defaultDamage = GetDamage(); // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈눇�뙼�맧彛묈뜝�럩留띰옙�쐻占쎈윥占쎌몝占쎈쐻占쎈윞占쎈륵 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐪筌먦끉�룯�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윪獄��뇯�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쎗�뜝�뜴�쐻�뜝占� 占쎈쐻占쎈윥占쏙옙占썼��遺얘턁筌�硫⑤쐻占쎈쑆�젆釉낅쑏占쎈꺋占쎌굲嶺뚮엪�삕占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윥占쏙옙占쏙옙�쐻占쎈윥�댆�뿥�쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�젵.
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
    std::array<Stat, 10> tempSubStatArray; // Flower에만 적용될 것이다.

    // Character를 10개를 복사한 다음에, 각 Character에게 부옵이 전부 비어있는 Artifact를 준다.
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