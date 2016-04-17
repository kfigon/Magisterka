#include "Sygnal.h"

SygnalBipolarny::SygnalBipolarny(const std::vector<int>& ciagBinarny) :
mCiag(ciagBinarny)
{
	for (size_t i = 0; i < ciagBinarny.size(); i++)
		mCiag[i] = map(ciagBinarny[i]);
}

SygnalBipolarny::SygnalBipolarny(const std::vector<int>& ciagBinarny, int indeksPierwszejJedynkiPoZerach) :
SygnalBipolarny(ciagBinarny)
{
	mIndeksPoczatku = indeksPierwszejJedynkiPoZerach;
}

SygnalBipolarny::SygnalBipolarny(const SygnalBipolarny& sygnalDoSkopiowania)
{
	for (size_t i = 0; i < sygnalDoSkopiowania.getDlugosc(); i++)
		mCiag.push_back(sygnalDoSkopiowania[i]);

	mIndeksPoczatku = sygnalDoSkopiowania.mIndeksPoczatku;
}