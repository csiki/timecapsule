
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


		return vector<T>();
	}

	vector<T> decrypt(const vector<T>& cdata, const SecByteBlock& key, const SecByteBlock& iv)
	{
		return vector<T>();
	}
};

#endif  //_ENCRYPTOR_H
