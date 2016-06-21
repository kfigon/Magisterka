#include "Odbiornik.h"


void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<WynikKorelacji>& wynikKorelacji)
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

void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<WynikKorelacji>& wynikKorelacji)
{
	PrzygotujPlikDanychKorelacji(sciezkaDoPliku, wynikKorelacji);

	std::stringstream polecenie;
	polecenie << "python Wykres.py " << sciezkaDoPliku;
	system(polecenie.str().c_str());
}

long long liczIteracje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI)
{
	long long suma = 0;
	for (size_t i = 0; i < dane.size(); i++)
		suma += dane[i].I * ciagI.getElement(i);
	
	return suma;
}

void liczKorelacje(std::vector<Data>& dane, int coKtoraPominac, BinaryReader::Endian endian)
{
	cout << "Wynik dla: " << coKtoraPominac << "\n";
	auto ciagI = GeneratorCiagow::generujCiagI();

	vector<WynikKorelacji> outTab(dane.size(), 0);

	for (size_t i = 0; i < outTab.size(); i++)
	{
		PiszPostepPetli(i, outTab.size());
		outTab[i].offset = i;
		outTab[i].wartosc = liczIteracje(dane, *ciagI);/*/GeneratorCiagow::DLUGOSC_CIAGU;*/

		ciagI->przesunWLewo();
		if ((i % coKtoraPominac == 0) && (i != 0))
			ciagI->przesunWLewo();
	}

	std::stringstream nazwaPliku;
	nazwaPliku << "ASD_" << coKtoraPominac <<
		((endian == BinaryReader::Endian::Little) ? "_le" : "_be")
		<< ".txt";

	RysujWykres(nazwaPliku.str(), outTab);
}
