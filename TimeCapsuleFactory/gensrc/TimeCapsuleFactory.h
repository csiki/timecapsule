
//  @ Project : TimeCapsuleFactory
//  @ File Name : TimeCapsuleFactory.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_TIMECAPSULEFACTORY_H)
#define _TIMECAPSULEFACTORY_H


template < DataType> class TimeCapsuleFactory
{
public:
	bool createTimeCapsule(Puzzle& puzzle, vector<DataType> data, chrono_seconds duration);
	bool createTimeCapsule(Puzzle& puzzle, vector<DataType> data, chrono_seconds duration, chrono_nanoseconds optime);
};

#endif  //_TIMECAPSULEFACTORY_H
