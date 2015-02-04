
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <type_traits>
#include "TimeCapsuleFactory.h"
#include "Capsule.h"
#include "test_functions.h"

using namespace std;

// TODO log more frequently
// TODO update StarUML when ready
// TODO test binary files
// TODO implement TimeCapsule module
// TODO Integer return ComplexityFunctionnek ?
// TODO change all in the form #include "cryptopp/[XY].h" and make a readme for installing crypto++; should be able to include in the TimeCapsule module the same way
// TODO multithreading?
// TODO csak kulon menupont/argumentum legyen alra hogy dekodol vagy factory-t hasznal
// FIXME TODO dur=600sec kodolasnal 1119sec hiba
// TODO kulon threaden idokzonkent mentsuk a decryption allapotat
// TODO timeforcreation should be divided or logged to be converted to maxStepTimeToTest

template <typename T>
vector<T> readFileToVector(string filepath)
{
	std::ifstream fin;
	if (std::is_same<T, char>::value)
		fin.open(filepath);
	else if (sizeof(T) == sizeof(char))
		fin.open(filepath, std::ios::binary);
	else
		return vector<T>();

	if (!fin.is_open()) return vector<T>(); // no file found

	fin.seekg(0, std::ios::end);
	size_t size = fin.tellg();
	fin.seekg(0, std::ios::beg);

	vector<T> buffer(size);
	fin.read((char*)buffer.data(), size);
	return buffer;
}

template <typename T>
vector<T> readRawToVector(int argc, char* argv[], int from)
{
	vector<T> buffer;
	string acc;
	for (size_t i = from; i < argc; ++i)
	{
		acc += argv[i];
		acc += ' ';
	}
	for (size_t i = 0; i < acc.length() - 1; ++i) // emit the last space
		buffer.push_back(acc.at(i));

	return buffer;
}

/// Command line arguments:
/// argv[1] - datatype: ft | fb | rt | rb (text file, binary file, raw text, or raw binary)
/// argv[2] - minimum time needed for decryption (in seconds)
/// argv[3] - time for the capsule creation (in seconds; the more, the better; at least 1 minute)
/// argv[4] - puzzle type (optional; disregarded if given as only the repeated squaring puzzle is implemented)
/// argv[5] - number of puzzle steps (if unknown give 0; if given min. time for decryption is disregarded)
/// argv[6] - file name and path of the capsule to save (no spaces)
/// rest... - data to encrypt (path of file with name if datatype==f_ or data itself if datatype=r_)
int main(int argc, char* argv[])
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long> distr(5, 20);

	ofstream logfile("log.txt", ios::app);

	Logger::printPromptlyToStdOut();
	for (int i = 50; i < 100; ++i)
	{
		Logger::log("Test with ID #" + to_string(i) + " is started.");

		testFactory(genRandomString(i), seconds(distr(gen)), seconds(distr(gen)), i);

		Logger::print(logfile);
		Logger::clear();
		cout << endl;
	}
	
	/*Logger::printPromptlyToStdOut();

	if (argc < 8)
	{
		cerr << "More arguments needed! Arguments in order:" << endl
			<< " - datatype: ft | fb | rt | rb (text file, binary file, raw text or raw binary)" << endl
			<< " - minimum time needed for decryption (in seconds)" << endl
			<< " - time for the capsule creation roughly (in seconds; the more, the better; at least 1 minute)" << endl
			<< " - puzzle type (disregarded as only the repeated squaring puzzle is implemented)" << endl
			<< " - number of puzzle steps (if unknown give 0; if given min. time for decryption is disregarded)" << endl
			<< " - file name and path of the capsule to save (may not contain white-spaces)" << endl
			<< " - data to encrypt (path of file with name if datatype==f_ or data itself if datatype=r_)" << endl << endl
			<< "Example command: tcfactory rt 31536000 120 _ 0 path/to/capsule.dat Hey! Please enrypt me for 1 year! It will take about 120*log(120) seconds!" << endl;
		return 1;
	}

	/// datatype independent stuff
	string puzzleType(argv[4]); // not used
	string capsulePath(argv[6]);
	unsigned long long duration;
	unsigned long long timeForCreation;
	unsigned long long numberOfSteps;
	if ( !(sscanf(argv[2], "%llu", &duration)
		&& sscanf(argv[3], "%llu", &timeForCreation)
		&& sscanf(argv[5], "%llu", &numberOfSteps)) )
	{
		cerr << "One of the minimum time needed for decryption, the time for the capsule creation, or the number of puzzle steps integer arguments cannot be interpreted as number!" << endl;
		return 4;
	}
	// determination of sampleThreshold: least nanoseconds that the hardware is able to measure * 2 + 1
	Puzzle puzzle;
	HardwareSpeedTester hst(100000, seconds(timeForCreation));

	/// datatype dependent stuff
	char datatype = argv[1][1];
	if (datatype == 't') // text
	{
		// retrieve data
		vector<char> data;
		if (argv[1][0] == 'f')
		{
			data = readFileToVector<char>(argv[7]);
		}
		else if (argv[1][0] == 'r')
			data = readRawToVector<char>(argc, argv, 7);
		else
		{
			cerr << "Wrong datatype! Possible values: ft | fb | rt | rb (text file, binary file, raw text or raw binary)!" << endl;
			return 2;
		}
		// check for empty data
		if (data.size() == 0)
		{
			cerr << "No raw data or file is found, or empty file is given!" << endl;
			return 3;
		}

		// create & save time capsule
		TimeCapsuleFactory<char> factory(hst);
		Capsule<char> capsule;
		if (numberOfSteps == 0)
			capsule = factory.createTimeCapsule(puzzle, data, seconds(duration));
		else
			capsule = factory.createTimeCapsule(puzzle, data, seconds(duration), numberOfSteps);
		
		if (capsule.save(capsulePath))
			Logger::log("Capsule is successfully saved to " + capsulePath);
	}
	else if (datatype == 'b') // binary
	{
		// retrieve data
		vector<unsigned char> data;
		if (argv[1][0] == 'f')
			data = readFileToVector<unsigned char>(argv[7]);
		else if (argv[1][0] == 'r')
			data = readRawToVector<unsigned char>(argc, argv, 7);
		else
		{
			cerr << "Wrong datatype! Possible values: ft | fb | rt | rb (text file, binary file, raw text or raw binary)!" << endl;
			return 2;
		}
		// check for empty data
		if (data.size() == 0)
		{
			cerr << "No raw data is found, or empty file is given!" << endl;
			return 3;
		}

		// create & save time capsule
		TimeCapsuleFactory<unsigned char> factory(hst);
		Capsule<unsigned char> capsule;
		if (numberOfSteps == 0)
			capsule = factory.createTimeCapsule(puzzle, data, seconds(duration));
		else
			capsule = factory.createTimeCapsule(puzzle, data, seconds(duration), numberOfSteps);
		
		if (capsule.save(capsulePath))
			Logger::log("Capsule is successfully saved to " + capsulePath);
	}
	else
	{
		cerr << "Wrong datatype! Possible values: ft | fb | rt | rb (text file, binary file, raw text or raw binary)!" << endl;
		return 2;
	}

	ofstream fout("log.txt", ios::app);
	Logger::print(fout);*/

	return 0;
}

