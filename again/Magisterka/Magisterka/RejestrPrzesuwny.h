#pragma once
#include <vector>
#include "Struktury.h"

// lista odczepow, w formie:
// 11,13,14,16
// 1 == x1 :(, jedynka ignorowana
class RejestrPrzesuwny
{
public:

	RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy);
	~RejestrPrzesuwny() = default;

	int getVal();

private:
	const long long mStanPoczatkowy = 13; // 0b1101; dowolny stan jest ok

	long long mStan = mStanPoczatkowy;
	size_t mDlugoscRejestryWBitach = 0;
	std::vector<int> mOdczepy;
};