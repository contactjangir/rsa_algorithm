#ifndef __RSA_CRT_H__
#define __RSA_CRT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* format specifier for different cosole colors */
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KWHT  "\x1B[37m"

/* Max array size */
#define MAX_SIZE 100


typedef long long int llint;

/* structyre for holding RSA key pair */
struct rsa_key_pair
{
  llint n;
  llint d[MAX_SIZE];
  llint e[MAX_SIZE];
};

/* structyre for holding CRT parameters */
struct crt_params
{
  llint dP;
  llint dQ;
  llint qInv;
};

/* Basic arithmetic operations: */
double do_sqrt (llint number);
int prime (llint);
llint findInv (llint, llint, llint);

/* RSA calculation functions: */
void calculate_e (struct rsa_key_pair *, llint, llint, llint);
long int calculate_d (llint, llint);
llint rsa_encrypt (struct rsa_key_pair *, llint);
llint rsa_decrypt (struct rsa_key_pair *, llint);
llint rsa_decrypt_crt (struct rsa_key_pair *, llint, struct crt_params,
		       llint, llint);
#endif
