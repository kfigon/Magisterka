#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>
#include "GeneratorCiagow.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyGeneratora)
	{
	public:

		TEST_METHOD(testCiaguI)
		{
			auto ciag = GeneratorCiagow::generujCiagI();
			testujCiagPn(*ciag);
		}

		TEST_METHOD(testCiaguQ)
		{
			auto ciag = GeneratorCiagow::generujCiagQ();
			testujCiagPn(*ciag);
		}

		// zatwierdzony ciag
		TEST_METHOD(integracjaCiagI)
		{

			//const std::string sciezka = "C:\\Users\\Kamil\\Desktop\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagI.txt";
            const std::string sciezka = "C:\\Users\\kfigon\\Desktop\\uczelnia\\mgr\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagI.txt";
            auto ciag = GeneratorCiagow::generujCiagI();
			testZatwierdzonegoCiagu(sciezka, *ciag);
		}

		TEST_METHOD(integracjaCiagQ)
		{
			//const std::string sciezka = "C:\\Users\\Kamil\\Desktop\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagQ.txt";
            const std::string sciezka = "C:\\Users\\kfigon\\Desktop\\uczelnia\\mgr\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagQ.txt";
            auto ciag = GeneratorCiagow::generujCiagQ();
			testZatwierdzonegoCiagu(sciezka, *ciag);

		}

        TEST_METHOD(ciagWalsha0)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(0);
            std::vector<int> expected(64, 0);
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha1)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(1);
            std::vector<int> expected{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
                0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
                0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha2)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(2);
            std::vector<int> expected{ 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
                0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
                0, 0, 1, 1, 0, 0, 1, 1 };
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha3)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(3);
            std::vector<int> expected{ 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 
                0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha4)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(4);
            std::vector<int> expected{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 
                0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha5)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(5);
            std::vector<int> expected{ 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 
                0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha6)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(6);
            std::vector<int> expected{ 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
                0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0 };
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha7)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(7);
            std::vector<int> expected{ 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 
                0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha8)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(8);
            std::vector<int> expected{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
                0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha9)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(9);
            std::vector<int> expected{ 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 
                0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }

        TEST_METHOD(ciagWalsha32)
        {
            auto ciag = GeneratorCiagow::generujBinarnyCiagWalsha(32);
            std::vector<int> expected{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
            Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_WASHA, expected.size(), L"ciag expected musi miec dlugosc 64 bity");
            porownajCiagi(expected, ciag);
        }


        TEST_METHOD(zamianaDziesietnychNaBinarne0)
        {
            std::vector<int> expected{ 0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(0));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne1)
        {
            std::vector<int> expected{ 1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(1));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne2)
        {
            std::vector<int> expected{ 1, 0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(2));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne3)
        {
            std::vector<int> expected{ 1,1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(3));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne4)
        {
            std::vector<int> expected{ 1,0,0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(4));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne5)
        {
            std::vector<int> expected{ 1,0,1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(5));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne7)
        {
            std::vector<int> expected{ 1,1,1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(7));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne13)
        {
            std::vector<int> expected{ 1,1,0,1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(13));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne15)
        {
            std::vector<int> expected{ 1,1,1,1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(15));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne16)
        {
            std::vector<int> expected{ 1,0,0,0,0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(16));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne17)
        {
            std::vector<int> expected{ 1, 0, 0, 0, 1 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(17));
        }
        TEST_METHOD(zamianaDziesietnychNaBinarne18)
        {
            std::vector<int> expected{ 1, 0, 0, 1, 0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(18));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne20)
        {
            std::vector<int> expected{ 1, 0, 1, 0, 0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(20));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne32)
        {
            std::vector<int> expected{ 1,0,0,0,0,0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(32));
        }

        TEST_METHOD(zamianaDziesietnychNaBinarne34)
        {
            std::vector<int> expected{ 1,0,0,0,1,0 };
            porownajCiagi(expected, GeneratorCiagow::zamienNaBinarne(34));
        }


		void testZatwierdzonegoCiagu(const std::string& sciezkaDoPliku, SygnalBipolarny& ciag)
		{
			auto data = czytajPlikZZatwierdzonymCiagiem(sciezkaDoPliku);

			Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_PN, data.size());
			for (size_t i = 0; i < data.size(); i++)
				Assert::AreEqual<int>(data[i], SygnalBipolarny::unmap(ciag.getElement(i)), L"niezgodnosc ciagu ze wzorcem");
		}

        void porownajCiagi(const std::vector<int>& expected, const std::vector<int>& actual)
        {
            Assert::AreEqual(expected.size(), actual.size(), L"nieprawidlowe dlugosci ciagow");
            for (size_t i = 0; i < expected.size(); i++)
            {
                Assert::AreEqual(expected[i], actual[i], (L"nie zgadza sie, idx: " + std::to_wstring(i)).c_str());
            }
        }

	private:
		void testujCiagPn(SygnalBipolarny& ciag)
		{
			Assert::AreEqual(static_cast<size_t>(pow(2, 15)), GeneratorCiagow::DLUGOSC_CIAGU_PN);
			Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU_PN, ciag.getDlugosc(), L"Ciag jest za krotki!");
			sprawdzIloscJedynekIZer(ciag);
			sprawdzPoczatek(ciag);
            sprawdzKoniec(ciag);
		}

		// sprawdzamy czy czy na poczatku nie ma zera
		void sprawdzPoczatek(SygnalBipolarny& ciag)
		{
			auto pierwszy = ciag.getElement(0);
			Assert::AreEqual(1, SygnalBipolarny::unmap(pierwszy));

            // zapetlenie, powinna byc ta sama liczba
            Assert::AreEqual(1, SygnalBipolarny::unmap(GeneratorCiagow::DLUGOSC_CIAGU_PN));
		}

        // na koncu powinno byc 15 zer
        void sprawdzKoniec(SygnalBipolarny& ciag)
        {
            const auto ostatniIdx = GeneratorCiagow::DLUGOSC_CIAGU_PN -15;
            for (size_t i = ostatniIdx; i < GeneratorCiagow::DLUGOSC_CIAGU_PN; i++)
                Assert::AreEqual(0, SygnalBipolarny::unmap(ciag.getElement(i)));
        }

		// ilosc jedynek i zer musi byc jednakowa
		void sprawdzIloscJedynekIZer(SygnalBipolarny& ciag)
		{
			int iloscJedynek = 0;
			int iloscZer = 0;
			for (size_t i = 0; i < ciag.getDlugosc(); i++)
			{
				int symbol = ciag.getElement(i);
				if (SygnalBipolarny::unmap(symbol) == 0)
					iloscZer++;
				else
					iloscJedynek++;
			}
			int polowaDlugosciCiagu = GeneratorCiagow::DLUGOSC_CIAGU_PN / 2;
			Assert::AreEqual(polowaDlugosciCiagu, iloscJedynek, L"Ilosc jedynek niezgodna");
			Assert::AreEqual(polowaDlugosciCiagu, iloscZer, L"Ilosc zer niezgodna");
		}

		std::vector<char> czytajPlikZZatwierdzonymCiagiem(const std::string& sciezka)
		{
			using namespace std;
			fstream plik;
			plik.open(sciezka.c_str(), ios::in);

			if (plik.is_open() == false)
				Assert::Fail(L"Nie udalo sie otworzyc pliku z danymi!");

			plik.seekg(0, plik.end);

			const auto rozmiar = plik.tellg();
			plik.seekg(0, plik.beg);

			Assert::AreEqual<int>(GeneratorCiagow::DLUGOSC_CIAGU_PN, rozmiar);

			std::vector<char> buf(rozmiar, 0);
			plik.read(buf.data(), rozmiar);
			
			// przeczytane sa znaki, trzeba zmienic na liczbe
			for (size_t i = 0; i < buf.size(); i++)
			{
				buf[i] = buf[i] - '0';
			}
			plik.close();

			return buf;
		}

	};
}