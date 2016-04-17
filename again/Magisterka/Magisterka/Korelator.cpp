#include "Korelator.h"

namespace Korelator
{
	std::vector<int> liczKorelacje(SygnalBipolarny& sygnal)
	{
		// sygnal oryginalny i jego kopia
		return liczKorelacje(sygnal, SygnalBipolarny{ sygnal });
	}

	// drugi sygnal jest przesuwany
	std::vector<int> liczKorelacje(SygnalBipolarny& a, SygnalBipolarny& b)
	{
		std::vector<int> outTab(a.getDlugosc(), 0);
		for (size_t i = 0; i < a.getDlugosc(); i++)
		{
			outTab[i] = liczIteracjeKorelacji(a, b);
			b.przesunWLewo();
		}
		return outTab;
	}

	int liczIteracjeKorelacji(SygnalBipolarny& a, SygnalBipolarny& b)
	{
		int wynik = 0;
		for (size_t i = 0; i < a.getDlugosc(); i++)
			wynik += a.getElement(i) * b.getElement(i);

		return wynik;
	}
}