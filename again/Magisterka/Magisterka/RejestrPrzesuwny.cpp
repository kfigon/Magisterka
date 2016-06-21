#include "RejestrPrzesuwny.h"

RejestrPrzesuwny::RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy) :
mOdczepy(odczepy),
mDlugoscRejestryWBitach(dlugoscRejestryWBitach)
{}

int RejestrPrzesuwny::getVal()
{
	UINT wynik = 0;

	for (UINT nrOdczepu : mOdczepy)
		wynik ^= ((mStan >> (mDlugoscRejestryWBitach - nrOdczepu)) & 1);

	mStan >>= 1;
	mStan |= (wynik << (mDlugoscRejestryWBitach - 1));
	return wynik;
}