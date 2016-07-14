#include "GeneratorCiagow.h"

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagI()
{
	std::vector<int> odczepy{ 15, 13, 9, 8, 7, 5, 0 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagQ()
{
	std::vector<int> odczepy{ 15, 12, 11, 10, 6, 5, 4, 3, 0 };
	return generujCiagPn(RejestrPrzesuwny{ 15, odczepy });
}

std::unique_ptr<SygnalBipolarny> GeneratorCiagow::generujCiagPn(RejestrPrzesuwny& rejestr)
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
				// wstawiamy dodatkowe zero po 14 zerach
				data[i] = 0;
				indeksPierwszejJedynkiPoZerach = i + 1;
			}
		}
		else
			iloscZerPodRzad = 0;
	}
	return std::make_unique<SygnalBipolarny>(data, indeksPierwszejJedynkiPoZerach);
}
