
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_HARDWARESPEEDTESTER_H)
#define _HARDWARESPEEDTESTER_H

#include <chrono>
#include <vector>
#include <functional>
#include "Puzzle.h"

using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::vector;
using std::function;
typedef function<unsigned long long (unsigned long long)> ComplexityFunc;

class HardwareSpeedTester
{
private:
	vector<ComplexityFunc> complexities;
	ComplexityFunc fitComplexity(Puzzle& puzzle, ComplexityFunc complexity, double& var);

public:
	HardwareSpeedTester(); // TODO implement
	ComplexityFunc testPuzzleComplexity(Puzzle& puzzle, double& var, unsigned int maxStepToTest = 1000, nanoseconds maxStepTimeToTest = seconds(30));
	unsigned long long estimateStepsNeeded(ComplexityFunc complexity, seconds duration);
};

#endif  //_HARDWARESPEEDTESTER_H
