
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "HardwareSpeedTester.h"

ComplexityFunc HardwareSpeedTester::testPuzzleComplexity(Puzzle& puzzle, double& var, unsigned int maxStepToTest, nanoseconds maxStepTimeToTest)
{
	// test all complexity functions
	auto complexities = puzzle.getPossibleComplexities();
	for (auto cf : complexities)
	{
		// sample till maxStepTimeToTest or maxStepToTest is reached

	}

	return ComplexityFunc(nullptr);
}

unsigned long long estimateStepsNeeded(ComplexityFunc complexity, seconds duration)
{
	return 0;
}

ComplexityFunc HardwareSpeedTester::fitComplexity(Puzzle& puzzle, ComplexityFunc complexity, double& var)
{
	return ComplexityFunc(nullptr);
}
