#include "stdafx.h"
#include "CppUnitTest.h"
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

	private:
		void testujCiag(SygnalBipolarny& ciag)
		{
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
	};
}