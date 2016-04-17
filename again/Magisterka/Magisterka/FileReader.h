#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Struktury.h"
#include <assert.h>

using namespace std;

// podstawowa klasa opisu plikow, zawiera plik, jego rozmiar i sciezke
// pelni funkcje RAII dla klasy fstream
class fstreamWrapper : public fstream
{
private:
	size_t mRozmiarPliku = 0;
	const string mSciezkaDoPliku;

public:
	fstreamWrapper(const string& sciezkaDoPliku);
	virtual ~fstreamWrapper();

	// gettery
	string zwrocSciezkeDoPliku() const { return mSciezkaDoPliku; }
	size_t zwrocRozmiarPliku() const { return mRozmiarPliku; }
	bool czyOk() const { return (is_open() && zwrocRozmiarPliku() != 0); }
};

class CzytaczoPisaczBinarnychPlikow :public fstreamWrapper
{
public:

	enum Endian{ Big, Little };

	CzytaczoPisaczBinarnychPlikow(const string& sciezkaDoPliku);
	~CzytaczoPisaczBinarnychPlikow() = default;

	std::vector<Data> Czytaj(size_t dlugoscWektoraWyjsciowego, Endian endian);
	size_t liczWymanaganyRozmiarTablicy() const { return zwrocRozmiarPliku() / sizeof(Data) - 1; }

	// pomocniczo do zrzucania danych do plikow:
	static void ZapiszPlik(const std::vector<Data>& buf, const string& sciezkaDoPlikuWyjsciowego);
	static void ZapiszPlik(const std::vector<char>& buf, const string& sciezkaDoPlikuWyjsciowego);
};

//class ParserStacjiBazowych :public fstreamWrapper
//{
//public:
//	ParserStacjiBazowych(const string& sciezkaDoPliku);
//	~ParserStacjiBazowych() = default;
//
//	vector<Stacja> ParsujStacje();
//
//private:
//	Stacja ParsujLinijke(const string& linijka);
//	vector<string> podzielStringPoZnaku(const string& napis, char znak);
//	WspolrzedneDMS wydobadzWspolrzednezNapisu(const string& napis);
//};
