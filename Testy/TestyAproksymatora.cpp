#include "stdafx.h"
#include "CppUnitTest.h"
#include "AproksymatorLiniowy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testy
{
    TEST_CLASS(TestyAproksymatoraLiniowego)
    {
        void AreAlmostEqual(double exp, double val, double tolerance = 0.001)
        {
            if (abs(exp - val) > tolerance)
            {
                std::wstringstream msg;
                msg << L"expected: " << exp << ", actual " << val;
                Assert::Fail(msg.str().c_str());
            }
        }

        std::vector<WartosciFunkcji<float>> dane;
    public:

        TEST_METHOD_INITIALIZE(init)
        {
            dane = { { 2, 2.5 }, { 4, 10 }, { 6, 32 }, { 8, 40 }, { 10, 60 } };
        }

        TEST_METHOD(testWspolczynnikow)
        {
            
            AproksymatorLiniowy<float> a{ dane };

            AreAlmostEqual(7.25, a.getA());
            AreAlmostEqual(-14.6, a.getB());
        }

        TEST_METHOD(interpolacja)
        {
            AproksymatorLiniowy<float> a{ dane };
            Assert::AreEqual(-14.6f, a.getVal(0));
            AreAlmostEqual(-7.35, a.getVal(1));

            AreAlmostEqual(21.65, a.getVal(5));
            AreAlmostEqual(28.9, a.getVal(6));
        }

        TEST_METHOD(extrapolacja)
        {
            AproksymatorLiniowy<float> a{ dane };

            AreAlmostEqual(65.15, a.getVal(11));
            AreAlmostEqual(72.4, a.getVal(12));
        }
    };
}
