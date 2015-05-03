#pragma once
#include <iostream>
#include <vector>
using namespace std;

#include "Struktury.h"
#include "RejestrPrzesuwny.h"

// bipolaryzacja
#define mapujBit(bit)	((bit == 0) ? 1 : -1)

class CiagRozpraszajacy
{
public:
	vector<char> mSygnal;

	CiagRozpraszajacy() = default;
	virtual ~CiagRozpraszajacy() = default;

	void piszCiag() const;
	void przesunCyklicznieWprawo();
	void przesunCykliczniewLewo();

protected:
	virtual void generujCiag() = 0;
	void przesunRejestrWprawo();
};

class CiagWalsha : public CiagRozpraszajacy
{
private:
	const UCHAR mStopienCiagu;	//ilu bitowy ciag
	const UCHAR mIndeksCiagu;

public:
	CiagWalsha(const UCHAR& stopienCiagu, const UCHAR& indeksCiagu);
	~CiagWalsha() = default;

private:
	void generujCiag() override;
};

// przykladowe uzycie: CiagPn ciag(5, ODCZEPY_1 | ODCZEPY_x2 | ODCZEPY_x5);
// da to wielomian 1 + x2 + x5. Pamietac ze nie ma x1, od razu po 1 jest x2!
class CiagPn : public CiagRozpraszajacy
{
private:
	const UINT mMaksymalnaDlugoscCiagu;
	RejestrPrzesuwny mRejestr;
	
public:
	CiagPn(const UCHAR& iloscRejestrowRejestruPrzesuwnego, const USHORT& odczepy);
	~CiagPn() = default;

	void PrzesunDoOffsetu(const USHORT& offsetPn);

private:
	void generujCiag() override;
};