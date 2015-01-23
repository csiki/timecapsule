
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Puzzle.h"

void Puzzle::initComplexities()
{

}

nanoseconds Puzzle::funcdur(unsigned long long times)
{
	return nanoseconds();
}

SecByteBlock Puzzle::setup(const SecByteBlock& key, unsigned long long times, unsigned long& base, Integer& n)
{
	return SecByteBlock(nullptr, 0);
}

SecByteBlock Puzzle::solve(const SecByteBlock& ckey, unsigned long long times, unsigned long base, const Integer& n)
{
	return SecByteBlock(nullptr, 0);
}

SecByteBlock cryptoInt2SBB(const Integer& integer)
{
	byte* bytearr = new byte[integer.ByteCount()];
	for (unsigned int i = 0; i < integer.ByteCount(); ++i)
	{
		bytearr[i] = integer.GetByte(i);
	}
	SecByteBlock sbb(bytearr, integer.ByteCount());
	return sbb;
}