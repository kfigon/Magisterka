#include "RejestrPrzesuwny.h"
#include <assert.h>

RejestrPrzesuwny::RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy) :
mOdczepy(odczepy),
mStan(dlugoscRejestryWBitach, 1)
{
	// najwyzsza wartosc odczepy powinna byc rowna dlugosci - odczep na pierwszej komorce
	assert(dlugoscRejestryWBitach == mOdczepy[0]);
}

void RejestrPrzesuwny::przesun()
{
	for (int i = 0; i < mStan.size() - 1; i++)
		mStan[i] = mStan[i + 1];
}

int RejestrPrzesuwny::getVal()
{
	auto wynikXora = 0;

	for (size_t i = 1; i < mOdczepy.size(); i++)
		wynikXora ^= mStan[mOdczepy[i]];
		

	const auto ostatni = mStan[0];
	przesun();

	// nadpisz ostatni
	mStan[mOdczepy[0] - 1] = wynikXora;

	return ostatni;
}