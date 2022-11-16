#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

bool checkifprime (long long);
int countchars (char *);
int gcd (long long, long long);
long long ExtendedEuclidAlgo (long long, long long, long long *, long long *);
long long linearCongruence (long long, long long);
long long inverse (long long, long long);
void encrypt (char *, long long *, long long, long long);
void decrypt (long long *, long long *, long long, long long, int);
long long encrypt_decrypt_value (long long, long long, long long);
void displaytimestamp ();
long long  power (long long, long long, long long);
long long huge_multiply (long long, long long, long long);
int firstdigit (unsigned long long);


int
main ()
{
  int i;
  int prime1, prime2;
  bool primefound;
  char buffer[255];
#define INPUT_MESSAGE_SIZE 255

  printf ("RSA CRYPTO SIMULATION\n\n");
  
  // Gather 2 prime numbers and message as input.
  primefound = false;
  while (!primefound)
    {
      printf ("Enter the first prime: ");
      fgets (buffer, sizeof (buffer), stdin);
      sscanf (buffer, "%d", &prime1);
      primefound = checkifprime (prime1);
    }

  primefound = false;
  while (!primefound)
    {
      printf ("Enter the second prime: ");
      fgets (buffer, sizeof (buffer), stdin);
      sscanf (buffer, "%d", &prime2);
      primefound = checkifprime (prime2);
    }

  char input_message[INPUT_MESSAGE_SIZE];
  printf ("Enter the message to encrypt: ");
  fgets (input_message, sizeof (input_message), stdin);
  //printf("\nyou entered :%s", input_message);


  // **********************************************************************************
  // calculate N, totient(N) 
  // **********************************************************************************
  long long totient, n, count;
  totient = ((long long)prime1 - 1) * ((long long)prime2 - 1);
  n = (long long) prime1 *(long long) prime2;
  printf ("\n--------------------------\n");
  printf ("*** Totient =:%lld\n", totient);


  // **********************************************************************************
  // choose a value for e such that totient > e > 1 and coprime to the totient.  start about 3/4 the way to totient */
  // **********************************************************************************
  //printf (">Starting to calclate e\n");
  //displaytimestamp ();
  long long e;
  if (totient > 10000000)
    {
      e = (totient / 100000) - 1;
    }
  else
    {
      e = totient / 1.25;
    }

  //GCD of e and totient must be 1 to ensure only one solution for d.
  while (e < totient)
    {
      //printf("GCD check for %d, %d \n",e,totient);
      count = gcd (e, totient);

      if (count == 1)
	break;
      else
	e--;
    }

  printf ("*** Public key (e,n) = %lld,%lld\n", e, n);

  // **********************************************************************************
  // Calculate the decryption key.  Solve ed == 1 mod(totient) */
  // **********************************************************************************
  long long d;
  d = linearCongruence(e, totient);
  printf ("*** Decryption key (d)  =:%lld\n", d);
    
  //alternate way to calculate d is to loop from 1 to totient but that is not efficient.
  //d = inverse (e, totient);
  //printf ("*** Decryption key via <inverse> =:%lld\n", d);
  printf ("---------------------------------------------------\n\n");


  // **********************************************************************************
  // encrypt the message.  Pass the pointer to the message.  Pass the public key e,n
  // **********************************************************************************
  long long cipher_text[INPUT_MESSAGE_SIZE];

  printf ("==> beginning Encryption\n");
  //displaytimestamp ();
  encrypt (input_message, cipher_text, e, n);
  
  // display the cipher_text
  int input_size = countchars (input_message);
  printf ("*** Cipher Text =: ");
  for (i = 0; i < input_size; i++)
    {
      printf ("%lld", cipher_text[i]);
    }
  printf ("\n");

  // **********************************************************************************
  // Decrypt the message.  Pass the pointer to the message.  Pass the public key e,d
  // **********************************************************************************
  long long decrypted_text[INPUT_MESSAGE_SIZE];

  printf ("\n==> beginning Decryption\n");
  //displaytimestamp ();
  decrypt (decrypted_text, cipher_text, d, n, input_size);
  printf ("\n*** Decrypted Text =: ");
  for (i = 0; i < input_size; i++)
    {
      printf ("%c", (int) decrypted_text[i]);
    }

  //printf ("\n==> ending Decryption\n");
  //displaytimestamp ();


  return 0;
};

bool
checkifprime (long long checkvalue)
{
  long long i;
  for (i = 2; i <= checkvalue / 2; ++i)
    {

      // if checkvalue is divisible by i, then n is not prime
      if (checkvalue % i == 0)
	{
	  printf ("%lld is not a prime number.  --> please re-enter\n",
		  checkvalue);
	  return false;
	}
    }

  return true;


}

// Count the num of chars in an input string_input
int
countchars (char *string_input)
{
  int totChar, i;
  totChar = 0;
  for (i = 0; string_input[i] != '\0'; i++)
    {
      totChar++;
    }
  totChar = totChar - 1;
  return totChar;
}

//to find gcd
//uses the recursive euclidien algorithm 
int
gcd (long long a, long long h)
{
  int temp;
  while (1)
    {
      temp = a % h;
      if (temp == 0)
	return h;
      a = h;
      h = temp;
    }
}

long long
ExtendedEuclidAlgo (long long a, long long b, long long *x, long long *y)
{

  // Base Case
  if (b == 0)
    {
      *x = 1;
      *y = 0;
      return a;
    }
  else
    {

      // Store the result of recursive call
      long long gcd = ExtendedEuclidAlgo (b, a % b, x, y);

      // Update x and y using results of
      // recursive call
      long long x1 = *x, y1 = *y;
      *x = y1;
      *y = x1 - (a / b) * y1;


      return gcd;
    }

}

// Function to give the distinct
// solutions of ax = b (mod n).  Set B = 1
long long
linearCongruence (long long e, long long totient)
{

  long long B = 1, u = 0, v = 0;
  
  e = e % totient;

  long long d;
  d = ExtendedEuclidAlgo (e, totient, &u, &v);

  // No solution exists
  if (1 % d != 0)
    {
      printf (" NO SOLUTION EXISTS\n");
      return 0;
    }

  // initialize the value of x0
  long long x0 = (u * (B / d)) % totient;
  if (x0 < 0)
    x0 += totient;


  return x0;

}

long long
inverse (long long e, long long totient)
{

  long i;
  unsigned long long tmp = 0, MI = 0;


 for (i = 0; i <= totient - 1; i++)   {
      tmp = (unsigned long long)i *(unsigned long long)e;
      MI = tmp % (unsigned long long) totient;
      //printf	("calculating inverse. i=%ld \t tmp= %llu, totient=%ld \t MI=%llu\n",	 i, tmp, totient, MI);
      if (MI == 1)
	{
	  break;
	}


    }
  return i;
}

void
encrypt (char *message, long long *working_message, long long e, long long n)
{
  int i = 0;
  int input_size = countchars (message);

  // this converts the cipher text to an array of integers, to individually be converted.  later we can use chunking          
  for (i = 0; i < input_size; i++)
    {
      //add 100 to make sure all the ascii values are 3 digits
      working_message[i] = encrypt_decrypt_value (((long long)message[i]), e, n);
      //printf 	("Char: %c \t translates to a numerical value=%d and is stored in array as Cipher Text:%ld\n",	 message[i], message[i], working_message[i]);
    }

  return;
}

void
decrypt (long long *decrypted_text, long long *cipher_text, long long d, long long n, int size)
{
  int i = 0;

  // this converts the cipher text to an array of integers, to individually be converted.  later we can use chunking          
  for (i = 0; i < size; i++)
    {
      decrypted_text[i] = encrypt_decrypt_value ((cipher_text[i]), d, n);
    }

  return;
}

long long
encrypt_decrypt_value (long long input_code, long long ed, long long n)
{
  // to encypt calculate  M^e congruent C (mod n)
  long long tmp2 = 1;
  long long tmp3 = 0;
  long long i = 1, k = 1;
 
  // calculate m*m mod n.
  /* while (i <= ed)
     {
     k = i;
     tmp2 = (tmp2 * input_code);
     tmp3 = tmp2 % n;
     i++;
        //printf("Loop%ld:  \t x itself value = %ld\t value after modulous %ld=%ld\n", k, tmp2, n, tmp3);
     tmp2 = tmp3;
     } */
   

  // Alternatively, use the square and multiply method, reducing to base 2.
  tmp2 = power ((long long)input_code, ed, n);
  
  // Alternatively, use the square and multiply method, reducing to base 2.
  //tmp2 = power2 ((long)input_code, ed, n);
  
  printf
    ("input_code: %lld\t ^exp %lld\t  = %lld value after modulous %lld\n",
     input_code, ed, tmp2, n);

  return tmp2;
}

void
displaytimestamp ()
{

  time_t rawtime;
  struct tm *timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  //printf ( "------------------------------->  Current local time and date: %s", asctime (timeinfo) );
  printf (" %s", asctime (timeinfo));

  return;
}

/* Iterative Function to calculate (x^y)%p in O(log y) */
long long
power (long long x, long long y, long long n)
{
 // printf     ("\n\nPower funcion starting with input_code = %lld\t E=%lld\t mod=%lld\n\n", x, y, n);
  //printf (" iteration #\t\tSTART VALUES\t\t\t\t\t\tEND VALUES\n");
  //printf ("\t\tRes\t\tx\t\ty\t\t\tRes\t\tx\t\ty\n");
 // printf (" ========\t===\t\t====\t\t====\t\t\t====\t\t====\t\t====\n");
  long long res = 1;			// Initialize result
  int count = 1;
  unsigned long long int tmpx;
  unsigned long long int tmpres;
  x = x % n;			// Update x if it is more than or
  // equal to p
  //printf("x: initial value after part 1=:%ld\n",x);

  if (x == 0)
    return 0;			// In case x is divisible by p;

  while (y > 0)
    {
      // If y is odd, multiply x with result
/*      if (y & 1)
	{
	  printf ("*");
	}
*/
      //printf ("%d\t\t%lld\t\t%lld\t\t%lld", count, res, x, y);
      if (y & 1)
	{
	  //printf("---ODD START! res=%ld\n",res);
    // Check for possible overlow
    // max value for unsigned long long is 18,446,744,073,709,551,615  (20 digits)
    // 10 digits x 10 digits prouces 20 but dont max out unless the first digit x first digit > 18
/*      if (res > 999999999 && x > 9999999999)
	{
	  printf(" ERR1a"); 
	 if (   (firstdigit(res) * firstdigit(x)) > 1)
	  {
	      printf(" ERR1b");  
	      
	  }
	}  */
	
	  tmpres = huge_multiply(res,x,n);
	  //tmpres = ((unsigned long long int) res * (unsigned long long int) x) % 	    (unsigned long long int) n;
	  res = tmpres;
	  //printf("---ODD END! res=%ld\n",res);
	}



      // y must be even now
      y = y >> 1;		// y = y/2
      //y = y/2; // y = y/2
    // Check for possible overlow
/*      if (x > 999999999)
	{
	  	  printf(" ERR2a");  
	  	  if (   (firstdigit(x) * firstdigit(x)) > 1)
	  {
	      printf(" ERR2b");  
	      
	  }
	}
*/


      tmpx = huge_multiply(x,x,n);
      //tmpx = ((unsigned long long int) x * (unsigned long long int) x) % 	(unsigned long long int) n;
      x = tmpx;
      count++;
      //printf ("\t\t\t%lld\t\t%lld\t\t%lld\n", res, x, y);
      //printf("End of power funcion iteration.  res=:%ld\t x value=%ld\t E=%ld\t mod=%ld\n",res, x, y, n);
    }
  //printf ("\npower funcion returning value = %lld\n", res);
  //printf ("-------------------------------------\n\n");
  return res;
}



/* Iterative Function to calculate (x^y)%p in O(log y) */
int firstdigit (unsigned long long n)
{

    int result;
    // Remove last digit from number
    // till only one digit is left
    while (n >= 10)
        n /= 10;
     
    // return the first digit
    //printf ("First digit =%d ",(int)n);
    return (int) n;
}

// a * b mod (m) reqires special handling when a,b,b are maxing out the value of the data type.  overflow may occur.
 long long
huge_multiply (long long a, long long b, long long mod)
{
  
    long long res = 0;  // Initialize result
 
    // Update a if it is more than
    // or equal to mod
    a %= mod;
 
    while (b)
    {
        // If b is odd, add a with result
        if (b & 1)
            res = (res + a) % mod;
 
        // Here we assume that doing 2*a
        // doesn't cause overflow
        a = (2 * a) % mod;
 
        b >>= 1;  // b = b / 2
    }
 
    return res;

}

