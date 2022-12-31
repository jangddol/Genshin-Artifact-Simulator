#ifndef ARTFLOWER_HH
#define ARTFLOWER_HH

#include "../Artifact.hh"

class ArtFlower : public Artifact
{
public:
	ArtFlower()
	{
		mType = 1;
		mProbabiltyWeight = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		mCummulatedWeight = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	}
	~ArtFlower() {}
};

#endif