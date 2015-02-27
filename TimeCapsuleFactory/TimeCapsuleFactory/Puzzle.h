
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_PUZZLE_H)
#define _PUZZLE_H

#include <chrono>
#include <random>
#include <functional>
#include <vector>
#include <cmath>
#include <secblock.h>
#include <integer.h>
#include <dh2.h>
#include <nbtheory.h>
#include <cryptlib.h>
#include <osrng.h>

using std::chrono::nanoseconds;
using CryptoPP::SecByteBlock;
using CryptoPP::Integer;
using CryptoPP::PrimeAndGenerator;
using CryptoPP::AutoSeededRandomPool;
using std::vector;
using std::function;
using std::chrono::high_resolution_clock;
typedef function<unsigned long long (unsigned long long)> ComplexityFunc; 
typedef vector<std::pair<unsigned long long, nanoseconds>> DurationSamples;

class Puzzle
{
private:
	virtual void initComplexities();

protected:
	unsigned long base;
	vector<ComplexityFunc> possibleComplexities;

public:
	Puzzle();
	Puzzle(unsigned long base_);
	vector<ComplexityFunc> getPossibleComplexities() { return possibleComplexities; }
	unsigned long getBase() { return base; }

	virtual DurationSamples funcdur(size_t maxStepToTest, nanoseconds maxTimeToTest, nanoseconds sampleThreshold);
	virtual Integer setup(const SecByteBlock& key, unsigned long long times, Integer& n);
	virtual SecByteBlock solve(const Integer& ckey, unsigned long long times, const Integer& n);
};

SecByteBlock cryptoInt2SBB(const Integer& integer);
inline Integer cryptoSBB2Int(const SecByteBlock& sbb);

#endif  //_PUZZLE_H
