#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileReader.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
    TEST_CLASS(TestyCzytaniaPlikow)
    {
    public:
        /*const string katalog = "D:\\Kamil\\_magisterka\\pomiary";*/
        const std::string katalog = "C:\\Users\\kfigon\\Desktop\\uczelnia\\mgr\\_magisterka\\pomiary\\Probki-2014-04-12-11-41-40.dat";

        TEST_METHOD(integracjaCzytaniaPliku_LE)
        {
            BinaryReader czytacz{ katalog};
            auto dane = czytacz.Czytaj(8, BinaryReader::Endian::Little);
            Assert::IsFalse(dane.empty());
    
            Assert::AreEqual<short>(0xEA17, dane[0].I);
            Assert::AreEqual<short>(0x082D, dane[0].Q);
        }

        TEST_METHOD(integracjaCzytaniaPliku_BE)
        {
            BinaryReader czytacz{ katalog };
            auto dane = czytacz.Czytaj(8, BinaryReader::Endian::Big);
            Assert::IsFalse(dane.empty());

            Assert::AreEqual<short>(0x17EA, dane[0].I);
            Assert::AreEqual<short>(0x2D08, dane[0].Q);
        }

    };
}
