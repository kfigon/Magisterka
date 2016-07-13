#pragma once
#include <vector>
#include "Struktury.h"

// lista odczepow
// jedynka ignorowana
// chcac dodac  x^16 + x^14 + x^13 + x^11 + 1
// robimy: std::vector<int> odczepy{ 11, 13, 14, 16 };
class RejestrPrzesuwny
{
public:

	// stan poczatkowy 0b1101; dowolny stan jest ok
	RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy, long long stanPoczatkowy = 13);
	~RejestrPrzesuwny() = default;

	int getVal();
	int ileWartosciMozeWygenerowac() const { return pow(2, mDlugoscRejestryWBitach) - 1; }

private:
	long long mStan;
	size_t mDlugoscRejestryWBitach = 0;
	std::vector<int> mOdczepy;
};

//class RejestrPrzesuwnyAsd
//{
//public:
//
//	// stan poczatkowy 0b1101; dowolny stan jest ok
//	RejestrPrzesuwnyAsd(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy, long long stanPoczatkowy = 13);
//	~RejestrPrzesuwnyAsd() = default;
//
//	int getVal();
//	int ileWartosciMozeWygenerowac() const { return static_cast<int>(pow(2, getDlugoscRejestruWBitach()) - 1); }
//	int getDlugoscRejestruWBitach() const { return mStan.size(); }
//
//private:
//	std::vector<int> mStan;
//	std::vector<int> mOdczepy;
//
//	void przesun();
//};