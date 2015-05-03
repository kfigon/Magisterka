#pragma once

struct WspolrzedneDMS
{
	unsigned char Stopnie;
	unsigned char Minuty;
	unsigned char Sekundy;

	WspolrzedneDMS(unsigned char st = 0, unsigned char min = 0, unsigned char sek = 0) :
		Stopnie(st), Minuty(min), Sekundy(sek)
	{}
	~WspolrzedneDMS() = default;
};
struct PozycjaGeograficznaDMS
{
	WspolrzedneDMS Dlugosc;		// [E]
	WspolrzedneDMS Szerokosc;	// [N]

	PozycjaGeograficznaDMS(const WspolrzedneDMS& dl=0, const WspolrzedneDMS& sz=0) :
		Dlugosc(dl), Szerokosc(sz)
	{}
	~PozycjaGeograficznaDMS() = default;
};

// stopnie dziesietne, inaczej format DD
struct PozycjaGeograficznaStopnieDziesietne
{
	double Dlugosc;		// [E]
	double Szerokosc;	// [N]

	PozycjaGeograficznaStopnieDziesietne(const double& dl=0, const double& sz=0) :
		Dlugosc(dl), Szerokosc(sz)
	{}
	~PozycjaGeograficznaStopnieDziesietne() = default;
};

#define pi 3.14159265358979323846
#define radiany(stopnie)	stopnie * pi / 180
#define stopnie(rad)		rad* 180 / pi

class PozycjaGeograficzna
{
private:
	PozycjaGeograficznaDMS mPozycjaDMS;
	PozycjaGeograficznaStopnieDziesietne mPozycjaDD;

public:
	PozycjaGeograficzna();
	PozycjaGeograficzna(const PozycjaGeograficznaDMS& pozycja);
	PozycjaGeograficzna(const WspolrzedneDMS& dlugosc, const WspolrzedneDMS& szerokosc);
	PozycjaGeograficzna(const PozycjaGeograficznaStopnieDziesietne& pozycja);
	PozycjaGeograficzna(const double& dlugosc, const double& szerokosc);
	~PozycjaGeograficzna() = default;

	void piszWspolrzedneDMS();

	// wartosci w stopniach dziesietnych, wyjscie w kilometrach
	static double liczOdlegloscGeograficzna(const PozycjaGeograficzna& skad, const PozycjaGeograficzna& dokad);

	// wartosci w stopniach dziesietnych, wyjscie w kilometrach
	static double liczOdlegloscGeograficzna(const double& szerokoscA, const double& szerokoscB, const double& dlugoscA, const double& dlugoscB);

	// metody konwersji
	static PozycjaGeograficznaDMS konwertuj(const PozycjaGeograficznaStopnieDziesietne& pozycja);
	static PozycjaGeograficznaStopnieDziesietne konwertuj(const PozycjaGeograficznaDMS& pozycja);
	static double konwertuj(const WspolrzedneDMS& wspolrzedna);
	static WspolrzedneDMS konwertuj(const double& wspolrzedna);

	// gettery
	const PozycjaGeograficznaDMS& zwrocPozycjeDMS() const { return mPozycjaDMS; }
	const PozycjaGeograficznaStopnieDziesietne& zwrocPozycjeStopnie() const { return mPozycjaDD; }

	// settery
	void ustawPozycje(const PozycjaGeograficznaDMS& pozycja)
	{
		mPozycjaDMS = pozycja;
		mPozycjaDD = konwertuj(mPozycjaDMS);
	}
	void ustawPozycje(const WspolrzedneDMS& dlugosc, const WspolrzedneDMS& szerokosc)
	{
		mPozycjaDMS.Dlugosc = dlugosc;
		mPozycjaDMS.Szerokosc = szerokosc;
		mPozycjaDD = konwertuj(mPozycjaDMS);
	}
	void ustawPozycje(const PozycjaGeograficznaStopnieDziesietne& pozycja)
	{
		mPozycjaDD = pozycja;
		mPozycjaDMS = konwertuj(mPozycjaDD);
	}
	void ustawPozycje(const double& dlugosc, const double& szerokosc)
	{
		mPozycjaDD.Dlugosc = dlugosc;
		mPozycjaDD.Szerokosc = szerokosc;
		mPozycjaDMS = konwertuj(mPozycjaDD);
	}
};