
#include <chrono>
#include <iostream>
#include <random>
#include <ctime>
#include <integer.h>
#include <secblock.h>

using CryptoPP::Integer;
using namespace std::chrono;

int main(int argc, char* argv[])
{
	hours till(24*365*200);
	nanoseconds acc(0);

	std::cout << duration_cast<nanoseconds>(till).count() << std::endl << std::endl;

	for (int i = 0; acc < till; ++i)
	{
		acc = duration_cast<nanoseconds>(acc + nanoseconds(99999999999999999));
		std::cout << acc.count() << std::endl;
	}
	std::cout << acc.count();
	
	/*std::srand(std::time(0));

	unsigned long long a = 3;
	unsigned long long b = 3;
	unsigned long long c = 0;

	CryptoPP::word words[100] = {0};
	words[0] = 2;

	CryptoPP::IntegerSecBlock isb(100);
	isb.resize(100);
	isb.Assign(words, 100);
	Integer i1 = 9999999999999999999;
	i1 = i1 * i1 * i1 * i1;
	Integer i2 = 2;
	Integer dummy;

	auto start1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 2300; ++i)
	{
		dummy = i1 * i1;
	}
	auto end1 = std::chrono::high_resolution_clock::now();

	std::cout << (end1 - start1).count() << std::endl;

	//auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 21; ++i)
	{
		auto start2 = std::chrono::high_resolution_clock::now();
		i2 *= i2;
		auto end2 = std::chrono::high_resolution_clock::now();

		auto insec = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);

		std::cout << i << ": " << insec.count() << std::endl;
	}*/
	//auto end2 = std::chrono::high_resolution_clock::now();
	
	//std::cout << "a: " << a << std::endl;

	/*auto start2 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 25; ++i)
		dummy = i2 * i2;
	auto end2 = std::chrono::high_resolution_clock::now();*/

	//std::cout << (end1 - start1).count() << std::endl; << (end2 - start2).count() << std::endl;

	return 0;
}