#pragma once
#include "Struktury.h"
#include <vector>
using namespace std;

namespace Odczepy
{
	enum Odczepy
	{
		ODCZEPY_1 = 0x1,
		ODCZEPY_x2 = 0x2,
		ODCZEPY_x3 = 0x4,
		ODCZEPY_x4 = 0x8,
		ODCZEPY_x5 = 0x10,
		ODCZEPY_x6 = 0x20,
		ODCZEPY_x7 = 0x40,
		ODCZEPY_x8 = 0x80,
		ODCZEPY_x9 = 0x100,
		ODCZEPY_x10 = 0x200,
		ODCZEPY_x11 = 0x400,
		ODCZEPY_x12 = 0x800,
		ODCZEPY_x13 = 0x1000,
		ODCZEPY_x14 = 0x2000,
		ODCZEPY_x15 = 0x4000
	};
	const int ILOSC_REJESTROW_I = 15;
	const int ILOSC_REJESTROW_Q = 15;

	const int ODCZEPY_I = ODCZEPY_x15 | ODCZEPY_x13 | ODCZEPY_x9 | ODCZEPY_x8 | ODCZEPY_x7 | ODCZEPY_x5 | ODCZEPY_1;
	const int ODCZEPY_Q = ODCZEPY_x15 | ODCZEPY_x12 | ODCZEPY_x11 | ODCZEPY_x10 | ODCZEPY_x6 | ODCZEPY_x5 | ODCZEPY_x4 | ODCZEPY_x3 | ODCZEPY_1;

}

// przykladowe uzycie: Rejestr rejestr(5, ODCZEPY_1 | ODCZEPY_x2 | ODCZEPY_x5);
// da to wielomian 1 + x2 + x5. Pamietac ze nie ma x1, od razu po 1 jest x2!
class RejestrPrzesuwny
{
private:
	const UCHAR mIloscRejestrowRejestruPrzesuwnego;
	const USHORT mOdczepy;	// maska bitowa
	vector<char> mRejestrPrzesuwny;

public:
	RejestrPrzesuwny(const UCHAR& iloscRejestrowRejestruPrzesuwnego, const USHORT& odczepy);
	~RejestrPrzesuwny() = default;

	char generujBit();
	const char zwrocOstatniElement() const{ return mRejestrPrzesuwny[mIloscRejestrowRejestruPrzesuwnego - 1]; }

private:
	void przesunRejestrCyklicznieWprawo();
	void przesunRejestrWprawo();
};
