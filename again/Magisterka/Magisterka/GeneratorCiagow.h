#pragma once
#include "Struktury.h"
#include "RejestrPrzesuwny.h"
#include "Sygnal.h"

// klasa alokuje pamiec!
class GeneratorCiagow
{
public:
	static const size_t DLUGOSC_CIAGU = 32768;

	static SygnalBipolarny* generujCiagI();
	static SygnalBipolarny* generujCiagQ();

private:
	static SygnalBipolarny* generujCiagPn(RejestrPrzesuwny& rejestr);
};

