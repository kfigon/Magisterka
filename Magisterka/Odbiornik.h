#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "Struktury.h"
#include "GeneratorCiagow.h"
#include "FileReader.h"
#include "AproksymatorLiniowy.h"
#include <complex>

void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<WynikKorelacji>& wynikKorelacji);
void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<double>& wynikKorelacji);
void PrzygotujPlikDanychKorelacji(const std::string& sciezka, const std::vector<complex<long long>>& liczbyZespolone);

void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<WynikKorelacji>& wynikKorelacji);
void RysujWykres(const std::string& sciezkaDoPliku, const std::vector<double>& wynikKorelacji);
void RysujWykres(const std::vector<std::string>& sciezkaDoPlikow, const std::vector<double>& wykresI, const std::vector<double>& wykresQ);
void RysujKonstelacje(const std::string& sciezkaDoPliku, const std::vector<complex<long long>>& liczbyZespolone);

class KalkulatorDlugosciCiagow
{
    // KHz
    double mCzestotliwoscProbkowania = 1;
    double mCzestotliwosc = 1;
public:
    KalkulatorDlugosciCiagow(double czProbkowania, double czSygnalu) :
        mCzestotliwoscProbkowania(czProbkowania),
        mCzestotliwosc(czSygnalu)
    {}

    KalkulatorDlugosciCiagow()
    {}

    // w [ms]
    double liczCzasTrwaniaSygnaluWzorcowego(size_t dlugoscCiaguWzorcowego) const
    {
        return static_cast<double>(dlugoscCiaguWzorcowego) / (mCzestotliwosc);
    }

    size_t ileProbek(size_t dlugoscCiaguWzorcowego) const 
    { 
        const double czasTrwaniaCiaguMs = liczCzasTrwaniaSygnaluWzorcowego(dlugoscCiaguWzorcowego);
        const double licznik = mCzestotliwoscProbkowania*czasTrwaniaCiaguMs;
        const double mianownik = 1000;

        return static_cast<size_t>(licznik * 1000 / mianownik);
    }
};

class Odbiornik
{
    // fs danych = 1,28MHz, szybkosc transmisji 1,2288Mcps
    // wiec sie rozjedzie po 24 probkach
    // skip dane, skip ciag - stosunek czestotliwosci probkowania np 25/24
    // jesli jest 2/3 (dane 3, ciag 2)

    const int mSkipDane;
    const int mSkipCiag;
public:
    Odbiornik(int skipDane = 0, int skipCiag = 0) :
        mSkipDane(skipDane),
        mSkipCiag(skipCiag)
    {}

    // niedopasowanie czestotliwosci probkowania powoduje potrzebe
    // wyliczenia docelowych dlugosci ciagow
    double getWspolczynnikProporcjonalnosciCiagow() const
    {
        if (mSkipCiag == 0)
            return 1;

        return (static_cast<double>(mSkipDane) / static_cast<double>(mSkipCiag));
    }

    std::vector<complex<long long>> liczKorelacje(const std::vector<Data>& dane, const SygnalBipolarny& ciagI);

    // warunek na dopasowanie czestotliwosci wyciagniety do funkcji
    // ochrona przed nieskonczona petla + samo skipowanie -> powtorz element z krotszego ciagu
    __forceinline
        void aktualizujIndeksCiagu(size_t& indeksCiagu, size_t indeksDanych)
    {
        if ((indeksCiagu >= (mSkipDane - 1)) && (indeksDanych % mSkipDane == mSkipCiag))
        {
            indeksCiagu--;
        }
    }

    // jeden warunek zeby bylo szybciej sprawdzac
    __forceinline
        bool czySkipowac() const { return (mSkipDane != 0); }

    std::vector<complex<long long>> mnozenieZespoloneISumowanie(const std::vector<Data>& dane, const SygnalBipolarny& ciagI, int offset, int dlugoscDaych, bool czyCiagQ = false);

    // zwraca wyniki w radianach
    std::vector<double> liczWartosciKatow(const std::vector<complex<long long>>& liczbyZespolone);
    
    std::vector<WynikKorelacji> liczModuly(const std::vector<complex<long long>>& daneZespolone);
    
    std::vector<WynikKorelacji> znajdzMaksimaKorelacji(const std::vector<WynikKorelacji>& calaKorelacja, long long prog);
    
    // prog - roznica miedzyszczytowa, okres fazy
    std::vector<double> rozwinFaze(const std::vector<double>& fazy, double prog = 2*3.14159);
    
    // przyjmuje rozwiniete fazy i dlugosci podciagow, ktory byly sumowane
    // zwraca przedzialami zaproksymowane wartosci fazy ktorymi mozna skorygowac
    // odchylki czestotliwosci
    std::vector<double> aproksymujFazyDoKorekty(const std::vector<double>& rozwinietaFaza, int dlugoscPodciagu);
    
    std::vector<complex<double>> wyznaczKorekte(const std::vector<double>& zaproksymowaneFazy);
    __forceinline 
        complex<double> wyznaczKorekte(double faza);

    std::vector<complex<long long>> skupWidmo(const std::vector<Data>& dane, const SygnalBipolarny& ciagI, const SygnalBipolarny& ciagQ, const SygnalBipolarny& ciagWalsha, size_t offsetPn);
    std::vector<complex<long long>> korygujFaze(const std::vector<complex<long long>>& skupionyCiag, const std::vector<complex<double>>& korektyFazy);
    // przedzial calkowania
    std::vector<int> demodulacja(const std::vector<complex<long long>>& skupioneWidmo, int przedzialSumowania);
    std::vector<int> rozplot(const std::vector<int>& dane);
};

// klasa do obslugi operacji rozplotu
class Rozplatacz
{
    // parametry m i J, dla przypadku uzycia w programie 
    // (kanal synchronizacyjny - 128 bitow) sa rowne 7 i 1
    const int mM = 7;
    const int mJ = 1;

    int getM() const { return mM; }
    int getJ() const { return mJ; }

public:
    Rozplatacz(int m = 7, int j = 1) :
        mM(m),
        mJ(j)
    {}

    std::vector<int> rozplot(const std::vector<int>& ciag) const;

    __forceinline int getId(int index) const { return pow(2, getM()) * (index%getJ()) + bro((int)index / getJ()); }

    // BRO_m(y) wypisuje liczbe y binarnie od konca (m bitow)
    // np. BRO_3(6) = 3
    // 6 = b110 -> b011
    int bro(int liczba) const;
};

