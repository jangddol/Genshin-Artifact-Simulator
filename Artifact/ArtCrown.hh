#ifndef ARTCROWN_HH
#define ARTCROWN_HH

#include "../Artifact.hh"

class ArtCrown : public Artifact
{
public:
	ArtCrown() {}
	~ArtCrown() {}

protected:
	int mType = 5;
	int probabiiltyWeight[19]
		= { 5,  5, 11,  0,  0, 11,  0,  2, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5 };
	int cummulatedWeight[19]
		= { 5, 10, 21, 21, 21, 32, 32, 34, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 50 };

private:
	
};

#endif