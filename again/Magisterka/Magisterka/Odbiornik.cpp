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

long long liczIteracje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI, int coKtoraPominac)
{
	long long suma = 0;
	size_t indeksCiagu = 0;
	
	for (size_t indeksDanych = 0; indeksDanych < dane.size(); indeksDanych++)
	{
		const auto pierwszy = dane[indeksDanych].I * ciagI[indeksCiagu];
		const auto drugi = dane[indeksDanych].Q * ciagI[indeksCiagu];
		
		const auto potegaPierwszego = pow(pierwszy, 2);
		const auto potegaDrugiego = pow(drugi, 2);
		const long long sums = potegaDrugiego + potegaPierwszego;
		const auto asd = sqrt(sums);
		
		suma += sqrt(
			pow(pierwszy, 2) +
			pow(drugi, 2));


		// fs danych = 1,28MHz, szybkosc transmisji 1,2288Mcps
		// wiec sie rozjedzie po 24 probkach
		if ((indeksDanych % coKtoraPominac == 0) && (indeksDanych != 0))
			indeksDanych++; /*ciagI->przesunWLewo();*/

		indeksCiagu++;
	}
	return suma;
}

std::vector<WynikKorelacji> liczKorelacje(std::vector<Data>& dane, int coKtoraPominac)
{
	cout << "Wynik dla: " << coKtoraPominac << "\n";
	auto ciagI = GeneratorCiagow::generujCiagI();
	auto ciagQ = GeneratorCiagow::generujCiagQ();
	std::vector<WynikKorelacji> outTab(dane.size(), 0);

	for (size_t i = 0; i < outTab.size(); i++)
	{
		PiszPostepPetli(i, outTab.size());
		outTab[i].offset = i;

		long long real = 0;
		long long imag = 0;
		for (size_t indeksDanych = 0; indeksDanych < dane.size(); indeksDanych++)
		{
			auto cI = ciagI->getElement(indeksDanych + i);
			auto cQ = 0; // ciagQ->getElement(indeksCiagu);
			auto dI = dane[indeksDanych].I;	
			auto dQ = dane[indeksDanych].Q;

			real = cI*dI + cQ*dQ;
			imag = cQ*dI - dQ*cI;	// sprzezenie included

			// fs danych = 1,28MHz, szybkosc transmisji 1,2288Mcps
			// wiec sie rozjedzie po 24 probkach
			//if (coKtoraPominac != 0)
			//{
			//	if ((indeksDanych % coKtoraPominac == 0) && (indeksDanych != 0))
			//		indeksDanych++; /*ciagI->przesunWLewo();*/
			//}
			auto len = sqrt(real*real + imag*imag);
			outTab[i].wartosc += static_cast<long long>(len);

		}

		//ciagI->przesunWLewo();
		//ciagQ->przesunWLewo();
	}
	printf("\n");

	return outTab;
}
