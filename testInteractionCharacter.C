#include "top.hh"


using namespace std;

void testConstructor()
{
    cout << "========== testConstructor ==========" << endl;

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar.get()) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar.get()) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar.get()) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar.get()) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar.get()) << endl;
    cout << "artSetSTat is used : " << artSetStat->IsUsingThis(simChar.get()) << endl;
}


bool IsSameArtifactValue(std::shared_ptr<Artifact> art1, std::shared_ptr<Artifact> art2)
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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> tempweapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> tempartinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> tempartinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> tempartinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> tempartinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> tempartinit5 = std::make_shared<ArtCrown>();

    tempartinit1->Generation();
    tempartinit2->Generation();
    tempartinit3->Generation();
    tempartinit3->SetMainType(4);
    tempartinit4->Generation();
    tempartinit4->SetMainType(2);
    tempartinit5->Generation();
    tempartinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> tempartSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> tempweapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> tempartinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> tempartinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> tempartinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> tempartinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> tempartinit5 = std::make_shared<ArtCrown>();

    tempartinit1->Generation();
    tempartinit2->Generation();
    tempartinit3->Generation();
    tempartinit3->SetMainType(4);
    tempartinit4->Generation();
    tempartinit4->SetMainType(2);
    tempartinit5->Generation();
    tempartinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> tempartSetStat = std::make_shared<EmblemOfSeveredFate>();

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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);

    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar.get()) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar.get()) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar.get()) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar.get()) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar.get()) << endl;

    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(simChar.get())) << endl;
    cout << "artinit2 is not used : " << !(artinit2->IsUsingThis(simChar.get())) << endl;
    cout << "artinit3 is not used : " << !(artinit3->IsUsingThis(simChar.get())) << endl;
    cout << "artinit4 is not used : " << !(artinit4->IsUsingThis(simChar.get())) << endl;
    cout << "artinit5 is not used : " << !(artinit5->IsUsingThis(simChar.get())) << endl;
}


void testArtifactSetter()
{
    cout << "========== testArtifactSetter ==========" << endl;

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    // first setter
    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);


    // Check for first setter
    cout << "artinit1 is used : " << artinit1->IsUsingThis(simChar.get()) << endl;
    cout << "artinit2 is used : " << artinit2->IsUsingThis(simChar.get()) << endl;
    cout << "artinit3 is used : " << artinit3->IsUsingThis(simChar.get()) << endl;
    cout << "artinit4 is used : " << artinit4->IsUsingThis(simChar.get()) << endl;
    cout << "artinit5 is used : " << artinit5->IsUsingThis(simChar.get()) << endl;

    std::shared_ptr<ArtFlower> newflower = std::make_shared<ArtFlower>();
    newflower->Generation();

    // second setter
    simChar->SetArtFlower(newflower);


    // Check for second setter
    cout << "artinit1 is not used : " << !(artinit1->IsUsingThis(simChar.get())) << endl;
    cout << "newflower is used : " << newflower->IsUsingThis(simChar.get()) << endl;
}


void testArtSetStatSetter()
{
    cout << "========== testArtSetStatSetter ==========" << endl;

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    // first setter
    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

    Stat resonanceStat = Stat();
    resonanceStat.SetZero();
    resonanceStat.SetAttackPer(25.);
    simChar->SetResonanceStat(resonanceStat);


    // Check for first setter
    cout << "artSetStat is used : " << artSetStat->IsUsingThis(simChar.get()) << endl;

    std::shared_ptr<ArtSetStat> newArtSetStat = std::make_shared<ArtSetStat>();

    // second setter
    simChar->SetArtSetStat(newArtSetStat);


    // Check for second setter
    cout << "artSetStat is not used : " << !(artSetStat->IsUsingThis(simChar.get())) << endl;
    cout << "newArtSetStat is used : " << newArtSetStat->IsUsingThis(simChar.get()) << endl;
}


void testArtifactModification()
{
    cout << "========== testArtifactModification ==========" << endl;

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    // first setter
    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

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

    std::shared_ptr<EngulfingLightning> weapon = std::make_shared<EngulfingLightning>();

	std::shared_ptr<ArtFlower> artinit1 = std::make_shared<ArtFlower>();
	std::shared_ptr<ArtFeather> artinit2 = std::make_shared<ArtFeather>();
	std::shared_ptr<ArtClock> artinit3 = std::make_shared<ArtClock>();
	std::shared_ptr<ArtCup> artinit4 = std::make_shared<ArtCup>();
	std::shared_ptr<ArtCrown> artinit5 = std::make_shared<ArtCrown>();

    artinit1->Generation();
    artinit2->Generation();
    artinit3->Generation();
    artinit3->SetMainType(4);
    artinit4->Generation();
    artinit4->SetMainType(2);
    artinit5->Generation();
    artinit5->SetMainType(0);
	
    std::shared_ptr<EmblemOfSeveredFate> artSetStat = std::make_shared<EmblemOfSeveredFate>();

    // first setter
    std::shared_ptr<Raiden> simChar = std::make_shared<Raiden>(weapon, artSetStat, artinit1, artinit2, artinit3, artinit4, artinit5);

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