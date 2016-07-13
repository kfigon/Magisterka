#include "RejestrPrzesuwny.h"

RejestrPrzesuwny::RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy, long long stanPoczatkowy) :
mOdczepy(odczepy),
mDlugoscRejestryWBitach(dlugoscRejestryWBitach)
{
	mStan = stanPoczatkowy;
}

int RejestrPrzesuwny::getVal()
{
	UINT wynik = 0;

	for (UINT nrOdczepu : mOdczepy)
	{
		wynik ^= ((mStan >> (mDlugoscRejestryWBitach - nrOdczepu)) & 1);
	}
	mStan >>= 1;
	mStan |= (wynik << (mDlugoscRejestryWBitach - 1));
	return wynik;
}


//#include "RejestrPrzesuwny.h"
//
//RejestrPrzesuwnyAsd::RejestrPrzesuwnyAsd(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy, long long stanPoczatkowy) :
//mOdczepy(odczepy),
//mStan(dlugoscRejestryWBitach, 0)
//{
//	//mStan = stanPoczatkowy;
//	mStan[0] = 1;
//}
//
////int RejestrPrzesuwnyAsd::getVal()
////{
////	UINT wynik = 0;
////
////	for (UINT nrOdczepu : mOdczepy)
////	{
////		wynik ^= ((mStan >> (mDlugoscRejestryWBitach - nrOdczepu)) & 1);
////	}
////	mStan >>= 1;
////	mStan |= (wynik << (mDlugoscRejestryWBitach - 1));
////	return wynik;
////}
//
//void RejestrPrzesuwnyAsd::przesun()
//{
//	for (int i = mStan.size() - 2; i >= 0; i--)
//		mStan[i + 1] = mStan[i];
//}
//
//int RejestrPrzesuwnyAsd::getVal()
//{
//	auto wynikXora = 0;
//
//	for (auto odczep : mOdczepy)
//		wynikXora ^= mStan[odczep - 1];
//
//	const auto ostatni = mStan[getDlugoscRejestruWBitach() - 1];
//
//	przesun();
//	mStan[0] = wynikXora;
//
//	return ostatni;
//
//	//wynikXora ^= self.__stan[odczep - 1]
//
//	//	ostatni = self.__stan[self.__iloscRejestrow - 1]
//	//	self.__przesun()
//	//	self.__stan[0] = wynikXora
//
//	//	return ostatni
//}