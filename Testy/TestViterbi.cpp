#include "stdafx.h"
#include "CppUnitTest.h"
#include "fec.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Testy
{
    TEST_CLASS(TestyViterbiego)
    {
#if 0
        TEST_METHOD(ASD)
        {
#define MAXBYTES 10000

            const int frameBits = 32;
            const int symLen = 8 * 2 * (MAXBYTES + 8);
            unsigned char symbols[symLen]; // input
            unsigned char data[MAXBYTES]; // output

            memset(symbols, 0, symLen);
            memset(data, 0, MAXBYTES);

            v29* vp = (v29*)create_viterbi29_port(frameBits);
            Assert::IsNotNull(vp);

            init_viterbi29_port(vp, 0);
            update_viterbi29_blk_port(vp, symbols, frameBits);

            chainback_viterbi29_port(vp, data, frameBits, 0);

            delete_viterbi29_port(vp);
        }
#endif
    };

}