#include "Geografia.h"
#include <iostream>
#include <cmath>

PozycjaGeograficzna::PozycjaGeograficzna() :
mPozycjaDD(0, 0),
mPozycjaDMS(konwertuj(mPozycjaDD))
{}

PozycjaGeograficzna::PozycjaGeograficzna(const PozycjaGeograficznaDMS& pozycja):
mPozycjaDMS(pozycja),
mPozycjaDD(konwertuj(pozycja))
{}

PozycjaGeograficzna::PozycjaGeograficzna(const WspolrzedneDMS& dlugosc, const WspolrzedneDMS& szerokosc):
mPozycjaDMS(dlugosc, szerokosc),
mPozycjaDD(konwertuj(mPozycjaDMS))
{}

PozycjaGeograficzna::PozycjaGeograficzna(const PozycjaGeograficznaStopnieDziesietne& pozycja):
mPozycjaDD(pozycja),
mPozycjaDMS(konwertuj(mPozycjaDD))
{}

PozycjaGeograficzna::PozycjaGeograficzna(const double& dlugosc, const double& szerokosc):
mPozycjaDD(dlugosc, szerokosc),
mPozycjaDMS(konwertuj(mPozycjaDD))
{}

void PozycjaGeograficzna::piszWspolrzedneDMS()
{
	printf("%d ,%d\',%d\" [E]\t", mPozycjaDMS.Dlugosc.Stopnie,
		mPozycjaDMS.Dlugosc.Minuty,
		mPozycjaDMS.Dlugosc.Sekundy);

	printf("%d ,%d\',%d\" [N]", mPozycjaDMS.Szerokosc.Stopnie,
		mPozycjaDMS.Szerokosc.Minuty,
		mPozycjaDMS.Szerokosc.Sekundy);
}

double PozycjaGeograficzna::liczOdlegloscGeograficzna(const PozycjaGeograficzna& skad, const PozycjaGeograficzna& dokad)
{
	return liczOdlegloscGeograficzna(
		konwertuj(skad.mPozycjaDMS.Szerokosc),
		konwertuj(dokad.mPozycjaDMS.Szerokosc),
		konwertuj(skad.mPozycjaDMS.Dlugosc),
		konwertuj(dokad.mPozycjaDMS.Dlugosc));
}

double PozycjaGeograficzna::liczOdlegloscGeograficzna(const double& szerokoscA, const double& szerokoscB, const double& dlugoscA, const double& dlugoscB)
{
	const double deltaDlugosci = radiany(dlugoscB) - radiany(dlugoscA);
	const double deltaSzerokosci = radiany(szerokoscB) - radiany(szerokoscA);

	const double temp = sin(deltaSzerokosci / 2)*sin(deltaSzerokosci / 2) +
		cos(radiany(szerokoscA))*cos(radiany(szerokoscB))*sin(deltaDlugosci / 2)*sin(deltaDlugosci / 2);
	const double temp2 = 2 * atan2(sqrt(temp), sqrt(1 - temp));
	const double odleglosc = 6371 * temp2;

	return odleglosc;
}

PozycjaGeograficznaDMS PozycjaGeograficzna::konwertuj(const PozycjaGeograficznaStopnieDziesietne& pozycja)
{
	PozycjaGeograficznaDMS out = { 0 };
	out.Dlugosc = konwertuj(pozycja.Dlugosc);
	out.Szerokosc = konwertuj(pozycja.Szerokosc);
	return out;
}

PozycjaGeograficznaStopnieDziesietne PozycjaGeograficzna::konwertuj(const PozycjaGeograficznaDMS& pozycja)
{
	PozycjaGeograficznaStopnieDziesietne out = { 0 };
	out.Dlugosc = konwertuj(pozycja.Dlugosc);
	out.Szerokosc = konwertuj(pozycja.Szerokosc);
	return out;
}

double PozycjaGeograficzna::konwertuj(const WspolrzedneDMS& wspolrzedna)
{
	return wspolrzedna.Stopnie + (double)wspolrzedna.Minuty / 60 + (double)wspolrzedna.Sekundy / 3600;
}

WspolrzedneDMS PozycjaGeograficzna::konwertuj(const double& wspolrzedna)
{
	WspolrzedneDMS out = { 0 };
	out.Stopnie = (unsigned char)(wspolrzedna);
	const double czescMinut = (wspolrzedna - out.Stopnie) * 60;
	out.Minuty = (unsigned char)czescMinut;
	const double czescSekund = (czescMinut - out.Minuty) * 60;
	out.Sekundy = (unsigned char)czescSekund;
	return out;
}