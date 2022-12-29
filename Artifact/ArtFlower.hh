#ifndef ARTFLOWER_HH
#define ARTFLOWER_HH

#include "../Artifact.hh"

class ArtFlower : public Artifact
{
public:
	ArtFlower() {}
	~ArtFlower() {}

protected:
	int mType = 1;
	int probabiiltyWeight[19] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int cummulatedWeight[19]  = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

private:
	
};

#endif