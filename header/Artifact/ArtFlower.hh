#ifndef ARTFLOWER_HH
#define ARTFLOWER_HH


#include "../Artifact.hh"


class Artifact;


class ArtFlower : public Artifact
{
public:
	ArtFlower()
	{
		mType = 1;
		mProbabilityWeight = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(6);
	}
	ArtFlower(const ArtFlower* artflower) : Artifact(artflower)
	{
		mType = 1;
		mProbabilityWeight = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(6);
	}
	ArtFlower(const ArtFlower& artflower) : Artifact(artflower)
	{
		mType = 1;
		mProbabilityWeight = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(6);
	}
	~ArtFlower() override {}

	static std::vector<int> GetPossibleMainOption() { return {6}; }

};

#endif