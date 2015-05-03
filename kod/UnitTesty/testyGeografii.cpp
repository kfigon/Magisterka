#pragma once
#include "stdafx.h"

#include "CppUnitTest.h"
#include "../Geografia.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesty
{
	TEST_CLASS(testyGeografii)
	{
	public:

		TEST_METHOD(konwersjaWspolrzednychGeograficznych)
		{
			WspolrzedneDMS aDMS(47, 2, 24);
			WspolrzedneDMS bDMS(73, 28, 48);

			auto aDD = PozycjaGeograficzna::konwertuj(aDMS);
			auto bDD = PozycjaGeograficzna::konwertuj(bDMS);

			Assert::AreEqual(aDD, 47.04, L"Konwersja DMS->stopnie dziesietne nie udala sie");
			Assert::AreEqual(bDD, 73.48, L"Konwersja DMS->stopnie dziesietne nie udala sie");
			
			WspolrzedneDMS powrotADMS = PozycjaGeograficzna::konwertuj(aDD);

			// nie testuje sekund bo moze byc blad jednej sekundy - konwersja double-uchar
			Assert::AreEqual(powrotADMS.Stopnie, aDMS.Stopnie, L"Konwersja DMS->stopnie dziesietne nie udala sie");
			Assert::AreEqual(powrotADMS.Minuty, aDMS.Minuty, L"Konwersja stopnie->DSM nie udala sie");
		}

		TEST_METHOD(liczenieOdleglosci)
		{
			auto wyliczonaOdleglosc = 
				PozycjaGeograficzna::liczOdlegloscGeograficzna(52.23239058, 52.228526677,
				21.011927604, 21.0196094510);

			auto spodziewanyWynik = 0.67698500542490525;
			Assert::AreEqual(wyliczonaOdleglosc, spodziewanyWynik);
		}
	};
}
