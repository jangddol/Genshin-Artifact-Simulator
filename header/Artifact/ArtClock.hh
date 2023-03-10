#ifndef ARTCLOCK_HH
#define ARTCLOCK_HH


#include "../Artifact.hh"


class Artifact;


class ArtClock : public Artifact
{
public:
	ArtClock()
	{
		mType = 3;
		mProbabilityWeight
			= { 0, 0, 8, 0,  3,  8,  0,  3,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
		mCummulatedWeight
			= { 0, 0, 8, 8, 11, 19, 19, 22, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 };
	}
	ArtClock(const ArtClock* artclock) : Artifact(artclock)
	{
		mType = 3;
		mProbabilityWeight
			= { 0, 0, 8, 0,  3,  8,  0,  3,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
		mCummulatedWeight
			= { 0, 0, 8, 8, 11, 19, 19, 22, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 };
	}
	ArtClock(const ArtClock& artclock) : Artifact(artclock)
	{
		mType = 3;
		mProbabilityWeight
			= { 0, 0, 8, 0,  3,  8,  0,  3,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
		mCummulatedWeight
			= { 0, 0, 8, 8, 11, 19, 19, 22, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 };
	}
	~ArtClock() override {}

	static std::vector<int> GetPossibleMainOption() { return {2, 4, 5, 7, 8}; }
};

#endif