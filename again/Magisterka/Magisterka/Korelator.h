#include <vector>
#include "Sygnal.h"

namespace Korelator
{
	std::vector<int> liczKorelacje(SygnalBipolarny& sygnal);

	// drugi sygnal jest przesuwany
	std::vector<int> liczKorelacje(SygnalBipolarny& a, SygnalBipolarny& b);

	int liczIteracjeKorelacji(SygnalBipolarny& a, SygnalBipolarny& b);

	double liczWartoscSrednia(const std::vector<int>& ciag);
	std::vector<int> liczIloczynCiagow(const std::vector<int>& a, const std::vector<int>& b);
	double liczWariancje(const std::vector<int>& ciag);
	double liczWariancje(const std::vector<int>& a, const std::vector<int>& b);
	double liczWspolczynnikKorelacji(const std::vector<int>& a, const std::vector<int>& b);
}