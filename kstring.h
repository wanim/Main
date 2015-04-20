/**
*   @file   Kstring.h
*   @brief  functions to work with memory and c-strings
*   @remark No 0-checks for all mem-function
*   @remark 0-checks for all str-functions

*   Created 2015-04-14 by Kyrylo Nykytuk aka wanim
*/
#ifndef KSTRING_H_INCLUDED
#define KSTRING_H_INCLUDED

#include <stddef.h>
#define NULL ((void *)0)
#define END_OF_STRING '\254'

/** Define one of this only if compiler is not smart enough to use vector registers */
// #define USE_64_BIT_REG
// #define USE_32_BIT_REG
/** */

#ifdef __cplusplus
extern "C" {
#endif
/*Searches for the first occurrence of the character c (an unsigned char) in the first n bytes of the string pointed to by the argument str. */
void *memchr_k(const void *str, int c, size_t n);
/*Compares the first n bytes of str1 and str2.    */
int memcmp_k(const void *str1, const void *str2, size_t n);
/*Copies n characters from src to dest. Locations should not overlap*/
void *memcpy_k(void *dest, const void *src, size_t n);
/*Copies n characters from src to dest. Locations could overlap*/
void *memmove_k(void *dest, const void *src, size_t n);
/*Copies the character c (an unsigned char) to the first n bytes of the memory pointed to by the argument str.*/
void *memset_k(void *str, int c, size_t n);
/*Appends the string pointed to by src to the end of the string pointed to by dest.*/
char *strcat_k(char *dest, const char *src);
/*Appends the string pointed to by src to the end of the string pointed to by dest up to n characters long.*/
char *strncat_k(char *dest, const char *src, size_t n);
/*Searches for the first occurrence of the character c (an unsigned char) in the string pointed to by the argument str.*/
char *strchr_k(const char *str, int c);
/*Compares the string pointed to by str1 to the string pointed to by str2.*/
int strcmp_k(const char *str1, const char *str2);
/*Compares at most the first n bytes of str1 and str2.*/
int strncmp_k(const char *str1, const char *str2, size_t n);
/*Copies the string pointed to by src to dest.*/
char *strcpy_k(char *dest, const char *src);
/*Copies up to n characters from the string pointed to by src to dest.*/
char *strncpy_k(char *dest, const char *src, size_t n);
/*Calculates the length of the initial segment of str1 which consists entirely of characters not in str2.*/
size_t strcspn_k(const char *str1, const char *str2);
/*Computes the length of the string str up to but not including the terminating character.*/
size_t strlen_k(const char *str);
/*Finds the first character in the string str1 that matches any character specified in str2.*/
char *strpbrk_k(const char *str1, const char *str2);
/*Searches for the last occurrence of the character c (an unsigned char) in the string pointed to by the argument str.*/
char *strrchr_k(const char *str, int c);
/*Calculates the length of the initial segment of str1 which consists entirely of characters in str2.*/
size_t strspn_k(const char *str1, const char *str2);
/*Finds the first occurrence of the entire string needle (not including the terminating character) which appears in the string haystack.*/
char *strstr_k(const char *haystack, const char *needle);

#ifdef __cplusplus
}
#endif
#endif /* KSTRING_H_INCLUDED */
