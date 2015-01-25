
//  @ Project : TimeCapsuleFactory
//  @ File Name : Capsule.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_CAPSULE_H)
#define _CAPSULE_H

#include <vector>
#include <string>
#include <secblock.h>
#include <integer.h>

using std::vector;
using std::string;
using CryptoPP::SecByteBlock;
using CryptoPP::Integer;

template <typename T>
class Capsule
{
private:
	vector<T> cdata;
	SecByteBlock iv;
	Integer ckey;
	Integer n;
	unsigned long long times;
	unsigned long base;

public:
	Capsule() : times(0), base(0) {}

	Capsule(const vector<T>& cdata_, const Integer& ckey_, const SecByteBlock& iv_, const Integer& n_, unsigned long long times_, unsigned long base_)
		: cdata(cdata_), ckey(ckey_), iv(iv_), n(n_), times(times_), base(base_) {}

	bool save(string filepath)
	{
		// TODO
	}

	bool load(string filepath)
	{
		// TODO
	}

	vector<T> getCryptedData()
	{
		return cdata;
	}

	Integer getCryptedKey()
	{
		return ckey;
	}

	SecByteBlock getIV()
	{
		return iv;
	}

	Integer getN()
	{
		return n;
	}

	unsigned long long getNumberOfOperations()
	{
		return times;
	}

	unsigned long getBase()
	{
		return base;
	}
};

#endif  //_CAPSULE_H
