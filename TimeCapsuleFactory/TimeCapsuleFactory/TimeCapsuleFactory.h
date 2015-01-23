
//  @ Project : TimeCapsuleFactory
//  @ File Name : TimeCapsuleFactory.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_TIMECAPSULEFACTORY_H)
#define _TIMECAPSULEFACTORY_H

#include <vector>
#include <chrono>
#include <exception>
#include <secblock.h>
#include "Puzzle.h"
#include "Capsule.h"
#include "HardwareSpeedTester.h"
#include "Encryptor.h"
#include "Logger.h"

using std::vector;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using CryptoPP::SecByteBlock;

template <typename DataType = char>
class TimeCapsuleFactory
{
public:

	Capsule<DataType> createTimeCapsule(Puzzle& puzzle, vector<DataType> data, seconds duration)
	{
		if (sizeof(DataType) != sizeof(byte))
			throw std::logic_error("Different size of byte and DataType! DataType is not convertible to byte!");

		// determine operation time of a single step in solving the puzzle
		double stdev;
		HardwareSpeedTester hst;
		auto opcomp = hst.testPuzzleComplexity(puzzle, stdev);
		Logger::log("Best complexity function is fitted with standard deviation: " + std::to_string(stdev));

		// determine number of steps needed to spend the given duration
		auto times = hst.estimateStepsNeeded(opcomp, duration);
		Logger::log("Number of steps needed: " + std::to_string(times));

		return createTimeCapsule(puzzle, data, duration, times);
	}

	Capsule<DataType> createTimeCapsule(Puzzle& puzzle, vector<DataType> data, seconds duration, unsigned long long times)
	{
		if (sizeof(DataType) != sizeof(byte))
			throw std::logic_error("Different size of byte and DataType! DataType is not convertible to byte!");

		// encrypt data
		Encryptor<DataType> enc;
		SecByteBlock key, iv;
		auto cdata = enc.encrypt(data, key, iv);

		// generate puzzle, cypther key
		unsigned long base;
		Integer n;
		auto ckey = puzzle.setup(key, times, base, n);
		Logger::log("Base of the puzzle: " + std::to_string(base));

		return Capsule<DataType>(cdata, ckey, iv, n, times, base);
	}
};

#endif  //_TIMECAPSULEFACTORY_H
