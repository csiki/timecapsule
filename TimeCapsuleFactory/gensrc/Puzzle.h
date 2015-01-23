
//  @ Project : TimeCapsuleFactory
//  @ File Name : Puzzle.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_PUZZLE_H)
#define _PUZZLE_H


class Puzzle
{
public:
	chrono_nanoseconds funcdur(unsigned long times);
	SecByteBlock setup(const SecByteBlock& key, unsigned long times, unsigned long& base);
	SecByteBlock solve(const SecByteBlock& ckey, unsigned long times, unsigned long base);
};

#endif  //_PUZZLE_H
