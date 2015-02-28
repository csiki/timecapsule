
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "HardwareSpeedTester.h"

#include <iostream> // TODO rm

HardwareSpeedTester::HardwareSpeedTester(size_t maxStepToTest_, nanoseconds maxTimeToTest_, nanoseconds sampleThreshold_)
	: maxStepToTest(maxStepToTest_), maxTimeToTest(maxTimeToTest_), sampleThreshold(sampleThreshold_) {}

ComplexityFunc HardwareSpeedTester::testPuzzleComplexity(Puzzle& puzzle, long double& mse, DurationSamples samples)
{
	// determine sample threshold if not given previously
	if (sampleThreshold.count() == 0)
	{
		auto sample = puzzle.funcdur(1, seconds(60), nanoseconds(1)); // 60 seconds parameter could be anything that is >0
		sampleThreshold = sample[0].second;//nanoseconds(1);
	}

	// sample calculation times
	if (samples.empty())
		samples = puzzle.funcdur(maxStepToTest, maxTimeToTest, sampleThreshold);
	Logger::log("Number of samples acquired: " + std::to_string(samples.size()));

	// test & fit all complexity functions on the samples
	long double bestmse = std::numeric_limits<long double>::max();
	auto complexities = puzzle.getPossibleComplexities();
	ComplexityFunc bestfit = nullptr;
	for (auto& cf : complexities)
	{
		auto tmpfit = fitComplexity(samples, cf);
		auto tmpmse = calcMSE(samples, tmpfit);

		if (tmpmse < bestmse)
		{
			bestmse = tmpmse;
			bestfit = tmpfit;
		}
	}
	mse = bestmse;
	
	return bestfit;
}

ComplexityFunc HardwareSpeedTester::fitComplexity(const DurationSamples& samples, const ComplexityFunc& complexity)
{
	auto n = samples.size();
	unsigned long long innerProduct = 0;
	unsigned long long sumX = 0;
	unsigned long long sumX2 = 0;
	unsigned long long sumY = 0;
	
	for (auto& sample : samples)
	{
		sumX += complexity(sample.first);
		sumX2 += complexity(sample.first) * complexity(sample.first);
		sumY += sample.second.count();
		innerProduct += complexity(sample.first) * sample.second.count();
	}

	// linear regression on given complexity function: y = m * cf(x) + b
	auto m = (n * innerProduct - sumX * sumY) / (n * sumX2 - sumX * sumX);
	auto b = (sumY - m * sumX) / n;

	// generate fitting function
	ComplexityFunc fit = [complexity, b, m] (unsigned long long n) {
		auto res = b + m * complexity(n); // FIXME +1 because it shifts one somehow ???!!!
		if (res < 0) return (unsigned long long)0;
		return (unsigned long long)res;
	};

	return fit;
}

unsigned long long HardwareSpeedTester::estimateStepsNeeded(const ComplexityFunc& complexity, seconds duration, seconds& err)
{
	unsigned long long t;
	nanoseconds currdur(0);
	for (t = 0; currdur < duration && currdur.count() >= 0; ++t)
		currdur = nanoseconds(complexity(t + 1));

	if (currdur.count() < 0)
		throw std::overflow_error("Duration is too long, cannot handle in nanoseconds (not fit in long long converted to nanoseconds)!");

	err = std::chrono::duration_cast<seconds>(currdur - duration);
	return t;
}

bool HardwareSpeedTester::cramerEquationSolver(long double a11, long double a12, long double a21, long double a22, long double y1, long double y2, long double& x1, long double& x2)
{
	long double determinant = a11 * a22 - a12 * a21;
	if (std::abs(determinant) > std::numeric_limits<long double>::epsilon())
	{
		x1 = (y1 * a22 - a12 * y2) / determinant;
		x2 = (a11 * y2 - a21 * y1) / determinant;
		return true;
	}
	return false;
}

long double HardwareSpeedTester::calcMSE(const DurationSamples& samples, const ComplexityFunc& fit)
{
	unsigned long long mse = 0;
	for (auto& sample : samples)
	{
		auto diff = (fit(sample.first) - sample.second.count());
		mse += diff * diff;
	}
	return mse / samples.size();
}