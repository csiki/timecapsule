
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

	static string errCharSubstitue;
	static T erroneousByte;

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
		{
			if (std::is_same<T, char>::value
				&& c == erroneousByte)
				fout << errCharSubstitue; // special string instead of EOF
			else
				fout << c;
		}

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
		fin.get(c); // first linebreak
		while (fin.get(c))
		{
			if (std::is_same<T, char>::value && c == errCharSubstitue[0])
				c = handleEOFread(fin);
			cdata.push_back(c);
		}

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

	static T handleEOFread(std::ifstream& fin)
	{
		char c;
		auto refpos = fin.tellg();
		for (size_t i = 1; i < errCharSubstitue.length(); ++i)
		{
			fin.get(c);
			if (c != errCharSubstitue[i]) // not the eof string afterall
			{
				fin.seekg(refpos);
				return errCharSubstitue[0];
			}
		}

		return erroneousByte;
	}
};

#endif  //_CAPSULE_H
