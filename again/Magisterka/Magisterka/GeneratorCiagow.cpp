#include "GeneratorCiagow.h"

SygnalBipolarny* GeneratorCiagow::generujCiagI()
{
	std::vector<int> odczepy{ 5, 7, 8, 9, 13, 15 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

SygnalBipolarny* GeneratorCiagow::generujCiagQ()
{
	std::vector<int> odczepy{ 3, 4, 5, 6, 10, 11, 12, 15 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

SygnalBipolarny* GeneratorCiagow::generujCiagPn(RejestrPrzesuwny& rejestr)
{
	std::vector<int> data(GeneratorCiagow::DLUGOSC_CIAGU, 0);
	int iloscZerPodRzad = 0;
	int indeksPierwszejJedynkiPoZerach = 0;

	for (size_t i = 0; i < data.size(); i++)
	{
		auto input = rejestr.getVal();
		data[i] = input;
		if (input == 0)
		{
			iloscZerPodRzad++;
			if (iloscZerPodRzad == 14)
			{
				DbgPrint("14 zer pod rzad pod indeksem %d", i);
				i++;
				data[i] = 0;
				indeksPierwszejJedynkiPoZerach = i + 1;
			}
		}
		else
			iloscZerPodRzad = 0;
	}

	return new SygnalBipolarny{ data, indeksPierwszejJedynkiPoZerach };
}
