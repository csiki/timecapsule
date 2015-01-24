
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



int main(int argc, char* argv[])
{
	AutoSeededRandomPool rnd;
	SecByteBlock key(0x00, AES::MAX_KEYLENGTH);
	rnd.GenerateBlock(key, key.size());
	std::cout << key << std::endl;
	auto key2 = cryptoInt2SBB(cryptoSBB2Int(key));
	std::cout << key2 << std::endl;

	//auto num = Integer("0x9749473949997765447889865465749444474739463877384944");
	//auto ckey = keyconv + num;
	//auto key2 = cryptoInt2SBB(ckey - num);
	

	//char arr[] = {'H', 'E', 'L', 'L', 'O', '!'};
	//TimeCapsuleFactory<> factory;
	//Puzzle puzzle;
	//std::vector<char> data(arr, arr + 6);
	//factory.createTimeCapsule(puzzle, data, seconds(10));
	//Logger::log("dawwdadaw");
	//Logger::print(std::cout);
	
	return 0;
}
