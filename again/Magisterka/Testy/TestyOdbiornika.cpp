#include "stdafx.h"
#include "CppUnitTest.h"
#include "Struktury.h"
#include "Odbiornik.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// kompilator mial problemy z porownywaniem long longa...
namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<__int64>(const __int64 &t)
            {
                RETURN_WIDE_STRING(t);
            }
        }
    }
}

namespace Testy
{
    void sprawdzajCiagi(const std::vector<complex<long long>>& expected, const std::vector<complex<long long>>& wynik)
    {
        Assert::AreEqual(expected.size(), wynik.size(), L"nieprawidlowe dlugosci ciagow");
        for (size_t i = 0; i < expected.size(); i++)
        {
            Assert::AreEqual(expected[i].real(), wynik[i].real(), (L"nie zgadza sie real, idx: " + std::to_wstring(i)).c_str());
            Assert::AreEqual(expected[i].imag(), wynik[i].imag(), (L"nie zgadza sie imag, idx: " + std::to_wstring(i)).c_str());
        }
    }

    void porownajCiagi(const std::vector<int>& expected, const std::vector<int>& actual)
    {
        Assert::AreEqual(expected.size(), actual.size(), L"nieprawidlowe dlugosci ciagow");
        for (size_t i = 0; i < expected.size(); i++)
        {
            Assert::AreEqual(expected[i], actual[i], (L"nie zgadza sie, idx: " + std::to_wstring(i)).c_str());
        }
    }

    void AreAlmostEqual(double exp, double val, double tolerance = 0.01)
    {
        if (abs(exp - val) > tolerance)
        {
            std::wstringstream msg;
            msg << L"expected: " << exp << ", actual " << val;
            Assert::Fail(msg.str().c_str());
        }
    }

    TEST_CLASS(TestyOdbiornika)
	{
	public:
        TEST_METHOD(korelacja)
        {
            std::vector<Data> dane{
                Data{ 1, 2 },
                Data{ 2, 1 },
                Data{ 3, 1 },
                Data{ 4, 4 },
                Data{ 5, 2 },

                Data{ 3, 4 },
                Data{ 1, 5 },
                Data{ 3, 2 },
                Data{ 1, 1 },
                Data{ 2, 3 }
            };

            SygnalBipolarny ciagI{ { 1, 0, 1, 0, 1 } };

            Odbiornik o{};
            const auto wynik = o.liczKorelacje(dane, ciagI);
            std::vector<double> expected{ 3, 6.083, 2, 5, 2.236, 6.708 };

            Assert::AreEqual(expected.size(), wynik.size(), L"nieprawidlowe dlugosc ciagow");

            for (size_t i = 0; i < expected.size(); i++)
            {
                const auto c = wynik[i];
                const auto modul = sqrt(c.real()*c.real() + c.imag()*c.imag());
                AreAlmostEqual(expected[i], modul);
            }
        }

        // czestotliwosc - ile razy ma byc powtorzony symbol
        std::vector<Data> generujDane(const std::vector<int>& ciagBinarny, int czestotliwosc)
        {
            std::vector<Data> out(ciagBinarny.size()*czestotliwosc);

            int idWyjscia = 0;
            for (size_t i = 0; i < ciagBinarny.size(); i++)
            {
                const auto bit = ciagBinarny[i];

                for (size_t j = 0; j < czestotliwosc; j++)
                {
                    if (bit == 0)
                        out[idWyjscia] = Data{ 1 };
                    else
                        out[idWyjscia] = Data{ -1 };
                    
                    idWyjscia++;
                }
            }

            return out;
        }

        // sa dopasowane, tylko jeden jest dluzszy - czesciej probkowany
        // stosunek 2:3.
        TEST_METHOD(korelacja_2do3_skip)
        {
            // 10001101
            // dane (bipolarne) - 3 razy czesciej probkowane
            // skopiowane (powtorzone 2 razy jak w rzeczywistym scenariuszu)
            auto dane = generujDane({ 1,0,0,0,1,1,0,1, 1,0,0,0,1,1,0,1}, 3);

            // ciag 2 razy czesciej probkowany
            SygnalBipolarny ciagI{ { 1,1,           //1
                0,0,                                //0
                0,0,                                //0
                0, 0,                               //0            
                1,1,                                //1
                1,1,                                //1
                0,0,                                //0
                1, 1 } };                           //1

            Odbiornik o{ 3, 2 };
            auto wynik = o.liczKorelacje(dane, ciagI);
            std::vector<int> expected{ 16, 12, 8, 2, -2, -6, -8, -6, -4, -2, 0, 2, 2, 0, -2, -4, -6, -8, -10, -6, -2, 4, 8, 12, 16 };

            Assert::AreEqual(expected.size(), wynik.size(), L"nieprawidlowe dlugosci ciagow");

            for (size_t i = 0; i < expected.size(); i++)
                AreAlmostEqual(expected[i], wynik[i].real());
        }

        TEST_METHOD(korelacja_4do5_skip)
        {
            // 10001101
            // dane (bipolarne) - 5 razy czesciej probkowane
            // skopiowane (powtorzone 2 razy jak w rzeczywistym scenariuszu)
            auto dane = generujDane({1,0,0,0,1,1,0,1, 1,0,0,0,1,1,0,1}, 5);

            // ciag 4 razy czesciej probkowany
            SygnalBipolarny ciagI{ { 1, 1, 1,1,          //1
                0, 0, 0,0,                               //0
                0, 0, 0,0,                               //0
                0, 0, 0,0,                              //0            
                1, 1, 1,1,                               //1
                1, 1, 1,1,                               //1
                0, 0, 0,0,                               //0
                1, 1, 1, 1 } };                           //1

            Odbiornik o{ 5, 4 };
            auto wynik = o.liczKorelacje(dane, ciagI);

            std::vector<int> expected{32,26,20,14,6,-2,-4,-6,-8,-10,-12,-10,-8,-6,-2,2,2,2,2,2,2,0,-2,
            -4,-6,-8,-10,-12,-14,-18,-22,-16,-10,-4,2,8,12,16,20,26,32};
           
            Assert::AreEqual(expected.size(), wynik.size(), L"nieprawidlowe dlugosci ciagow");

            for (size_t i = 0; i < expected.size(); i++)
                AreAlmostEqual(expected[i], wynik[i].real());

        }
                
        TEST_METHOD(testProgowaniaKorelacji)
        {
            const std::vector<WynikKorelacji> calaKorelacja{
                {0,12},
                {1,14},
                {2,5},
                {3,4},
                {4,6}
            };

            Odbiornik o;
            auto wynik = o.znajdzMaksimaKorelacji(calaKorelacja, 10);

            const std::vector<WynikKorelacji> expected{
                { 0, 12 }, { 1, 14 } };

            Assert::AreEqual(expected.size(), wynik.size());
            for (size_t i = 0; i < expected.size(); i++)
            {
                Assert::AreEqual(expected[i].offset, wynik[i].offset);
                Assert::AreEqual(expected[i].wartosc, wynik[i].wartosc);
            }
        }

        TEST_METHOD(liczWartosciFaz)
        {
            std::vector<complex<long long>> zespolone{
                { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 },
                { 2, 1 }, { -3, 2 }, { -1, -2 }, { 4, -1 } };

            Odbiornik o;
            auto wynik = o.liczWartosciKatow(zespolone);

            std::vector<double> expected{0, 3.1415, 1.57, -1.57,
                0.464, 2.554, -2.034, -0.245};
            
            Assert::AreEqual(expected.size(), wynik.size());
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }

        TEST_METHOD(aproksymacjaFazy)
        {
            const std::vector<double> dane{ 1.50, 1.65, 1.8, 1.95, 2.1 };

            Odbiornik o;

            const auto wynik = o.aproksymujFazyDoKorekty(dane, 3);
            std::vector<double> expected{ 1.5, 1.55, 1.6,
                1.65, 1.7, 1.75,
                1.8, 1.85, 1.9,
                1.95, 2, 2.05,
                2.1, 2.15, 2.2,
                2.25, 2.3, 2.35 };

            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }

        TEST_METHOD(wyznaczanieKorekty)
        {
            const std::vector<double> fazy{ 0, 3.14159, 1.570795, 
                -1.570795, 1.446, 1.681,
                -0.5 };

            Odbiornik o;

            const auto wynik = o.wyznaczKorekte(fazy);
            std::vector <complex<double>> expected{ { 1, 0 }, { -0.999, 0 }, { 0, -0.998 },
            { 0, 0.998 }, { 0.124, -0.992 }, { -0.109, -0.994 },
            { 0.877, 0.479 } };

            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
            {
                
                AreAlmostEqual(expected[i].real(), wynik[i].real());
                AreAlmostEqual(expected[i].imag(), wynik[i].imag());

                // moduly musza byc jednostkowe, zeby korekta nie zmianiala modulu sygnalu
                const auto modul = sqrt(wynik[i].real()*wynik[i].real() + wynik[i].imag()*wynik[i].imag());
                AreAlmostEqual(1, modul);
            }
        }

        TEST_METHOD(skupienieWidma)
        {
            std::vector<Data> dane{
            Data{ 1, 2 }, Data{ 2, 1 },
            Data{ 3, 1 }, Data{ 4, 4 },
            Data{ 5, 2 }, Data{ 3, 4 },
            Data{ 1, 5 }, Data{ 3, 2 },
            Data{ 1, 1 }, Data{ 2, 3 } };

            SygnalBipolarny ciagI{ { 1, 0, 0, 1, 1 } };
            SygnalBipolarny ciagQ{ { 0, 0, 1, 0, 0 } };
            SygnalBipolarny walsh{ { 0,0,0,0} };

            Odbiornik o;
            const auto wynik = o.skupWidmo(dane, ciagI, ciagQ, walsh, 0);
            std::vector<complex<long long>> expected{ { -3, -1 }, { 1, 3 }, { 4, -2 }, { -8, 0 },
            { -7, 3 }};

            sprawdzajCiagi(expected, wynik);
        }

        TEST_METHOD(skupienieWidma_offset1)
        {
            std::vector<Data> dane{
            Data{ 1, 2 }, Data{ 2, 1 },
            Data{ 3, 1 }, Data{ 4, 4 },
            Data{ 5, 2 }, Data{ 3, 4 },
            Data{ 1, 5 }, Data{ 3, 2 },
            Data{ 1, 1 }, Data{ 2, 3 } };

            SygnalBipolarny ciagI{ { 1, 0, 0, 1, 1 } };
            SygnalBipolarny ciagQ{ { 0, 0, 1, 0, 0 } };
            SygnalBipolarny walsh{ { 0, 0, 0, 0 } };

            Odbiornik o;
            const auto wynik = o.skupWidmo(dane, ciagI, ciagQ, walsh, 1);
            std::vector<complex<long long>> expected{ { -3, 1 }, { 2, 4 }, { 8, 0 }, { -7, 3 },
            { -7, -1 }};

            sprawdzajCiagi(expected, wynik);
        }

        TEST_METHOD(korekcjaFazy)
        {
            std::vector<complex<long long>> dane{ { -2, -1 }, { 3, 1 }, { 4, 4 }, 
            { -5, -2 }, { -3, -4 }, { -1, -5 }, { 3, 2 } };

            std::vector<complex<double>> korekty{ { -1, 0 }, { 0.999, 0 }, { 0, -0.998 },
            { 0, 0.998 }, { -0.124, -0.992 }, { 0.109, -0.994 }, { -0.877, 0.479 } };

            Odbiornik o;
            const auto wynik = o.korygujFaze(dane, korekty);

            std::vector<complex<long long>> expected{ { 2, 1 },
            { 3, 1},
            { 4, -4},
            { 2, -5},
            { -4, 3},
            { -5, 0},
            { -4, 0} };

            sprawdzajCiagi(expected, wynik);
        }
    };

    // nie jest to skipowanie, tylko powtarzanie
    TEST_CLASS(TestyDopasowaniaCzestotliwosciProbek)
    {

        TEST_METHOD(wyliczanieSkipow_czestotliwoscTakieSame_1_1)
        {
            // czestotliwosc rowne 1Hz
            Odbiornik o{ 1, 1 };
            Assert::AreEqual(0, o.getSkipDane());
            Assert::AreEqual(0, o.getSkipCiag());
        }

        TEST_METHOD(wyliczanieSkipow_czestotliwoscTakieSame_200_200)
        {
            Odbiornik o{ 200, 200 };
            Assert::AreEqual(0, o.getSkipDane());
            Assert::AreEqual(0, o.getSkipCiag());
        }

        TEST_METHOD(wyliczanieSkipow_1_28M_1_2288M)
        {
            Odbiornik o{ static_cast<long long>(1.28 * 1000000), static_cast<long long>(1.2288 * 1000000) };
            Assert::AreEqual(25, o.getSkipDane());
            Assert::AreEqual(24, o.getSkipCiag());
        }

        TEST_METHOD(wyliczanieSkipow_24_25)
        {
            Odbiornik o{ 25, 24 };
            Assert::AreEqual(25, o.getSkipDane());
            Assert::AreEqual(24, o.getSkipCiag());
        }

        TEST_METHOD(wyliczanieSkipow_5_4)
        {
            Odbiornik o{ 5, 4 };
            Assert::AreEqual(5, o.getSkipDane());
            Assert::AreEqual(4, o.getSkipCiag());
        }

        TEST_METHOD(skipowanie_takieSameCzestotliwosci_brakPowtarzania)
        {
            Odbiornik o{};

            Assert::IsFalse(o.czySkipowac());

            size_t idCiagu = 0;
            size_t idDanych = 0;
            for (idDanych = 0; idDanych < 50; idDanych++)
            {
                o.aktualizujIndeksCiagu(idCiagu, idDanych);
                idCiagu++;
            }

            Assert::AreEqual(idDanych, idCiagu);
        }


        TEST_METHOD(skipowanie_2do3)
        {
            Odbiornik o{ 3, 2 };

            Assert::IsTrue(o.czySkipowac());

            size_t idCiagu = 0;
            size_t idDanych = 0;
            for (idDanych = 0; idDanych < 24; idDanych++)
            {
                o.aktualizujIndeksCiagu(idCiagu, idDanych);
                idCiagu++;
            }

            // potwarzanie powinno wystapic 8 razy
            // dla idDanych 2,5,8,11,14,17,20 i 23
            Assert::AreEqual(idDanych, idCiagu + 8);
        }

        TEST_METHOD(skipowanie_2do3_offset2)
        {
            Odbiornik o{ 3, 2 };

            size_t idCiagu = 0;
            const size_t offset = 2;

            o.aktualizujIndeksCiagu(idCiagu, 2 - offset);
            Assert::AreEqual<size_t>(0, idCiagu, L"powinno nie zmienic");

            idCiagu++;
            o.aktualizujIndeksCiagu(idCiagu, 3 - offset);
            Assert::AreEqual<size_t>(1, idCiagu, L"nie powinno zmienic");

            idCiagu++;
            o.aktualizujIndeksCiagu(idCiagu, 4 - offset);
            Assert::AreEqual<size_t>(1, idCiagu, L"element powinien byc powtorzony");
        }

        TEST_METHOD(skipowanie_4do5)
        {
            Odbiornik o{ 5, 4 };

            Assert::IsTrue(o.czySkipowac());

            size_t idCiagu = 0;
            size_t idDanych = 0;
            for (idDanych = 0; idDanych < 30; idDanych++)
            {
                o.aktualizujIndeksCiagu(idCiagu, idDanych);
                idCiagu++;
            }

            // potwarzanie powinno wystapic 6 razy
            Assert::AreEqual(idDanych, idCiagu + 6);
        }
    };

    TEST_CLASS(TestyDemodulatora)
    {
        TEST_METHOD(demodulator_calkowaniePo1)
        {
            std::vector<complex<long long>> dane{ { -2, -1 }, { 3, 1 }, { 4, 4 },
            { -5, 2 }, { 3, -4 }, { -1, -5 }, { 3, 2 } };

            Odbiornik o;
            const auto wynik = o.demodulacja(dane, 1);
            std::vector<int> expected{ 1, 1, 0, 0, 0, 0,
                1, 0, 0, 1, 1, 1, 0, 0 };

            porownajCiagi(expected, wynik);
        }

        TEST_METHOD(demodulator_calkowaniePo3)
        {
            std::vector<complex<long long>> dane{ { -2, -1 }, { 3, 1 }, { 4, 4 },
            { -5, 2 }, { 3, -4 }, { -1, -5 },
            { 3, 2 } }; // wolna probka, odrzucona

            Odbiornik o;
            const auto wynik = o.demodulacja(dane, 3);
            std::vector<int> expected{ 0, 0, 1, 1 };

            porownajCiagi(expected, wynik);
        }
    };

    TEST_CLASS(TestyRozplatacza_7_1)
    {
        Rozplatacz r{ 7, 1 };
        TEST_METHOD(rozplataczGetId0)
        {
            Assert::AreEqual(0, r.getId(0));
        }
        TEST_METHOD(rozplataczGetId1)
        {
            Assert::AreEqual(64, r.getId(1));
        }
        TEST_METHOD(rozplataczGetId2)
        {
            Assert::AreEqual(32, r.getId(2));
        }
        TEST_METHOD(rozplataczGetId16)
        {
            Assert::AreEqual(4, r.getId(16));
        }

        TEST_METHOD(rozplataczGetId18)
        {
            Assert::AreEqual(36, r.getId(18));
        }


        std::vector<int> wygenerujExpectedDlaRozplatacza(size_t ileBitow)
        {
            // dane ida 1,1,2,2,3,3,4,4,5,5,6,6... 64,64
            std::vector<int> expected(ileBitow, 0);

            // potrzebny licznik do 2
            int licznik = 1;
            for (size_t i = 0; i < expected.size(); i++)
            {
                if (i != 0 && i % 2 == 0)
                    licznik++;
                expected[i] = licznik;
            }

            return expected;
        }

        TEST_METHOD(rozplot1)
        {
            Rozplatacz rozplatacz;

            std::vector<int> splecione{ 1, 33, 17, 49, 9, 41, 25, 57, 5, 37, 21, 53, 13, 45, 29, 61,
                3, 35, 19, 51, 11, 43, 27, 59, 7, 39, 23, 55, 15, 47, 31, 63,
                2, 34, 18, 50, 10, 42, 26, 58, 6, 38, 22, 54, 14, 46, 30, 62,
                4, 36, 20, 52, 12, 44, 28, 60, 8, 40, 24, 56, 16, 48, 32, 64,
                1, 33, 17, 49, 9, 41, 25, 57, 5, 37, 21, 53, 13, 45, 29, 61,
                3, 35, 19, 51, 11, 43, 27, 59, 7, 39, 23, 55, 15, 47, 31, 63,
                2, 34, 18, 50, 10, 42, 26, 58, 6, 38, 22, 54, 14, 46, 30, 62,
                4, 36, 20, 52, 12, 44, 28, 60, 8, 40, 24, 56, 16, 48, 32, 64 };

            const auto expected = wygenerujExpectedDlaRozplatacza(128);

            porownajCiagi(expected, rozplatacz.rozplot(splecione));
        }
    };

    TEST_CLASS(TestyFunkcjiBRO_7_1)
    {
        Rozplatacz r{ 7, 1 };

        TEST_METHOD(bro0)
        {
            Assert::AreEqual(0, r.bro(0));
        }

        TEST_METHOD(bro1)
        {
            Assert::AreEqual(64, r.bro(1));
        }

        TEST_METHOD(bro2)
        {
            Assert::AreEqual(32, r.bro(2));
        }

        TEST_METHOD(bro3)
        {
            Assert::AreEqual(96, r.bro(3));
        }

        TEST_METHOD(bro4)
        {
            Assert::AreEqual(16, r.bro(4));
        }

        TEST_METHOD(bro5)
        {
            Assert::AreEqual(80, r.bro(5));
        }

        TEST_METHOD(bro6)
        {
            Assert::AreEqual(48, r.bro(6));
        }
        TEST_METHOD(bro7)
        {
            Assert::AreEqual(112, r.bro(7));
        }
        TEST_METHOD(bro8)
        {
            Assert::AreEqual(8, r.bro(8));
        }
        TEST_METHOD(bro9)
        {
            Assert::AreEqual(72, r.bro(9));
        }
        TEST_METHOD(bro10)
        {
            Assert::AreEqual(40, r.bro(10));
        }
        TEST_METHOD(bro11)
        {
            Assert::AreEqual(104, r.bro(11));
        }
        TEST_METHOD(bro12)
        {
            Assert::AreEqual(24, r.bro(12));
        }
        TEST_METHOD(bro13)
        {
            Assert::AreEqual(88, r.bro(13));
        }
        TEST_METHOD(bro14)
        {
            Assert::AreEqual(56, r.bro(14));
        }
        TEST_METHOD(bro15)
        {
            Assert::AreEqual(120, r.bro(15));
        }
        TEST_METHOD(bro16)
        {
            Assert::AreEqual(4, r.bro(16));
        }
        TEST_METHOD(bro17)
        {
            Assert::AreEqual(68, r.bro(17));
        }
        TEST_METHOD(bro18)
        {
            Assert::AreEqual(36, r.bro(18));
        }
        TEST_METHOD(bro31)
        {
            Assert::AreEqual(124, r.bro(31));
        }
        TEST_METHOD(bro32)
        {
            Assert::AreEqual(2, r.bro(32));
        }
        TEST_METHOD(bro34)
        {
            Assert::AreEqual(34, r.bro(34));
        }
        TEST_METHOD(bro114)
        {
            Assert::AreEqual(39, r.bro(114));
        }
        TEST_METHOD(bro115)
        {
            Assert::AreEqual(103, r.bro(115));
        }
    };

    TEST_CLASS(TestyLiczeniaFaz)
    {
        std::vector<Data> dane;
        SygnalBipolarny ciag = { { 1, 0, 1, 1, 0, 0, 0, 1, 1, 1 } };


        TEST_METHOD_INITIALIZE(setup)
        {
          dane = {
            Data{ 1, 2 },
            Data{ 2, 1 },
            Data{ 3, 1 },
            Data{ 4, 4 },
            Data{ 5, 2 },
            Data{ 3, 4 },
            Data{ 1, 5 },
            Data{ 3, 2 },
            Data{ 1, 1 },
            Data{ 2, 3 },
            Data{ 2, 3 },
            Data{ -1, -3 }
            };
        }
    
        TEST_METHOD(mnozenieZespoloneISumowanie_offset0_podciag2)
        {
            Odbiornik o;
            const auto wynik = o.mnozenieZespoloneISumowanie(dane, ciag, 0, 2);

            std::vector<complex<long long>> expected = { { 1, -1 }, { -7, -5 },
            { 8, 6 }, { -2, 3 }, { -3, -4 } };

            sprawdzajCiagi(expected, wynik);
        }

        TEST_METHOD(mnozenieZespoloneISumowanie_offset1_podciag2)
        {
            Odbiornik o;
            const auto wynik = o.mnozenieZespoloneISumowanie(dane, ciag, 1, 2);

            std::vector<complex<long long>> expected = { { 1, 0 }, { -9, -6 }, { 4, 9 },
            { 2, 1 }, { -4, -6 } };

            sprawdzajCiagi(expected, wynik);
        }

        TEST_METHOD(mnozenieZespoloneISumowanie_offset2_podciag_2)
        {
            Odbiornik o;
            const auto wynik = o.mnozenieZespoloneISumowanie(dane, ciag, 2, 2);

            std::vector<complex<long long>> expected = { { 1, 3 }, { -8, -6 }, { 4, 7 }, { -1, -2 }, { -1, 0 } };

            sprawdzajCiagi(expected, wynik);

        }

        TEST_METHOD(mnozenieZespoloneISumowanie_offset2_podciag3)
        {
            Odbiornik o;
            const auto wynik = o.mnozenieZespoloneISumowanie(dane, ciag, 2, 3);

            std::vector<complex<long long>> expected = { {-4,1}, {1,3}, {-3,-5} };

            sprawdzajCiagi(expected, wynik);
        }

        /*TEST_METHOD(mnozenieZespoloneISumowanie_offset0_podciag_2_skip2do3)
        {
            Assert::Fail();
        }*/
    };

    TEST_CLASS(TestyRozwinieciaFazy)
    {

        TEST_METHOD(rozwinFaze_malejaca1)
        {
            std::vector<double>fazy{ 3, 2.5, 2, 1.5, 1, 0.5, 0, -0.5, -1,
                -1.5, -2, -2.5, -3, 
                2.5, 2, 1.5, 1 // skok
            };

            Odbiornik o;
                    // taki prog tutaj pasuje, normalnie bedzie 2pi
            const auto wynik = o.rozwinFaze(fazy, 6);
            const std::vector<double> expected{ 3, 2.5, 2, 1.5, 1, 0.5, 0,
                -0.5, -1, -1.5, -2, -2.5, -3, -3.5, -4, -4.5, -5 };

            Assert::AreEqual(expected.size(), fazy.size(), L"dlugosci nie powinny sie zmienic!");
            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }
        
        TEST_METHOD(rozwinFaze_malejaca2)
        {
            std::vector<double>fazy { 0,
                -1.5728, -1.5747, -1.5772, -1.5790,
                -1.5816, -1.5852, -1.5877,
                -1.5922, -1.5976, -1.6044,
                -1.6129, -1.6269, -1.6512,
                -1.6998, -1.8621, 1.7252,   // zmiana znaku, trzeba odwinac
                1.6124, 1.5930, 1.5916,
                1.5708, 1.5708, 1.5708 };

            Odbiornik o;
            const auto wynik = o.rozwinFaze(fazy);
            const std::vector<double> expected{ 0, -1.5728, -1.5747, -1.5772, -1.579, -1.5816, -1.5852, -1.5877, -1.5922, -1.5976, -1.6044, -1.6129, -1.6269, -1.6512, -1.6998, -1.8621, -4.55798, -4.67078, -4.69018, -4.69158, -4.71238, -4.71238, -4.71238 };

            Assert::AreEqual(expected.size(), fazy.size(), L"dlugosci nie powinny sie zmienic!");
            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }

        TEST_METHOD(rozwinFaze_rosnaca1)
        {
            std::vector<double>fazy{ 0, 0.5, 1, 1.5, 2, 2.5, 3, 
                -2.5, -2, -1.5, -1};

            Odbiornik o;
            const auto wynik = o.rozwinFaze(fazy, 6);
            const std::vector<double> expected{ 0, 0.5, 1, 1.5, 2, 2.5, 3,
                3.5, 4, 4.5, 5 };

            Assert::AreEqual(expected.size(), fazy.size(), L"dlugosci nie powinny sie zmienic!");
            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }

        TEST_METHOD(rozwinFaze_rosnaca2)
        {
            std::vector<double>fazy{ 0,
                 1.5728,  1.5747,  1.5772,  1.5790,
                 1.5816,  1.5852,  1.5877,
                 1.5922,  1.5976,  1.6044,
                 1.6129,  1.6269,  1.6512,
                 1.6998,  1.8621, -1.7252,   // zmiana znaku, trzeba odwinac
                -1.6124, -1.5930, -1.5916,
                -1.5708, -1.5708, -1.5708 };

            Odbiornik o;
            const auto wynik = o.rozwinFaze(fazy);
            const std::vector<double> expected{ 0, 1.5728, 1.5747, 1.5772, 1.579, 1.5816, 
                1.5852, 1.5877, 1.5922, 1.5976, 1.6044, 1.6129, 1.6269, 1.6512, 
                1.6998, 1.8621, 4.55798, 4.67078, 4.69018, 4.69158, 
                4.71238, 4.71238, 4.71238 };

            Assert::AreEqual(expected.size(), fazy.size(), L"dlugosci nie powinny sie zmienic!");
            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci nie powinny sie zmienic!");
            for (size_t i = 0; i < wynik.size(); i++)
                AreAlmostEqual(expected[i], wynik[i]);
        }
    };

    TEST_CLASS(TestyKalkulatoraDlugosciCiagow)
    {
        // todo: policzyc jeszcze raz na papierze te dlugosci i uzyc kalkulatora do
        // okreslenia tych czestotliwosci i dlugosci wynikowych ciagow skladowych

        // ile probek danych (fs=1.28MHz) przypada na
        // 32768 (2^15) ciagu wzorcowego PN o predkosci 1.2288 Mcps
        // czas transmisji 26.66666667 ms
        TEST_METHOD(ileProbekNaCiagWzorcowy_25do24)
        {
            KalkulatorDlugosciCiagow k{ Stale::CZESTOTLIWOSC_PROBKOWANIA_HZ, Stale::CZESTOTLIWOSC_SYGNALU_HZ };
            Assert::AreEqual<size_t>(34133, k.ileProbek(GeneratorCiagow::DLUGOSC_CIAGU_PN));
        }

        TEST_METHOD(liczCzasTrwaniaSygnalu)
        {
            KalkulatorDlugosciCiagow k{ Stale::CZESTOTLIWOSC_PROBKOWANIA_HZ, Stale::CZESTOTLIWOSC_SYGNALU_HZ };
            AreAlmostEqual(26.666667, k.liczCzasTrwaniaSygnaluWzorcowego(GeneratorCiagow::DLUGOSC_CIAGU_PN));
        }

        TEST_METHOD(ileProbekNaCiagWzorcowy_1do1)
        {
            KalkulatorDlugosciCiagow k;
            Assert::AreEqual<size_t>(50, k.ileProbek(50));
        }

        TEST_METHOD(liczCzasTrwaniaSygnalu_1do1)
        {
            KalkulatorDlugosciCiagow k;
            // wynik w [ms]!
            AreAlmostEqual(50000, k.liczCzasTrwaniaSygnaluWzorcowego(50));
        }
    };

    TEST_CLASS(TestNwd)
    {
        TEST_METHOD(nwd_4_4)
        {
            Assert::AreEqual(4, Odbiornik::liczNwd(4, 4));
        }

        TEST_METHOD(nwd_12288_12800) 
        {
            Assert::AreEqual(512, Odbiornik::liczNwd(12288, 12800));
            Assert::AreEqual(512, Odbiornik::liczNwd(12800, 12288));
        }
        
        TEST_METHOD(nwd_1228800_1280000)
        {
            Assert::AreEqual(51200, Odbiornik::liczNwd(1228800, 1280000));
            Assert::AreEqual(51200, Odbiornik::liczNwd(1280000, 1228800));
        }


        TEST_METHOD(nwd_1989_867)
        {
            Assert::AreEqual(51, Odbiornik::liczNwd(1989, 867));
            Assert::AreEqual(51, Odbiornik::liczNwd(867, 1989));
        }

        TEST_METHOD(nwd_42_56)
        {
            Assert::AreEqual(14, Odbiornik::liczNwd(42, 56));
            Assert::AreEqual(14, Odbiornik::liczNwd(56, 42));
        }

        TEST_METHOD(nwd_36_48)
        {
            Assert::AreEqual(12, Odbiornik::liczNwd(48, 36));
            Assert::AreEqual(12, Odbiornik::liczNwd(36, 48));
        }
    };

}