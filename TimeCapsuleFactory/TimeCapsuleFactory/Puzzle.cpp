
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Puzzle.h"

#include <fstream> // TODO rm

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
	ComplexityFunc flin = [] (unsigned long long n) {
		return n;
	};
	ComplexityFunc flog = [] (unsigned long long n) {
		return std::log(n) + .5;
	};
	/*ComplexityFunc fexp = [] (unsigned long long n) {
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
	};*/
	possibleComplexities.push_back(flin);
	possibleComplexities.push_back(flog);
	//possibleComplexities.push_back(fexp);
	//possibleComplexities.push_back(f2exp);
	//possibleComplexities.push_back(fnexp);
	//possibleComplexities.push_back(fnnexp);
}
#include <iostream> // TODO rm
DurationSamples Puzzle::funcdur(size_t maxStepToTest, nanoseconds maxTimeToTest, nanoseconds sampleThreshold)
{
	assert(maxTimeToTest > sampleThreshold);

	// sample till maxTimeToTest or maxStepToTest is reached
	unsigned long long i = 1;
	nanoseconds steptime(0), acc(0);
	Integer num = base;
	DurationSamples samples;
	Integer dummyN("12229383811045573685848053022622583812200572264847222457958666283913892496704472374216935266226370028369363700663851905943499894012742029033944904866255912319413490752666665287286848964979906780401875254855060330898629291312510227546682219016983240617679001992784839278463725212442657020780554682222476687733706323455893761454849924072029324231767608456683468363679557280265089694061856284533110914641941257101538229218685701308296747564274895835725808715437977019026068935996566569823652290547728488194485533119027512280676613892276116673392463368565692544735252165483704525722463842266861806261832651155344705738903");

	while (samples.size() < maxStepToTest && acc < maxTimeToTest)
	{
		auto start = high_resolution_clock::now();
		num = ((num % dummyN) * (num % dummyN)) % dummyN; // ((a % n) * (a % n)) % n
		auto end = high_resolution_clock::now();

		steptime = end - start;
		acc += steptime;
		if (steptime > sampleThreshold)
			samples.push_back(std::make_pair(i, acc));
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
	auto b = CryptoPP::a_exp_b_mod_c(Integer(base), e, n);
	auto ckey = (cryptoSBB2Int(key) + b) % n;

	return ckey; // return key as integer so no further Integer-SecByteBlock conversion is needed here, neither at solve
}

// should be optimal as possible
SecByteBlock Puzzle::solve(const Integer& ckey, unsigned long long times, const Integer& n)
{
	Integer a(base);
	Integer e(2);
	//CryptoPP::a_exp_b_mod_c(); CryptoPP::ModularExponentiation(a, e, n);
	while (times--) a = ((a % n) * (a % n)) % n; // a^2 mod n = (a*a) mod n = ((a mod n) * (a mod n)) mod n
	//auto b = a % n;

	return cryptoInt2SBB(ckey - a);
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