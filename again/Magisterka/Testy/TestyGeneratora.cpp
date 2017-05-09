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
			testujCiag(*ciag);
		}

		TEST_METHOD(testCiaguQ)
		{
			auto ciag = GeneratorCiagow::generujCiagQ();
			testujCiag(*ciag);
		}

		// zatwierdzony ciag
		TEST_METHOD(integracjaCiagI)
		{
			const std::string sciezka = "C:\\Users\\Kamil\\Desktop\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagI.txt";
			auto ciag = GeneratorCiagow::generujCiagI();
			testZatwierdzonegoCiagu(sciezka, *ciag);
		}

		TEST_METHOD(integracjaCiagQ)
		{
			const std::string sciezka = "C:\\Users\\Kamil\\Desktop\\repoMgr\\again\\Magisterka\\Testy\\zatwierdzonyCiagQ.txt";
			auto ciag = GeneratorCiagow::generujCiagQ();
			testZatwierdzonegoCiagu(sciezka, *ciag);

		}

		void testZatwierdzonegoCiagu(const std::string& sciezkaDoPliku, SygnalBipolarny& ciag)
		{
			auto data = czytajPlikZZatwierdzonymCiagiem(sciezkaDoPliku);

			Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU, data.size());
			for (size_t i = 0; i < data.size(); i++)
				Assert::AreEqual<int>(data[i], SygnalBipolarny::unmap(ciag.getElement(i)), L"niezgodnosc ciagu ze wzorcem");
		}

	private:
		void testujCiag(SygnalBipolarny& ciag)
		{
			Assert::AreEqual(static_cast<size_t>(pow(2, 15)), GeneratorCiagow::DLUGOSC_CIAGU);
			Assert::AreEqual(GeneratorCiagow::DLUGOSC_CIAGU, ciag.getDlugosc(), L"Ciag jest za krotki!");
			sprawdzIloscJedynekIZer(ciag);
			sprawdzPoczatek(ciag);
		}

		// sprawdzamy czy czy na poczatku nie ma zera
		void sprawdzPoczatek(SygnalBipolarny& ciag)
		{
			auto pierwszy = ciag.getElement(0);
			Assert::AreNotEqual(0, SygnalBipolarny::unmap(pierwszy));
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
			int polowaDlugosciCiagu = GeneratorCiagow::DLUGOSC_CIAGU / 2;
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

			Assert::AreEqual<int>(GeneratorCiagow::DLUGOSC_CIAGU, rozmiar);

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