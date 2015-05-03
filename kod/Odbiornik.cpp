#pragma once

#include "Odbiornik.h"
using namespace std;

Odbiornik::Odbiornik(const string& sciezkaDoPlikuZeStacjami):
mDane(nullptr),
mRozmiarTablicyDanych(0)
{
    ParserStacjiBazowych parserStacji(sciezkaDoPlikuZeStacjami);
	mStacje = parserStacji.ParsujStacje();
}

Odbiornik::Odbiornik(const string& sciezkaDoPlikuZeStacjami, const string& sciezkaDoPlikuzDanymi) :
Odbiornik(sciezkaDoPlikuZeStacjami)
{
	// todo: jesli plik nie istnieje to mamy nullptr exception, trzeba sie jakos zabezpieczyc
	LadujNowyPlikDanych(sciezkaDoPlikuzDanymi);
}

void Odbiornik::czyscDane()
{
	if (mDane != nullptr)
	{
		delete[] mDane;
		mDane = nullptr;
		mRozmiarTablicyDanych = 0;
	}
}

Odbiornik::~Odbiornik()
{
	czyscDane();
}
double Odbiornik::IteracjaKorelacji(size_t rozmiarDanych, const CiagRozpraszajacy& ciag, int opoznienie)
{

	long long suma = 0;
	size_t indeksCiagu = 0;
	for (size_t indeksDanych = 0; indeksDanych < rozmiarDanych; indeksDanych++)
	{
		if (indeksDanych != 0 && indeksDanych % INDEKS_PROBKI_DO_POMINIECIA == 0)
			indeksDanych++;

		suma += mDane[indeksDanych].I * ciag.mSygnal[indeksCiagu + opoznienie];

		indeksCiagu++;
	}
	return suma;
}

 // todo
void Odbiornik::SkupWidmo()
{
	CiagPn ciag(Odczepy::ILOSC_REJESTROW_I, Odczepy::ODCZEPY_I);
	long long suma = 0;

	USHORT offset = 0;
#define DLUGOSC_SKUPIANEGO_PN 4096

	auto iloscProbekDanych = static_cast<size_t>(ileProbek(26.6*1000, CZESTOTLIWOSC_PROBKOWANIA_DANYCH_MHZ));
	size_t indeksCiagu = 0;

	vector<WynikKorelacji> wynik;
	for (size_t i = 0; i < 512; i++)
	{
		PiszPostepPetli(i, 512);
		suma = IteracjaKorelacji(512, ciag, i);
		ciag.PrzesunDoOffsetu(i);
		wynik.push_back(WynikKorelacji(i, suma));
	}
	printf("skonczone, zrzucam...\n");
	fstream plik;
	plik.open("ASD.txt", ios::out);
	for (size_t i = 0; i < wynik.size(); i++)
	{
		plik << i << "\t" << wynik[i].wartosc << "\n";
	}
	plik.close();
}

bool Odbiornik::LadujNowyPlikDanych(const string& sciezkaDoPliku)
{
	czyscDane();
	CzytaczoPisaczBinarnychPlikow czytaczoPisaczPlikuDanych(sciezkaDoPliku);

	mRozmiarTablicyDanych = czytaczoPisaczPlikuDanych.zwrocRozmiarPliku() / sizeof(Data)-1;
	//mRozmiarTablicyDanych = 50;     // todo: debug hook
	mDane = new Data[mRozmiarTablicyDanych];
	memset(mDane, 0, mRozmiarTablicyDanych*sizeof(Data));
	return czytaczoPisaczPlikuDanych.Czytaj((char*)mDane, mRozmiarTablicyDanych*sizeof(Data));
}

void Odbiornik::SparsujDaneDoPliku(const std::string& sciezkaDoPliku)
{
    CzytaczoPisaczBinarnychPlikow::ZapiszPlik(mDane, mRozmiarTablicyDanych, sciezkaDoPliku);
}

void Odbiornik::PiszStacje()
{
    if (!mStacje.empty())
    {
        printf("\nStacje:\n");

        printf("Nr stacji\tLokalizacja\t\tCiagi PN\tDlugosc geo[E]\tSzerokosc geo[N]\n");
        for (size_t i = 0; i < mStacje.size(); i++)
        {
            printf("%d\t\t", mStacje[i].NrStacji);

			// ilosc tabow - ladne formatowanie 
            if (mStacje[i].Lokalizacja.size() < 8)
                printf("%s\t\t\t", mStacje[i].Lokalizacja.c_str());

            else if (mStacje[i].Lokalizacja.size() < 15)
                printf("%s\t\t", mStacje[i].Lokalizacja.c_str());

            else
                printf("%s\t", mStacje[i].Lokalizacja.c_str());


            printf("%d, %d, %d\t", mStacje[i].numeryCiagowPn[0], 
				mStacje[i].numeryCiagowPn[1], 
				mStacje[i].numeryCiagowPn[2]);

			mStacje[i].pozycjaGeoStacji.piszWspolrzedneDMS();

            printf("\n");
        }
        printf("\n");
    }
}
