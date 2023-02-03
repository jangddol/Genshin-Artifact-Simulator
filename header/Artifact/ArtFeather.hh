#ifndef ARTFEATHER_HH
#define ARTFEATHER_HH


#include "../Artifact.hh"


class Artifact;


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
	ArtFeather(const ArtFeather* artfeather) : Artifact(artfeather)
	{
		mType = 2;
		mProbabilityWeight = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		SetMainType(3);
	}
	~ArtFeather() override {}

	static std::vector<int> GetPossibleMainOption() { return {3}; }

};

#endif