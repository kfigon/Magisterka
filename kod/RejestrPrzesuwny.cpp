#include "RejestrPrzesuwny.h"

RejestrPrzesuwny::RejestrPrzesuwny(const UCHAR& iloscRejestrowRejestruPrzesuwnego, const USHORT& odczepy) :
mIloscRejestrowRejestruPrzesuwnego(iloscRejestrowRejestruPrzesuwnego),
mOdczepy(odczepy)
{
	for (size_t i = 0; i < iloscRejestrowRejestruPrzesuwnego; i++)
	{
		mRejestrPrzesuwny.push_back(0);
	}
	// stan poczatkowy to 1 i zera
	mRejestrPrzesuwny[0] = 1;
}

char RejestrPrzesuwny::generujBit()
{
	char wyjscieSumatora = 0;
	for (size_t i = 1; i < mIloscRejestrowRejestruPrzesuwnego; i++)
	{
		if (((mOdczepy >> i) & 1) == 1)
		{
			wyjscieSumatora ^= mRejestrPrzesuwny[i];
		}
	}
	przesunRejestrWprawo();
	mRejestrPrzesuwny[0] = wyjscieSumatora;

	return zwrocOstatniElement();
}

void RejestrPrzesuwny::przesunRejestrCyklicznieWprawo()
{
	const char ostatniElement = mRejestrPrzesuwny[mRejestrPrzesuwny.size() - 1];
	przesunRejestrWprawo();
	mRejestrPrzesuwny[0] = ostatniElement;
}

void RejestrPrzesuwny::przesunRejestrWprawo()
{
	for (int i = mRejestrPrzesuwny.size() - 1; i >= 1; i--)
	{
		mRejestrPrzesuwny[i] = mRejestrPrzesuwny[i - 1];
	}
	mRejestrPrzesuwny[0] = 0;
}
