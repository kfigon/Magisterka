#include "stdafx.h"
#include "CppUnitTest.h"
#include "RejestrPrzesuwny.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	// testy z matlaba
	TEST_CLASS(TestyRejestruPrzesuwnego)
	{
	public:
		TEST_METHOD(testRejestru1)
		{
			// x^4 + x^1 + 1

			//clear all;
			//g = [1 0 0 1 1]; % g= [4 1 0]
			//init = [1 1 1 1];
			//curr = [1 1 1 1];
			//mask = [0 0 0 1];
			//NoOfOutBits = 15;
			//h = commsrc.pn('GenPoly', g, ...
			//	'InitialStates', init, ...
			//	'CurrentStates', curr, ...
			//	'Mask', mask, ...
			//	'NumBitsOut', NoOfOutBits)
			//	h.generate()

			RejestrPrzesuwny rej{ 4, { 4, 1, 0}};
			Assert::AreEqual(static_cast<int>(pow(2, 4)) - 1, rej.ileWartosciMozeWygenerowac());
			
			for (size_t i = 0; i < 10; i++)
			{
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
			}
		}

		TEST_METHOD(testRejestru2)
		{
			//clear all;
			//g = [5 2 0];
			//init = [1 1 1 1 1];
			//curr = [1 1 1 1 1];
			//mask = [0 0 0 0 1];
			//NoOfOutBits = 31;
			//h = commsrc.pn('GenPoly', g, ...
			//	'InitialStates', init, ...
			//	'CurrentStates', curr, ...
			//	'Mask', mask, ...
			//	'NumBitsOut', NoOfOutBits)
				
			
			RejestrPrzesuwny rej{ 5, { 5, 2, 0 } };
			Assert::AreEqual(static_cast<int>(pow(2, 5)) - 1, rej.ileWartosciMozeWygenerowac());

			for (size_t i = 0; i < 10; i++)
			{
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(1, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
				Assert::AreEqual(0, rej.getVal());
			}
		}

		TEST_METHOD(testRejestru3)
		{
			//clear all;
			//g = [15 13 9 8 7 5 0];
			//init = [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1];
			//curr = [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1];
			//mask = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 1];
			//NoOfOutBits = 200;
			//h = commsrc.pn('GenPoly',       g, ...
			//	'InitialStates', init,   ...
			//	'CurrentStates', curr,   ...
			//	'Mask',          mask,   ...
			//	'NumBitsOut',    NoOfOutBits)

			RejestrPrzesuwny rej{ 15, { 15, 13, 9, 8, 7, 5, 0 } };
			Assert::AreEqual(static_cast<int>(pow(2, 15)) - 1, rej.ileWartosciMozeWygenerowac());
				
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
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
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
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
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
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
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
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
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
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
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
			Assert::AreEqual(0, rej.getVal());
			Assert::AreEqual(1, rej.getVal());
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
		}

#if 0 
		TEST_METHOD(testPrzykladWikipedia)
		{
			//result = commsrc.pn('GenPoly', [16 14 13 11 1], 'Mask', [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1])

			//	% Output 10 PN bits
			//	set(result, 'NumBitsOut', 2 ^ 16);
			//generate(result);

			//% Reset(to the initial shift register state values)
			//	reset(result);

			const unsigned short start_state = 0xFFFF;  /* Any nonzero start state will work. */
			unsigned short lfsr = start_state;
			unsigned short bit = 0;    /* Must be 16bit to allow bit<<15 later in the code */
			int out = 0;

			RejestrPrzesuwny rejestr{ 16, { 16, 14, 13, 11 } };
			Assert::AreEqual(static_cast<int>(pow(2, 16)) - 1, rejestr.ileWartosciMozeWygenerowac());

			do
			{
				/* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
				bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
				out = lfsr & 1;
				lfsr = (lfsr >> 1) | (bit << 15);

				Assert::AreEqual(out, rejestr.getVal());
			} while (lfsr != start_state);
		}
#endif

	};
}