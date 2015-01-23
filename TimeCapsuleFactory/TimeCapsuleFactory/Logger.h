
//  @ Project : TimeCapsuleFactory
//  @ File Name : Logger.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_LOGGER_H)
#define _LOGGER_H

#include <vector>
#include <string>
#include <ostream>

using std::vector;
using std::string;
using std::ostream;

class Logger
{
private:
	static vector<string> logs;
	Logger();

public:
	static void log(string line);
	static void save(string filename);
	static void print(ostream& out);
};

#endif  //_LOGGER_H
