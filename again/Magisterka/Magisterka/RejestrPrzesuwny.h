#pragma once
#include <vector>
#include "Struktury.h"

// lista odczepow
// chcac dodac  x^16 + x^14 + x^13 + x^11 + 1
// robimy: std::vector<int> odczepy{ 16,14,13,11,0 };
class RejestrPrzesuwny
{
public:
	RejestrPrzesuwny(size_t dlugoscRejestryWBitach, const std::vector<int>& odczepy);
	~RejestrPrzesuwny() = default;

	int getVal();
	int ileWartosciMozeWygenerowac() const { return static_cast<int>(pow(2, getDlugoscRejestruWBitach()) - 1); }
	int getDlugoscRejestruWBitach() const { return mStan.size(); }

private:
	std::vector<int> mStan;
	const std::vector<int> mOdczepy;

	void przesun();
};