#include "stdafx.h"
#include "CppUnitTest.h"
#include "RamkaSyncCh.cpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<StatusRamki>(const StatusRamki& s)
            {
                const auto msg = RamkaSyncCh::StatusRamkiToString(s);
                return std::wstring (msg.begin(), msg.end());
            }
        }
    }
}

namespace Testy
{
    using namespace std;

    TEST_CLASS(TestyPreprocesoraRamek)
    {
        PreprocesorRamek p;

        TEST_METHOD(getSize)
        {
            Assert::AreEqual<size_t>(0, p.getStreamSize());

            p.dopisz("111000");
            Assert::AreEqual<size_t>(6, p.getStreamSize());

            p.dopisz("111");
            Assert::AreEqual<size_t>(9, p.getStreamSize());
        }

        TEST_METHOD(getZaKrotkiCiag)
        {
            p.dopisz("10001110");

            // nie ma co odrzucac - za krotki ciag
            Assert::AreEqual(string{ "10001110" }, p.getFrame());
        }

        TEST_METHOD(get_1Ramka)
        {
            //  bit[0] to Start of message
            p.dopisz("10100000000000000000000000000000");

            // odrzuc bit startowy
            Assert::AreEqual(string{ "0100000000000000000000000000000" }, p.getFrame());
        }

        TEST_METHOD(flushPoGetFrame)
        {
            Assert::AreEqual(string{ "" }, p.getFrame());

            //  bit[0] to Start of message
            p.dopisz("10100000000000000000000000000000");

            // odrzuc bit startowy
            Assert::AreEqual(string{ "0100000000000000000000000000000" }, p.getFrame());
            
            Assert::AreEqual<size_t>(0, p.getStreamSize());
            Assert::AreEqual(string{ "" }, p.getFrame());
        }

        TEST_METHOD(dopisz_get_dopisz_get)
        {
            p.dopisz("111001");

            Assert::AreEqual(string{ "111001" }, p.getFrame());
            Assert::AreEqual<size_t>(0, p.getStreamSize());

            p.dopisz("00011");
            Assert::AreEqual(string{ "00011" }, p.getFrame());
        }

        TEST_METHOD(czySprawdzeniePoprawnosciRobiKopie)
        {
            p.dopisz("101010110101000");
            // przy okazji - za krotka to tez nie dobra
            Assert::IsFalse(p.sprawdzPoprawnoscCiagu());

            Assert::AreEqual(string{ "101010110101000" }, p.getFrame());
        }

        TEST_METHOD(sprawdzPoprawnosc_niepoprawnaDrugaRamka)
        {                                             // tutaj jest 1 zamiast 0
            p.dopisz("1000000010000010000100000001000010000000101000100101000100010000");
            Assert::IsFalse(p.sprawdzPoprawnoscCiagu());
        }

        TEST_METHOD(sprawdzPoprawnosc_poprawne2Ramki)
        {                                             
            p.dopisz("100000001000001000010000000100000000000010100010010100010001000000011000101000100101000100011111");
            Assert::IsTrue(p.sprawdzPoprawnoscCiagu());
        }

        TEST_METHOD(sprawdzPoprawnosc_poprawne3Ramki)
        {                                             // tutaj jest 1 zamiast 0
            p.dopisz("1000000010000010000100000001000000000000101000100101000100010000");
            Assert::IsTrue(p.sprawdzPoprawnoscCiagu());
        }

        TEST_METHOD(sprawdzPoprawnosc_niepoprawne3Ramki)
        {
            p.dopisz("10000000100000100001000000010000");
            p.dopisz("00000000101000100101000100010000");
            p.dopisz("10011000101000100101000100011111"); // SOM powinien byc 0

            Assert::IsFalse(p.sprawdzPoprawnoscCiagu());
        }

        TEST_METHOD(get_2Ramki)
        {
            p.dopisz("1000000010000010000100000001000000000000101000100101000100010000");
            Assert::AreEqual(string{ "00000001000001000010000000100000000000101000100101000100010000" }, p.getFrame());
        }

        TEST_METHOD(get_2Ramki_append)
        {
            p.dopisz("10000000100000100001000000010000");
            p.dopisz("00000000101000100101000100010000");
            Assert::AreEqual(string{ "00000001000001000010000000100000000000101000100101000100010000" }, p.getFrame());
        }

        TEST_METHOD(szukajPoczatkuWStrumieniu_ok)
        {
            p.dopisz("10000000001111111000101010010101");
            p.dopisz("00000010101010101010101101110010");
            p.dopisz("01101011011101010101000001011100");

            Assert::IsTrue(p.sprawdzPoprawnoscCiagu());
            p.odrzucNiepoprawnaCzescRamki();

            Assert::IsTrue(p.sprawdzPoprawnoscCiagu());
            Assert::AreEqual(string{ "000000000111111100010101001010100000101010101010101011011100101101011011101010101000001011100" },
                p.getFrame());        
        }

        TEST_METHOD(szukajPoczatkuWStrumieniu_drugaRamkaNaPoczatku)
        {
            p.dopisz("00000010101010101010101101110010");
            p.dopisz("01101011011101010101000001011100");
            p.dopisz("10000000001111111000101010010101");
            p.dopisz("00000010101010101010101101110010");
            
            Assert::IsFalse(p.sprawdzPoprawnoscCiagu());
            p.odrzucNiepoprawnaCzescRamki();

            Assert::IsTrue(p.sprawdzPoprawnoscCiagu(), L"ramka nie zostala odrzucona");
            Assert::AreEqual(string{ "00000000011111110001010100101010000010101010101010101101110010" },
                p.getFrame());
        }

        TEST_METHOD(szukajPoczatkuWStrumieniu_trzeciaRamkaNaPoczatku)
        {
            p.dopisz("01101011011101010101000001011100");
            p.dopisz("10000000001111111000101010010101");
            p.dopisz("00000010101010101010101101110010");
            p.dopisz("00000000001111111000101010010101");
            
            Assert::IsFalse(p.sprawdzPoprawnoscCiagu());
            p.odrzucNiepoprawnaCzescRamki();

            Assert::IsTrue(p.sprawdzPoprawnoscCiagu(), L"ramka nie zostala odrzucona");
            Assert::AreEqual(string{ "000000000111111100010101001010100000101010101010101011011100100000000001111111000101010010101" },
                p.getFrame());
        }
    };

    TEST_CLASS(TestParseraRamek)
    {
    public:
        TEST_METHOD(poprawnoscRamki_zaKrotkaRamka1)
        {
            RamkaSyncCh ramka{ "1" };
            Assert::AreEqual<StatusRamki>(StatusRamki::ZaKrotkaRamka, ramka.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_zaKrotkaRamka2)
        {
            RamkaSyncCh ramka{ "101000100011111111101010101010101110101" };
            Assert::AreEqual<StatusRamki>(StatusRamki::ZaKrotkaRamka, ramka.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_zaDlugaRamka)
        {
            std::string dane(2041, '1');
            RamkaSyncCh ramka{ dane };
            Assert::AreEqual<StatusRamki>(StatusRamki::ZaDlugaRamka, ramka.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_zaKrotkieBodyLen1)
        {
             // 8 bajtow ramka
            // 64-8-30 = 26 ciala
            RamkaSyncCh r{ "000010001111110001101011110111101111111000110101111011110100010" };
            Assert::AreEqual<StatusRamki>(StatusRamki::ZaMaleMsg, r.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_zaDlugieBodyLen2)
        {   
            // 8 bajtow ramka
            // 64-8-30 = 26 ciala
            RamkaSyncCh r{ "00001000111111000110110111110111101111111000110101111011110100010" };
            Assert::AreEqual<StatusRamki>(StatusRamki::ZaDuzeMsg, r.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_niewlasciweCrc)
        {
            Assert::Fail();
        }

        TEST_METHOD(poprawnoscRamki_ok)
        {
            Assert::Fail();
        }

        TEST_METHOD(getHeaderRaw1)
        {
            RamkaSyncCh r{ "0101000100011111111101010101010101110101" };
            Assert::AreEqual(string{ "01010001" }, r.getHeaderRaw());
        }

        TEST_METHOD(getHeaderRaw2)
        {
            RamkaSyncCh r{ "01010001" };
            Assert::AreEqual(string{ "01010001" }, r.getHeaderRaw());
        }

        TEST_METHOD(getBodyRaw)
        {
            // 2 bity ciala - rozmiar 40 bitow
            // 40/8 = 5 bajtow w headerze
            // crc pomijam
            RamkaSyncCh r{ "0000010111" };
            Assert::AreEqual(string{ "11" }, r.getBodyRaw());
        }

        TEST_METHOD(getBodyRaw2)
        {
            // 3 bity ciala - rozmiar 41 bitow
            // 41/8 = 5.125 w headerze->6 bajtow czyli 6*8 = 48 calosci
            // zakladam ze dodane beda zera
            RamkaSyncCh r{ "0000011000000111" };
            Assert::AreEqual(string{ "00000111" }, r.getBodyRaw());
        }

        TEST_METHOD(getBodyRaw3)
        {
            // 32 bity ciala - rozmiar 70 bitow
            // 70 bitow kodujemy na 9 bajtach
            // zakladam ze dodane beda zera
            RamkaSyncCh r{ "0000100110000000100000100001000000010000" };
            Assert::AreEqual(string{ "10000000100000100001000000010000" }, r.getBodyRaw());
        }

        TEST_METHOD(getBodyRaw4)
        {
            // 26 bity ciala - rozmiar 64 bity
            // 8 bajtow
            RamkaSyncCh r{ "0000100011111100011010111101111011" };
            Assert::AreEqual(string{ "11111100011010111101111011" }, r.getBodyRaw());
        }

        TEST_METHOD(getCrcRaw)
        {
            // 2 bity ciala - rozmiar 40 bitow
            // 40/8 = 5 bajtow w headerze
            RamkaSyncCh r{ "0000010100111111000110101111011110100010" };
            Assert::AreEqual(string{"111111000110101111011110100010"}, r.getCrcRaw());
        }

        TEST_METHOD(getCrc)
        {
            // 2 bity ciala - rozmiar 40 bitow
            // 40/8 = 5 bajtow w headerze
            // crc pomijam
            RamkaSyncCh r{ "0000010100111111000110101111011110100010" };
            Assert::AreEqual<unsigned int>(1058731938, r.getCrc());
        }


        TEST_METHOD(getMsgLen0)
        {
            RamkaSyncCh r{ "00000000" };
            Assert::AreEqual(0, r.getMsgLen());
        }

        TEST_METHOD(getMsgLen1)
        {
            RamkaSyncCh r{ "11111111" };
            Assert::AreEqual(2040, r.getMsgLen());
        }

        TEST_METHOD(getMsgLen2)
        {
            RamkaSyncCh r{ "11001011" };
            Assert::AreEqual(1624, r.getMsgLen());
        }

        TEST_METHOD(getMsgLen3)
        {
            RamkaSyncCh r{ "11000101" };
            Assert::AreEqual(1576, r.getMsgLen());
        }

        TEST_METHOD(getMsgLen4)
        {
            RamkaSyncCh r{ "00000001" };
            Assert::AreEqual(8, r.getMsgLen());
        }

        TEST_METHOD(checkCrc_valid)
        {
            Assert::Fail();
        }

        TEST_METHOD(checkCrc_invalid)
        {
            Assert::Fail();
        }
    };

    TEST_CLASS(TestyCrc)
    {
        // left shift register
        class Reg
        {
            std::vector<int> mState;
        public:
            Reg(size_t len) :
                mState(len, 0)
            {}

            void calculate(const std::vector<int>& polynomial, const std::vector<int>& data)
            {
                // clear state
                for (auto& bit : mState)
                    bit = 0;

                for (auto bit : data)
                {
                    shift(bit);
                    if (isMsbOne())
                        xor(polynomial);
                }
            }
            const std::vector<int> getCrc() const { return mState; }
            bool isCrcGood() const
            {
                for (auto bit : mState)
                {
                    if (bit != 0)
                    {
                        return false;
                    }
                }
                return true;
            }
            void dodajPadding(std::vector<int>& data) const
            {
                for (size_t i = 0; i < getPaddingSize(); i++)
                {
                    data.push_back(0);
                }
            }

            int getPaddingSize() const { return mState.size() - 1; }
        private:
            void shift(int bit)
            {
                for (size_t i = 1; i < mState.size(); i++)
                {
                    mState[i - 1] = mState[i];
                }
                mState[mState.size() - 1] = bit;
            }
            bool isMsbOne() const { return (mState[0] == 1); }

            void xor(const std::vector<int>& polynomial)
            {
                for (size_t i = 0; i < mState.size(); i++)
                {
                    mState[i] = mState[i] ^ polynomial[i];
                }
            }
        };
        
        void sprawdz(const std::vector<int>& expected, const std::vector<int>& wynik)
        {
            Assert::AreEqual(expected.size(), wynik.size(), L"dlugosci!");
            for (size_t i = 0; i < expected.size(); i++)
            {
                Assert::AreEqual(expected[i], wynik[i]);
            }
        }

        TEST_METHOD(asd)
        {
            // padding dodany!

            //b11000010 00000000), Polynomial = b100011101
            std::vector<int> dane{ 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //padding8
            const std::vector<int> poly{ 1, 0, 0, 0, 1, 1, 1, 0, 1 };//9

            Reg r{ poly.size() };
            r.calculate(poly, dane);
            const auto wynik = r.getCrc();
            const std::vector<int> expected{ 0, 0, 0, 0, 0, 1, 1, 1, 1 };//9
            sprawdz(expected, wynik);
        }
        
        TEST_METHOD(asd1)
        {
            std::vector<int> dane{ 1,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,0};//padding 3
            const std::vector<int> poly{ 1,0,1,1};

            Reg r{ poly.size() };
            r.calculate(poly, dane);

            const auto wynik = r.getCrc();
            const std::vector<int> expected{ 0,1,0,0 };
            sprawdz(expected, wynik);
        }

        TEST_METHOD(asd2)
        {
            std::vector<int> dane{ 1,0,1,0,1,1,1,0,0,0,0,0,0,0 ,0};//padding 8
            const std::vector<int> poly{ 1,0,0,0,0,0,1,1,1 };//9

            Reg r{ poly.size() };
            r.calculate(poly, dane);

            const auto wynik = r.getCrc();
            const std::vector<int> expected{ 0,1,0,1,0,0,0,1,0 };//9
            sprawdz(expected, wynik);
        }
        
        TEST_METHOD(badCrcs)
        {
            Assert::Fail();
            const string poly = "1011";
        /*    CrcCalc c{ poly };

            Assert::IsFalse(c.sprawdzPoprawnoscWiadomosci("11010011101100101"));
            Assert::IsFalse(c.sprawdzPoprawnoscWiadomosci("11010011100100100"));
            Assert::IsFalse(c.sprawdzPoprawnoscWiadomosci("11010011111100100"));
            Assert::IsFalse(c.sprawdzPoprawnoscWiadomosci("11010111101100100"));*/
        }

        TEST_METHOD(addPadding)
        {
            Assert::Fail();
        }
    };
}
