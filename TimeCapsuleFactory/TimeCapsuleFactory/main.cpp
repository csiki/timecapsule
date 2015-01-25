
#include <cstdio>
#include <iostream>
#include <osrng.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cryptlib.h>
#include <hex.h>
#include <filters.h>
#include <des.h>
#include <modes.h>
#include <secblock.h>
#include <iostream>
#include <string>
#include <modes.h>
#include <aes.h>
#include <filters.h>
#include <integer.h>

#include "TimeCapsuleFactory.h"
#include "Puzzle.h"
#include "Logger.h"
#include "Encryptor.h"

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::SecByteBlock;
using CryptoPP::AES;
using CryptoPP::CFB_Mode;
using CryptoPP::Integer;

using namespace std;

// TEST FUNCTIONS
void testFactory();
void testSpeedTester();
void testEncryptor();
void testPuzzle();

// TODO update StarUML when ready

/// Command line arguments:
/// argv[0] - datatype: ft | fb | rt | rb as text file, binary file, raw text, raw binary
/// argv[1] - duration of encryption (in seconds)
/// argv[2] - puzzle type [disregarded]
/// argv[3] - number of puzzle steps (if known already, if not type zero (0))
/// argv[4] - data (filepath if datatype=f_ or data itself if datatype=r_)
int main(int argc, char* argv[])
{
	// TODO

	return 0;
}

void testFactory()
{
	Puzzle p;
	TimeCapsuleFactory<char> tcf;
	char rawdata[] = "Hey, I hope it will finish in time.. tho probably not :D ...";
	vector<char> data(rawdata, rawdata + sizeof(rawdata));
	auto duration = seconds(5);
	auto capsule = tcf.createTimeCapsule(p, data, duration);

	auto start = chrono::high_resolution_clock::now();
	auto key = p.solve(capsule.getCryptedKey(), capsule.getNumberOfOperations(), capsule.getN());
	auto end = chrono::high_resolution_clock::now();

	cout << "Time specified to decode: " << duration.count() << " seconds" << endl;
	cout << "Time taken to decode: " << chrono::duration_cast<seconds>(end - start).count() << " seconds" << endl;
	
	Encryptor<char> cr;
	auto databack = cr.decrypt(capsule.getCryptedData(), key, capsule.getIV());
	cout << "Data decrypted: " << databack.data() << endl;

	Logger::print(std::cout);
}

void testSpeedTester()
{
	DurationSamples samples;
	samples.push_back(std::make_pair((unsigned long long) 15, nanoseconds(1975700)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 17, nanoseconds(4976700)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 18, nanoseconds(11981400)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 19, nanoseconds(34999300)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 20, nanoseconds(99043200)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 21, nanoseconds(292173300)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 22, nanoseconds(892574500)));		// 0
	samples.push_back(std::make_pair((unsigned long long) 23, nanoseconds(2643745200)));	// 2
	samples.push_back(std::make_pair((unsigned long long) 24, nanoseconds(7937287000)));	// 7
	samples.push_back(std::make_pair((unsigned long long) 25, nanoseconds(23825904400)));	// 23
	samples.push_back(std::make_pair((unsigned long long) 26, nanoseconds(71674877300)));	// 71

	Puzzle p(2);
	HardwareSpeedTester hpt;
	long double stdev;

	auto func = hpt.testPuzzleComplexity(p, stdev, 1000, seconds(30), nanoseconds(50), samples);

	cout << endl << "ESTIMATE: " << endl;
	for (size_t i = 1; i < 50; ++i)
		cout << i << ": " << func(i) / 1000000000.0 << endl;
	cout << "STDEV:" << stdev << endl;
}

void testPuzzle()
{
	Puzzle p(2);
	cout << "base: " << p.getBase() << endl;
	AutoSeededRandomPool rnd;
	SecByteBlock key(0x00, AES::MAX_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());

	// setup & solve
	Integer n;
	auto ckey = p.setup(key, 10, n);
	auto keyback = p.solve(ckey, 10, n);
	assert(key == keyback);
	cout << "Key converted back successfully!" << endl;

	// funcdur
	auto samples = p.funcdur(1000, seconds(5), nanoseconds(1000));
	for (auto& s : samples)
		cout << s.first << ": " << s.second.count() << endl;
}

void testEncryptor()
{
	char plainText[] = "Hello! How are you.";
	vector<char> data(plainText, plainText + 20);
	Encryptor<char> enc;
	SecByteBlock key;
	SecByteBlock iv;

	auto cdata = enc.encrypt(data, key, iv);
	auto databack = enc.decrypt(cdata, key, iv);
	assert(data == databack);

	cout << cdata.data() << endl;
	cout << databack.data() << endl;
}