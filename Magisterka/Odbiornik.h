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
    // Hz
    const long long mCzestotliwoscProbkowania;;
    const long long mCzestotliwosc;;
public:
    // dane w Hz
    KalkulatorDlugosciCiagow(long long czProbkowania=1, long long czSygnalu=1) :
        mCzestotliwoscProbkowania(czProbkowania),
        mCzestotliwosc(czSygnalu)
    {}

    // w [ms]
    double liczCzasTrwaniaSygnaluWzorcowego(size_t dlugoscCiaguWzorcowego) const
    {
        return static_cast<double>(1000 * dlugoscCiaguWzorcowego) / static_cast<double>(mCzestotliwosc);
    }

    size_t ileProbek(size_t dlugoscCiaguWzorcowego) const 
    { 
        const double czasTrwaniaCiaguMs = liczCzasTrwaniaSygnaluWzorcowego(dlugoscCiaguWzorcowego);
        const double licznik = static_cast<double>(mCzestotliwoscProbkowania)*czasTrwaniaCiaguMs;
        const double mianownik = 1000;

        return static_cast<size_t>(licznik / mianownik);
    }
};


class Odbiornik
{

    // fs danych = 1,28MHz, szybkosc transmisji 1,2288Mcps
    // wiec sie rozjedzie po 24 probkach
    // skip dane, skip ciag - stosunek czestotliwosci probkowania np 25/24
    // jesli jest 2/3 (dane 3, ciag 2)
    int mSkipDane;
    int mSkipCiag;

    const long long mCzestotliwosc;
    const long long mCzestotliwoscProbkowania;
public:
    // dane w Hz
    Odbiornik(long long czestotliwoscProbkowania = 1, long long czestotliwosc = 1) :
        mCzestotliwosc(czestotliwosc),
        mCzestotliwoscProbkowania(czestotliwoscProbkowania)
    {
        // zalozenie - cz. probkowania >= czestotliwosc sygnalu.
        assert(mCzestotliwoscProbkowania >= mCzestotliwosc);

        if (mCzestotliwosc == mCzestotliwoscProbkowania)
        {
            mSkipCiag = 0;
            mSkipDane = 0;
        }
        else
        {
            const auto nwd = liczNwd(czestotliwosc, czestotliwoscProbkowania);
            mSkipDane = czestotliwoscProbkowania/ nwd;
            mSkipCiag = czestotliwosc / nwd;
        }
    }

    int getSkipDane() const { return mSkipDane; }
    int getSkipCiag() const { return mSkipCiag; }

    // w Hz
    long long getCzestotliwoscProbkowania() const { return mCzestotliwoscProbkowania; }
    // w Hz
    long long getCzestotliwoscSygnalu() const { return mCzestotliwosc; }

    static int liczNwd(int a, int b)
    {
        if (a == b)
        {
            return a;
        }

        int mniejsza = (a < b) ? a : b;
        int roznica = abs(a - b);

        return liczNwd(mniejsza, roznica);
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

    std::vector<int> demodulacja(const std::vector<complex<long long>>& skupioneWidmo, int przedzialCalkowania);
    std::vector<std::complex<long long>> calkowanie(const std::vector<complex<long long>>& skupioneWidmo, int przedzialCalkowania);
    std::vector<int> rozplot(const std::vector<int>& dane);

    static std::string toString(const std::vector<int>& ciag);
    
    // start - odrzuca co drugi bit zaczynajac od pierwszego albo drugiego
    // data: 1234
    // start == 1 -> 13
    // start == 2 -> 24
    static std::string odrzucPowtorzenia(const std::string& data, int start);

    // gdy juz dostane wystarczajaco dlugi ciag bitow
    // nastepuje rozplot, odrzucanie powtorek i dekodowanie.
    // potem interpretacja danych
    void obrobkaKanaluSynchronizacyjnego(const std::vector<int>& bity);
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

