#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
//#include "prog1.h"

typedef struct letter_type
{
   int code; 
   char type[80];
   struct letter_type *next;
} letter;

void RemoveNonSpaces (char *);
int CountNumbersInString (char *);
bool FindNextNumber (char *, int *, char *);
void ParseString (char *);
void PrintResultsTable (void);
int OpenDataInputFile (char *);
struct letter_type *letter_start;
struct letter_type *letter_current;
#define LOWER_CASE 1
#define UPPER_CASE 2
#define DIGIT 3
#define WHITE_SPACE 4
#define SPECIAL 5

int main ()
{
   //int count;
   char inputstring[256];
   char *p;


   OpenDataInputFile(inputstring);
   p = inputstring; // Set p to point to first character in inputstring
   
   // Get rid of hard-to-understand formatting characters
   RemoveNonSpaces(p);
   //printf("Spaces Removed: '%s'\n", p);
   
   // Count number of numbers just as a cross-check.  This not actually used later.
   //count = CountNumbersInString(p);
   //printf("the number of letters is :\t%d\n", count);
   
   ParseString(p);

   PrintResultsTable();
   return(0);
}

int OpenDataInputFile (char *inputstring)
{
   FILE *f;

   // open the input file and read the string
   f = fopen("ascii_in", "r"); 
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

void PrintResultsTable()
{

   // OK we have found all the numbers.  Now print.
   letter_current = letter_start;
   while (letter_current->next != NULL)
   {
      printf("Code: %d\t\tType: %s\tASCII Char: %c\n",
             letter_current->code,
             letter_current->type,
             letter_current->code);
  
      letter_current = letter_current->next;
   }
   return;
}


void ParseString (char *p)
{   
   int index;
   bool found;
   char number_string[20];

   // Create first node in linked list to store letters
   letter_start = (letter *) malloc(sizeof(letter));
   // Create pointer to current letter to traverse the list
   letter_current = letter_start;

   index = 0;
   while (index <= strlen(p))
   {
      found = FindNextNumber(p, &index, number_string);
      //printf("%d****>>>>%s\n",found,number_string);
      if (found) 
      {
         // Found a valid number.  Process it.
         letter_current->code = atoi(number_string);

         // Set the type
         if (letter_current->code == 0  || // NULL
             letter_current->code == 7  || // Bell
             letter_current->code == 8  || // Backspace
             letter_current->code == 9  || // Tab 
             letter_current->code == 11 || // Tab 
             letter_current->code == 10 || // Line Feed 
             letter_current->code == 13 || // Carriage Return 
             letter_current->code == 32 || // Sub
             letter_current->code == 12)   // Form Feed
         {
            strcpy(letter_current->type, "White Space");
         }
         else if (letter_current->code >= 33 && 
                  letter_current->code <= 47)
         {
            strcpy(letter_current->type, "Special");
         }
         else if (letter_current->code >= 48 && 
                  letter_current->code <= 57)
         {
            strcpy(letter_current->type, "Digit");
         }
         else if (letter_current->code >= 58 && 
                  letter_current->code <= 64)
         {
            strcpy(letter_current->type, "Special");
         }
         else if (letter_current->code >= 65 && 
                  letter_current->code <= 90)
         {
            strcpy(letter_current->type, "Upper Case");
         }
         else if (letter_current->code >= 91 && 
                  letter_current->code <= 96)
         {
            strcpy(letter_current->type, "Special");
         }
         else if (letter_current->code >= 97 && 
                  letter_current->code <= 122)
         {
            strcpy(letter_current->type, "Lower Case");
         }
         else if (letter_current->code > 122) 
         {
            strcpy(letter_current->type, "Special");
         }

         letter_current->next = (letter *) malloc(sizeof(letter));
         letter_current = letter_current->next;
         letter_current->next = NULL; // Just in case we don't need this next one
      } // End Elsif
   } // End While
   return;
}


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
