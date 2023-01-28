#include "src/Artifact.cc"
#include "src/Stat.cc"
#include "header/Artifact/ArtFlower.hh"
#include "header/Artifact/ArtFeather.hh"
#include "header/Artifact/ArtClock.hh"
#include "header/Artifact/ArtCup.hh"
#include "header/Artifact/ArtCrown.hh"
#include <vector>


using namespace std;


void testArtifact()
{
    gStyle->SetOptStat(kFALSE);
	gRandom->SetSeed(0);
    
    ArtFlower* flower = new ArtFlower();
    
	ArtFlower* flower2 = new ArtFlower(flower);

	vector<int> tempVec = flower->GetCummulatedWeight();
	for (int i = 0; i < tempVec.size(); i++)
	{
		cout << tempVec[i] << endl;
	}
	vector<int> tempVec2 = flower2->GetCummulatedWeight();
	for (int i = 0; i < tempVec.size(); i++)
	{
		cout << tempVec2[i] << endl;
	}
}