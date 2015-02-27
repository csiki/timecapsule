
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

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		Capsule<char> capsule;
		capsule.load(argv[1]);

		Puzzle puzzle(capsule.getBase());
		auto start = std::chrono::high_resolution_clock::now();
		auto key = puzzle.solve(capsule.getCryptedKey(), capsule.getNumberOfOperations(), capsule.getN());
		auto end = std::chrono::high_resolution_clock::now();

		Logger::log("Puzzle solved in: " + to_string(duration_cast<seconds>(end - start).count()) + " seconds.");

		Encryptor<char> enc;
		auto detidata = enc.decrypt(capsule.getCryptedData(), key, capsule.getIV());

		Logger::log("Decoded data is saved to: " + string(argv[2]));

		ofstream fout(argv[2], ios::trunc);
		for (auto c : detidata)
			fout << c;

		Logger::print(cout);
	}
	else
	{
		cerr << "The following arguments are mandatory:" << endl
			<< " - path to the capsule file" << endl
			<< " - path to the file that would contain the data after decryption" << endl;
		return 1;
	}

	return 0;
}