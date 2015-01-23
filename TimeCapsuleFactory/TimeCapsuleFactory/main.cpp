
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

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::SecByteBlock;
using CryptoPP::AES;
using CryptoPP::CFB_Mode;
using CryptoPP::Integer;


int main(int argc, char* argv[])
{


	//char arr[] = {'H', 'E', 'L', 'L', 'O', '!'};
	//TimeCapsuleFactory<> factory;
	//Puzzle puzzle;
	//std::vector<char> data(arr, arr + 6);
	//factory.createTimeCapsule(puzzle, data, seconds(10));
	Logger::print(std::cout);

	/*AutoSeededRandomPool rnd;

	// Generate a random key
	SecByteBlock key(0x00, AES::DEFAULT_KEYLENGTH);
	rnd.GenerateBlock( key, key.size() );

	// Generate a random IV
	byte iv[AES::BLOCKSIZE];
	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	char plainText[] = "Hello! How are you.";
	int messageLen = (int)strlen(plainText) + 1;

	//////////////////////////////////////////////////////////////////////////
	// Encrypt
	CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
	cfbEncryption.ProcessData((byte*)plainText, (byte*)plainText, messageLen);

	//////////////////////////////////////////////////////////////////////////
	// Decrypt
	CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
	cfbDecryption.ProcessData((byte*)plainText, (byte*)plainText, messageLen);*/
	
	return 0;
}
