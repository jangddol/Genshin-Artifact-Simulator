#ifndef ARTCUP_HH
#define ARTCUP_HH


#include "../Artifact.hh"


class Artifact;


class ArtCup : public Artifact
{
public:
	ArtCup()
	{
		mType = 4;
		mProbabilityWeight
		= { 0, 0, 23,  0,  0, 23,  0,  3, 23,  0,  6,  6,  6,  6,   6,   6,   6,   6,   0 };
		mCummulatedWeight
		= { 0, 0, 23, 23, 23, 46, 46, 49, 72, 72, 78, 84, 90, 96, 102, 108, 114, 120, 120 };
	}
	ArtCup(const ArtCup* artcup) : Artifact(artcup)
	{
		mType = 4;
		mProbabilityWeight
		= { 0, 0, 23,  0,  0, 23,  0,  3, 23,  0,  6,  6,  6,  6,   6,   6,   6,   6,   0 };
		mCummulatedWeight
		= { 0, 0, 23, 23, 23, 46, 46, 49, 72, 72, 78, 84, 90, 96, 102, 108, 114, 120, 120 };
	}
	ArtCup(const ArtCup& artcup) : Artifact(artcup)
	{
		mType = 4;
		mProbabilityWeight
			= { 0, 0, 23,  0,  0, 23,  0,  3, 23,  0,  6,  6,  6,  6,   6,   6,   6,   6,   0 };
		mCummulatedWeight
			= { 0, 0, 23, 23, 23, 46, 46, 49, 72, 72, 78, 84, 90, 96, 102, 108, 114, 120, 120 };
	}
	~ArtCup() override {}

	static std::vector<int> GetPossibleMainOption() { return {2, 5, 7, 8, 10, 11, 12, 13, 14, 15, 16, 17}; }

};

#endif