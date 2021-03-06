
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
private:
	HardwareSpeedTester speedTester;

public:
	TimeCapsuleFactory(HardwareSpeedTester speedTester_ = HardwareSpeedTester()) : speedTester(speedTester_) {}
	Capsule<DataType> createTimeCapsule(Puzzle& puzzle, vector<DataType> data, seconds duration)
	{
		if (sizeof(DataType) != sizeof(byte))
			throw std::logic_error("Different size of byte and DataType! DataType is not convertible to byte!");

		// determine operation time of a single step in solving the puzzle
		long double mse;
		auto opcomp = speedTester.testPuzzleComplexity(puzzle, mse);
		Logger::log("Best complexity function is fitted with a mean squared error (nanosec magnitude): " + std::to_string(mse));

		// determine number of steps needed to spend the given duration
		seconds err;
		auto times = speedTester.estimateStepsNeeded(opcomp, duration, err);
		Logger::log("Number of steps needed: " + std::to_string(times)
			+ "; estimated error: " + std::to_string(err.count()) + " seconds");

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
		Integer n;
		auto base = puzzle.getBase();
		auto ckey = puzzle.setup(key, times, n);
		Logger::log("Base of the puzzle: " + std::to_string(base));

		return Capsule<DataType>(cdata, ckey, iv, n, times, base);
	}
};

#endif  //_TIMECAPSULEFACTORY_H
