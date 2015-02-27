
//  @ Project : TimeCapsuleFactory
//  @ File Name : Logger.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Logger.h"

vector<string> Logger::logs;
bool Logger::promptPrint = false;

void Logger::log(string line)
{
	if (!line.empty())
		logs.push_back(line);
	
	if (promptPrint)
	{
		std::cout << "| " << line << std::endl;
		std::flush(std::cout);
	}
}

void Logger::save(string filename)
{
	std::ofstream outfile(filename, std::ios_base::trunc);
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	outfile << "=====================================" << std::endl
		<< "TimeCapsule LOG | "
		<< std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")
		<< std::endl << "====================================="
		<< std::endl;
	
	for (auto& line : logs)
		outfile << "| " << line << std::endl;
}

void Logger::print(ostream& out)
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	out << "=====================================" << std::endl
		<< "TimeCapsule LOG | "
		<< std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X")
		<< std::endl << "====================================="
		<< std::endl;
	
	for (auto& line : logs)
		out << "| " << line << std::endl;
}
