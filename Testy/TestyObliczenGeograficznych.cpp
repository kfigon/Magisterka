#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "KalkulatorOdleglosci.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<>
            static std::wstring ToString<WspolrzednaGeograficzna>(const WspolrzednaGeograficzna& wsp)
            {
                std::wstringstream s;
                s << wsp.getStopnie() << "-" << wsp.getMinuty() << "-" << wsp.getSekundy();
                return s.str();
            }

            template<>
            static std::wstring ToString<PozycjaGeograficzna>(const PozycjaGeograficzna& poz) 
            {

                return L"Koordynaty nie sa rowne. Dlugosc: " + ToString(poz.dlugosc) + L" Szerokosc: " + ToString(poz.szerokosc);
            }
        }
    }
}

namespace Testy
{
    TEST_CLASS(TestyObliczenGeograficznych)
    {
        void AreAlmostEqual(double exp, double val, double tolerance = 0.001)
        {
            if (abs(exp - val) > tolerance)
            {
                std::wstringstream msg;
                msg << L"expected: " << exp << ", actual " << val;
                Assert::Fail(msg.str().c_str());
            }
        }

    public:
        TEST_METHOD(lateracja)
        {
            std::vector<PozycjaGeograficzna> stacje(3);
            stacje[0] = PozycjaGeograficzna{ { 121.963477 }, { 37.418436 } };
            stacje[1] = PozycjaGeograficzna{ { 121.961889 }, { 37.417243 } };
            stacje[2] = PozycjaGeograficzna{ { 121.960194 }, { 37.418692 } };

            std::vector<int> odleglosci{ 265, 234, 54 };
            // https://gis.stackexchange.com/questions/66/trilateration-using-3-latitude-and-longitude-points-and-3-distances
            // https://en.wikipedia.org/wiki/Trilateration
            //    37.418436, -121.963477   0.265710701754km
            //    37.417243, -121.961889   0.234592423446km
            //    37.418692, -121.960194   0.0548954278262km
            // out: 37.417959,-121.961954
            PozycjaGeograficzna expected{ { 121.961954 }, { 37.417959 } };
            const auto wynik = KalkulatorOdleglosci::lateracja(stacje, odleglosci);

            Assert::AreEqual<PozycjaGeograficzna>(expected, wynik);

            Assert::Fail();
        }

        TEST_METHOD(liczOdleglosc1)
        {
            PozycjaGeograficzna gdansk{ {18,38,47}, {54,21,7} };
            PozycjaGeograficzna warszawa{ { 21, 0, 44 }, { 52, 13, 46 } };

            Assert::AreEqual(281758, KalkulatorOdleglosci::liczOdleglosc(gdansk, warszawa));
        }

        TEST_METHOD(liczOdleglosc2)
        {
            PozycjaGeograficzna gdansk{ { 18, 38, 0 }, { 54, 22,0} };
            PozycjaGeograficzna slupsk{ { 17, 1, 0}, { 54,28,8} };

            Assert::AreEqual(105465, KalkulatorOdleglosci::liczOdleglosc(gdansk, slupsk));
        }

        TEST_METHOD(postacDziesietna1)
        {
            PozycjaGeograficzna gdansk{ { 18, 38, 47 }, { 54, 21, 7 } };

            AreAlmostEqual(18.646, gdansk.dlugosc.getPostacDziesietna());
            AreAlmostEqual(54.352, gdansk.szerokosc.getPostacDziesietna());
        }

        TEST_METHOD(postacDziesietna2)
        {
            PozycjaGeograficzna warszawa{ { 21, 0, 44 }, { 52, 13, 46 } };
            
            AreAlmostEqual(21.012, warszawa.dlugosc.getPostacDziesietna());
            AreAlmostEqual(52.229, warszawa.szerokosc.getPostacDziesietna());
        }

        TEST_METHOD(postacDziesietna3)
        {
            PozycjaGeograficzna punkt{ { 19,47,57}, { 50,4,10 } };

            AreAlmostEqual(19.79925, punkt.dlugosc.getPostacDziesietna());
            AreAlmostEqual(50.06966, punkt.szerokosc.getPostacDziesietna());
        }

        TEST_METHOD(postacDziesietnaNaWspolrzedne1)
        {
            WspolrzednaGeograficzna wsp{ 50.06966 };
            Assert::AreEqual(50, wsp.getStopnie());
            Assert::AreEqual(4, wsp.getMinuty());
            Assert::AreEqual(10, wsp.getSekundy());
        }

        TEST_METHOD(postacDziesietnaNaWspolrzedne2)
        {
            WspolrzednaGeograficzna wsp{ 19.79925 };
            Assert::AreEqual(19, wsp.getStopnie());
            Assert::AreEqual(47, wsp.getMinuty());
            Assert::AreEqual(57, wsp.getSekundy());
        }

        TEST_METHOD(postacDziesietnaNaWspolrzedne3)
        {
            WspolrzednaGeograficzna wsp{ 21.012 };
            Assert::AreEqual(21, wsp.getStopnie());
            Assert::AreEqual(0, wsp.getMinuty());
            Assert::AreEqual(43, wsp.getSekundy());  // blad zaokraglenia tu wchodzi
        }

        TEST_METHOD(dodawanieWspolrzednych)
        {
            WspolrzednaGeograficzna a{ 1,2,3 };
            WspolrzednaGeograficzna b{ 3, 4, 5 };

            WspolrzednaGeograficzna expected{ 4, 6, 8 };
            Assert::AreEqual<WspolrzednaGeograficzna>(expected, a + b);
        }

        TEST_METHOD(odejmowanieWspolrzednych)
        {
            WspolrzednaGeograficzna a{ 1, 2, 3 };
            WspolrzednaGeograficzna b{ 3, 4, 5 };

            WspolrzednaGeograficzna expected{ 2,2,2};
            Assert::AreEqual<WspolrzednaGeograficzna>(expected, b-a);
        }

        TEST_METHOD(mnozenieWspolrzednych)
        {
            WspolrzednaGeograficzna a{ 1, 2, 3 };
            WspolrzednaGeograficzna b{ 3, 4, 5 };

            WspolrzednaGeograficzna expected{ 3,8,15 };
            Assert::AreEqual<WspolrzednaGeograficzna>(expected, a * b);
        }

        TEST_METHOD(overflowMinut)
        {
            WspolrzednaGeograficzna a{ 1, 60, 3 };
            WspolrzednaGeograficzna expected{ 2, 0, 3 };
            Assert::AreEqual<WspolrzednaGeograficzna>(expected, a);

        }
    };
}
