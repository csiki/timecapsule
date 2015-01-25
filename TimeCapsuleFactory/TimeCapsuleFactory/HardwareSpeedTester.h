
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
#include <exception>
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
	ComplexityFunc fitComplexity(const DurationSamples& samples, const ComplexityFunc& complexity, long double& stdev);
	// TODO maxStepToTest, sampleThreshold, maxStepTimeToTest attributumokat ! + constructort hozzájuk, igy testPuzzleComplexity parameterek nem kellenek

public:
	ComplexityFunc testPuzzleComplexity(Puzzle& puzzle, long double& stdev, size_t maxStepToTest = 1000, nanoseconds maxStepTimeToTest = seconds(10), nanoseconds sampleThreshold = nanoseconds(1000), DurationSamples samples = DurationSamples());
	unsigned long long estimateStepsNeeded(const ComplexityFunc& complexity, seconds duration, seconds& err);
	static long double calcMSE(const DurationSamples& samples, const ComplexityFunc& fit);
	static bool cramerEquationSolver(long double a11, long double a12, long double a21, long double a22, long double y1, long double y2, long double& x1, long double& x2);
};

#endif  //_HARDWARESPEEDTESTER_H
