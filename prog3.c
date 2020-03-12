#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

// https://codeforwin.org/2017/12/c-program-rotate-number-bits.html
/* Function declarations */
int rotateLeft(int num, unsigned int rotation);
int rotateRight(int num, unsigned int rotation);
void TestThreeOfLastFourBits (int);
void PrintBinaryNumber (int, int);

int main ()
{

   int value = 0x0000FACE;

   // Use initial value "FACE"
   printf("0x%08X\n",value);
   TestThreeOfLastFourBits (value);

   // Reverse Byte Order 
   value = 0x0000CEFA;
   printf("0x%08X\n",value);
   TestThreeOfLastFourBits (value);

   // Rotate six to left
   printf("0x%08X -> Six to Left -> ",value);
   value = rotateLeft (value, 6);
   printf("0x%08X\n",value);
   TestThreeOfLastFourBits (value);

   // Rotate four to right
   printf("0x%08X -> Four to Right -> ",value);
   value = rotateRight (value, 4);
   printf("0x%08X\n",value);
   TestThreeOfLastFourBits (value);

   return(0);
}

void TestThreeOfLastFourBits (int value)
{

   // test 3 of last 4 bits
   int mask, count = 0;

   for (mask = 1 << 3; mask > 0; mask = mask >> 1)
   {
      count += (mask & value) ? 1 : 0;
   }
   printf("last 4 in binary: ");
   PrintBinaryNumber (value, 4);
   printf ("\n3 of last 4? : %d", count);
   if (count >= 3)
   { 
      printf(" (TRUE)\n");
   }
   else
   { 
      printf(" (FALSE)\n");
   }   
}

void PrintBinaryNumber (int binnum, int numdigits)
{
   int i=0;   
   
   //printf("0b");
   for (i = 1 << (numdigits-1); i > 0; i = i >> 1) 
   {
       //printf("\ni=%d",i);
        (binnum & i) ? printf("1") : printf("0"); 
   }
   return;
}

#define INT_SIZE sizeof(int)        // Size of int in bytes
#define INT_BITS INT_SIZE * 8 - 1   // Size of int in bits - 1

// https://codeforwin.org/2017/12/c-program-rotate-number-bits.html

/**
 * Function to rotate bits of a number to left.
 *
 * @num         Number to rotate.
 * @rotation    Number of times to rotate left.
 */
int rotateLeft(int num, unsigned int rotation)
{
    int DROPPED_MSB;

    // The effective rotation
    rotation %= INT_BITS;


    // Loop till rotation becomes 0
    while(rotation--)
    {
        // Get MSB of num before it gets dropped
        DROPPED_MSB = (num >> (INT_BITS)) & 1; 

        // Left rotate num by 1 and 
        // Set its dropped MSB as new LSB
        num = (num << 1) | DROPPED_MSB;
    }

    return num;
}



/**
 * Function to rotate bits of a number to right.
 *
 * @num         Number to rotate.
 * @rotation    Number of times to rotate right.
 */
int rotateRight(int num, unsigned int rotation)
{
    int DROPPED_LSB;

    // The effective rotation
    rotation %= INT_BITS;


    // Loop till rotation becomes 0
    while(rotation--)
    {
        // Get LSB of num before it gets dropped
        DROPPED_LSB = num & 1;

        // Right shift num by 1 and 
        // Clear its MSB
        num = (num >> 1) & (~(1 << (INT_BITS)));

        // Set its dropped LSB as new MSB
        num = num | (DROPPED_LSB << (INT_BITS));
    }

    return num;
}

