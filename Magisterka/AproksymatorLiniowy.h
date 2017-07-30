#pragma once
#include <vector>

template<class T>
struct WartosciFunkcji
{
    int arg = 0;
    T val = 0;

    WartosciFunkcji(int arg=0, T val=0) :
        arg(arg),
        val(val)
    {}
};

template<class T>
class AproksymatorLiniowy
{
private:
    double mA = 0;
    double mB = 0;

public:
    AproksymatorLiniowy(const std::vector<WartosciFunkcji<T>>& dane)
    {
        float sumaObu = 0;
        float sumaX = 0;
        float sumaY = 0;
        float sumaKwadratowX = 0;

        for (size_t i = 0; i < dane.size(); i++)
        {
            sumaObu += dane[i].arg * dane[i].val;
            sumaX += dane[i].arg;
            sumaY += dane[i].val;
            sumaKwadratowX += dane[i].arg*dane[i].arg;
        }

        const auto mianownik = dane.size()*sumaKwadratowX - (sumaX * sumaX);
        const auto licznikA = dane.size() * sumaObu - (sumaX * sumaY);
        const auto licznikB = sumaY*sumaKwadratowX - sumaX*sumaObu;

        mA = licznikA / mianownik;
        mB = licznikB / mianownik;
    }

    ~AproksymatorLiniowy() = default;

    double getA() const { return mA; }
    double getB() const { return mB; }

    T getVal(int arg)
    {
        return getA()*arg + getB();
    }
};

