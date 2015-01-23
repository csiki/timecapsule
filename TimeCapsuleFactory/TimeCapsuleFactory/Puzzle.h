
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_PUZZLE_H)
#define _PUZZLE_H

#include <chrono>
#include <secblock.h>
#include <integer.h>
#include <functional>
#include <vector>

using std::chrono::nanoseconds;
using CryptoPP::SecByteBlock;
using CryptoPP::Integer;
using std::vector;
using std::function;
typedef function<unsigned long long (unsigned long long)> ComplexityFunc;

class Puzzle
{
private:
	virtual void initComplexities();

protected:
	unsigned long base;
	vector<ComplexityFunc> possibleComplexities;

public:
	Puzzle() : base(2) { initComplexities(); }
	Puzzle(unsigned long base_) : base(base_) { initComplexities(); }
	vector<ComplexityFunc> getPossibleComplexities() { return possibleComplexities; }
	unsigned long getBase() { return base; }

	virtual nanoseconds funcdur(unsigned long long times);
	virtual SecByteBlock setup(const SecByteBlock& key, unsigned long long times, unsigned long& base, Integer& n);
	virtual SecByteBlock solve(const SecByteBlock& ckey, unsigned long long times, unsigned long base, const Integer& n);
};

SecByteBlock cryptoInt2SBB(const Integer& integer);

#endif  //_PUZZLE_H
