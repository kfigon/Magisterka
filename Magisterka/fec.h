/* User include file for libfec
 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef _FEC_H_
#define _FEC_H_


/* r=1/2 k=9 convolutional encoder polynomials */
#define V29POLYA        0x1af
#define V29POLYB        0x11d

typedef union { unsigned int w[256]; } metric_t;
typedef union { unsigned long w[8];} decision_t;

/* State info for instance of Viterbi decoder */
struct v29 {
  metric_t metrics1; /* path metric buffer 1 */
  metric_t metrics2; /* path metric buffer 2 */
  decision_t *dp;          /* Pointer to current decision */
  metric_t *old_metrics,*new_metrics; /* Pointers to path metrics, swapped on every bit */
  decision_t *decisions;   /* Beginning of decisions for block */
};


void *create_viterbi29_port(int len);
void set_viterbi29_polynomial_port(int polys[2]);
int init_viterbi29_port(v29 *p,int starting_state);
int chainback_viterbi29_port(v29 *p,unsigned char *data,unsigned int nbits,unsigned int endstate);
void delete_viterbi29_port(v29 *p);
int update_viterbi29_blk_port(v29 *p,unsigned char *syms,int nbits);

void partab_init(void);

static inline int parityb(unsigned char x){
    extern unsigned char Partab[256];
    extern int P_init;
    if (!P_init){
        partab_init();
    }
    return Partab[x];
}

static inline int parity(int x){
    /* Fold down to one byte */
    x ^= (x >> 16);
    x ^= (x >> 8);
    return parityb(x);
}

#endif