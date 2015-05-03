#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "CiagRozpraszajacy.h"
#include "CzytaczoPisaczPlikow.h"
#include "Korelator.h"
#include "Geografia.h"
#include "Struktury.h"

using namespace std;


class Odbiornik
{
private:
    vector<Stacja> mStacje;

    Data *mDane;
    int mRozmiarTablicyDanych;

public:
    Odbiornik(const string& sciezkaDoPlikuZeStacjami);
	Odbiornik(const string& sciezkaDoPlikuZeStacjami, const string& sciezkaDoPlikuzDanymi);
    ~Odbiornik();

	// logika z usuwaniem czytacza plikow i danych zeby latwiej zaciagac nowy plik. Metoda zaprojektowana na wywolywanie
	// przed kazdorazowa proba skupiania widma
    bool LadujNowyPlikDanych(const string& sciezkaDoPliku);

	// todo
	void SkupWidmo();
	void SparsujDaneDoPliku(const std::string& sciezkaDoPliku);
    void PiszStacje();

	// gettery
	Data* zwrocDane() const { return mDane; }
	int zwrocRozmiarTablicyDanych() const { return mRozmiarTablicyDanych; }

private:
	void czyscDane();
	double IteracjaKorelacji(size_t rozmiarDanych, const CiagRozpraszajacy& ciag, int opoznienie);
};