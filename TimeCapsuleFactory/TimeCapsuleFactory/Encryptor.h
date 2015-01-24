
//  @ Project : TimeCapsuleFactory
//  @ File Name : Encryptor.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_ENCRYPTOR_H)
#define _ENCRYPTOR_H

#include <vector>
#include <secblock.h>

using std::vector;
using CryptoPP::SecByteBlock;

template <typename T>
class Encryptor
{
public:
	vector<T> encrypt(vector<T> data, SecByteBlock& key, SecByteBlock& iv)
	{
		AutoSeededRandomPool rnd;

		// generate a random key
		SecByteBlock key(0x00, AES::MAX_KEYLENGTH);
		rnd.GenerateBlock(key, key.size());

		// generate a random IV
		byte iv[AES::BLOCKSIZE];
		rnd.GenerateBlock(iv, AES::BLOCKSIZE);

		// encrypt
		CFB_Mode<AES>::Encryption cfbEncryption(Integer(10), 10);
		//CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), iv);
		cfbEncryption.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());

		return data;
	}

	vector<T> decrypt(vector<T> cdata, const SecByteBlock& key, const SecByteBlock& iv)
	{
		CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv);
		cfbDecryption.ProcessData((byte*)cdata.data(), (byte*)cdata.data(), cdata.size());

		return cdata;
	}
};

#endif  //_ENCRYPTOR_H
