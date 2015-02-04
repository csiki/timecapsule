
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "HardwareSpeedTester.h"

HardwareSpeedTester::HardwareSpeedTester(size_t maxStepToTest_, nanoseconds maxStepTimeToTest_, nanoseconds sampleThreshold_)
	: maxStepToTest(maxStepToTest_), maxStepTimeToTest(maxStepTimeToTest_), sampleThreshold(sampleThreshold_) {}

ComplexityFunc HardwareSpeedTester::testPuzzleComplexity(Puzzle& puzzle, long double& stdev, DurationSamples samples)
{
	// determine sample threshold if not given previously
	if (sampleThreshold.count() == 0)
	{
		auto sample = puzzle.funcdur(1, seconds(60), nanoseconds(1)); // 60 seconds parameter could be anything that is >0
		sampleThreshold = sample[0].second * 2 + nanoseconds(1);
	}

	// sample calculation times
	if (samples.empty())
		samples = puzzle.funcdur(maxStepToTest, maxStepTimeToTest, sampleThreshold);

	// test & fit all complexity functions on the samples
	long double bestmse = std::numeric_limits<long double>::max();
	stdev = std::numeric_limits<long double>::max();
	auto complexities = puzzle.getPossibleComplexities();
	ComplexityFunc bestfit = nullptr;
	for (auto& cf : complexities)
	{
		long double tmpstdev;
		auto tmpfit = fitComplexity(samples, cf, tmpstdev);
		auto tmpmse = calcMSE(samples, tmpfit);
		if (tmpmse < bestmse)
		{
			bestmse = tmpmse;
			stdev = tmpstdev;
			bestfit = tmpfit;
		}
	}
	return bestfit;
}

ComplexityFunc HardwareSpeedTester::fitComplexity(const DurationSamples& samples, const ComplexityFunc& complexity, long double& stdev)
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
	long double bmean = std::accumulate(biasvals.begin(), biasvals.end(), (long double)0.0);
	long double mmean = std::accumulate(mulvals.begin(), mulvals.end(), (long double)0.0);
	bmean /= biasvals.size(); // bias mean
	mmean /= mulvals.size();  // multiplier mean

	vector<long double> diff(biasvals.size());
	std::transform(biasvals.begin(), biasvals.end(), diff.begin(),
		std::bind2nd(std::minus<long double>(), bmean));
	auto sqsum = std::inner_product(diff.begin(), diff.end(), diff.begin(), (long double)0.0);
	auto bstd = std::sqrt(sqsum / biasvals.size());	// bias standard deviation

	diff.clear(); diff.resize(mulvals.size());
	std::transform(mulvals.begin(), mulvals.end(), diff.begin(),
		std::bind2nd(std::minus<long double>(), mmean));
	sqsum = std::inner_product(diff.begin(), diff.end(), diff.begin(), (long double)0.0);
	auto mstd = std::sqrt(sqsum / mulvals.size());	// multiplier standard deviation

	stdev = (bstd + mstd) / 2.0; // mean of the 2 standard deviations

	// generate most fitting function
	ComplexityFunc fit = [complexity, bmean, mmean] (unsigned long long n) {
		auto res = bmean + mmean * complexity(n + 1); // FIXME +1 because it shifts one somehow ???!!!
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