#include "stdafx.h"
#include "CppUnitTest.h"
#include "Korelator.cpp"
#include "SygnalZespolony.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyKorelacji)
	{
	public:
		TEST_METHOD(testAutokorelacja1)
		{
			SygnalBipolarny sygnal{ std::vector<int>{1, 0, 1, 1, 1, 0, 0} };
			testujCiagi(std::vector<int>{ 7, -1, -1, -1, -1, -1, -1 }, Korelator::liczKorelacje(sygnal));
		}

		TEST_METHOD(testKorelacja_2takieSame)
		{
			SygnalBipolarny a{ std::vector<int>{1, 0, 1, 1, 1, 0, 0} };
			SygnalBipolarny b{ std::vector<int>{1, 0, 1, 1, 1, 0, 0} };

			testujCiagi(std::vector<int>{7, -1, -1, -1, -1, -1, -1}, Korelator::liczKorelacje(a, b));
		}

		TEST_METHOD(wartoscSrednia1)
		{
			std::vector<int> ciag{ 3, 1, 6, 3, 4 };
			Assert::AreEqual(3.4, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(wartoscSrednia2)
		{
			std::vector<int> ciag{ 1, 5, 3, 4, 3 };
			Assert::AreEqual(3.2, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(wartoscSrednia3)
		{
			std::vector<int> ciag{ 3, 5, 18, 12, 12 };
			Assert::AreEqual(10.0, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(iloczynCiagow)
		{
			std::vector<int> a{ 3, 1, 6, 3, 4 };
			std::vector<int> b{ 1, 5, 3, 4, 3 };

			testujCiagi({ 3, 5, 18, 12, 12 }, Korelator::liczIloczynCiagow(a, b));
		}

		TEST_METHOD(wariancja1)
		{
			std::vector<int> ciag{ 3, 1, 6, 3, 4 };
			czyPrawieRowne(1.625, Korelator::liczWariancje(ciag));
		}

		TEST_METHOD(wariancja2)
		{
			std::vector<int> ciag{ 1, 5, 3, 4, 3 };
			czyPrawieRowne(1.327, Korelator::liczWariancje(ciag));
		}

		TEST_METHOD(wariancja_1i2)
		{
			std::vector<int> a{ 3, 1, 6, 3, 4 };
			std::vector<int> b{ 1, 5, 3, 4, 3 };

			czyPrawieRowne(-0.88, Korelator::liczWariancje(a, b));
		}

		TEST_METHOD(wspolczynnikKorelacji1)
		{
			std::vector<int> a{ 3, 1, 6, 3, 4 };
			std::vector<int> b{ 1, 5, 3, 4, 3 };

			czyPrawieRowne(-0.408, Korelator::liczWspolczynnikKorelacji(a, b));
		}

		TEST_METHOD(wspolczynnikKorelacji2)
		{
			std::vector<int> a{ 0, 1, 1, 2, 3, 2, 1, 4, 5, 6, 8, 7, 5, 6, 4, 4, 3, 2, 1, 1 };
			std::vector<int> b{ 2, 3, 3, 4, 3, 2, 5, 8, 7, 6, 6, 5, 4, 6, 3, 2, 2, 0, 1, 0 };
		
			czyPrawieRowne(0.6438, Korelator::liczWspolczynnikKorelacji(a, b));
		}

	private:
		void testujCiagi(const std::vector<int>& expected, const std::vector<int>& actual)
		{
			if (expected.size() != actual.size())
			{
				std::wstringstream komunikat;
				komunikat << "Rozne dlugosci ciagow! " << expected.size() << " != " << actual.size();
				Assert::Fail(komunikat.str().c_str());
			}

			for (size_t i = 0; i < actual.size(); i++)
			{
				if (expected[i] != actual[i])
				{
					std::wstringstream komunikat;
					komunikat << "Blad na pozycji [" << i << "] -> " << expected[i] << " != " << actual[i];
					Assert::Fail(komunikat.str().c_str());
				}
			}
		}
		// czy wartosci sa rowne z pewnym bledem
		void czyPrawieRowne(double exp, double actual, double epsilon = 0.001)
		{
			if (!(exp < actual + epsilon) || !(exp > actual - epsilon))
			{
				std::wstringstream komunikat;
				komunikat << "actual: " << actual << " ; expected: " << exp;
				Assert::Fail(komunikat.str().c_str());
			}
		}
	};

	TEST_CLASS(TestSygnaluZespolonego)
	{
	private:
		SygnalZespolony s{ 3 };
	public:
		TEST_METHOD_INITIALIZE(init)
		{
			s.set(0, 1, 2);
			s.set(1, 2, 3);
			s.set(2, 3, 0);
		}

		TEST_METHOD(getRegular)
		{
			Assert::AreEqual<int>(1, s.get(0).real());
			Assert::AreEqual<int>(2, s.get(0).imag());

			Assert::AreEqual<int>(2, s.get(1).real());
			Assert::AreEqual<int>(3, s.get(1).imag());

			Assert::AreEqual<int>(3, s.get(2).real());
			Assert::AreEqual<int>(0, s.get(2).imag());

		}

		TEST_METHOD(getOutOfBounds)
		{
			for (int i = -4; i < 0; i++)
			{
				Assert::AreEqual<int>(0, s.get(i).real());
				Assert::AreEqual<int>(0, s.get(i).imag());
			}

			for (int i = s.getSize(); i < 50; i++)
			{
				Assert::AreEqual<int>(0, s.get(i).real());
				Assert::AreEqual<int>(0, s.get(i).imag());
			}
		}
	};

	TEST_CLASS(TestyNowejKorelacji)
	{
	public:

		void testujKorelacje(std::vector<std::complex<int>> expected, std::vector < std::complex<int>> wynik)
		{
			Assert::AreEqual(expected.size(), wynik.size(), L"Niewlasciwa dlugosc");
			for (size_t i = 0; i < wynik.size(); i++)
			{
				Assert::AreEqual(expected[i].real(), wynik[i].real());
				Assert::AreEqual(expected[i].imag(), wynik[i].imag());
			}
		}

		TEST_METHOD(korelacja1)
		{
			std::vector<std::complex<int>> expected{
				{ 1, -2 },
				{ 3, -7 },
				{ 7, -13 },
				{ 7, -11 },
				{ 6, -3 } };

			SygnalZespolony x{ 3 };
			x.set(0, 1, 0);
			x.set(1, 2, 0);
			x.set(2, 3, 0);

			SygnalZespolony y{ 3 };
			y.set(0, 2, 1);
			y.set(1, 1, 3);
			y.set(2, 1, 2);

			testujKorelacje(expected, liczKorelacje(x, y));
		}

		TEST_METHOD(korelacja2)
		{
			SygnalZespolony a{ std::vector<int>{ 0, 1, 1, 2, 3, 2, 1, 4, 5, 6, 8, 7, 5, 6, 4, 4, 3, 2, 1, 1 } };
			SygnalZespolony b{ std::vector<int>{ 2, 3, 3, 4, 3, 2, 5, 8, 7, 6, 6, 5, 4, 6, 3, 2, 2, 0, 1, 0} };

			std::vector<std::complex<int>> expected{
				{ 0, }, { 0, }, { 1, }, { 1, }, { 4 }, { 7 }, { 11 }, { 20, }, { 30, }, { 41, }, { 59, },
				{ 75 }, { 95 }, { 131 }, { 162, }, { 190 }, { 227 }, { 245 }, { 265 }, { 301 }, { 319 }, { 322 },
				{ 318 }, { 298 }, { 270 }, { 254 }, { 233 }, { 203 }, { 173 }, { 140, }, { 106, },
				{ 82, }, { 61, }, { 42, }, { 30 }, { 19 }, { 10 }, { 5 }, { 2 } };

			testujKorelacje(expected, liczKorelacje(a, b));
		}

		TEST_METHOD(korelacja3)
		{
			SygnalZespolony a{ std::vector<int>{ 0, 1, 1, 2, 3, 2, 1, 4, 5, 6, 8, 7, 5, 6, 4, 4, 3, 2, 1, 1 } };
			SygnalZespolony b{ std::vector<int>{ 2, 3, 3, 4, 3, 2, 5, 8, 7, 6, 6, 5, 4, 6, 3, 2, 2, 0, 1, 0} };

			std::vector<std::complex<int>> expected{
				{ 2 }, { 5 }, { 10 }, { 19 }, { 30 }, { 42 }, { 61 }, { 82 }, { 106 }, { 140 }, { 173 },
				{ 203 }, { 233 }, { 254 }, { 270 }, { 298 }, { 318 }, { 322 }, { 319 }, { 301 }, { 265 }, { 245 }, { 227 },
				{ 190 }, { 162 }, { 131 }, { 95 }, { 75 }, { 59 }, { 41 }, { 30 }, { 20 }, { 11 }, { 7 }, { 4 },
				{ 1 }, { 1 }, { 0 }, { 0 }
			};

			testujKorelacje(expected, liczKorelacje(b, a));
		}

	};
}
