#include "../header/RandomInitialization.hh"
#include <random>

std::random_device rd;     // Only used once to initialise (seed) engine
std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)	
std::uniform_int_distribution<int> uni5(0, 4); // Guaranteed unbiased
std::uniform_int_distribution<int> uni4(0, 3); // Guaranteed unbiased

int GetRdUni5() { return uni5(rng); }
int GetRdUni4() { return uni4(rng); }
int GetRdUni(int number)
{
    std::uniform_int_distribution<int> uniTemp(0, number - 1); // Guaranteed unbiased
    return uniTemp(rng);
}