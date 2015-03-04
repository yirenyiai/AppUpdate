﻿#ifndef __MD5_H__
#define __MD5_H__

#include <stdio.h>
#include <time.h>
#include <string.h>

#pragma once

/* typedef a 32 bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
	UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
	UINT4 buf[4];                                    /* scratch buffer */
	unsigned char in[64];                              /* input buffer */
	unsigned char digest[16];     /* actual digest after MD5Final call */
} MD5_CTX;

void MD5Init (MD5_CTX *mdContext);
void MD5Update (MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void MD5Final (MD5_CTX *mdContext);
void Transform (UINT4 *buf, UINT4 *in);
void MDPrint (MD5_CTX *mdContext, char* outString);
void MDTimeTrial ();
void MDString (char *inString);
void MDFile (char *filename, char *outString);
void MDFilter ();

#endif // __MD5_H__
