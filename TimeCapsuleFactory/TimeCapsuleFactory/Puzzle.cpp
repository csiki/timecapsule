
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Puzzle.h"

void Puzzle::initComplexities()
{
	ComplexityFunc fnlog = [] (unsigned long long n) {
		return n * std::log(n) + .5;
	};
	ComplexityFunc fn2 = [] (unsigned long long n) {
		return n * n;
	};
	ComplexityFunc fexp = [] (unsigned long long n) {
		return std::exp(n) + .5;
	};
	ComplexityFunc f2exp = [] (unsigned long long n) {
		return std::pow(2, n) + .5;
	};
	possibleComplexities.resize(4);
	possibleComplexities.push_back(fnlog);
	possibleComplexities.push_back(fn2);
	possibleComplexities.push_back(fexp);
	possibleComplexities.push_back(f2exp);
}

DurationSamples Puzzle::funcdur(size_t maxStepToTest, nanoseconds maxStepTimeToTest)
{
	// sample till maxStepTimeToTest or maxStepToTest is reached
	unsigned long long i = 0;
	nanoseconds steptime(0);
	Integer num = base;
	DurationSamples samples(maxStepToTest);
	while (i < maxStepToTest && steptime < maxStepTimeToTest)
	{
		auto start = high_resolution_clock::now();
		num *= num; // squaring
		auto end = high_resolution_clock::now();
		
		steptime = end - start;
		samples.push_back(std::make_pair(i, steptime));
		++i;
	}

	return samples;
}

SecByteBlock Puzzle::setup(const SecByteBlock& key, unsigned long long times, unsigned long& base, Integer& n)
{
	return SecByteBlock(nullptr, 0);
}

SecByteBlock Puzzle::solve(const SecByteBlock& ckey, unsigned long long times, unsigned long base, const Integer& n)
{
	return SecByteBlock(nullptr, 0);
}

SecByteBlock cryptoInt2SBB(const Integer& integer)
{
	byte* bytearr = new byte[integer.ByteCount()];
	for (unsigned int i = 0; i < integer.ByteCount(); ++i)
	{
		bytearr[i] = integer.GetByte(i);
	}
	SecByteBlock sbb(bytearr, integer.ByteCount());
	return sbb;
}