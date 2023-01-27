#ifndef ARTFEATHER_HH
#define ARTFEATHER_HH


#include "../Artifact.hh"


class ArtFeather : public Artifact
{
public:
	ArtFeather()
	{
		mType = 2;
		mProbabilityWeight = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(3);
	}
	ArtFeather(ArtFeather* artfeather) : Artifact(artfeather)
	{
		mType = 2;
		mProbabilityWeight = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(3);
	}
	~ArtFeather() override {}
};

#endif