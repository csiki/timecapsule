
#include <chrono>
#include <iostream>
#include <random>
#include <ctime>
#include <integer.h>
#include <secblock.h>

#include "../../TimeCapsuleFactory/TimeCapsuleFactory/Capsule.h"
#include "../../TimeCapsuleFactory/TimeCapsuleFactory/Encryptor.h"
#include "../../TimeCapsuleFactory/TimeCapsuleFactory/Puzzle.h"
#include "../../TimeCapsuleFactory/TimeCapsuleFactory/Logger.h"

using CryptoPP::Integer;
using namespace std;
using namespace std::chrono;

template <typename T>
void createCapsule(int argc, char* argv[])
{
	Capsule<T> capsule;
	capsule.load(argv[1]);

	Puzzle puzzle(capsule.getBase());
	auto start = std::chrono::high_resolution_clock::now();
	auto key = puzzle.solve(capsule.getCryptedKey(), capsule.getNumberOfOperations(), capsule.getN());
	auto end = std::chrono::high_resolution_clock::now();

	Logger::log("Puzzle solved in: " + to_string(duration_cast<seconds>(end - start).count()) + " seconds.");

	Encryptor<T> enc;
	auto detidata = enc.decrypt(capsule.getCryptedData(), key, capsule.getIV());

	Logger::log("Decoded data is saved to: " + string(argv[2]));

	ofstream fout(argv[2], ios::trunc);
	for (auto c : detidata)
		fout << c;
}

int main(int argc, char* argv[])
{
	if (argc > 3)
	{
		Logger::printPromptlyToStdOut();
		if (argv[3][0] == 'b') // binary
			createCapsule<unsigned char>(argc, argv);
		else // text by default
			createCapsule<char>(argc, argv);
	}
	else
	{
		cerr << "The following arguments are mandatory:" << endl
			<< " - path to the capsule file" << endl
			<< " - path to the file that will contain the data after decryption" << endl
			<< " - type of encrypted data (text or binary)";
		return 1;
	}

	return 0;
}