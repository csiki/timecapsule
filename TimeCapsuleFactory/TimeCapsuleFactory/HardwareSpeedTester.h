
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_HARDWARESPEEDTESTER_H)
#define _HARDWARESPEEDTESTER_H

#include <chrono>
#include <vector>
#include <functional>
#include <limits>
#include <cmath>
#include <limits>
#include <numeric>
#include "Puzzle.h"

using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::vector;
using std::function;
typedef function<unsigned long long (unsigned long long)> ComplexityFunc;
typedef vector<std::pair<unsigned long long, nanoseconds>> DurationSamples;

class HardwareSpeedTester
{
private:
	ComplexityFunc fitComplexity(const DurationSamples& samples, const ComplexityFunc& complexity, double& stdev);
	bool cramerEquationSolver(long double a11, long double a12, long double a21, long double a22, long double y1, long double y2, long double& x1, long double& x2);

public:
	ComplexityFunc testPuzzleComplexity(Puzzle& puzzle, double& stdev, size_t maxStepToTest = 1000, nanoseconds maxStepTimeToTest = seconds(30));
	unsigned long long estimateStepsNeeded(ComplexityFunc complexity, seconds duration);
};

#endif  //_HARDWARESPEEDTESTER_H
