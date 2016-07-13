#include "stdafx.h"
#include "CppUnitTest.h"
#include "RejestrPrzesuwny.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyRejestruPrzesuwnego)
	{
	public:
		TEST_METHOD(testRejestru1)
		{
			// x^4 + x^3 + 1
			std::vector<int> odczepy = { 3, 4 };
			RejestrPrzesuwny rejestr{ 4, odczepy };

			Assert::AreEqual(static_cast<int>(pow(2, 4)) - 1, rejestr.ileWartosciMozeWygenerowac());

			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(0, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
			Assert::AreEqual(1, rejestr.getVal());
		}

		TEST_METHOD(testRejestru2)
		{
			// x + 1
			RejestrPrzesuwny rej{ 3, {1}, 3 };

			Assert::AreEqual(static_cast<int>(pow(2, 3)) - 1, rej.ileWartosciMozeWygenerowac());


			// cykliczne 1001011, 1001011 itd.
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
		}

		TEST_METHOD(testRejestru3)
		{
			// x^6 + x + 1
			RejestrPrzesuwny rej{ 6, { 1, 6 }, 1};
			Assert::AreEqual(static_cast<int>(pow(2, 6)) - 1, rej.ileWartosciMozeWygenerowac());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
		}

		TEST_METHOD(testRejestru4)
		{
			// x^6 + x^5 + x^3 + x + 1
			RejestrPrzesuwny rej{ 6, { 1, 3, 5, 6 }, 1 };
			Assert::AreEqual(static_cast<int>(pow(2, 6)) - 1, rej.ileWartosciMozeWygenerowac());

			// stan = 1
			//poly = [6 5 3 1 0]
			//	ileBitow = 20
			//	maska = [0 0 0 0 0 1]
			//result = commsrc.pn('GenPoly', poly, 'Mask', maska)

			//	% Output 10 PN bits
			//	set(result, 'NumBitsOut', ileBitow);
			//generate(result)

			//	% Reset(to the initial shift register state values)
			//	reset(result);

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
		}

		TEST_METHOD(testRejestru5)
		{
			RejestrPrzesuwny rej{ 3, { 3, 2 }, 1 };
			Assert::AreEqual(static_cast<int>(pow(2, 3)) - 1, rej.ileWartosciMozeWygenerowac());

			// cyklicznie ten sam ciag
			for (size_t i = 0; i < 10; i++)
			{
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
			}
		}

		TEST_METHOD(testRejestru6)
		{
			RejestrPrzesuwny rej{ 15, { 15, 13, 9, 8, 7, 5 }, 1 };
			Assert::AreEqual(static_cast<int>(pow(2, 15)) - 1, rej.ileWartosciMozeWygenerowac());

			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());

			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
		}

		TEST_METHOD(testPrzykladWikipedia)
		{
			// tutaj wynik jest inny w matlabie!

			//result = commsrc.pn('GenPoly', [16 14 13 11 1], 'Mask', [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1])

			//	% Output 10 PN bits
			//	set(result, 'NumBitsOut', 2 ^ 16);
			//generate(result);

			//% Reset(to the initial shift register state values)
			//	reset(result);

			const long start_state = 1;  /* Any nonzero start state will work. */
			long lfsr = start_state;
			int bit = 0;    /* Must be 16bit to allow bit<<15 later in the code */

			RejestrPrzesuwny rejestr{ 16, { 11, 13, 14, 16 }, 1 };

			Assert::AreEqual(static_cast<int>(pow(2, 16)) - 1, rejestr.ileWartosciMozeWygenerowac());

			do
			{
				/* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
				bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
				lfsr = (lfsr >> 1) | (bit << 15);

				Assert::AreEqual(bit, rejestr.getVal());
			} while (lfsr != start_state);
		}
	};
}