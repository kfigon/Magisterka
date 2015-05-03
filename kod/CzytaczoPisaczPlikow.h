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
class fstreamWrapper
{
protected:
	fstream mPlik;
	size_t mRozmiarPliku;
	const string mSciezkaDoPliku;

public:
	fstreamWrapper(const string& sciezkaDoPliku);
	virtual ~fstreamWrapper();

	// gettery
	string zwrocSciezkeDoPliku() const { return mSciezkaDoPliku; }
	size_t zwrocRozmiarPliku() const { return mRozmiarPliku; }
};

class CzytaczoPisaczBinarnychPlikow :public fstreamWrapper
{
public:
	CzytaczoPisaczBinarnychPlikow(const string& sciezkaDoPliku);
	~CzytaczoPisaczBinarnychPlikow() = default;

	// bufor wyjsciowy i wejsciowo ile danych przeczytac
	bool Czytaj(char* buf, size_t rozmiarBufora);

	// pomocniczo do zrzucania danych do plikow:
	static void ZapiszPlik(Data* dane, size_t rozmiarTablicyDanych, const string& sciezkaDoPlikuWyjsciowego);
	static void ZapiszPlik(char* dane, size_t rozmiar, const string& sciezkaDoPlikuWyjsciowego);
};

class ParserStacjiBazowych :public fstreamWrapper
{
public:
	ParserStacjiBazowych(const string& sciezkaDoPliku);
	~ParserStacjiBazowych() = default;

	vector<Stacja> ParsujStacje();

private:
	Stacja ParsujLinijke(const string& linijka);
	vector<string> podzielStringPoZnaku(const string& napis, char znak);
	WspolrzedneDMS wydobadzWspolrzednezNapisu(const string& napis);
};
