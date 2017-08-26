#include <iostream>
#include <chrono>
#include <random>
#include "Random.h"

// Generates a random boolean
bool Random::nextBoolean() const
{
	// generates a random integer
	int value = _uniformIntDistribution(_generator);

	// Turns the integer into a boolean. If the integer generated is even (thus value % 2 = 0) the function returns true, otherwise, false.
	if (value % 2 == 0)
	{
		return true;
	}
	
	return false;
}

void Random::nextBytes(char bytes[], int numBytes) const
{
	std::uniform_int_distribution<int> distribution(0, 255);
	_generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
	
	for (int i = 0; i < numBytes; i++)
	{
		bytes[i] = distribution(_generator);
	}
}

// Generates a random double
double Random::nextDouble() const
{
	// declares a uniform real distribution of type double with bounds of 0.0 and 1.0
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	return distribution(_generator);
}

// Generates a random floating point value between 0.0 and 1.0
float Random::nextFloat() const
{
	// declares a uniform real distribution of type double with bounds of 0.0 and 1.0
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	// if float not returned, put <double>, then typecast result to float.

	return distribution(_generator);
}

// Returns a random double w/in a Normal distrubution with mean 0.0 and std dev 1.0
double Random::nextGaussian() const
{
	// declares a normal / gaussian distribution with mean 0.0 and std dev 1.0
	std::normal_distribution<double> distribution (0.0,1.0);

	return distribution(_generator);
}

// Generates a random integer
int	Random::nextInt() const
{
	// assigns a random integer to the identifier "randomInt"
	int randomInt = _uniformIntDistribution(_generator);

	return randomInt;
}

// Generates a random integer between 0 (inclusive) and a given interger (exclusive)
int	Random::nextInt(int n) const
{
	// declares a uniform int distribution between 0 inclusive and n exclusive.
	// The values given in parentheses are both inclsuive, so to make n exclsuive, 1 is subtracted
	std::uniform_int_distribution<int> distribution(0, n-1);

	return distribution(_generator);
}

// Generates a random integer between the bounds given
int	Random::nextInt(int low, int high) const
{
	// declares a unform int distribution between low (inclsuvie) and high (inclusive).
	std::uniform_int_distribution<int> distribution(low, high);

	return distribution(_generator);
}

// Generates a random value of type long
long Random::nextLong() const
{
	// defines a uniform integer distribution of type long
	std::uniform_int_distribution<long> distribution;

	return distribution(_generator);
}

void Random::setSeed(long seed)
{
	// Sets the seed for the random value generator with whateever was passed to the function.
	_generator.seed(seed);
}
