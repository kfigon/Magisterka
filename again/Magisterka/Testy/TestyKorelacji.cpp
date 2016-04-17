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
