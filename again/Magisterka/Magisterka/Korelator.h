#include <vector>
#include "Sygnal.h"

namespace Korelator
{
	std::vector<int> liczKorelacje(SygnalBipolarny& sygnal);

	// drugi sygnal jest przesuwany
	std::vector<int> liczKorelacje(SygnalBipolarny& a, SygnalBipolarny& b);

	int liczIteracjeKorelacji(SygnalBipolarny& a, SygnalBipolarny& b);
}