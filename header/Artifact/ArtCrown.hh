#ifndef ARTCROWN_HH
#define ARTCROWN_HH


#include "../Artifact.hh"


class ArtCrown : public Artifact
{
public:
	ArtCrown()
	{
		mType = 5;
		mProbabilityWeight
		= { 5,  5, 11,  0,  0, 11,  0,  2, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5 };
		mCummulatedWeight
		= { 5, 10, 21, 21, 21, 32, 32, 34, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 50 };
	}
	ArtCrown(ArtCrown* artcrown) : Artifact(artcrown)
	{
		mType = 5;
		mProbabilityWeight
		= { 5,  5, 11,  0,  0, 11,  0,  2, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5 };
		mCummulatedWeight
		= { 5, 10, 21, 21, 21, 32, 32, 34, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 50 };
	}
	~ArtCrown() {}
};

#endif