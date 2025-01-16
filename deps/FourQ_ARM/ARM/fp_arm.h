/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: modular arithmetic and other low-level operations for ARM processors
************************************************************************************/

#ifndef __FP_ARM_H__
#define __FP_ARM_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


#include "../table_lookup.h"
#include "../FourQ_params.h"

static const digit_t mask_7fff = (digit_t) (-1) >> 1;
static const digit_t prime1271_0 = (digit_t) (-1);
#define prime1271_1 mask_7fff


void digit_x_digit(digit_t a, digit_t b, digit_t *c);


void fpcopy1271(felm_t a, felm_t c);


void fpzero1271(felm_t a);


void fpadd1271(felm_t a, felm_t b, felm_t c);


void fpsub1271(felm_t a, felm_t b, felm_t c);


void fpneg1271(felm_t a);


void fpmul1271(felm_t a, felm_t b, felm_t c);


void fpsqr1271(felm_t a, felm_t c);


void mod1271(felm_t a);


void mp_mul(const digit_t *a, const digit_t *b, digit_t *c, const unsigned int nwords);


unsigned int mp_add(digit_t *a, digit_t *b, digit_t *c, unsigned int nwords);


void fpexp1251(felm_t a, felm_t af);


void fpinv1271(felm_t a);


void multiply(const digit_t *a, const digit_t *b, digit_t *c);


unsigned int add(const digit_t *a, const digit_t *b, digit_t *c, const unsigned int nwords);


unsigned int subtract(const digit_t *a, const digit_t *b, digit_t *c, const unsigned int nwords);


void subtract_mod_order(const digit_t *a, const digit_t *b, digit_t *c);


void add_mod_order(const digit_t *a, const digit_t *b, digit_t *c);


void Montgomery_multiply_mod_order(const digit_t *ma, const digit_t *mb, digit_t *mc);


void modulo_order(digit_t *a, digit_t *c);


void conversion_to_odd(digit_t *k, digit_t *k_odd);


void fpdiv1271(felm_t a);


void fp2div1271(f2elm_t a);


#ifdef __cplusplus
}
#endif


#endif
