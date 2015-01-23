
//  @ Project : TimeCapsuleFactory
//  @ File Name : Encryptor.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_ENCRYPTOR_H)
#define _ENCRYPTOR_H


class Encryptor
{
public:
	vector<T> encrypt<T>(vector<T> data, SecByteBlock& key, SecByteBlock& iv);
	vector<T> decrypt<T>(const vector<T>& cdata, const SecByteBlock& key, const SecByteBlock& iv);
};

#endif  //_ENCRYPTOR_H
