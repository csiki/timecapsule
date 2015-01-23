
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "HardwareSpeedTester.h"

ComplexityFunc HardwareSpeedTester::testPuzzleComplexity(Puzzle& puzzle, double& stdev, size_t maxStepToTest, nanoseconds maxStepTimeToTest)
{
	// sample calculation times
	auto samples = puzzle.funcdur(maxStepToTest, maxStepTimeToTest);

	// test & fit all complexity functions on the samples
	double tmpstdev;
	stdev = std::numeric_limits<double>::max();
	auto complexities = puzzle.getPossibleComplexities();
	ComplexityFunc bestfit = nullptr;
	for (auto cf : complexities)
	{
		auto tmpfit = fitComplexity(samples, cf, tmpstdev);
		if (tmpstdev < stdev)
		{
			stdev = tmpstdev;
			bestfit = tmpfit;
		}
	}
	return bestfit;
}

ComplexityFunc HardwareSpeedTester::fitComplexity(const DurationSamples& samples, const ComplexityFunc& complexity, double& stdev)
{
	// take every sample pairs
	vector<long double> biasvals(samples.size() * (samples.size() - 1));
	vector<long double> mulvals(samples.size() * (samples.size() - 1));
	for (auto i = samples.begin(); i != samples.end(); ++i)
	{
		for (auto j = i + 1; j != samples.end(); ++j)
		{
			// calculate bias and multiplier for the complexity function
			long double tmpbias, tmpmul;
			if (cramerEquationSolver(1, complexity(i->first), 1, complexity(j->first),
				i->second.count(), j->second.count(), tmpbias, tmpmul))
			{
				biasvals.push_back(tmpbias);
				mulvals.push_back(tmpmul);
			}
		}
	}

	// calculate mean and standard deviation
	auto bmean = std::accumulate(biasvals.begin(), biasvals.end(), 0);
	auto mmean = std::accumulate(mulvals.begin(), mulvals.end(), 0);
	bmean /= biasvals.size(); // bias mean
	mmean /= mulvals.size();  // multiplier mean

	vector<long double> diff(biasvals.size());
	std::transform(biasvals.begin(), biasvals.end(), diff.begin(),
		std::bind2nd(std::minus<long double>(), bmean));
	auto sqsum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
	auto bstd = std::sqrt(sqsum / biasvals.size());	// bias standard deviation

	diff.clear(); diff.resize(mulvals.size());
	std::transform(mulvals.begin(), mulvals.end(), diff.begin(),
		std::bind2nd(std::minus<long double>(), mmean));
	sqsum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
	auto mstd = std::sqrt(sqsum / mulvals.size());	// multiplier standard deviation

	stdev = (bstd + mstd) / 2.0; // mean of the 2 standard deviations

	// generate most fitting function
	ComplexityFunc fit = [complexity, bmean, mmean] (unsigned long long n) { // FIXME TODO is this alright with the ComplexityFunc type?
		return bmean + mmean * complexity(n);
	};

	return fit;
}

unsigned long long HardwareSpeedTester::estimateStepsNeeded(ComplexityFunc complexity, seconds duration)
{
	return 0;
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