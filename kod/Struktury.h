#pragma once
#include <string>
#include "Geografia.h"

#define VERBOSE

#ifdef VERBOSE
#define DbgPrint(x, ...)					printf("[%s] "x, __FUNCTION__, ##__VA_ARGS__)
#define PiszPostepPetli(aktualny, calosc)	printf("\r%d/%d", aktualny, calosc)
#define FuncEntry()							printf("[%s] Entry\n" ,__FUNCTION__)
#define FuncExit()							printf("[%s] Exit\n" ,__FUNCTION__)
#endif

#ifndef VERBOSE
#define DbgPrint(x) 
#define PiszPostepPetli(aktualny, calosc) 
#define FuncEntry()
#define FuncExit()
#endif

#define ILOSC_CIAGOW_PN_NA_STACJE 3

#define SZYBKOSC_TRANSMISJI_CIAGU_ROZPRASZAJACEGO_MHZ		1.2288
#define CZESTOTLIWOSC_PROBKOWANIA_DANYCH_MHZ				1.28
#define SZYBKOSC_TRANSMISJI_DANYCH_KHZ						19.2

typedef unsigned char UCHAR;	
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;

struct Data
{
    short I;
    short Q;
};

struct Stacja
{
    UINT NrStacji;
    std::string Lokalizacja;
    UINT numeryCiagowPn [ILOSC_CIAGOW_PN_NA_STACJE];
	PozycjaGeograficzna pozycjaGeoStacji;
};