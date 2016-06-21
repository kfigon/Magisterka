#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Struktury.h"
#include "GeneratorCiagow.h"
#include "FileReader.h"

//class Odbiornik
//{
//public:
//	Odbiornik();
//	~Odbiornik();
//};
//


struct WynikKorelacji
{
	UINT offset;
	long long wartosc;

	WynikKorelacji(UINT offset = 0, long long wartosc = 0) :
		offset(offset),
		wartosc(wartosc)
	{}
};

void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<WynikKorelacji>& wynikKorelacji);
void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<WynikKorelacji>& wynikKorelacji);
long long liczIteracje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI);
void liczKorelacje(std::vector<Data>& dane, int coKtoraPominac, BinaryReader::Endian endian);
