#include "stdafx.h"
#include "CppUnitTest.h"
#include "Sygnal.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
	TEST_CLASS(TestySygnalu)
	{
	private:
		SygnalBipolarny* sygnal;

	public:
		TEST_METHOD_INITIALIZE(init)
		{
			std::vector<int> dane{ 1, 0, 1, 1, 1, 0, 0 };
			sygnal = new SygnalBipolarny{ dane };
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			delete sygnal;
		}

		TEST_METHOD(testMap)
		{
			Assert::AreEqual(1, SygnalBipolarny::map(0));
			Assert::AreEqual(-1, SygnalBipolarny::map(1));
		}

		TEST_METHOD(testUnmap)
		{
			Assert::AreEqual(0, SygnalBipolarny::unmap(1));
			Assert::AreEqual(1, SygnalBipolarny::unmap(-1));
		}

        TEST_METHOD(getDlugosc)
        {
            Assert::AreEqual<size_t>(7, sygnal->getDlugosc());
        }

		TEST_METHOD(testIdx)
		{
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(0));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(1));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(3));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(4));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(5));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(6));
		}

		TEST_METHOD(testIdxOverflow)
		{
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(7));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(8));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(9));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(10));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(11));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(12));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(13));
		}

		TEST_METHOD(testPrzesuniecie_jeden)
		{
			sygnal->przesunWLewo();
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(0));
		}

		TEST_METHOD(testPrzesuniecie_calosc)
		{
			sygnal->przesunWLewo();

			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(0));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(1));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(3));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(4));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(5));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(6));
		}

		TEST_METHOD(testKopiowanie)
		{
			SygnalBipolarny nowy{ *sygnal };
			for (int i = 0; i < nowy.getDlugosc(); i++)
				Assert::AreEqual(sygnal->getElement(i), nowy.getElement(i));
		}

        TEST_METHOD(getEx)
        {
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(0));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(1));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(2));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(3));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(4));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(5));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(6));
        }

        TEST_METHOD(getEx_ujemne)
        {
            for (int i = -20; i < 0; i++)
                Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(i));
        }

        TEST_METHOD(getEx_ponadDlugosc)
        {
            const auto dlugoscSygnalu = sygnal->getDlugosc();
            for (size_t i = dlugoscSygnalu; i < dlugoscSygnalu + 50; i++)
                Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(i));
        }

        TEST_METHOD(ograniczonaDlugosc)
        {
            sygnal->ograniczDlugosc(4);
            Assert::AreEqual<size_t>(4, sygnal->getDlugosc());
        }

        TEST_METHOD(get_zOgraniczonaDlugoscia)
        {
            sygnal->ograniczDlugosc(4);
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(0));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(1));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(3));
        }
	};

	TEST_CLASS(TestySygnaluZOffsetem)
	{
	private:
		SygnalBipolarny* sygnal;

	public:
		TEST_METHOD_INITIALIZE(init)
		{
			std::vector<int> dane{ 1, 0, 1, 1, 1, 0, 0 };
			sygnal = new SygnalBipolarny{ dane, 2 };
		}

		TEST_METHOD_CLEANUP(cleanup)
		{
			delete sygnal;
		}

		TEST_METHOD(get)
		{
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(0));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(1));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(3));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(4));
												   
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(5));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(6));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(7));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(8));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(9));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(10));
		}					

		TEST_METHOD(przesunWlewo)
		{
			sygnal->przesunWLewo();
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(0));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(1));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(2));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(3));
												   
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(4));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(5));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(6));
												   
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(7));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(8));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(9));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(10));												   
		}

		TEST_METHOD(przesunWPrawo)
		{
			sygnal->przesunWPrawo();
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(0));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(1));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(3));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(4));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(5));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(6));

			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(7));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(8));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(9));
			Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(10));
			Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(11));
		}

        TEST_METHOD(getEx)
        {
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(0));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(1));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(2));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(3));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(4));

            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElementEx(5));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(6));
        }

        TEST_METHOD(getEx_ujemne)
        {
            for (int i = -50; i < 0; i++)
                Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(i));
        }

        TEST_METHOD(getEx_ponadDlugosc)
        {
            const auto dlugoscSygnalu = sygnal->getDlugosc();
            for (int i = dlugoscSygnalu; i < dlugoscSygnalu + 50; i++)
                Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElementEx(i));
        }

        TEST_METHOD(getDlugosc)
        {
            Assert::AreEqual<size_t>(7, sygnal->getDlugosc());
        }

        TEST_METHOD(ograniczonaDlugosc)
        {
            sygnal->ograniczDlugosc(4);
            Assert::AreEqual<size_t>(4, sygnal->getDlugosc());
        }

        TEST_METHOD(get_zOgraniczonaDlugoscia)
        {
            sygnal->ograniczDlugosc(4);

            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(0));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(1));
            Assert::AreEqual(SygnalBipolarny::map(1), sygnal->getElement(2));
            Assert::AreEqual(SygnalBipolarny::map(0), sygnal->getElement(3));
        }

        TEST_METHOD(get_zOgraniczonaDlugoscia_zapetleniePrzezKoniec)
        {
            SygnalBipolarny s{ { 1, 0, 1, 1, 1, 0, 0 }, 5 };
            s.ograniczDlugosc(4);

            Assert::AreEqual(SygnalBipolarny::map(0), s.getElement(0));
            Assert::AreEqual(SygnalBipolarny::map(0), s.getElement(1));
            Assert::AreEqual(SygnalBipolarny::map(1), s.getElement(2));
            Assert::AreEqual(SygnalBipolarny::map(0), s.getElement(3));
        }
	};
}