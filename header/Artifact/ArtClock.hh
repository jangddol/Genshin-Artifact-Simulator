#ifndef ARTCLOCK_HH
#define ARTCLOCK_HH


#include "../Artifact.hh"


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
	~ArtClock() override {}
};

#endif