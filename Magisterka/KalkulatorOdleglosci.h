#pragma once
#include <vector>
#include <assert.h>
#include <iostream>

class WspolrzednaGeograficzna
{
private:
    int stopnie;
    int minuty;
    int sekundy;

public:
    WspolrzednaGeograficzna(int st, int min, int sek) :
        stopnie(st),
        minuty(min),
        sekundy(sek)
    {
        if (sekundy >= 60)
        {
            sekundy -= 60;
            minuty++;
        }

        if (minuty >= 60)
        {
            minuty -= 60;
            stopnie++;
        }
    }

    WspolrzednaGeograficzna() :
        stopnie(0),
        minuty(0),
        sekundy(0)
    {}

    WspolrzednaGeograficzna(double wsp)
    {
        stopnie = static_cast<int>(wsp);
        const auto ulamkiStopni = wsp - stopnie;

        const auto ulamekMinut = ulamkiStopni * 60;
        minuty = static_cast<int>(ulamekMinut);

        const auto ulamekSekund = ulamekMinut - minuty;
        sekundy = static_cast<int>(ulamekSekund * 60);
    }

    int getStopnie() const { return stopnie; }
    int getMinuty() const { return minuty; }
    int getSekundy() const { return sekundy; }

    double getPostacDziesietna() const
    {
        return static_cast<double>(stopnie)+static_cast<double>(minuty) / 60 + static_cast<double>(sekundy) / 3600;
    }

    bool operator==(const WspolrzednaGeograficzna& rhs) const
    {
        return (stopnie == rhs.stopnie && minuty == rhs.minuty && sekundy == rhs.sekundy);
    }

    WspolrzednaGeograficzna operator+(const WspolrzednaGeograficzna& rhs) const
    {
        return WspolrzednaGeograficzna{getStopnie() + rhs.getStopnie(),
                                        getMinuty() + rhs.getMinuty(),
                                        getSekundy() + rhs.getSekundy()};
    }

    WspolrzednaGeograficzna operator-(const WspolrzednaGeograficzna& rhs) const
    {
        return WspolrzednaGeograficzna{ getStopnie() - rhs.getStopnie(),
            getMinuty() - rhs.getMinuty(),
            getSekundy() - rhs.getSekundy() };
    }

    WspolrzednaGeograficzna operator*(const WspolrzednaGeograficzna& rhs) const
    {
        return WspolrzednaGeograficzna{ getStopnie() * rhs.getStopnie(),
            getMinuty() * rhs.getMinuty(),
            getSekundy() * rhs.getSekundy() };
    }
};

struct PozycjaGeograficzna
{
    WspolrzednaGeograficzna dlugosc;    // E/W
    WspolrzednaGeograficzna szerokosc;  // N/S

    PozycjaGeograficzna(const WspolrzednaGeograficzna& dl = {}, const WspolrzednaGeograficzna& szer = {}) :
        dlugosc(dl),
        szerokosc(szer)
    {}

    bool operator==(const PozycjaGeograficzna& rhs) const
    {
        return (dlugosc == rhs.dlugosc && szerokosc == rhs.szerokosc);
    }
};

namespace KalkulatorOdleglosci
{
    // odleglosc w metrach
    int liczOdleglosc(const PozycjaGeograficzna& a, const PozycjaGeograficzna& b);
    
    // lateracja hiperboliczna
    // in: wspolrzedne geograficzne stacji i odpowiadajace im odleglosci w metrach
    PozycjaGeograficzna lateracja(const std::vector<PozycjaGeograficzna>& stacje, const std::vector<int>& odleglosci);

};

