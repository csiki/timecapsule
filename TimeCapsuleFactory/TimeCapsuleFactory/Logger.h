
//  @ Project : TimeCapsuleFactory
//  @ File Name : Logger.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#if !defined(_LOGGER_H)
#define _LOGGER_H

#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>

using std::vector;
using std::string;
using std::ostream;

class Logger
{
private:
	static vector<string> logs;
	static bool promptPrint;
	Logger() {}

public:
	static void log(string line);
	static void save(string filename);
	static void print(ostream& out);
	static void printPromptlyToStdOut() { promptPrint = true; }
};

#endif  //_LOGGER_H
