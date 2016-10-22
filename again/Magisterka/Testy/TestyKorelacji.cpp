#include "stdafx.h"
#include "CppUnitTest.h"
#include "Korelator.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyKorelacji)
	{
	public:
		TEST_METHOD(testAutokorelacja)
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

		TEST_METHOD(wartoscSrednia_1)
		{
			std::vector<int> ciag{ 3, 1, 6, 3, 4 };
			Assert::AreEqual(3.4, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(wartoscSrednia_2)
		{
			std::vector<int> ciag{ 1, 5, 3, 4, 3 };
			Assert::AreEqual(3.2, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(wartoscSrednia_3)
		{
			std::vector<int> ciag{ 3, 5, 18, 12, 12 };
			Assert::AreEqual(10.0, Korelator::liczWartoscSrednia(ciag));
		}

		TEST_METHOD(iloczynCiagow)
		{
			std::vector<int> a{ 3, 1, 6, 3, 4 };
			std::vector<int> b{ 1, 5, 3, 4, 3 };

			testujCiagi({ 3, 5, 18, 12, 12 }, Korelator::iloczynCiagow(a, b));
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
	};
}
