#include "CzytaczoPisaczPlikow.h"


fstreamWrapper::fstreamWrapper(const string& sciezkaDoPliku) :
mSciezkaDoPliku(sciezkaDoPliku)
{
    mPlik.open(sciezkaDoPliku, ios::binary | ios::in | ios::ate);
    if (mPlik.is_open())
    {
		mRozmiarPliku = static_cast<size_t>(mPlik.tellg());
        mPlik.seekg(0);
    }
    else
    {
        mRozmiarPliku = 0;
		printf("Nie udalo sie przeczytac pliku %s\n", sciezkaDoPliku.c_str());
		assert(false);
    }
}

fstreamWrapper::~fstreamWrapper()
{
    mPlik.close();
}

CzytaczoPisaczBinarnychPlikow::CzytaczoPisaczBinarnychPlikow(const string& sciezkaDoPliku) :
fstreamWrapper(sciezkaDoPliku)
{}

bool CzytaczoPisaczBinarnychPlikow::Czytaj(char* buf, size_t rozmiarBufora)
{
    if (mPlik.is_open())
    {
        mPlik.read(buf, rozmiarBufora);
        printf("Przeczytano plik %s\n", mSciezkaDoPliku.c_str());
        return true;
    }
    else
    {
        printf("Nie udalo sie przeczytac pliku %s\n", mSciezkaDoPliku.c_str());
		assert(false);
        return false;
    }
}

void CzytaczoPisaczBinarnychPlikow::ZapiszPlik(Data* dane, size_t rozmiarTablicyDanych, const string& sciezkaDoPlikuWyjsciowego)
{
    fstream zapisDanych;
    zapisDanych.open(sciezkaDoPlikuWyjsciowego, ios::out);

    if (zapisDanych.is_open())
    {
        printf("Zapisuje dane do %s\n", sciezkaDoPlikuWyjsciowego.c_str());
        for (size_t i = 0; i < rozmiarTablicyDanych; i++)
        {
            zapisDanych << "I:\t" << dane[i].I << "\tQ:\t" << dane[i].Q << "\n";
			PiszPostepPetli(i, rozmiarTablicyDanych);
        }
        zapisDanych.close();
        printf("\n");
    }
}

void CzytaczoPisaczBinarnychPlikow::ZapiszPlik(char* dane, size_t rozmiar, const string& sciezkaDoPlikuWyjsciowego)
{
	fstream zapisDanych;
	zapisDanych.open(sciezkaDoPlikuWyjsciowego, ios::out);

	if (zapisDanych.is_open())
	{
		printf("Zapisuje dane do %s\n", sciezkaDoPlikuWyjsciowego.c_str());
		for (size_t i = 0; i < rozmiar; i++)
		{
			zapisDanych << i <<"\t" << (int)dane[i] << "\n";
			PiszPostepPetli(i, rozmiar);
		}
		zapisDanych.close();
		printf("\n");
	}
}

ParserStacjiBazowych::ParserStacjiBazowych(const string& sciezkaDoPliku) :
fstreamWrapper(sciezkaDoPliku)
{}

vector<Stacja> ParserStacjiBazowych::ParsujStacje()
{
    vector<Stacja> stacje;
    if (mPlik.is_open())
    {
        string pierwszaLinijka;		// naglowek do pominiecia
		getline(mPlik, pierwszaLinijka);

        while (!mPlik.eof())
        {
            string linijka;
            getline(mPlik, linijka);
            if (linijka == "" || linijka == "\r" || linijka == "\n")
                break;

			stacje.push_back(ParsujLinijke(linijka));
        }
    }
    else
    {
        printf("Blad odczytu pliku ze stacjami!\n");
    }
    return stacje;
}

Stacja ParserStacjiBazowych::ParsujLinijke(const string& linijka)
{
#define INDEKS_NR_STACJI		0
#define INDEKS_LOKALIZACJI		1
#define	INDEKS_CIAGOW_PN		2
#define	INDEKS_DLUGOSCI_GEO		3
#define	INDEKS_SZEROKOSCI_GEO	4

    const auto kolejneSegmentyParametrowStacjiPoTabulacjach = 
		podzielStringPoZnaku(linijka, '\t');

    Stacja stacja = { 0 };
	stacja.NrStacji = atoi(kolejneSegmentyParametrowStacjiPoTabulacjach[INDEKS_NR_STACJI].c_str());
	stacja.Lokalizacja = kolejneSegmentyParametrowStacjiPoTabulacjach[INDEKS_LOKALIZACJI];

	const auto ciagiPN = podzielStringPoZnaku(kolejneSegmentyParametrowStacjiPoTabulacjach[INDEKS_CIAGOW_PN], ',');
    for (size_t i = 0; i < ILOSC_CIAGOW_PN_NA_STACJE; i++)
    {
        stacja.numeryCiagowPn[i] = atoi(ciagiPN[i].c_str());
    }

	stacja.pozycjaGeoStacji.ustawPozycje(
		wydobadzWspolrzednezNapisu(kolejneSegmentyParametrowStacjiPoTabulacjach[INDEKS_DLUGOSCI_GEO]),
		wydobadzWspolrzednezNapisu(kolejneSegmentyParametrowStacjiPoTabulacjach[INDEKS_SZEROKOSCI_GEO]));
		
    return stacja;
}

vector<string> ParserStacjiBazowych::podzielStringPoZnaku(const string& napis, char znak)
{
    vector<string> kolejneSegmenty;
    string segment;

    for (size_t i = 0; i < napis.length(); i++)
    {
        if (napis[i] == znak)
        {
            kolejneSegmenty.push_back(segment);
            segment.clear();
            continue;
        }
        segment.push_back(napis[i]);
    }
    // ostatni segment nie jest przeciez rozdzielony
    kolejneSegmenty.push_back(segment);

    return kolejneSegmenty;
}

WspolrzedneDMS ParserStacjiBazowych::wydobadzWspolrzednezNapisu(const string& napisZeWspolrzednymiGeo)
{
	WspolrzedneDMS wsp = { 0 };
	const auto stopnieMinutySekundy = podzielStringPoZnaku(napisZeWspolrzednymiGeo, '°');
	wsp.Stopnie = atoi(stopnieMinutySekundy[0].c_str());

	const auto minutyiSekundy = podzielStringPoZnaku(stopnieMinutySekundy[1], '\'');
    wsp.Minuty = atoi(minutyiSekundy[0].c_str());
    wsp.Sekundy = atoi(minutyiSekundy[1].c_str());

    return wsp;
}