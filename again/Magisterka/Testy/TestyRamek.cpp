#include "stdafx.h"
#include "CppUnitTest.h"
#include "RamkaSyncCh.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<RamkaSyncCh::Status>(const RamkaSyncCh::Status& s)
            {
                const auto msg = RamkaSyncCh::statusToString(s);
                
                return std::wstring (msg.begin(), msg.end());
            }
        }
    }
}
namespace Testy
{
    using namespace std;

    TEST_CLASS(TestyParseraRamki)
    {
        ParserRamek p;

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

        TEST_METHOD(szukajPoczatkuWStrumieniu)
        {
            // todo: ?
        }
    };

    TEST_CLASS(TestyDepeszy)
    {
    public:
        TEST_METHOD(poprawnoscRamki_zaKrotkaRamka1)
        {
            RamkaSyncCh ramka{ "1" };
            Assert::AreEqual<RamkaSyncCh::Status>(RamkaSyncCh::Status::ZaKrotkaRamka, ramka.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_zaKrotkaRamka2)
        {
            RamkaSyncCh ramka{ "1001010110" };
            Assert::AreEqual<RamkaSyncCh::Status>(RamkaSyncCh::Status::ZaKrotkaRamka, ramka.czyOk());
        }

        TEST_METHOD(poprawnoscRamki_invalidBodyLen1)
        {
            Assert::Fail();
        }

        TEST_METHOD(poprawnoscRamki_invalidBodyLen2)
        {   
            Assert::Fail();
        }

        TEST_METHOD(poprawnoscRamki_niewlasciweCrc)
        {
            Assert::Fail();
        }

        TEST_METHOD(poprawnoscRamki_ok)
        {
            Assert::Fail();
        }

        TEST_METHOD(getHeaderRaw)
        {
            Assert::Fail();
        }

        TEST_METHOD(getBodyRaw)
        {
            Assert::Fail();
        }

        TEST_METHOD(getBodyRaw2)
        {
            Assert::Fail();
        }

        TEST_METHOD(getCrcRaw)
        {
            Assert::Fail();
        }

        TEST_METHOD(getMsgLen1)
        {
            Assert::Fail();
        }

        TEST_METHOD(getMsgLen2)
        {
            Assert::Fail();
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
}
