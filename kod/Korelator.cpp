#include "Korelator.h"

namespace Korelator
{
	void RysujWykres(const std::string& sciezkaDoPliku, vector<WynikKorelacji>& wynikKorelacji)
	{
		PrzygotujPlikDanychKorelacji(sciezkaDoPliku, wynikKorelacji);

		stringstream polecenie;
		polecenie << "python Wykres.py " << sciezkaDoPliku;
		system(polecenie.str().c_str());
	}

	vector<WynikKorelacji> LiczKorelacjeDlaI(Data* dane, CiagRozpraszajacy &ciag)
	{
		FuncEntry();

		vector<WynikKorelacji> wynikObliczen;

		long long suma = 0;

		for (size_t i = 0; i < ciag.mSygnal.size(); i++)
		{
			size_t indeksKorelacji = 0;
			PiszPostepPetli(i, ciag.mSygnal.size());

			suma = 0;
			for (size_t j = 0; j < ciag.mSygnal.size(); j++)
			{
				if (j % INDEKS_PROBKI_DO_POMINIECIA == 0)
					indeksKorelacji++;

				suma += dane[indeksKorelacji].I * ciag.mSygnal[j];
				indeksKorelacji++;
			}
			wynikObliczen.push_back(WynikKorelacji(i, suma));
			ciag.przesunCyklicznieWprawo();
		}
		return wynikObliczen;
	}

	vector<WynikKorelacji> LiczKorelacjeDlaQ(Data* dane, CiagRozpraszajacy &ciag)
	{
		vector<WynikKorelacji> wynikObliczen;

		for (size_t offset = 0; offset < ciag.mSygnal.size(); offset++)
		{
			long long suma = 0;
			PiszPostepPetli(offset, ciag.mSygnal.size());

			for (size_t indeksProbki = offset; indeksProbki < ciag.mSygnal.size(); indeksProbki++)
			{
				if (indeksProbki % INDEKS_PROBKI_DO_POMINIECIA == 0)
					continue;

				suma += dane[indeksProbki].Q * ciag.mSygnal[indeksProbki];
			}
			wynikObliczen.push_back(WynikKorelacji(offset, suma));
		}
		return wynikObliczen;
	}

	void PrzygotujPlikDanychKorelacji(const string& sciezka, vector<WynikKorelacji>& wynikKorelacji)
	{
		fstream plik;
		plik.open(sciezka.c_str(), ios::out);
		if (plik.is_open())
		{
			for (size_t i = 0; i < wynikKorelacji.size(); i++)
			{
				PiszPostepPetli(i, wynikKorelacji.size());
				plik << wynikKorelacji[i].offset << "\t" << wynikKorelacji[i].wartosc << "\n";
			}
		}
		plik.close();
	}

	WynikKorelacji SzukajMaksa(const vector<WynikKorelacji>& wynikKorelacji)
	{
		WynikKorelacji wynik(0, 0);
		return wynik;
	}
}