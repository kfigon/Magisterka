/*
All rights reserved by Krishna Pillai, http://www.dsplog.com
The file may not be re-distributed without explicit authorization
from Krishna Pillai.
Author  : Krishna Pillai
Email   : krishna@dsplog.com
Version : 1.1
Date    : 21 August 2009

Script for computing the BER for BPSK modulation in AWGN with
Hard and Soft decision Viterbi decoding with traceback*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float exp10(float a)
{
    return pow(10, a);
}

void main() 
{

    float EbN0Lin, theoryBer, EcN0Lin, EcN0dB;
    int ii, jj;
    float r, a, n_re, n_im;
    float pi;

    int b, cHat[2], c[2]; //bit sequence
    float temp1, temp2;
    int nBit, N, err, nErrHard, nErrSoft; // simulation parameters
    float s_re[2], s_im[2], op, y_re[2], y_im[2]; //modulated signal
    int delay[3];
    float hdh[4], hds[4], ed[4];

    float gen_rand_uniform(); //function for generating uniform [0,1) random numbers
    int viterbiDecode(int nBit, float *p_pm, int *p_sp, int *p_bStore, float *p_hd, int N, int fc, int lc, int startTB);

    // initialization
    N = 1000000;
    pi = 3.14159265358979;

    const int decodeDel = 200;
    const int TB = 15;

    int sph[decodeDel + TB][4], sps[decodeDel + TB][4];
    int bStore[decodeDel + TB];
    int nErrHardTB, nErrSoftTB;
    int fc, lc;
    int startTB;
    int kk, tt;

    float pmh[4], pms[4];

    for (ii = 0; ii<10; ii++)
    {
        EcN0dB = (float)(ii - 3);
        EcN0Lin = exp10((float)-1 * EcN0dB / 20);

        nBit = 0;
        delay[2] = 0; delay[1] = 0; delay[0] = 0;
        pmh[0] = 0; pmh[1] = 0; pmh[2] = 0; pmh[3] = 0;
        pms[0] = 0; pms[1] = 0; pms[2] = 0; pms[3] = 0;
        //pm_n[0]= 0;pm_n[1] = 0;pm_n[2] = 0;pm_n[3] = 0;
        nErrHard = 0;
        nErrSoft = 0;
        while (nBit < N + 2)
        {

            /* generating random bits 0, 1 */
            b = (gen_rand_uniform()>0.5) ? 1 : 0;

            /* storing the bits, reordering the bits in the memory */
            bStore[((nBit - TB) % decodeDel) + TB] = b;
            if ((nBit > (decodeDel + TB)) && ((((nBit - TB) % decodeDel) + TB) == (decodeDel - 1))) {
                for (kk = 0; kk<TB; ++kk){
                    bStore[kk] = bStore[decodeDel + kk];
                    for (tt = 0; tt<4; tt++) {
                        sph[kk][tt] = sph[decodeDel + kk][tt];
                        sps[kk][tt] = sph[decodeDel + kk][tt];
                    }
                }
            }

            /* tail bits */
            if (nBit >= N)
                b = 0;

            /* convolutional coding  */
            delay[2] = delay[1];
            delay[1] = delay[0];
            delay[0] = b;

            c[0] = (delay[2] ^ delay[1]) ^ delay[0];
            c[1] = (delay[2] ^ delay[0]);

            /* BPSK modulation */
            s_re[0] = 2 * c[0] - 1;
            s_re[1] = 2 * c[1] - 1;
            s_im[0] = 0;
            s_im[1] = 0;

            /* AWGN channel (using Box-Muller transform) */
            temp1 = gen_rand_uniform();
            temp2 = gen_rand_uniform();
            r = sqrt(-2 * log(temp1));
            n_re = r*cos(2 * pi*temp2);
            n_im = r*sin(2 * pi*temp2);

            y_re[0] = s_re[0] + (1 / sqrt(2.0))*EcN0Lin*n_re;
            y_im[0] = s_im[0] + (1 / sqrt(2.0))*EcN0Lin*n_im;

            temp1 = gen_rand_uniform();
            temp2 = gen_rand_uniform();
            r = sqrt(-2 * log(temp1));
            n_re = r*cos(2 * pi*temp2);
            n_im = r*sin(2 * pi*temp2);

            y_re[1] = s_re[1] + (1 / sqrt(2.0))*EcN0Lin*n_re;
            y_im[1] = s_im[1] + (1 / sqrt(2.0))*EcN0Lin*n_im;

            /* Hard decision demodulation */
            cHat[0] = (y_re[0] >= 0) ? 1 : 0;
            cHat[1] = (y_re[1] >= 0) ? 1 : 0;

            /* Hamming distance computation for hard decision*/
            hdh[0] = (float)(cHat[0] ^ 0) + (float)(cHat[1] ^ 0);
            hdh[1] = (float)(cHat[0] ^ 0) + (float)(cHat[1] ^ 1);
            hdh[2] = (float)(cHat[0] ^ 1) + (float)(cHat[1] ^ 0);
            hdh[3] = (float)(cHat[0] ^ 1) + (float)(cHat[1] ^ 1);

            /* Euclidean distance computation for soft decision*/
            hds[0] = y_re[0] + y_re[1];
            hds[1] = y_re[0] - y_re[1];
            hds[2] = -1 * y_re[0] + y_re[1];
            hds[3] = -1 * y_re[0] + -1 * y_re[1];

            /* Traceback */
            fc = ((nBit == 0) || (nBit == 1)) ? 1 : 0;
            lc = ((nBit == N + 1)) ? 1 : 0;

            startTB = (((nBit - TB) % decodeDel) + TB) == (decodeDel + TB - 1);
            nErrHardTB = viterbiDecode(((nBit - TB) % decodeDel) + TB, &pmh[0], &sph[0][0], &bStore[0], &hdh[0], TB, fc, lc, startTB);
            nErrHard = nErrHard + nErrHardTB;

            nErrSoftTB = viterbiDecode(((nBit - TB) % decodeDel) + TB, &pms[0], &sps[0][0], &bStore[0], &hds[0], TB, fc, lc, startTB);
            nErrSoft = nErrSoft + nErrSoftTB;

            nBit++;
        } /* while loop */



        theoryBer = erfc(sqrt(exp10((double)ii / 10))) * 1 / 2;
        if (ii == 0)
        {
            printf("|BER for BPSK in AWGN with hard/soft Viterbi decoder \n");
            printf("|----------------------------------------------\n");
            printf("|Eb/N0 | BER (sim) | BER (sim) | BER (theory) |\n");
            printf("| (dB) | (hard)    | (soft)    | (uncoded)    |\n");
            printf("|----------------------------------------------\n");
        }

        printf("|  %d   | %f  | %f  | %f     |\n", ii, (float)nErrHard / (nBit - 2), (float)nErrSoft / (nBit - 2), theoryBer);


    } /* for ii loop */

    system("pause");
} /* main */

float gen_rand_uniform() {
    float u1;
    u1 = (double)rand() / ((double)RAND_MAX + (double)1);
    return(u1);
}

int viterbiDecode(int nBit, float *p_pm, int *p_sp, int *p_bStore, float *p_hd, int TB, int fc, int lc, int startTB) {

    int ipLut[4][4];
    // intialization
    ipLut[0][0] = 0; ipLut[0][1] = 0; ipLut[0][2] = 0; ipLut[0][3] = 0;
    ipLut[1][0] = 0; ipLut[1][1] = 0; ipLut[1][2] = 0; ipLut[1][3] = 0;
    ipLut[2][0] = 1; ipLut[2][1] = 1; ipLut[2][2] = 0; ipLut[2][3] = 0;
    ipLut[3][0] = 0; ipLut[3][1] = 0; ipLut[3][2] = 1; ipLut[3][3] = 1;

    int cs, ps, jj;
    float bm[2], pm_n[4];
    int err, nErr;
    int bHat;
    int tt1, tt2;
    int tbStCnt;
    int decodeStCnt;
    nErr = 0;

    if (fc == 1)
    {
        bm[0] = *p_pm + *p_hd;
        *(p_sp + nBit * 4 + 0) = 0;
        pm_n[0] = bm[0];

        bm[0] = *(p_pm + 2) + *(p_hd + 2);
        *(p_sp + nBit * 4 + 1) = 2;
        pm_n[1] = bm[0];

        bm[0] = *p_pm + *(p_hd + 3);
        *(p_sp + nBit * 4 + 2) = 0;
        pm_n[2] = bm[0];

        bm[0] = *(p_pm + 2) + *(p_hd + 1);
        *(p_sp + nBit * 4 + 3) = 2;
        pm_n[3] = bm[0];

    }
    else {

        bm[0] = *p_pm + *p_hd;
        bm[1] = *(p_pm + 1) + *(p_hd + 3);
        *(p_sp + nBit * 4 + 0) = (bm[0] < bm[1]) ? 0 : 1;
        pm_n[0] = bm[(*(p_sp + nBit * 4 + 0))];

        bm[0] = *(p_pm + 2) + *(p_hd + 2);
        bm[1] = *(p_pm + 3) + *(p_hd + 1);
        *(p_sp + nBit * 4 + 1) = (bm[0] < bm[1]) ? 2 : 3;
        pm_n[1] = bm[(*(p_sp + nBit * 4 + 1)) - 2];

        bm[0] = *p_pm + *(p_hd + 3);
        bm[1] = *(p_pm + 1) + *p_hd;
        *(p_sp + nBit * 4 + 2) = (bm[0] < bm[1]) ? 0 : 1;
        pm_n[2] = bm[(*(p_sp + nBit * 4 + 2))];

        bm[0] = *(p_pm + 2) + *(p_hd + 1);
        bm[1] = *(p_pm + 3) + *(p_hd + 2);
        *(p_sp + nBit * 4 + 3) = (bm[0] < bm[1]) ? 2 : 3;
        pm_n[3] = bm[(*(p_sp + nBit * 4 + 3)) - 2];

    }
    *p_pm = pm_n[0];
    *(p_pm + 1) = pm_n[1];
    *(p_pm + 2) = pm_n[2];
    *(p_pm + 3) = pm_n[3];

    // trace back

    if ((startTB == 1) || (lc == 1))
    {
        tt1 = (pm_n[0] < pm_n[1]) ? 0 : 1;
        tt2 = (pm_n[2] < pm_n[3]) ? 2 : 3;
        if (pm_n[tt1] < pm_n[tt2]) {
            cs = tt1;
        }
        else{
            cs = tt2;
        }

        tbStCnt = nBit;
        decodeStCnt = tbStCnt - TB;
        if (lc == 1)
        {
            cs = 0;
            decodeStCnt = tbStCnt - 2;
        }


        for (jj = tbStCnt; jj >= 0; jj--)
        {
            ps = *(p_sp + 4 * jj + cs);

            /* counting the errors */
            if (jj < decodeStCnt)
            {
                bHat = ipLut[cs][ps];
                err = (bHat == *(p_bStore + jj)) ? 0 : 1;
                nErr = nErr + err;
            }
            cs = ps;

        }

    }

    return nErr;

}