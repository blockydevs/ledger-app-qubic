/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: table lookup functions
************************************************************************************/

#ifndef __TABLE_LOOKUP_H__
#define __TABLE_LOOKUP_H__


// For C++
#ifdef __cplusplus
extern "C" {
#endif


#include "FourQ_internal.h"


void table_lookup_1x8(point_extproj_precomp_t* table, point_extproj_precomp_t P, unsigned int digit, unsigned int sign_mask);

void table_lookup_fixed_base(point_precomp_t* table, point_precomp_t P, unsigned int digit, unsigned int sign);


#ifdef __cplusplus
}
#endif


#endif
