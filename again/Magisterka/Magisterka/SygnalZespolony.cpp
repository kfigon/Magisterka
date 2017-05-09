#include "SygnalZespolony.h"

SygnalZespolony::SygnalZespolony(size_t rozmiar) :
mTab(rozmiar, 0)
{}

SygnalZespolony::SygnalZespolony(std::vector<std::complex<int>> tab) :
mTab(tab)
{}

SygnalZespolony::SygnalZespolony(std::vector<int> rzeczywiste) :
mTab(rzeczywiste.size(), 0)
{
	for (size_t i = 0; i < mTab.size(); i++)
	{
		mTab[i] = { rzeczywiste[i], 0 };
	}
}


std::complex<int> SygnalZespolony::get(int idx) const
{
	if (idx < 0 || idx >= getSize())
		return std::complex<int>{0, 0};

	return mTab[idx];
}

std::vector<std::complex<int>> liczKorelacje(const SygnalZespolony& a, const SygnalZespolony& b)
{
#define max(a,b) (a>b)?a:b

	const int dlugosc = max(a.getSize(), b.getSize()) * 2 - 1;
	std::vector<std::complex<int>> out(dlugosc, { 0, 0 });
	const int zakres = static_cast<int>(dlugosc / 2); // podloga z tego
	// <-zakres : zakres>

	int indeksWyniku = 0;
	for (int l = -1 * zakres; l <= zakres; l++)
	{
		std::complex<int> wynikIteracji{ 0, 0 };
		for (int n = 0; n < dlugosc; n++)
		{
			const auto x = a.get(n + l);
			const auto y = std::conj(b.get(n));

			wynikIteracji += x*y;
		}
		out[indeksWyniku] = wynikIteracji;
		indeksWyniku++;
	}

	return out;
}
