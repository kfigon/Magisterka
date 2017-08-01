#pragma once
#include "Struktury.h"
#include "RejestrPrzesuwny.h"
#include "Sygnal.h"
#include <memory>

class GeneratorCiagow
{
public:
	// 2^15
	static const size_t DLUGOSC_CIAGU_PN = 32768;
    static const size_t DLUGOSC_CIAGU_WASHA = 64;

	static std::unique_ptr<SygnalBipolarny> generujCiagI();
	static std::unique_ptr<SygnalBipolarny> generujCiagQ();
    static std::vector<int> generujBinarnyCiagWalsha(int nr);
    static std::unique_ptr<SygnalBipolarny> generujCiagWalsha(int nr);

    // MSB...LSB
    static std::vector<int> zamienNaBinarne(int num);
private:
	static std::unique_ptr<SygnalBipolarny> generujCiagPn(RejestrPrzesuwny& rejestr);
    static int liczIleBitowTrzeba(int num);
    static void dopiszCiag(std::vector<int>& ciag, size_t& indeksCiagu, bool czyOdwracacBity);
};

