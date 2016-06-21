#pragma once
#include "Struktury.h"
#include "RejestrPrzesuwny.h"
#include "Sygnal.h"
#include <memory>


class GeneratorCiagow
{
public:
	static const size_t DLUGOSC_CIAGU = 32768;
	
	static std::unique_ptr<SygnalBipolarny> generujCiagI();
	static std::unique_ptr<SygnalBipolarny> generujCiagQ();

private:
	static std::unique_ptr<SygnalBipolarny> generujCiagPn(RejestrPrzesuwny& rejestr);
};

