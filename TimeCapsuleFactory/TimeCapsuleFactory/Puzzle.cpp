
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Puzzle.h"

#include <iostream> // TODO

Puzzle::Puzzle()
{
	initComplexities();
	
	// rand base
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long> basedis(2, 10);
	base = basedis(gen);
}

Puzzle::Puzzle(unsigned long base_) : base(base_)
{
	initComplexities();
}

void Puzzle::initComplexities()
{
	ComplexityFunc fexp = [] (unsigned long long n) {
		return std::exp(n) + .5;
	};
	ComplexityFunc f2exp = [] (unsigned long long n) {
		return std::pow(2, n) + .5;
	};
	ComplexityFunc fnexp = [] (unsigned long long n) {
		return n * std::exp(n) + .5;
	};
	ComplexityFunc fnnexp = [] (unsigned long long n) {
		return n * n * std::exp(n) + .5;
	};
	possibleComplexities.push_back(fexp);
	possibleComplexities.push_back(f2exp);
	possibleComplexities.push_back(fnexp);
	possibleComplexities.push_back(fnnexp);
}

DurationSamples Puzzle::funcdur(size_t maxStepToTest, nanoseconds maxStepTimeToTest, nanoseconds sampleThreshold)
{
	assert(maxStepTimeToTest > sampleThreshold);

	// sample till maxStepTimeToTest or maxStepToTest is reached
	unsigned long long i = 1;
	nanoseconds steptime(0), acc(0);
	Integer num = base;
	DurationSamples samples;

	std::cout << "ACTUAL SAMPLING: " << std::endl; // TODO
	while (i < maxStepToTest && steptime < maxStepTimeToTest)
	{
		auto start = high_resolution_clock::now();
		num *= num; // squaring
		auto end = high_resolution_clock::now();
		
		steptime = end - start;
		acc += steptime;
		if (steptime > sampleThreshold)
		{
			samples.push_back(std::make_pair(i, acc));
			std::cout << i << ": " << std::chrono::duration_cast<std::chrono::seconds>(acc).count() << std::endl; // TODO
		}
		++i;
	}

	return samples;
}

// according to http://www.hashcash.org/papers/time-lock.pdf
Integer Puzzle::setup(const SecByteBlock& key, unsigned long long times, Integer& n)
{
	// rand delta (1 or -1)
	std::random_device drd;
	std::mt19937 gen(drd());
	std::uniform_int_distribution<> yndis(0, 1);
	auto delta = yndis(gen);
	delta = (delta == 0) ? -1 : 1;

	// generate primes
	AutoSeededRandomPool rnd;
	PrimeAndGenerator pgen(delta, rnd, 1024, 1023);
	auto p = pgen.Prime();
	auto q = pgen.SubPrime();

	// generate composite modulus & fi(n)
	n = p * q;
	auto fin = (p - 1) * (q - 1);

	// encrypt key
	auto e = Integer::Power2(times) % fin;
	auto b = CryptoPP::ModularExponentiation(Integer(base), e, n);
	auto ckey = (cryptoSBB2Int(key) + b) % n;

	return ckey; // return key as integer so no further Integer-SecByteBlock conversion is needed here, neither at solve
}

// should be optimal as possible
SecByteBlock Puzzle::solve(const Integer& ckey, unsigned long long times, const Integer& n)
{
	Integer a(base);
	while (times--) a *= a;
	auto b = a % n;

	return cryptoInt2SBB(ckey - b);
}

SecByteBlock cryptoInt2SBB(const Integer& integer)
{
	byte* bytearr = new byte[integer.ByteCount()];
	for (int i = integer.ByteCount() - 1; i >= 0; --i)
		bytearr[i] = integer.GetByte(integer.ByteCount() - 1 - i);
	
	return SecByteBlock(bytearr, integer.ByteCount());
}

inline Integer cryptoSBB2Int(const SecByteBlock& sbb)
{
	return Integer(sbb.data(), sbb.size(), CryptoPP::Integer::UNSIGNED);
}