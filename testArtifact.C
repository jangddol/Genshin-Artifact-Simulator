#include "top.hh"


void testArtifact()
{
    ArtFlower flower = ArtFlower();
    ArtFeather feather = ArtFeather();
    ArtClock clock = ArtClock();
    ArtCup cup = ArtCup();
    ArtCrown crown = ArtCrown();

    flower.Generation();
    feather.Generation();
    clock.Generation();
    cup.Generation();
    crown.Generation();

    PrintArtifact(flower);
    PrintArtifact(feather);
    PrintArtifact(clock);
    PrintArtifact(cup);
    PrintArtifact(crown);
}