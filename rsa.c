
#include "rsa.h"

/*
* This is RSA algorithm program which takes an input as p and q with numeric plaintext,
* It encrypts the user provided plaintext and decrypts the ciphertext using RSA algorithm.
* We have also added RSA decryption using CRT(Chinese Remainder Theorem) and it prints
* the execution time of each encryption, decryption and decryption using CRT.
*/

/* The do_sqrt() function calculates the nonnegative value of the square root of x. */
double
do_sqrt (llint number)
{
  double sqrt = number;

  while ((sqrt - (double) number / sqrt) > 0.00001)	//loop until precision satisfied 
    {
      sqrt = (sqrt + number / sqrt) / 2;
    }
  return sqrt;
}


/* The findInv() function calculates modular inversion */
llint
findInv (llint x, llint y, llint n)
{
  llint d = 2, temp;

  while (d < n)
    {
      temp = ((d * x) - 1) % y;
      if (temp == 0)
	break;
      d++;
    }
  return d;
}


/* Function to check a given number is prime number or not */
int
prime (llint pr)
{
  int j = (int) do_sqrt (pr);
  for (int i = 2; i <= j; i++)
    {
      if (pr % i == 0)
	return 0;
    }
  return 1;
}

/* Function to calculate public key e */
void
calculate_e (struct rsa_key_pair *rkp, llint p, llint q, llint totient)
{
  int k = 0;
  for (llint i = 2; i < totient; i++)
    {
      if (totient % i == 0)
	continue;
      llint flag = prime (i);
      if (flag == 1 && i != p && i != q)
	{
	  rkp->e[k] = i;
	  flag = calculate_d (rkp->e[k], totient);
	  if (flag > 0)
	    {
	      rkp->d[k] = flag;
	      k++;
	    }
	  if (k == MAX_SIZE - 1)
	    break;
	}
    }
}


/* Function to calculate private key d */
long int
calculate_d (llint e, llint totient)
{
  llint k = 1;
  while (1)
    {
      k = k + totient;
      if (k % e == 0)
	{
	  return (k / e);
	}
    }
}

/* Function to encrypt the plaintext to cipher text */
llint
rsa_encrypt (struct rsa_key_pair * rkp, llint msg)
{
  llint pt, ct, key = rkp->e[0], len, k = 1;
  printf
    ("\n----------------------------------------------------------------------------------------\n\n");
  printf ("%sRSA Encryption:%s\n", KYEL, KWHT);
  printf ("\n\tPlaintext '%lld'", msg);
  printf ("\n\tPublic key (e, n) : (%lld, %lld)", key, rkp->n);
  pt = msg;
  k = 1;
  for (int j = 0; j < key; j++)
    {
      k = k * pt;
      k = k % rkp->n;
    }
  ct = k;

  printf ("\n\tThe encrypted message (ciphertext) is : ");
  printf ("%lld\n\n", ct);
  return ct;
}


/* Function to decrypt the ciphertext to plaintext */
llint
rsa_decrypt (struct rsa_key_pair * rkp, llint ct)
{
  llint pt, key = rkp->d[0], k = 1;
  printf
    ("\n----------------------------------------------------------------------------------------\n\n");
  printf ("%sRSA Decryption:%s\n", KYEL, KWHT);
  printf ("\n\tCiphertext %lld", ct);
  printf ("\n\tPrivate key (d , n) <--> (%lld , %lld)", key, rkp->n);
  k = 1;
  for (int j = 0; j < key; j++)
    {
      k = k * ct;
      k = k % rkp->n;
    }
  pt = k;
  printf ("\n\tThe decrypted message (plaintext) is : ");
  printf ("%lld\n\n", pt);
  return pt;
}



/* Function to decrypt the ciphertext to plaintext using CRT (Chinese Remainder Theorem) */
llint
rsa_decrypt_crt (struct rsa_key_pair * rkp, llint ct, struct crt_params crt_p,
		 llint p, llint q)
{
  llint key = rkp->d[0], pt, k = 1;
  llint m1 = 1, m2 = 1;
  printf
    ("\n----------------------------------------------------------------------------------------\n\n");
  printf ("%sDecryption usig CRT(Chine Remainder Theorem) :%s\n", KYEL, KWHT);
  printf ("\n\tCiphertext %lld", ct);
  printf ("\n\tPrivate Key (d, n):(%lld, %lld)", key, rkp->n);
  m1 = 1;
  m2 = 1;
  k = 1;
  //printing original message
  for (int j = 1; j <= crt_p.dP; j++)
    {
      m1 = (m1 * ct) % p;
    }

  for (int j = 1; j <= crt_p.dQ; j++)
    {
      m2 = (m2 * ct) % q;
    }

  llint h = (crt_p.qInv * (m1 + p - m2)) % p;
  k = m2 + (h * q);
  pt = k;
  printf ("\n\tThe decrypted message ( plaintext ) with CRT is : ");
  printf ("%lld\n\n", pt);
}


void
main ()
{

  llint p, q, totient, msg, ct, mcrt, j;

  struct rsa_key_pair rkp;
  struct crt_params crt_p;

  printf ("\nEnter first prime number (p): ");
  scanf ("%lld", &p);

  int flag = prime (p);
  if (flag == 0)
    {
      printf ("\n Input value %lld is not a prime number\n", p);
      exit (-1);
    }
  printf ("Enter another prime number (q): ");
  scanf ("%lld", &q);


  flag = prime (q);
  if (flag == 0 || p == q)
    {
      printf ("\n Input value %lld is not a prime number\n", q);
      exit (-1);
    }
  printf ("Enter a numeric message to encrypt (Plain text) : ");
  fflush (stdin);
  scanf ("%lld", &msg);

  rkp.n = p * q;
  printf ("\nCompute RSA modulus %s n %s= p * q = %lld * %lld = %lld", KGRN,
	  KWHT, p, q, rkp.n);

  totient = (p - 1) * (q - 1);
  printf
    ("\nCompute %s φ(n) %s= totient = (p – 1) * (q – 1) = %lld * %lld = %lld",
     KGRN, KWHT, p - 1, q - 1, totient);

  calculate_e (&rkp, p, q, totient);
  j = do_sqrt (p < q ? p : q);
  printf
    ("\nPossible values of (public key, private key) <--> %s(e, d)%s are :\n",
     KGRN, KWHT);
  for (int i = 0; i < j - 1; i++)
    printf ("(%lld, %lld)\n", rkp.e[i], rkp.d[i]);

  //calculating CRT exponents dP, dQ and qInv to make calculations shorter and faster
  crt_p.dP = findInv (rkp.e[0], p - 1, rkp.n);
  crt_p.dQ = findInv (rkp.e[0], q - 1, rkp.n);
  crt_p.qInv = findInv (q, p, rkp.n);
  printf
    ("\nThe computed parameters for CRT(Chinese Raminder Theorem): %s dP=%lld, dQ=%lld, qInv=%lld %s\n",
     KGRN, crt_p.dP, crt_p.dQ, crt_p.qInv, KWHT);


  clock_t start = clock ();
  llint e_msg = rsa_encrypt (&rkp, msg);
  double rsa_e_time = (double) (clock () - start) * 1000 / CLOCKS_PER_SEC;
  printf ("%s", KGRN);
  printf ("\n\tEncryption time is %f ms", rsa_e_time);
  start = clock ();
  printf ("%s\n", KWHT);
  llint d_msg = rsa_decrypt (&rkp, e_msg);
  double rsa_d_time = (double) (clock () - start) * 1000 / CLOCKS_PER_SEC;
  printf ("%s", KGRN);
  printf ("\n\tDeccryption time is %f ms\n", rsa_d_time);
  printf ("%s", KWHT);
  start = clock ();
  llint d_crt = rsa_decrypt_crt (&rkp, e_msg, crt_p, p, q);
  double crt_d_time = (double) (clock () - start) * 1000 / CLOCKS_PER_SEC;
  printf ("%s", KGRN);
  printf
    ("\n\tDecryption time with CRT(Chinese Remainder Theorem) is %f ms\n",
     crt_d_time);
  printf ("%s", KWHT);
  printf
    ("\n\t-------------------------------------------------------------------\n");
  printf
    ("\n\t Comparison between RSA Decryption Time and RSA Decryption Time using CRT(Chinese Remainder Theorem) \t\n");
  printf
    ("\n%s\t RSA Decryption Time using CRT = (RSA Decryption Time/CRT Decryption Time)/100= %f times fast\t%s",
     KGRN, (crt_d_time / rsa_d_time) * 100, KWHT);
  printf
    ("\n-------------------------------------------------------------\n");
}
