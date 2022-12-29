#ifndef ARTFEATHER_HH
#define ARTFEATHER_HH

#include "../Artifact.hh"

class ArtFeather : public Artifact
{
public:
	ArtFeather() {}
	~ArtFeather() {}

protected:
	int mType = 2;
	int probabiiltyWeight[19] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int cummulatedWeight[19]  = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

private:
	
};

#endif