#include <iostream>
#include <aes.h>
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

void testFactory(vector<char> datastr, seconds duration, seconds maxStepTimeToTest, int id = 0);
void testSpeedTester();
void testEncryptor();
void testPuzzle();

vector<char> genRandomString(size_t len);