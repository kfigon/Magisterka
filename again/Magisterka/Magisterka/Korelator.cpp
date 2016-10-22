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

	double liczWartoscSrednia(const std::vector<int>& ciag)
	{
		int suma = 0;
		for (auto i : ciag)
			suma += i;

		return static_cast<double>(suma) / static_cast<double>(ciag.size());
	}

	std::vector<int> liczIloczynCiagow(const std::vector<int>& a, const std::vector<int>& b)
	{
		if (a.size() != b.size())
			return std::vector<int>();

		std::vector<int> out(a.size(), 0);

		for (size_t i = 0; i < a.size(); i++)
		{
			out[i] = a[i] * b[i];
		}
		return out;
	}

	double liczWariancje(const std::vector<int>& ciag)
	{
		double suma = 0;
		const auto wartoscSrednia = liczWartoscSrednia(ciag);
		for (auto i : ciag)
			suma += pow((i - wartoscSrednia), 2);

		suma /= static_cast<double>(ciag.size());

		return sqrt(suma);
	}

	double liczWariancje(const std::vector<int>& a, const std::vector<int>& b)
	{
		const auto iloczynCiagow = liczIloczynCiagow(a, b);
		const auto wartoscSredniaIloczynu = liczWartoscSrednia(iloczynCiagow);
		const auto wartoscSredniaA = liczWartoscSrednia(a);
		const auto wartoscSredniaB = liczWartoscSrednia(b);

		return wartoscSredniaIloczynu - wartoscSredniaA*wartoscSredniaB;
	}

	double liczWspolczynnikKorelacji(const std::vector<int>& a, const std::vector<int>& b)
	{
		return liczWariancje(a, b) / (liczWariancje(a) * liczWariancje(b));
	}
}