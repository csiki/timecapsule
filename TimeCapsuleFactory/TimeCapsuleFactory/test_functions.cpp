#include "test_functions.h"

void testFactory()
{
	Puzzle p;
	HardwareSpeedTester hst(1000, seconds(20), seconds(10));
	TimeCapsuleFactory<char> tcf(hst);
	char rawdata[] = "Hey, I hope it will finish in time.. tho probably not :D ...";
	vector<char> data(rawdata, rawdata + sizeof(rawdata));
	auto duration = seconds(5);
	auto capsule = tcf.createTimeCapsule(p, data, duration);

	capsule.save("capsule.dat");

	Capsule<char> capsule2;
	capsule2.load("capsule.dat");

	Puzzle p2(capsule2.getBase());

	auto start = chrono::high_resolution_clock::now();
	auto key = p2.solve(capsule2.getCryptedKey(), capsule2.getNumberOfOperations(), capsule2.getN());
	auto end = chrono::high_resolution_clock::now();

	cout << "Time specified to decode: " << duration.count() << " seconds" << endl;
	cout << "Time taken to decode: " << chrono::duration_cast<seconds>(end - start).count() << " seconds" << endl;
	
	Encryptor<char> cr;
	auto databack = cr.decrypt(capsule2.getCryptedData(), key, capsule2.getIV());
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
	HardwareSpeedTester hpt(1000, seconds(30), nanoseconds(50));
	long double stdev;
	auto func = hpt.testPuzzleComplexity(p, stdev, samples);

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