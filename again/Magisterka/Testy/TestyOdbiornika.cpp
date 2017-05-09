#include "stdafx.h"
#include "CppUnitTest.h"
#include "Odbiornik.cpp"
#include "Struktury.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestyOdbiornika)
	{
	public:

		TEST_METHOD(testKorelacji)
		{
			std::vector<Data> input(5);
			input[0] = { 5, 8 };
			input[1] = { 2, 7 };
			input[2] = { 1, 6 };
			input[3] = { 8, 6 };
			input[4] = { 2, 5 };
			//input[5] = { 3, 1 };
			//input[6] = { 5, 4 };
			//input[7] = { 4, 2 };
			//input[8] = { 1, 5 };
			//input[9] = { 2, 7 };
			//input[10] = { 2, 7 };
			//input[11] = { 1, 9 };
			//input[12] = { 0, 8 };
			//input[13] = { 5, 3 };
			//input[14] = { 6, 4 };
			//input[15] = { 7, 6 };
			//input[16] = { 9, 2 };
			//input[17] = { 1, 1 };
			//input[18] = { 2, 8 };
			//input[19] = { 3, 1 };

			std::vector<int> ciagI{ -1, 1, -1, 1, -1 };
			std::vector<double> out(input.size(), 0);
			struct pies{
				long long real = 0;
				long long imag = 0;
			};
			std::vector<pies>compl;
			for (size_t l = 0; l < input.size(); l++)
			{
				long long real = 0;
				long long imag = 0;

				for (size_t n = 0; n < input.size(); n++)
				{

					int cI = 1;
					if (n + l < ciagI.size())
						cI = ciagI[n + l];

					auto dI = input[n].I;
					auto dQ = input[n].Q;

					real += cI*dI;
					imag += -1 * cI*dQ;
				}
				pies a;
				a.real = real;
				a.imag = imag;
				compl.push_back(a);
				out[l] = sqrt(real*real + imag*imag);
			}
			out;
			int sad = 1;
			sad;
			//auto wynik = liczKorelacje(input, 0);

			//3, 16227766016838	
			//	7, 07106781186548	
			//	6, 32455532033676	
			//	10, 6301458127347	
			//	2, 00000000000000	
			//	8, 48528137423857	
			//	8, 60232526704263	
			//	4, 24264068711929	
			//	10, 7703296142690	
			//	20, 1246117974981	
			//	12, 1655250605964	
			//	12, 7279220613579	
			//	5, 09901951359278	
			//	7, 28010988928052	
			//	13, 8924439894498	
			//	18, 3847763108502	
			//	21, 9544984001002	
			//	7, 28010988928052	
			//	4, 00000000000000	
			//	23, 3452350598575	
			//	11, 1803398874989	
			//	16, 0312195418814	
			//	8, 06225774829855	
			//	22, 3606797749979	
			//	28, 4429253066558	
			//	13	
			//	10, 4403065089106	
			//	7, 28010988928052	
			//	10, 0498756211209	
			//	25, 9615099714943	
			//	15, 0000000000000	
			//	1, 00000000000000	
			//	14, 3178210632764	
			//	5, 09901951359278	
			//	10, 1980390271856	
			//	3, 60555127546399	
			//	5, 38516480713451	
			//	16, 5529453572469	
			//	9, 43398113205661
		}
	};
}