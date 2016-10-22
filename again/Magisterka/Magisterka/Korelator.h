#include <vector>
#include "Sygnal.h"

namespace Korelator
{
	std::vector<int> liczKorelacje(SygnalBipolarny& sygnal);

	// drugi sygnal jest przesuwany
	std::vector<int> liczKorelacje(SygnalBipolarny& a, SygnalBipolarny& b);

	int liczIteracjeKorelacji(SygnalBipolarny& a, SygnalBipolarny& b);

	double liczWartoscSrednia(const std::vector<int>& ciag);
	std::vector<int> iloczynCiagow(const std::vector<int>& a, const std::vector<int>& b);
}