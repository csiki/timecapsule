
//  @ Project : TimeCapsuleFactory
//  @ File Name : Capsule.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_CAPSULE_H)
#define _CAPSULE_H

#include <vector>
#include <string>
#include <fstream>
#include <type_traits>

#include <secblock.h>
#include <integer.h>

#include "Puzzle.h"

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
		// file structure: [base]\n[times]\n[n]\n[ckey]\n[iv]\n[data]
		// iv is converted to integer for convinient decoding
		// data is converted to string
		Integer intiv = cryptoSBB2Int(iv);
		ofstream fout(filepath, std::ios_base::trunc);

		fout << base << std::endl;
		fout << times << std::endl;
		fout << n << std::endl;
		fout << ckey << std::endl;
		fout << intiv << std::endl;

		// data
		for (auto& c : cdata)
			fout << c;

		return true;
	}

	bool load(string filepath)
	{
		Integer intiv;
		ifstream fin(filepath);

		fin >> base;
		fin >> times;
		fin >> n;
		fin >> ckey;
		fin >> intiv;
		iv = cryptoInt2SBB(intiv);

		// data
		T c;
		while (fin >> c) cdata.push_back(c); // FIXME TODO irreliable, see capsule #57, #58

		return true;
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
