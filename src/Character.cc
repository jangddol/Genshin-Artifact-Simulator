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
	// 嶺뚳옙�솒占쏙옙遊쇔뜝�럡�댉 �뜝�럩湲욕뜝�럥占쏙옙 : 0 ~ 34, b0 ~ b2
    mStatAfterUpdateFromCharacterResonance.SetZero();
    for (int i = 0; i < 35; i++)
    {
        mStatAfterUpdateFromCharacterResonance.AddOption(i, mCharacterStat.GetOption(i));
    }
    for (int i = 0; i < 3; i++)
    {
        mStatAfterUpdateFromCharacterResonance.SetBaseOption(i, mCharacterStat.GetBaseOption(i));
    }

	// 占쏙옙�뫀踰됵㎖占� : 0, 2, 5, 7, 10 ~ 17, 27
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

    // 占쎈닱占쎈뼬�뵳占� �썒�슣�닔占쎄맙 : b0
    mStatAfterUpdateFromWeapon.SetBaseOption(0, mStatAfterUpdateFromWeapon.GetBaseOption(0) + WeaponMainStat.GetBaseOption(0));

	// 占쎈닱占쎈뼬�뵳占� 占쎄껀�뜝�룞�삕占쎄맙 : 0 ~ 18 // �뜝�럩�걦�뜝�럩�쓧�뜝�럩肉� �뛾�룄��ｏ옙�젷�뜝�럥彛� �뇦猿됲뜑�뜝�룞�삕 �뜝�럥�닡�뜝�럥六�
    for (int i = 0; i < 19; i++)
    {
        mStatAfterUpdateFromWeapon.AddOption(i, WeaponSubStat.GetOption(i));
    }

	// 占쎈닱占쎈뼬�뵳占� 占쎄껀�뜝�띂寃ュ뜝�룞�삕占쎄맙 : 0 ~ 23, 27, 28 // �뜝�럩�걦�뜝�럩�쓧�뜝�럩肉� �뛾�룄��ｏ옙�젷�뜝�럥彛� �뇦猿됲뜑�뜝�룞�삕 �뜝�럥�닡�뜝�럥六�
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

	// �뜝�럡�뎽�뜝�럩占쏙옙占쎈닱�뜝占� �뜝�럡�돪�뜝�럥諭� : 0 ~ 23, 27, 28 // �뜝�럩�걦�뜝�럩�쓧�뜝�럩肉� �뛾�룄��ｏ옙�젷�뜝�럥彛� �뇦猿됲뜑�뜝�룞�삕 �뜝�럥�닡�뜝�럥六�
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
    
    // �뜝�럡�뎽�뜝�럩占쏙옙占쎈닱�뜝占� �썒�슣�닔占쎄맙 : 0 ~ 8, 10 ~ 18
    
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
    
    // �뜝�럡�뎽�뜝�럩占쏙옙占쎈닱�뜝占� 占쎄껀�뜝�룞�삕占쎄맙 : 0 ~ 9
    
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
    ArtSetStat* tempArtSetStat; // 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럡援닷뜝�럥�맶�뜝�럥�쑅占쎈뙕�뜝�룞�삕占쎈쐪占쎈쐻占쎈짗占쎌굲占쎈쨨占쎌돧占쎌���뜝�럥�깱�뜝�럩援뀐옙�쐻占쎈윞占쎈젇�뜝�럥�맶�뜝�럥�쐾�뜝�럥�몘占쎈쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥占쏙옙�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈윥甕곗빢�삕占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗�뜝�럥�맶占쎈쐻�뜝占� 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌맽�뜝�럥�뇢占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽癲ル슢�뿪占쎌굲 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윥�뙴�뼲�삕占쎌맶�뜝�럥�쐾占쎈쇊�뜝�룞�삕占쎌맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル쉵�슦�굲 ResonanceStat�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앯솾�꺂�뒧占쎈�뽫뛾占쏙옙�뵛占쎌굲�뜝�럥痢먨뜝�럥�맶�뜝�럥�쑅�뜝�럩�꼨�뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�.
                                        // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥�룒筌욌갊�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ猷귨옙援�, �뜝�럥�맶�뜝�럥�쑋�뜝�럥�렊�뜝�럥�럸�뜝�럥利당춯占썹뙴��볦Ŀ�뜝�럥占쏙옙占쎈㎟�솒�굦�꼥�뜝�룞�삕��⑤㈇�렊 Stat�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥�룒筌욌갊�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥由э옙�쇊占쎈늉筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑋�뜝�럩�졁�뜝�럥�맶�뜝�럥�쑋�뜝�럥�렊�뜝�럥�럸占쎈쐻占쎈쑕占쎌맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윥�뜮�꽒�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗占쎈쐻占쎈윞占쎄틩 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�썒�슔援앾옙�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧占쎈역�뜝�럩援�.
                                        // Update�뜝�럥�맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�뜏�뜝�룞�삕�뤃�빢�삕�뜝�떥�슦�굲�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럥�몥�뜝�럩�뀋�넭怨λ즸�뜝�럩占쏙옙占쎌굲力놂옙占쎈뉴�빊占쏙옙�쐻占쎈짗占쎌굲�뜝�럥�꺙占쎈쐻占쎈윪�뤃轅⑤쐻占쎈윞�꽴�슪�삕占쎈��占쎈쐻占쎈쑕占쎌쟼�뜝�럥爾잌뜝�럩援꿨뜝�럥�맶�뜝�럥�쑅占쎌뵗占쎈쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗�뜝�럥�맶�뜝�럥吏쀥뜝�럥�룛占쎈쐻占쎈윪占쎈츧占쎈쐻占쎈윥筌뚮뜉�삕占쎌맶�뜝�럥�쑋占쎈쨨饔낃퀣�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈열�뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占�, �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆占쎌뒙占쎈뙔占쎌굲癲ル슢�뒧占쎄틣�뜝�럩援꿨뜝�럥����넭怨롫봿援꿴썒占쏙옙援욑옙�뤈�뜝�럩援뀐옙�쐻占쎈윥�뜝�룞�삕占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎈し�뜝�럥�돯占쎈쐻占쎈쑕占쎌맶�뜝�럥�쑋�뜝�럥夷��뜝�럥�맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎄괴占쎈쐻占쎈윞�맱�씢�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�꺐占쏙옙�벀�걠占쎄뎡.
                                    // 230131
                                        // ResonanceStat �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�룞�삕�뜝�뜴�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� Update Optimization 占쎈쐻占쎈윥占쎈㎍占쎈쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뙕占쎈젿占쎌맶�뜝�럥�쑅�뜝�럥臾꾬옙�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몝占쎈쐻占쎈윪鈺곌랬�삕占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈쨨野껋눖�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅鶯ㅼ룊�삕
                                        // �뜝�럩�뮋�뜝�럥堉붹뤆�룊�삕�뜝�럥�맶�뜝�럥堉℡뜝�럡�뀷櫻뗫봾理볩옙�굲�뜝�럩留띰옙�쐻占쎈윥占쎌맽癲ル슢�뵰占쎈떜�뜝�럩留띰옙�쐻占쎈윥筌욎�λ쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅占쎈뙕占쎌뿺占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�떀�뜝�럥�맶�뜝�럥�쐾�뜝�룞�삕�뜝�뜴�쐻占쎈윥占쎄광占쎈쎗占쎈즲占쎈뤁�뜝�럩援뀐옙�쐻占쎈윥占쎈짂�뜝�럩�쟼占쎈쇊癰귥빖�쓠占쎌뒙占쎌끋占쎄땀筌띾쑚�쐻占쎈윥占쎈㎍�뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈뼩占쎌뵰占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋쇰젿占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥利쇔뜝�럩�뀋�뜝�럡�땽�뜝�럥裕앾옙�쐻占쎈윥筌뚮뜉�삕占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎄괴占쎈쐻占쎈윞�맱�씢�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ猷귨옙援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎌녃域밟뫁�굲占쎈쐻占쎈윞占쎈젇 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎄괴占쎈쐻占쎈윥占쎈꺑占쎈쐻占쎈윪�뤃��먯삕占쎌맶�뜝�럥�쑅�뜝�럥�룇占쎌녇占쎄틓占쎈뮛�뜝�럥占쎈틶�쐻占쎈윥�댚恝�삕占쎌맶�뜝�럥�쑅占쎈쨨�뜝占�
                                        // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥�꽔�뜝�럩援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀧솾�꺂�뒧占쎈뎔�뜝�럥�뼔占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈닱熬곻옙占쎌맶�뜝�럥�쑅勇싲·猿딆맶�뜝�럥�쑅嶺뚯빖��占쎌맶�뜝�럥�쑋占쎈쨨�뜝占� �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윻占쎈쐻占쎈윥占쎈ぁ占쎌녃域민쇱굦�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅嶺뚯쉸占싸살맶�뜝�럥�쑋占쎈쨨饔낃퀣�뀋�뜝�럡�땽�뜝�럥裕앾옙�쐻占쎈윥筌뚮뜉�삕占쎌맶�뜝�럥�쑋占쎈쨨�뜝占�. 
                                        // 0 ~ 18 �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥�뜝�럥�쎖占쎄텛占쎈쐻占쎈쑕占쎌맶�뜝�럥�몷占쎈퉲�뜝�룞�삕筌뤿뙋�삕�뤃�쉻�삕�뤃占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈뼩占쎌뵰占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋쇰젿占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅�뜝�럥利쇔뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ猷귨옙援� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�뱥占쎈쐻占쎈윥占쎈눁占쎈쐻占쎈윥占쎈젒占쎈쐻占쎈윪筌띿±�쐻占쎈윥占쎈㎍占쎈쐻占쎈윥占쎌몗�뜝�럥夷⑨옙�쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩留썲뜝�럥�맶�뜝�럥�쑅占쎈쐻占쎈뼩占쎌뵰占쎄뎡占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쑅嶺뚋쇰젿占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅嶺뚮슢寃�占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑅占쎈㎟占쎈펱占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗占쎈쐻占쎈윥占쎈ぇ占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�.
                                        // ArtSetStat�뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앾옙�쐻占쎈짗占쎌굲�뜝�럥�꽔�뜝�럩援� �뜝�럩�뀋�뜝�럡�땽�뜝�럥裕쏉옙�쐻占쎈윥占쎈윻�뜝�럥�맶�뜝�럥�쑅嶺뚮슡�옱占쎌굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗癲ル쉵�슦�굲 �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥�젆占썲뜝�럥�맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윥占쎈젗�뜝�럥�맶�뜝�럥吏쀥뜝�럩援뀐옙�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀯옙�쐻占쎈윪占쎌읆�뜝�럥�맶占쎈쐻�뜝占�.


    double defaultDamage = GetDamage(); // �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�늾占쎈쇊占쎈㎣壤쏅쵆�쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�맶�뜝�럥�쐾�뜝�럥瑜� �뜝�럥�맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맚嶺뚮Ĳ�걠占쎈／占쎈쐻占쎈윪筌띾씛�삕占쎌맶�뜝�럥�쑅�뜝�럩紐쀥뜝�럥�맶�뜝�럥�쑋�뛾占쏙옙�눓占쎌맶�뜝�럥�쑅�뜝�럥�럪�뜝�럥�맶�뜝�럥�쑅�뜝�럩紐앭뜝�럥�럸占쎈쐻占쎈쑕占쎌맶占쎈쐻�뜝占� �뜝�럥�맶�뜝�럥�쑅�뜝�룞�삕�뜝�띁占쏙옙�겫�뼐�꼤嶺뚳옙筌롡뫀�맶�뜝�럥�몘占쎌젂�뇡�굝�몡�뜝�럥爰뗥뜝�럩援뀐┼�슢�뿪占쎌굲�뜝�럥�맶�뜝�럥�쑋占쎈쨨饔끸뫀�맶�뜝�럥�쑅�뜝�룞�삕�뜝�룞�삕占쎌맶�뜝�럥�쑅占쎈뙀占쎈엠占쎌맶�뜝�럥�쑋占쎈쨨占쏙옙癒��굲�뜝�럩留띰옙�쐻占쎈윥占쎌몗占쎈쐻占쎈윥占쎌졄.
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
    std::array<Stat, 10> tempSubStatArray; // Flower�뿉留� �쟻�슜�맆 寃껋씠�떎.

    // Character瑜� 10媛쒕�� 蹂듭궗�븳 �떎�쓬�뿉, 媛� Character�뿉寃� 遺��샃�씠 �쟾遺� 鍮꾩뼱�엳�뒗 Artifact瑜� 以��떎.
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

    for (int i = 0; i < 45; i++) // for臾몄쑝濡� 45�쉶�룞�븞, 
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

                // 媛��옣 �젏�닔媛� �넂��� �뒪�꺈�뿉 ����빐�꽌 ((5 - 二쇱샃�뿬遺�) 蹂대떎 �쟻寃� 梨꾩썱�뒗媛�?)瑜� �솗�씤�븯怨� 梨꾩슫�떎.
                    // If impossible,
                        // �떎�쓬 �젏�닔媛� �넂��� �뒪�꺈�뿉 ����빐�꽌 �솗�씤�븳�떎. (理쒕�� 5�쉶 諛섎났)
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