#include "top.hh"


void testConstructor()
{
    cout << "========== testConstructor ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar) << endl;
    cout << "artSetSTat is used : " << artSetStat->IsUsingThis(simChar) << endl;
}


bool IsSameArtifactValue(Artifact* art1, Artifact* art2)
{
    Stat mainStat1 = art1->GetMainStat();
    Stat mainStat2 = art2->GetMainStat();
    Stat subStat1 = art1->GetSubStat();
    Stat subStat2 = art2->GetSubStat();

    bool returnBool = true;
    for (int i = 0; i < 35; i++)
    {
        if (mainStat1.GetOption(i) != mainStat2.GetOption(i))
        {
            returnBool = false;
            break;
        }
    }
    for (int i = 0; i < 35; i++)
    {
        if (subStat1.GetOption(i) != subStat2.GetOption(i))
        {
            returnBool = false;
            break;
        }
    }
    return returnBool;
}


void testCopyConstructor()
{
    cout << "========== testCopyConstructor ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden simChar(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar.SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(&simChar) << endl;


    Raiden copiedChar(&simChar);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(&simChar) << endl;

    // artifact is also copied. Therefore the ptr of artifact is not shared at copying.
    cout << "artinit1 is not used at copiedChar : " << !(artinit1->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit2 is not used at copiedChar : " << !(artinit2->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit3 is not used at copiedChar : " << !(artinit3->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit4 is not used at copiedChar : " << !(artinit4->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit5 is not used at copiedChar : " << !(artinit5->IsUsingThis(&copiedChar)) << endl;
    cout << "artSetStat is used at copiedChar : " << artSetStat->IsUsingThis(&copiedChar) << endl;

    // But the value of artifact should be same
    cout << "artinit1 is same with one of copiedChar : " << IsSameArtifactValue(artinit1, copiedChar.GetArtFlower()) << endl;
    cout << "artinit2 is same with one of copiedChar : " << IsSameArtifactValue(artinit2, copiedChar.GetArtFeather()) << endl;
    cout << "artinit3 is same with one of copiedChar : " << IsSameArtifactValue(artinit3, copiedChar.GetArtClock()) << endl;
    cout << "artinit4 is same with one of copiedChar : " << IsSameArtifactValue(artinit4, copiedChar.GetArtCup()) << endl;
    cout << "artinit5 is same with one of copiedChar : " << IsSameArtifactValue(artinit5, copiedChar.GetArtCrown()) << endl;
}


void testMoveConstructor()
{
    cout << "========== testMoveConstructor ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden simChar(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar.SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(&simChar) << endl;

    Raiden movedChar(std::move(simChar));

    // simChar is transferred to R-Value by std::move. Therefore, artinit1 should be not used in simChar.
    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(&simChar)) << endl;
    cout << "artinit2 is not used : " << !(artinit2->IsUsingThis(&simChar)) << endl;
    cout << "artinit3 is not used : " << !(artinit3->IsUsingThis(&simChar)) << endl;
    cout << "artinit4 is not used : " << !(artinit4->IsUsingThis(&simChar)) << endl;
    cout << "artinit5 is not used : " << !(artinit5->IsUsingThis(&simChar)) << endl;
    cout << "artSetStat is not used : " << !(artSetStat->IsUsingThis(&simChar)) << endl;

    // 'move' is not need to copy the artifacts. Therefore, movedChar uses artinits.
    cout << "artinit1 is used at movedChar : " << artinit1->IsUsingThis(&movedChar) << endl;
    cout << "artinit2 is used at movedChar : " << artinit2->IsUsingThis(&movedChar) << endl;
    cout << "artinit3 is used at movedChar : " << artinit3->IsUsingThis(&movedChar) << endl;
    cout << "artinit4 is used at movedChar : " << artinit4->IsUsingThis(&movedChar) << endl;
    cout << "artinit5 is used at movedChar : " << artinit5->IsUsingThis(&movedChar) << endl;
    cout << "artSetStat is used at movedChar : " << artSetStat->IsUsingThis(&movedChar) << endl;

}


void testCopyAssignment()
{
    cout << "========== testCopyAssignment ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden simChar(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar.SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;

    EngulfingLightning* tempweapon = new EngulfingLightning();

	ArtFlower* tempartinit1 = new ArtFlower();
	ArtFeather* tempartinit2 = new ArtFeather();
	ArtClock* tempartinit3 = new ArtClock();
	ArtCup* tempartinit4 = new ArtCup();
	ArtCrown* tempartinit5 = new ArtCrown();

    tempartinit1->Generation();
    tempartinit2->Generation();
    tempartinit3->Generation();
    tempartinit3->SetMainType(4);
    tempartinit4->Generation();
    tempartinit4->SetMainType(2);
    tempartinit5->Generation();
    tempartinit5->SetMainType(0);
	
    EmblemOfSeveredFate* tempartSetStat = new EmblemOfSeveredFate();

    Raiden copiedChar(tempweapon, tempartSetStat, tempartinit1, tempartinit2, tempartinit3, tempartinit4, tempartinit5);
    copiedChar = simChar;

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(&simChar) << endl;

    // artifact is also copied. Therefore the ptr of artifact is not shared at copying.
    cout << "artinit1 is not used at copiedChar : " << !(artinit1->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit2 is not used at copiedChar : " << !(artinit2->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit3 is not used at copiedChar : " << !(artinit3->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit4 is not used at copiedChar : " << !(artinit4->IsUsingThis(&copiedChar)) << endl;
    cout << "artinit5 is not used at copiedChar : " << !(artinit5->IsUsingThis(&copiedChar)) << endl;
    cout << "artSetStat is used at copiedChar : " << artSetStat->IsUsingThis(&copiedChar) << endl;

    // But the value of artifact should be same
    cout << "artinit1 is same with one of copiedChar : " << IsSameArtifactValue(artinit1, copiedChar.GetArtFlower()) << endl;
    cout << "artinit2 is same with one of copiedChar : " << IsSameArtifactValue(artinit2, copiedChar.GetArtFeather()) << endl;
    cout << "artinit3 is same with one of copiedChar : " << IsSameArtifactValue(artinit3, copiedChar.GetArtClock()) << endl;
    cout << "artinit4 is same with one of copiedChar : " << IsSameArtifactValue(artinit4, copiedChar.GetArtCup()) << endl;
    cout << "artinit5 is same with one of copiedChar : " << IsSameArtifactValue(artinit5, copiedChar.GetArtCrown()) << endl;
}


void testMoveAssignment()
{
    cout << "========== testMoveAssignment ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden simChar = Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar.SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(&simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(&simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(&simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(&simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(&simChar) << endl;

    EngulfingLightning* tempweapon = new EngulfingLightning();

	ArtFlower* tempartinit1 = new ArtFlower();
	ArtFeather* tempartinit2 = new ArtFeather();
	ArtClock* tempartinit3 = new ArtClock();
	ArtCup* tempartinit4 = new ArtCup();
	ArtCrown* tempartinit5 = new ArtCrown();

    tempartinit1->Generation();
    tempartinit2->Generation();
    tempartinit3->Generation();
    tempartinit3->SetMainType(4);
    tempartinit4->Generation();
    tempartinit4->SetMainType(2);
    tempartinit5->Generation();
    tempartinit5->SetMainType(0);
	
    EmblemOfSeveredFate* tempartSetStat = new EmblemOfSeveredFate();

    Raiden movedChar(tempweapon, tempartSetStat, tempartinit1, tempartinit2, tempartinit3, tempartinit4, tempartinit5);
    movedChar = std::move(simChar);

    // simChar is transferred to R-Value by std::move. Therefore, artinit1 should be not used in simChar.
    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(&simChar)) << endl;
    cout << "artinit2 is not used : " << !(artinit2->IsUsingThis(&simChar)) << endl;
    cout << "artinit3 is not used : " << !(artinit3->IsUsingThis(&simChar)) << endl;
    cout << "artinit4 is not used : " << !(artinit4->IsUsingThis(&simChar)) << endl;
    cout << "artinit5 is not used : " << !(artinit5->IsUsingThis(&simChar)) << endl;
    cout << "artSetStat is not used : " << !(artSetStat->IsUsingThis(&simChar)) << endl;

    // 'move' is not need to copy the artifacts. Therefore, movedChar uses artinits.
    cout << "artinit1 is used at movedChar : " << artinit1->IsUsingThis(&movedChar) << endl;
    cout << "artinit2 is used at movedChar : " << artinit2->IsUsingThis(&movedChar) << endl;
    cout << "artinit3 is used at movedChar : " << artinit3->IsUsingThis(&movedChar) << endl;
    cout << "artinit4 is used at movedChar : " << artinit4->IsUsingThis(&movedChar) << endl;
    cout << "artinit5 is used at movedChar : " << artinit5->IsUsingThis(&movedChar) << endl;
    cout << "artSetStat is used at movedChar : " << artSetStat->IsUsingThis(&movedChar) << endl;
}


void testDestructor()
{
    cout << "========== testDestructor ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar) << endl;
    delete simChar;
    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(simChar)) << endl;
    cout << "artinit2 is not used : " << !(artinit2->IsUsingThis(simChar)) << endl;
    cout << "artinit3 is not used : " << !(artinit3->IsUsingThis(simChar)) << endl;
    cout << "artinit4 is not used : " << !(artinit4->IsUsingThis(simChar)) << endl;
    cout << "artinit5 is not used : " << !(artinit5->IsUsingThis(simChar)) << endl;
}


void testArtifactSetter()
{
    cout << "========== testArtifactSetter ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    // first setter
    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);


    // Check for first setter
    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar) << endl;

    ArtFlower* newflower = new ArtFlower();
    newflower->Generation();

    // second setter
    simChar->SetArtFlower(newflower);


    // Check for second setter
    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(simChar)) << endl;
    cout << "newflower is used : " << newflower->IsUsingThis(simChar) << endl;
}


void testArtSetStatSetter()
{
    cout << "========== testArtSetStatSetter ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    // first setter
    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);


    // Check for first setter
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(simChar) << endl;

    ArtSetStat* newArtSetStat = new ArtSetStat();

    // second setter
    simChar->SetArtSetStat(newArtSetStat);


    // Check for second setter
    cout << "artSetStat is not used : " << !(artSetStat->IsUsingThis(simChar)) << endl;
    cout << "newArtSetStat is used : " << newArtSetStat->IsUsingThis(simChar) << endl;
}


void testArtifactModification()
{
    cout << "========== testArtifactModification ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    // first setter
    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "updateState before Update is 0 : " << (0 == simChar->GetUpdateState()) << endl;

    simChar->Update();

    cout << "updateState after Update is 5 : " << (5 == simChar->GetUpdateState()) << endl;

    artinit1->SetSubStat(Stat());

    cout << "updateState after artifact modification is 3 : " << (3 == simChar->GetUpdateState()) << endl;
}


void testArtSetStatModification()
{
    cout << "========== testArtSetStatModification ==========" << endl;

    EngulfingLightning* weapon = new EngulfingLightning();

	ArtFlower* artinit1 = new ArtFlower();
	ArtFeather* artinit2 = new ArtFeather();
	ArtClock* artinit3 = new ArtClock();
	ArtCup* artinit4 = new ArtCup();
	ArtCrown* artinit5 = new ArtCrown();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    EmblemOfSeveredFate* artSetStat = new EmblemOfSeveredFate();

    // first setter
    Raiden* simChar = new Raiden(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "updateState before Update is 0 : " << (0 == simChar->GetUpdateState()) << endl;

    simChar->Update();

    cout << "updateState after Update is 5 : " << (5 == simChar->GetUpdateState()) << endl;

    artSetStat->SetOption(0, 0);

    cout << "updateState after artSetStat modification is 2 : " << (2 == simChar->GetUpdateState()) << endl;
}


void testInteractionCharacter()
{
    testConstructor();
    testCopyConstructor();
    testMoveConstructor();
    testCopyAssignment();
    testMoveAssignment();
    testDestructor();
    testArtifactSetter();
    testArtSetStatSetter();
    testArtifactModification();
    testArtSetStatModification();
}