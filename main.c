#include <stdio.h>
#include <windows.h>
#include <intrin.h>
#include <stdint.h>
#include "kstring.h"
long long ReadTSC() { // Returns time stamp counter
    int dummy[4]; // For unused returns
    volatile int DontSkip; // Volatile to prevent optimizing
    long long clock; // Time
    __cpuid(dummy, 0); // Serialize
    DontSkip = dummy[0]; // Prevent optimizing away cpuid
    clock = __rdtsc(); // Read time
    return clock;
}
LARGE_INTEGER start;
LARGE_INTEGER stop;

int i; long long time1;
enum { NumberOfTests = 5 }; // Number of times to test

void printk(char* ptr)
{
    if(ptr)
    {
        while(*ptr != END_OF_STRING)
        {
            putchar(*ptr++);
        }
    }
}

void scank(char* ptr)
{
    char val;
    if(ptr)
    {
        do
        {
            val = (char)getchar();
            *ptr++ = val;
        }
        while(val != END_OF_STRING);
    }
}

int main ()
{

    
    long long timediff[NumberOfTests];
    char  str1[10000] = "- This, a sample string.";
    char  str2[10000];
    int i = 0;
    while(i<1000)
        str2[i++]=5;
    str2[997] = 'd';
    str2[998] = 'f';
    str2[999] = END_OF_STRING;

	for (i = 0; i < NumberOfTests; i++) 
	{ 
		time1 = ReadTSC(); // Time before test
		/**------------------------------*/
		memcpy_k(str2, str1, 10000);

		/**------------------------------*/
		timediff[i] = ReadTSC() - time1;
	}

	printf("\nMy memcpy:"); // Print heading
	for (i = 0; i < NumberOfTests; i++)
	{ 
		printf("\n%2i %10I64i", i, timediff[i]);
	}

	for (i = 0; i < NumberOfTests; i++)
	{
		time1 = ReadTSC(); // Time before test
		/**------------------------------*/
		memcpy(str2, str1, 10000);

		/**------------------------------*/
		timediff[i] = ReadTSC() - time1;
	}

	printf("\n\nStandart memcpy:"); // Print heading
	for (i = 0; i < NumberOfTests; i++)
	{
		printf("\n%2i %10I64i", i, timediff[i]);
	}
	printf("\n");

      return 0;
}

