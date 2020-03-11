#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
//#include "prog1.h"

typedef struct triplet_type
{
   int value; 
   int radix;
   int opsize;
   char inputstring[80];
   struct triplet_type *next;
} triplet;

void RemoveNonSpaces (char *);
int CountNumbersInString (char *);
bool FindNextNumber (char *, int *, char *);
void ParseStringIntoTriplets (char *);
void PrintResultsTable (void);
void PrintBinaryNumber (int, int);
int OpenDataInputFile (char *);
struct triplet_type *triplet_start;
struct triplet_type *triplet_current;
#define VALUE 1
#define RADIX 2
#define OPSIZE 3

/***************************************************************
 *
 * Main Routine of prog1.c
 *
 * This program processes a file called datain
 * and reads from that file number triplets representing 
 * a value, the radix of that value, and then the number
 * of binary digits which will be used to hold that value.
 * These values are read and stored in a linked list structure
 * called triplet.  This structure is a global variable.
 * The values are then processed and output to screen in a
 * requested format, with values outside of range flagged
 * with an error message.
 *
 ***************************************************************/
int main ()
{
   //int count;
   char inputstring[256];
   char *p;


   OpenDataInputFile(inputstring);
   p = inputstring; // Set p to point to first character in inputstring
   
   // Get rid of hard-to-understand formatting characters
   RemoveNonSpaces(p);
   // printf("Spaces Removed: '%s'\n", p);
   
   // Count number of numbers just as a cross-check.  This not actually used later.
   //count = CountNumbersInString(p);
   //printf("the number of numbers are :\t%d\n", count);
   
   ParseStringIntoTriplets(p);

   PrintResultsTable();
   return(0);
}


/***************************************************************
 *
 *
 * Function OpenDataInputFile
 *
 * This function opens a file of name datain and reads the info 
 * to a string, which is stored in a provided address.  
 *
 *
 ***************************************************************/
int OpenDataInputFile (char *inputstring)
{
   FILE *f;

   // open the input file and read the string
   f = fopen("datain", "r"); 
   if (f == NULL)
   {
      perror("Error opening file"); // print error
      return(-1); 
   }
   else 
   { 
      fgets(inputstring, 256, f); // read from file
      //puts(inputstring); // print read content out to stdout
   }
   fclose(f); // close file
   return(0);
}

/***************************************************************
 *
 *
 * Function PrintResultsTable
 *
 * This function prints out the global triplet linked list
 * data structure to screen. Basic max binary value error 
 * checking is performed at this time.
 *
 *
 ***************************************************************/
void PrintResultsTable()
{
   int i, binary_max, binary_neg_max;

   // OK we have found all the numbers.  Now print.
   triplet_current = triplet_start;
   while (triplet_current->next != NULL)
   {
      // Set the maximum positive-only and pos-or-neg values for a given opsize
      binary_max = (1<<triplet_current->opsize)-1;                // 2^n -1
      binary_neg_max = ((1 << (triplet_current->opsize-1)) - 1);  // 2^(n-1) -1
      //printf("Debug: max value is %d!\n", binary_neg_max);
      
      // Print the input line, which was already formatted for us nicely             
      printf("%s", triplet_current->inputstring);

      // Print the output line which are just column headers
      printf("Output:\t\t\t\tValue\t\tMaximum\t\tMinimum\n");

      // Print the binary absolute value line 
      printf("Binary (abs):\t\t\t");
      if (abs(triplet_current->value) > binary_max)
      {
         // Value exceeds number of binary digits
         printf("Error");
      }
      else 
      {
         PrintBinaryNumber (abs(triplet_current->value), triplet_current->opsize);
      }
      printf("\t\t");
      PrintBinaryNumber (binary_max, triplet_current->opsize);
      printf("\t\t");
      PrintBinaryNumber (0, triplet_current->opsize);
      printf("\n");

      // Print the octal absolute value line
      printf("Octal (abs):\t\t\t%#*o\t\t%*o\t\t0\n",
              (int)floor(triplet_current->opsize/3),
              abs(triplet_current->value),
              (int)floor(triplet_current->opsize/3),
              binary_max); 

      // Print the decimal absolute value line
      printf("Decimal (abs):\t\t\t%d\t\t%d\t\t0\n",
              abs(triplet_current->value),
              binary_max); 

      // Print the hexadecimal absolute value line
      printf("Hexadecimal (abs):\t\t%#*X\t\t%#*X\t\t%#*X\n",
              (int) floor(triplet_current->opsize/4),
              abs(triplet_current->value),
              (int) floor(triplet_current->opsize/4),
              binary_max,
              (int) floor(triplet_current->opsize/4),
              0); 

      // One's Complement Line
      printf("Signed One’s Compliment:\t");
      if (abs(triplet_current->value) > binary_neg_max)
      {
         // Value exceeds number of binary digits
         printf("Error");
      }
      else 
      {
         PrintBinaryNumber (triplet_current->value-1, triplet_current->opsize);
      }
      printf("\t\t");
      PrintBinaryNumber ((1<<(triplet_current->opsize-1))-1, triplet_current->opsize); // Max
      printf("\t\t0b1");
      for (i = 1 << (triplet_current->opsize-2); i > 0; i = i / 2) 
        printf("0"); 
      printf("\n");   

      // Two's Complement Line
      printf("Signed Two’s Compliment:\t");
      if (abs(triplet_current->value) > binary_neg_max)
      {
         // Value exceeds number of binary digits
         printf("Error");
      }
      else 
      {
         PrintBinaryNumber ((triplet_current->value), triplet_current->opsize);
      }
      printf("\t\t");
      PrintBinaryNumber ((1<<(triplet_current->opsize-1))-1, triplet_current->opsize); // Max
      printf("\t\t0b1");
      for (i = 1 << (triplet_current->opsize-3); i > 0; i = i / 2) 
        printf("0"); 
      printf("1\n");  
 
      // Sign Magnitude Line 
      printf("Sign Magnitude:\t\t\t");
      if (abs(triplet_current->value) > binary_neg_max)
      {
         // Value exceeds number of binary digits
         printf("Error");
      }
      else 
      {
         printf("0b");
         (triplet_current->value > 0) ? printf("0") : printf("1"); 
         for (i = 1 << (triplet_current->opsize-2); i > 0; i = i >> 1) 
            (abs(triplet_current->value) & i) ? printf("1") : printf("0"); 
      }
      printf("\t\t");
      PrintBinaryNumber ((1<<(triplet_current->opsize-1))-1, triplet_current->opsize); 
      printf("\t\t");
      PrintBinaryNumber (binary_max, triplet_current->opsize);
      printf("\n");   
      triplet_current = triplet_current->next;
   }
   return;
}

/***************************************************************
 *
 *
 * Function PrintBinaryNumber
 *
 * This function prints out a binary number of specified 
 * # digits to screen, without linefeed.
 *
 *
 ***************************************************************/
void PrintBinaryNumber (int binnum, int numdigits)
{
   int i;   
   
   printf("0b");
   for (i = 1 << (numdigits-1); i > 0; i = i >> 1) 
        (binnum & i) ? printf("1") : printf("0"); 
   return;
}

/***************************************************************
 *
 * Function ParseStringIntoTriplets
 *
 * This function processes the input string into triplets for
 * use by the output printing function.
 *
 ***************************************************************/
void ParseStringIntoTriplets (char *p)
{   
   int state, index;
   bool found;
   char number_string[20];
   char value_string[20];
   char *end;

   // Create first node in linked list to store triplets
   triplet_start = (triplet *) malloc(sizeof(triplet));
   // Create pointer to current triplet to traverse the list
   triplet_current = triplet_start;

   state = VALUE;
   index = 0;
   while (index <= strlen(p))
   {
      found = FindNextNumber(p, &index, number_string);
      if (found) 
      {
         // Found a valid number.  Process it.
         //printf("Index: %d of %ld, %s\n", index, strlen(p), number_string);
         switch (state)
         {
            case VALUE:            
               // just capture the string.
               strcpy(value_string, number_string);
               state = RADIX;
               break;

            case RADIX:
               // OK, now process the value and the radix both
               triplet_current->radix = atoi(number_string);
               triplet_current->value = strtol(value_string, 
                                               &end, 
                                               triplet_current->radix);
               state = OPSIZE;
               break;

            case OPSIZE:
               triplet_current->opsize = atoi(number_string);
               sprintf(triplet_current->inputstring, 
                       "Input: %s\t\tRadix: %d\t Operand Size: %d\n", 
                       value_string,
                       triplet_current->radix,
                       triplet_current->opsize);
               state = VALUE;
               
               triplet_current->next = (triplet *) malloc(sizeof(triplet));
               triplet_current = triplet_current->next;
               triplet_current->next = NULL; // Just in case we don't need this next one
               break;   
            
         }
      }
   }
   return;
}

/***************************************************************
 *
 * Function FindNextNumber
 *
 * This function skips spaces and just grabs the start of the
 * next number, which may be a hex number, and then moves
 * up the string parsing index to where it stopped.  It 
 * returns a number if found and returns true if it found a
 * number and false if it exited without finding the start
 * of the next number, in which case it just advances the
 * string parsing index.  This function is called until the 
 * index reaches end of string.
 *
 ***************************************************************/
bool FindNextNumber (char *p, int *int_ptr, char *next_num_str)
{
   int i, innum, len;

   innum = 0;         
   len = strlen(p);

   i = *int_ptr;
   if (p[i] == ' ' || p[i] == '\0')  // If space, just kick back
   {
      *int_ptr = *int_ptr + 1;   // Move pointer up to current spot so we pickup from here
      //printf("%c, %d\n", p[i], *int_ptr);
      return(false);
   }  

   for (i = *int_ptr; i < len; i++) 
   { 
      // Valid numbers have a hex digitt or a '-' sign or an 'x' 
      if (isxdigit(p[i]) || p[i] == '-' || p[i] == 'x')  
      {
         innum++;         
         if (innum==1)
         {
            // This is the start of a number
            next_num_str[0] = p[i]; // Start capturing the number to the temp string array
         }
         else
         {
            // This is more of the number
            // Conveniently, innum has the current char position of temp string array 
            // So keep capturing the number until we get to a space to indicate the number's end
            next_num_str[innum-1] = p[i]; 
         } 
      }
      else
      {
         // Next digit is a non-number, so must be at end of number.
         if (innum)
         {
            // Then we found a numberish thing and are done capturing it
            next_num_str[innum] = '\0';  // Add end of string marker
            //printf("Sub found this: %d :: %s\n", innum, next_num_str);
            *int_ptr = i;                // Move pointer up to current spot so we pickup from here
            return(true);  
         }
         else
         { 
            // Non-space found at end of string
            *int_ptr = *int_ptr + 1;   // Move pointer up to current spot so we pickup from here
            //printf("%c, %d\n", p[i], *int_ptr);
            return(false);
         }
      }
   }   
   return(false);
}

/***************************************************************
 *
 * Function RemoveNonSpaces
 *
 * This function goes through a string and removes all bracket
 * and comma characters and replaces with spaces.
 *
 ***************************************************************/
void RemoveNonSpaces (char *p)
{
   int i;
   for (i=0; i<strlen(p); i++)
   {
      if (p[i]=='{' || p[i] == '}' || p[i] == ',')
      {
         p[i]=' ';
      }
   }
}


/***************************************************************
 *
 * Function CountNumbersInString
 *
 * This function counts the number of numbers in the string
 * and is only used for error checking and debug in this code.
 *
 ***************************************************************/
int CountNumbersInString (char *p)
{
   int i, count, innum, len;

   count = 0;
   len = strlen(p); 
   innum = 0;         // handle space at the beginning

   for(i = 0; i < len; i++) 
   { 
      if(isdigit(p[i]) || p[i] == '-')  // Valid numbers begin with a number or a '-' sign
      {
         if (!innum)
         {
            innum++;
            count++;
         }
      }
      else
      {
         innum=0;
      }
   }       
     
   return (count);
}
