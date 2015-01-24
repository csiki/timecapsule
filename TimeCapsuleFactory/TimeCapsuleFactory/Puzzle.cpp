
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Puzzle.h"

void Puzzle::initComplexities()
{
	ComplexityFunc fnlog = [] (unsigned long long n) {
		return n * std::log(n) + .5;
	};
	ComplexityFunc fn2 = [] (unsigned long long n) {
		return n * n;
	};
	ComplexityFunc fexp = [] (unsigned long long n) {
		return std::exp(n) + .5;
	};
	ComplexityFunc f2exp = [] (unsigned long long n) {
		return std::pow(2, n) + .5;
	};
	possibleComplexities.resize(4);
	possibleComplexities.push_back(fnlog);
	possibleComplexities.push_back(fn2);
	possibleComplexities.push_back(fexp);
	possibleComplexities.push_back(f2exp);
}

DurationSamples Puzzle::funcdur(size_t maxStepToTest, nanoseconds maxStepTimeToTest)
{
	// sample till maxStepTimeToTest or maxStepToTest is reached
	unsigned long long i = 0;
	nanoseconds steptime(0);
	Integer num = base;
	DurationSamples samples(maxStepToTest);
	while (i < maxStepToTest && steptime < maxStepTimeToTest)
	{
		auto start = high_resolution_clock::now();
		num *= num; // squaring
		auto end = high_resolution_clock::now();
		
		steptime = end - start;
		samples.push_back(std::make_pair(i, steptime));
		++i;
	}

	return samples;
}

// according to http://www.hashcash.org/papers/time-lock.pdf
Integer Puzzle::setup(const SecByteBlock& key, unsigned long long times, unsigned long& base, Integer& n)
{
	// rand delta (1 or -1)
	std::random_device drd;
	std::mt19937 gen(drd());
	std::uniform_int_distribution<> yndis(0, 1);
	auto delta = yndis(gen);
	delta = (delta == 0) ? -1 : 1;

	// generate primes
	AutoSeededRandomPool rnd;
	PrimeAndGenerator pgen(delta, rnd, 1024, 1024);
	auto p = pgen.Prime();
	auto q = pgen.SubPrime();

	// generate composite modulus & fi(n)
	n = p * q;
	auto fin = (p - 1) * (q - 1);
	
	// pick base
	std::uniform_int_distribution<unsigned long> basedis(2, 10);
	base = basedis(gen);

	// encrypt key
	auto e = Integer::Power2(times) % fin;
	auto b = CryptoPP::ModularExponentiation(Integer(base), e, n);
	auto ckey = cryptoSBB2Int(key) + b; // TODO + is append

	return ckey; // return key as integer so no further Integer-SecByteBlock conversion is needed here, neither at solve
}

// should be optimal as possible
SecByteBlock Puzzle::solve(const Integer& ckey, unsigned long long times, unsigned long base, const Integer& n)
{
	Integer num(base);
	--times; // one less as ModularExponentiation is used instead at the end
	while (times--) num *= num;
	num = CryptoPP::ModularExponentiation(num, num, n);

	return cryptoInt2SBB(ckey - num); // TODO check if the same key is converted back
}

SecByteBlock cryptoInt2SBB(const Integer& integer)
{

	// TODO IMPLEMENT THIS SHIT integer.cpp:3086
	/*for (size_t i=inputLen; i > 0; i--)
	{
		bt.Get(b);
		reg[(i-1)/WORD_SIZE] |= word(b) << ((i-1)%WORD_SIZE)*8;
	}*/

	byte* bytearr = new byte[integer.ByteCount()];
	for (int i = integer.ByteCount() - 1; i >= 0; --i)
	{
		bytearr[i] = integer.GetByte(integer.ByteCount() - 1 - i);
	}
	SecByteBlock sbb(bytearr, integer.ByteCount());
	return sbb;
}

inline Integer cryptoSBB2Int(const SecByteBlock& sbb)
{
	return Integer(sbb.data(), sbb.size(), CryptoPP::Integer::UNSIGNED);
}