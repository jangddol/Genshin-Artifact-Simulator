#ifndef RANDOMINITIALIZATION_HH
#define RANDOMINITIALIZATION_HH

#include <random>

std::random_device rd;     // Only used once to initialise (seed) engine
std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)	
std::uniform_int_distribution<int> uni(0, 4); // Guaranteed unbiased
std::uniform_int_distribution<int> uni4(0, 3); // Guaranteed unbiased

#endif