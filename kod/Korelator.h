#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "CiagRozpraszajacy.h"
#include "CzytaczoPisaczPlikow.h"
#include "Struktury.h"

struct WynikKorelacji
{
	UINT offset;
	long long wartosc;

	WynikKorelacji(const UINT& offset, const long long& wartosc) :
		offset(offset),
		wartosc(wartosc)
	{}
};

// co 24 probka (0 based?) jest rozjazd
#define INDEKS_PROBKI_DO_POMINIECIA	24

namespace Korelator
{
	vector<WynikKorelacji> LiczKorelacjeDlaI(Data* dane, CiagRozpraszajacy &ciag);
	vector<WynikKorelacji> LiczKorelacjeDlaQ(Data* dane, CiagRozpraszajacy &ciag);

	// todo
	WynikKorelacji SzukajMaksa(const vector<WynikKorelacji>& wynikKorelacji);	
	void RysujWykres(const std::string& sciezkaDoPliku, vector<WynikKorelacji>& wynikKorelacji);
	void PrzygotujPlikDanychKorelacji(const string& sciezka, vector<WynikKorelacji>& wynikKorelacji);
}

