#pragma once

#include "CiagRozpraszajacy.h"

void CiagRozpraszajacy::piszCiag() const
{
	for (size_t i = 0; i < mSygnal.size(); i++)
	{
		if (i % 4 == 0)
			printf(" ");

		printf("%d", mSygnal[i]);
	}
	printf("\n");
}

void CiagRozpraszajacy::przesunCyklicznieWprawo()
{
	const char ostatniElement = mSygnal[mSygnal.size() - 1];
	przesunRejestrWprawo();
	mSygnal[0] = ostatniElement;
}

void CiagRozpraszajacy::przesunRejestrWprawo()
{
	for (int i = mSygnal.size() - 1; i >= 1; i--)
	{
		mSygnal[i] = mSygnal[i - 1];
	}

	// jesli sygnal ma byc binarny to na poczatku powinno byc 0, jak bipolarny to jeden.
	// wynika z mapowania bipolarny - binarny
	mSygnal[0] = mapujBit(0);
}

void CiagRozpraszajacy::przesunCykliczniewLewo()
{
	auto pierwszyElement = mSygnal[0];

	for (size_t i = 0; i < mSygnal.size()-1; i++)
	{
		mSygnal[i] = mSygnal[i+1];
	}
	mSygnal[mSygnal.size() - 1] = pierwszyElement;
}

CiagWalsha::CiagWalsha(const UCHAR& stopienCiagu, const UCHAR& indeksCiagu) :
CiagRozpraszajacy(),
mStopienCiagu(stopienCiagu),
mIndeksCiagu(indeksCiagu)
{
	generujCiag();
}

void CiagWalsha::generujCiag()
{
	const char jedynka = -1;
	const char zero = 1;


	// pierwszy element w ciagach Walsha to zawsze 0
	mSygnal.push_back(zero);

	// wez najmlodszy bit
	UCHAR bit = mIndeksCiagu & 1;
	const UCHAR ileBitowwRzedzieCiaguWalsha = static_cast<UCHAR>(ceil(log2(mStopienCiagu)));

	for (size_t numerBitu = 1; numerBitu <= ileBitowwRzedzieCiaguWalsha; numerBitu++)
	{
		// bez tej zmiennej ciag.size() bedzie w kazdej iteracji sie zwiekszac i mamy nieskonczona petle ;)
		int ileDopisac = mSygnal.size();
		if (bit == 0)
		{
			for (int i = 0; i < ileDopisac; i++)
			{
				mSygnal.push_back(mSygnal[i]);
			}
		}
		else
		{
			for (int i = 0; i < ileDopisac; i++)
			{
				const char odwroconyBit = (mSygnal[i] == jedynka) ? zero : jedynka;
				mSygnal.push_back(odwroconyBit);
			}
		}
		bit = (mIndeksCiagu >> numerBitu) & 1;
	}
}

CiagPn::CiagPn(const UCHAR& iloscRejestrowRejestruPrzesuwnego, const USHORT& odczepy) :
CiagRozpraszajacy(),
mMaksymalnaDlugoscCiagu(static_cast<UINT>(pow(2, iloscRejestrowRejestruPrzesuwnego) - 1)),
mRejestr(iloscRejestrowRejestruPrzesuwnego, odczepy)
{
	generujCiag();
}

void CiagPn::generujCiag()
{
	mSygnal.push_back(mapujBit(mRejestr.zwrocOstatniElement()));

	for (size_t i = 1; i < mMaksymalnaDlugoscCiagu; i++)
	{
		mSygnal.push_back(mapujBit(mRejestr.generujBit()));
	}
}

void CiagPn::PrzesunDoOffsetu(const USHORT& offsetPn)
{
	// todo: 63-64?!
	for (size_t i = 0; i < offsetPn * 64; i++)
	{
		przesunCyklicznieWprawo();
	}
}
