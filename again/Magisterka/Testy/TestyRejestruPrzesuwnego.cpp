#include "stdafx.h"
#include "CppUnitTest.h"
#include "RejestrPrzesuwny.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyRejestruPrzesuwnego)
	{
	public:
		TEST_METHOD(testRejestruPrzesuwnego)
		{
			std::vector<int> odczepy = { 3, 4 };
			RejestrPrzesuwny rejestr{ 4, odczepy };

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

		TEST_METHOD(testPrzykladWikipedia)
		{
			const long start_state = 13;  /* Any nonzero start state will work. */
			long lfsr = start_state;
			int bit = 0;    /* Must be 16bit to allow bit<<15 later in the code */

			std::vector<int> odczepy{ 11, 13, 14, 16 };
			RejestrPrzesuwny rejestr{ 16, odczepy };

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