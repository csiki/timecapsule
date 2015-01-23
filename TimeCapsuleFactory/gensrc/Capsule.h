
//  @ Project : TimeCapsuleFactory
//  @ File Name : Capsule.h
//  @ Date : 2015.01.22.
//  @ Author : Viktor Toth
//
//


#if !defined(_CAPSULE_H)
#define _CAPSULE_H


template < T> class Capsule
{
private:
	vector<T> cdata;
	SecByteBlock ckey;
	SecByteBlock iv;
	unsigned long times;
	unsigned long base;
public:
	bool save(string filepath);
	bool load(string filepath);
	vector<T> getCryptedData();
	SecByteBlock getCryptedKey();
	SecByteBlock getIV();
	unsigned long getNumberOfOperations();
	unsigned long getBase();
};

#endif  //_CAPSULE_H
