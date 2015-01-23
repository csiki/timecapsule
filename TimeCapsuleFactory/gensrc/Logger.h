
//  @ Project : TimeCapsuleFactory
//  @ File Name : Logger.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_LOGGER_H)
#define _LOGGER_H


class Logger
{
private:
	static vector<string> logs;
	void Logger();
public:
	static void log(string line);
	static void save(string filename);
};

#endif  //_LOGGER_H
