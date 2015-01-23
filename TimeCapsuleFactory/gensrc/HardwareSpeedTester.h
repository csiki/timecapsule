
//  @ Project : TimeCapsuleFactory
//  @ File Name : HardwareSpeedTester.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_HARDWARESPEEDTESTER_H)
#define _HARDWARESPEEDTESTER_H


class HardwareSpeedTester
{
public:
	chrono_nanoseconds testPuzzleOpSpeed(Puzzle& puzzle);
};

#endif  //_HARDWARESPEEDTESTER_H
