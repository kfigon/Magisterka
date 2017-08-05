#pragma once

#define VERBOSE

#ifdef VERBOSE
#define DbgPrint(x, ...)					printf("[%s] "x, __FUNCTION__, ##__VA_ARGS__)
#define PiszPostepPetli(aktualny, calosc)	printf("\r%d/%d", aktualny, calosc)
#define FuncEntry()							printf("[%s] Entry\n" ,__FUNCTION__)
#define FuncExit()							printf("[%s] Exit\n" ,__FUNCTION__)
#endif

#ifndef VERBOSE
#define DbgPrint(x, ...) 
#define PiszPostepPetli(aktualny, calosc) 
#define FuncEntry()
#define FuncExit()
#endif

namespace Stale
{
	const double SZYBKOSC_TRANSMISJI_CIAGU_ROZPRASZAJACEGO_MHZ = 1.2288;
	const double CZESTOTLIWOSC_PROBKOWANIA_DANYCH_MHZ = 1.28;
	const double SZYBKOSC_TRANSMISJI_DANYCH_KHZ = 19.2;

	const int ILOSC_CIAGOW_PN_NA_STACJE = 3;

    const long long CZESTOTLIWOSC_PROBKOWANIA_HZ = 1280000;
    const long long CZESTOTLIWOSC_SYGNALU_HZ = 1228800;
}


typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;

struct WynikKorelacji
{
	UINT offset;
	long long wartosc;

	WynikKorelacji(UINT offset = 0, long long wartosc = 0) :
		offset(offset),
		wartosc(wartosc)
	{}
};

struct Data
{
	union
	{
		struct
		{
			short I;
			short Q;
		};
		int raw;	// cala probka raw, I oraz Q naraz
	};

	Data(short I = 0, short Q = 0) :
		I(I),
		Q(Q)
	{}

};