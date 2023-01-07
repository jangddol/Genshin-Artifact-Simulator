#include "top.hh"
#include <vector>


using namespace std;


int UseCummulatedWeight(vector<int> cummulatedWeight)
{
	// generate random integer from 0 to the sum of probability table
	int length = cummulatedWeight.size();
	int tempInt = gRandom->Integer(cummulatedWeight[length - 1]) + 1;

	int selectedInt = 0;
	int beforeElement = 0;
	int nowElement = 0;
	for (int i = 0; i < length; i++) // for문을 돌리면서 대소비교를 한다.
	{
		nowElement = cummulatedWeight[i];
		if (tempInt > beforeElement && tempInt <= nowElement)
		{
			selectedInt = i; // 랜덤 int를 가지고 어느 주옵인지 결정한다.
			break;
		}
		beforeElement = nowElement;
	}
	return selectedInt;
}


vector<int> GenerateStartOpt(vector<int> cummulatedWeight)
{
	vector<int> returnList(4);
	returnList[0] = UseCummulatedWeight(cummulatedWeight);
    FIRSTOPT.push_back(returnList[0]);
	for (int i = 1; i < 4; i++)
	{
		int temp = UseCummulatedWeight(cummulatedWeight);
		while (CheckIsThereIn(temp, returnList))
		{
			temp = UseCummulatedWeight(cummulatedWeight);
		}
		returnList[i] = temp;
	}
	return returnList;
}


void testArtifact()
{
    gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);
    
    ArtFlower* flower = new ArtFlower();
    ArtFeather* feather = new ArtFeather();
    ArtClock* clock = new ArtClock();
    ArtCup* cup = new ArtCup();
    ArtCrown* crown = new ArtCrown();

    flower->Generation();
    feather->Generation();
    clock->Generation();
    cup->Generation();
    crown->Generation();

    TH1D* hMainType = new TH1D("MainType", "", 19, -0.5, 18.5);
    TH1D* hSubStat = new TH1D("SubStat", "", 10, -0.5, 9.5);

    int hN = 10000;
    for (int i = 0; i < hN; i++)
    {
        flower->Generation();
        int MainType = flower->GetMainType();
        hMainType->Fill((double)MainType);
        for (int j = 0; j < 10; j++)
        {
            if (flower->GetSubStat().GetOption(j) > 0)
            {
                hSubStat->Fill(j);
            }
        }
    }


    TCanvas* can1 = new TCanvas("c1", "c1", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

    hMainType->SetDirectory(0);
    hMainType->Draw();

    TCanvas* can2 = new TCanvas("c2", "c2", 1200, 800);
	gPad->SetLeftMargin(0.12);
	gPad->SetBottomMargin(0.12);
	gPad->SetRightMargin(0.08);
	gPad->SetTopMargin(0.05);

    hSubStat->SetDirectory(0);
    hSubStat->Draw();
}