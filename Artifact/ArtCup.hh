#ifndef ARTCUP_HH
#define ARTCUP_HH

#include "../Artifact.hh"

class ArtCup : public Artifact
{
public:
	ArtCup() {}
	~ArtCup() {}

protected:
	int mType = 4;
	int probabiiltyWeight[19]
		= { 0, 0, 23,  0,  0, 23,  0,  3, 23,  0,  6,  6,  6,  6,   6,   6,   6,   6, 0 };
	int cummulatedWeight[19]
		= { 0, 0, 23, 23, 23, 46, 46, 49, 72, 72, 78, 84, 90, 96, 102, 108, 114, 120, 0 };

private:
	
};

#endif