
//  @ Project : TimeCapsuleFactory
//  @ File Name : Capsule.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_CAPSULE_H)
#define _CAPSULE_H

#include <vector>
#include <string>
#include <secblock.h>

using std::vector;
using std::string;
using CryptoPP::SecByteBlock;

template <typename T>
class Capsule
{
private:
	vector<T> cdata;
	SecByteBlock ckey;
	SecByteBlock iv;
	SecByteBlock n;
	unsigned long times;
	unsigned long base;

public:
	Capsule() : times(0), base(0) {}

	Capsule(const vector<T>& cdata_, const SecByteBlock& ckey_, const SecByteBlock& iv_, const SecByteBlock& n_, unsigned long times_, unsigned long base_)
		: cdata(cdata_), ckey(ckey_), iv(iv_), n(n_) times(times_), base(base_) {}

	bool save(string filepath)
	{

	}

	bool load(string filepath)
	{

	}

	vector<T> getCryptedData()
	{

	}

	SecByteBlock getCryptedKey()
	{

	}

	SecByteBlock getIV()
	{

	}

	unsigned long getNumberOfOperations()
	{

	}

	unsigned long getBase()
	{

	}

	SecByteBlock getN()
	{

	}
};

#endif  //_CAPSULE_H
