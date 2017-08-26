#pragma once
#ifndef _RANDOM_TESTER_H
#define _RANDOM_TESTER_H 1

#include <map>
#include <vector>
#include <iostream>

#include "Random.h"
#include "AbstractTester.hpp"

class RandomTester : public AbstractTester
{
public:
	RandomTester(const std::string& name = "Random (class)", std::ostream& out = std::cout) :
		AbstractTester{ name, out }
	{
	}

	virtual ~RandomTester() {}

	//
	// Run tests and report
	//
	virtual void runAll()
	{
		_os << _name << +"::Initiating Tests." << std::endl;

		_errorCountMap["nextBoolean"] = testNextBoolean();
		_errorCountMap["nextBytes"] = testNextBytes();
		_errorCountMap["nextDouble"] = testNextDouble();
		_errorCountMap["nextFloat"] = testNextFloat();
		_errorCountMap["nextGaussian"] = testNextGaussian();
		_errorCountMap["nextInt"] = testNextInt();
		_errorCountMap["nextInt(int)"] = testNextIntInt();
		_errorCountMap["nextInt(low, high)"] = testNextIntLowHigh();
		_errorCountMap["nextLong"] = testNextLong();
		_errorCountMap["setSeed"] = testSetSeed();
	}

protected:
	int testNextBoolean()
	{
		_os << "  " << "Test nextBoolean()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 100000;
		Random rng;
		unsigned countTrue = 0;
		unsigned countFalse = 0;
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (rng.nextBoolean()) countTrue++;
			else countFalse++;
		}

		_os << "  " << "Test nextBoolean()..." << countTrue
			<< " + " << countFalse << " = " << ITERATIONS;

		// 1% margin of error
		if (!epsilonEquals(countTrue, ITERATIONS / 2, (unsigned)(0.01 * ITERATIONS)))
		{
			emitError("nextBoolean() true Percentage problem ", (int)countTrue, (int)(ITERATIONS / 2));
			errors++;
		}

		// 1% margin of error
		if (!epsilonEquals(countFalse, ITERATIONS / 2, (unsigned)(0.01 * ITERATIONS)))
		{
			emitError("nextBoolean() false Percentage problem ", (int)countFalse, (int)(ITERATIONS / 2));
			errors++;
		}

		_os << " done." << std::endl;

		return errors;
	}

	int testNextBytes()
	{
		_os << "  " << "Test nextBytes()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned SIZE = 1000;
		Random rng;

		const unsigned ITERATIONS = 10000;
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			//
			// Generate counts
			//
			char ary[SIZE] = { 0 };
			rng.nextBytes(ary, SIZE);

			int sequenceCount = 0;
			for (unsigned i = 1; i < SIZE; i++)
			{
				if (ary[i - 1] == ary[i]) sequenceCount++;
				else sequenceCount = 0;

				if (sequenceCount > 2)
				{
					emitError("nextBytes():: unlikely sequence of bytes occurred [" + std::to_string(sequenceCount) + "] of them.");
					errors++;
				}
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextDouble()
	{
		_os << "  " << "Test nextDouble()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 20000000;
		const unsigned BUCKETS = 1000;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			int value = (unsigned)(rng.nextDouble() * BUCKETS);
			if (value >= 0.0 && value < BUCKETS)
			{
				counts[value]++;
			}
		}

		//
		// Check counts: 3% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 3% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.03 * ITERATIONS / BUCKETS)))
			{
				emitError("nextDouble() Percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextFloat()
	{
		_os << "  " << "Test nextFloat()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 20000000;
		const unsigned BUCKETS = 500;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			int value = (unsigned)(rng.nextFloat() * BUCKETS);
			if (value >= 0.0 && value < BUCKETS)
			{
				counts[value]++;
			}
		}

		//
		// Check counts: 3% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 3% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.03 * ITERATIONS / BUCKETS)))
			{
				emitError("nextDouble() Percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextGaussian()
	{
		_os << "  " << "Test nextGaussian()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 20000000;
		const unsigned BUCKETS = 8;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			double number = rng.nextGaussian() + BUCKETS / 2;

			if (number >= 0.0 && number <= BUCKETS)
			{
				counts[int(number)]++;
			}
		}

		std::cout << " counting done...";

		//
		// Check counts: 5% MOE
		//
		double EXPECTED_PERCENS[BUCKETS] = { 0.0013, 0.0215, 0.1359, 0.3413, 0.3413, 0.1359, 0.0215, 0.0013 };
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			if (b % (ITERATIONS / 10) == 0) std::cout << ".";

			// 5% margin of error
			if (!epsilonEquals(counts[b], (int)ITERATIONS * EXPECTED_PERCENS[b], (unsigned)(0.05 * ITERATIONS * EXPECTED_PERCENS[b])))
			{
				emitError("nextGaussian() Percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS * EXPECTED_PERCENS[b]));
				errors++;
			}
		}

		std::cout << " expectation...";

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextInt()
	{
		_os << "  " << "Test nextInt()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 10000000;
		const unsigned BUCKETS = 500;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			counts[(unsigned)(rng.nextInt() % BUCKETS)]++;
		}

		//
		// Check counts: 4% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 4% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.04 * ITERATIONS / BUCKETS)))
			{
				emitError("nextInt() Percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextIntInt()
	{
		_os << "  " << "Test nextInt(int)...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 10000000;
		const unsigned BUCKETS = 500;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			unsigned value = rng.nextInt(BUCKETS);

			if (value >= BUCKETS)
			{
				emitError("nextInt(int) value outside expected interval [" + std::to_string(value) + "]");
				errors++;
			}

			counts[value]++;
		}

		//
		// Check counts: 4% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 4% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.04 * ITERATIONS / BUCKETS)))
			{
				emitError("nextInt(int) percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextIntLowHigh()
	{
		_os << "  " << "Test nextInt(low, high)...";

		int errors = 0;

		//
		// Verify values are within the expected interval
		//
		Random rng;
		const unsigned ITERATIONS = 10000000;
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			//
			// Negatives
			//
			int value = rng.nextInt(-20, -5);
			if (value < -20 || value > -5)
			{
				emitError("nextInt(low, high) value outside expected interval [-20, -5]: [" + std::to_string(value) + "]");
				errors++;
			}

			//
			// Negatives and Positives
			//
			value = rng.nextInt(-20, 15);
			if (value < -20 || value > 15)
			{
				emitError("nextInt(low, high) value outside expected interval [-20, 15]: [" + std::to_string(value) + "]");
				errors++;
			}

			//
			// Positives
			//
			value = rng.nextInt(5, 10);
			if (value < 5 || value > 100)
			{
				emitError("nextInt(low, high) value outside expected interval [5, 100]: [" + std::to_string(value) + "]");
				errors++;
			}
		}

		_os << "(a) done...";

		//
		// Verify the spread of values
		//
		const unsigned LOW = 100;
		const unsigned HIGH = 400;
		const unsigned BUCKETS = HIGH - LOW + 1;

		//
		// Generate counts
		//
		unsigned counts[HIGH - LOW + 1] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			int value = rng.nextInt(LOW, HIGH);

			if (value < LOW || value > HIGH)
			{
				emitError("nextInt(low, high) value outside expected interval [" + std::to_string(LOW) + ", " + std::to_string(HIGH) + "]" + std::to_string(value) + "]");
				errors++;
			}

			counts[value - LOW]++;
		}

		_os << "(b) done...";

		//
		// Check counts: 3% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 3% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.03 * ITERATIONS / BUCKETS)))
			{
				emitError("nextInt(low, high) percentage problem [" + std::to_string(b + LOW) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testNextLong()
	{
		_os << "  " << "Test nextLong()...";

		int errors = 0;

		//
		// Construct several distributions; check probabilities
		//
		const unsigned ITERATIONS = 10000000;
		const unsigned BUCKETS = 500;
		Random rng;

		//
		// Generate counts
		//
		unsigned counts[BUCKETS] = { 0 };
		for (unsigned i = 0; i < ITERATIONS; i++)
		{
			if (i % (ITERATIONS / 10) == 0) std::cout << ".";

			counts[(unsigned)(rng.nextLong() % BUCKETS)]++;
		}

		//
		// Check counts: 4% MOE
		//
		for (unsigned b = 0; b < BUCKETS; b++)
		{
			// 4% margin of error
			if (!epsilonEquals(counts[b], ITERATIONS / BUCKETS, (unsigned)(0.04 * ITERATIONS / BUCKETS)))
			{
				emitError("nextInt() Percentage problem [" + std::to_string(b) + "]", (int)counts[b], (int)(ITERATIONS / BUCKETS));
				errors++;
			}
		}

		_os << " done." << std::endl;

		return errors;
	}

	//
	// 1) Allocate 1000 counting buckets;
	// 2) Generate random numbers
	// 3) Multiply the random number by 1000 and add to the bucket (tracking counts)
	// 4) Verify
	//
	int testSetSeed()
	{
		_os << "  " << "Test setSeed()...";

		int errors = 0;

		const int START_SEED = 5;
		Random rng{ START_SEED };

		//
		// Generate a sequence of values to verify acquiring after resetting the seed.
		//
		const int NUM_VALUES = 10;
		int expected[NUM_VALUES] = { 0 };
		for (unsigned i = 0; i < NUM_VALUES; i++)
		{
			expected[i] = rng.nextInt();
		}

		//
		// (1) Generate a random number of random numbers
		// (2) Stop, reset the seed. Generate X number of values for comparison
		// (3) Loop
		//
		const int TESTS = 100;
		for (int test = 0; test < TESTS; test++)
		{
			if (test && test % (TESTS / 5) == 0) std::cout << ".";

			// (1) Generate a random number of random numbers
			unsigned loops = rng.nextInt() % 1000;
			for (unsigned ell = 0; ell < loops; ell++)
			{
				rng.nextInt();
			}

			//
			// (2) Stop, reset the seed. Generate X number of values for comparison
			//
			rng.setSeed(START_SEED);
			for (unsigned i = 0; i < NUM_VALUES; i++)
			{
				int acquired = rng.nextInt();
				if (expected[i] != acquired)
				{
					emitError("setSeed() ", acquired, expected[i]);
					errors++;
				}
			}
		}

		_os << " done." << std::endl;

		return errors;
	}
};
#endif
