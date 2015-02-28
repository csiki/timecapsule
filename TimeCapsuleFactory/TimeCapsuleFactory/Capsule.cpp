
//  @ Project : TimeCapsuleFactory
//  @ File Name : Capsule.cpp
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth

#include "Capsule.h"

string Capsule<char>::errCharSubstitue = "_EOF////////////////////";
string Capsule<unsigned char>::errCharSubstitue = "_EOF////////////////////";
char Capsule<char>::erroneousByte = 26;
unsigned char Capsule<unsigned char>::erroneousByte = 26;