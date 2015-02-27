#include "test_functions.h"

void multipleFactoryTest()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long> distr(5, 20);

	ofstream logfile("log.txt", ios::app);

	Logger::printPromptlyToStdOut();
	for (int i = 50; i < 55; ++i)
	{
		Logger::log("Test with ID #" + to_string(i) + " is started.");

		//testFactory(genRandomString(i), seconds(distr(gen)), seconds(distr(gen)), i);
		testFactory(genRandomString(i), seconds(distr(gen)), seconds(distr(gen)), i);

		Logger::print(logfile);
		Logger::clear();
		cout << endl;
	}

	Logger::save("log.txt");
}

void testFactory(vector<char> data, seconds duration, seconds maxTimeToTest, int id)
{
	Puzzle p;
	HardwareSpeedTester hst(100, maxTimeToTest);
	TimeCapsuleFactory<char> tcf(hst);
	//char rawdata[] = "Hey, I hope it will finish in time.. tho probably not :D ...";
	auto capsule = tcf.createTimeCapsule(p, data, duration);
	capsule.save("capsule_" + to_string(id) + ".dat");

	Capsule<char> capsule2;
	capsule2.load("capsule_" + to_string(id) + ".dat");

	auto crdata = capsule.getCryptedData();
	auto crdata2 = capsule2.getCryptedData();

	assert(capsule.getBase() == capsule2.getBase());
	assert(capsule.getN() == capsule2.getN());
	assert(capsule.getIV() == capsule2.getIV());
	assert(capsule.getCryptedKey() == capsule2.getCryptedKey());
	assert(capsule.getNumberOfOperations() == capsule2.getNumberOfOperations());
	Logger::log("Original crypted data size: " + to_string(crdata.size()));
	Logger::log("Copypasted crypted data size: " + to_string(crdata2.size()));

	string datastr(crdata.begin(), crdata.end());
	string datastr2(crdata2.begin(), crdata2.end());
	Logger::log("Original   crypted data: " + datastr);
	Logger::log("Copypasted crypted data: " + datastr2);

	//cout << capsule.getCryptedData().size() << ", " << capsule2.getCryptedData().size() << endl;
	//assert(capsule.getCryptedData().size() == capsule2.getCryptedData().size());
	//for (size_t i = 0; i < capsule.getCryptedData().size(); ++i)
	//	assert(capsule.getCryptedData()[i] == capsule2.getCryptedData()[i]);

	Puzzle p2(capsule2.getBase());

	auto start = chrono::high_resolution_clock::now();
	auto key = p2.solve(capsule2.getCryptedKey(), capsule2.getNumberOfOperations(), capsule2.getN());
	auto end = chrono::high_resolution_clock::now();

	Logger::log("Time specified to decode: " + to_string(duration.count()) + " seconds");
	Logger::log("Time taken to decode: " + to_string(chrono::duration_cast<seconds>(end - start).count()) + " seconds");
	
	Encryptor<char> cr;
	auto databack = cr.decrypt(capsule2.getCryptedData(), key, capsule2.getIV());
	string tmpdata(data.begin(), data.end());
	string tmpdataback(databack.begin(), databack.end());
	Logger::log("Original  data: " + tmpdata);
	Logger::log("Data decrypted: " + tmpdataback);

	/*char dummychar;
	if (crdata.size() != crdata2.size())
	{
		Logger::log("PARA VAN!");
		cin >> dummychar;
	}*/
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

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

static const size_t alphanumlen = sizeof(alphanum);

vector<char> genRandomString(size_t len)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long> distr(0, alphanumlen - 1);
	vector<char> strvec;
	for (size_t i = 0; i < len; ++i)
		strvec.push_back(alphanum[distr(gen)]);
	return strvec;
}