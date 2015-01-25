
//  @ Project : TimeCapsuleFactory
//  @ File Name : Encryptor.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_ENCRYPTOR_H)
#define _ENCRYPTOR_H

#include <vector>
#include <secblock.h>
#include <aes.h>
#include <modes.h>

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
		SecByteBlock tmpkey(0x00, AES::MAX_KEYLENGTH);
		rnd.GenerateBlock(tmpkey, tmpkey.size());
		key = tmpkey;

		// generate a random IV
		byte tmpiv[AES::BLOCKSIZE];
		rnd.GenerateBlock(tmpiv, AES::BLOCKSIZE);
		iv = SecByteBlock(tmpiv, AES::BLOCKSIZE);

		// encrypt
		CFB_Mode<AES>::Encryption cfbEncryption(key, key.size(), tmpiv);
		cfbEncryption.ProcessData((byte*)data.data(), (byte*)data.data(), data.size());

		return data;
	}

	vector<T> decrypt(vector<T> cdata, const SecByteBlock& key, const SecByteBlock& iv)
	{
		CFB_Mode<AES>::Decryption cfbDecryption(key, key.size(), iv.BytePtr());
		cfbDecryption.ProcessData((byte*)cdata.data(), (byte*)cdata.data(), cdata.size());

		return cdata;
	}
};

#endif  //_ENCRYPTOR_H
