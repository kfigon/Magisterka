#include "Odbiornik.h"


void PrzygotujPlikDanychKorelacji(const std::string& sciezka, std::vector<WynikKorelacji>& wynikKorelacji)
{
	std::fstream plik;
	plik.open(sciezka.c_str(), std::ios::out);
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

void RysujWykres(const std::string& sciezkaDoPliku, std::vector<WynikKorelacji>& wynikKorelacji)
{
	PrzygotujPlikDanychKorelacji(sciezkaDoPliku, wynikKorelacji);

	std::stringstream polecenie;
	polecenie << "python Wykres.py " << sciezkaDoPliku;
	system(polecenie.str().c_str());
}

long long liczIteracje(std::vector<Data>& dane, SygnalBipolarny& ciagI)
{
	long long suma = 0;
	for (size_t i = 0; i < dane.size(); i++)
		suma += dane[i].I * ciagI.getElement(i);
	
	return suma;
}

void liczKorelacje(std::vector<Data>& dane, int coKtoraPominac, CzytaczoPisaczBinarnychPlikow::Endian endian)
{
	cout << "Wynik dla: " << coKtoraPominac << "\n";
	auto* ciagI = GeneratorCiagow::generujCiagI();

	vector<WynikKorelacji> outTab(dane.size());

	for (size_t i = 0; i < outTab.size(); i++)
	{
		PiszPostepPetli(i, outTab.size());
		outTab[i].offset = i;
		outTab[i].wartosc = liczIteracje(dane, *ciagI);

		ciagI->przesunWLewo();
		if ((i % coKtoraPominac == 0) && (i != 0))
			ciagI->przesunWLewo();
	}

	std::stringstream nazwaPliku;
	nazwaPliku << "ASD_" << coKtoraPominac <<
		((endian == CzytaczoPisaczBinarnychPlikow::Endian::Little) ? "_le" : "_be")
		<< ".txt";

	RysujWykres(nazwaPliku.str(), outTab);

	delete ciagI;
}
