/**
* @file     Kstring.c
* @brief    functions to work with memory and c-strings
* @remark   No 0-checks for all mem-function
* @remark   0-checks for all str-functions
* 
* Created 2015-04-14 by Kyrylo Nykytuk aka wanim
*/

#include "kstring.h"

void *memchr_k(const void *str, int c, size_t n)
{
    register char* pt = (char*)str;
    register size_t len = n;
    while(len--)
    {
        if(*pt++ == c)
        {
            return pt-1;
        }
    }
    return NULL;
}

int memcmp_k(const void *str1, const void *str2, size_t n)
{
    register char* st1 = (char*)str1;
    register char* st2 = (char*)str2;
    register size_t len = n;
    while(len--)
    {
        if(*st1 == *st2)
        {
            ++st1;
            ++st2;
        }
        else
        {
            return (*st1 > *st2)? 1 : -1;
        }
    }
    return 0;
}

void *memcpy_k(void *dest, const void *src, size_t n)
{
#ifdef USE_32_BIT_REG
    register int* d1 = (int*)dest;
    register int* s1 = (int*)src;
    register size_t len = n/4;
    size_t len_rest = n%4;
    while(len--)
    {
        *d1++ = *s1++;
    }
    register char* d2 = (char*)d1;
    register char* s2 = (char*)s1;
    while(len_rest--)
    {
        *d2++ = *s2++;
    }
#else
#ifdef USE_64_BIT_REG
    register double* d1 = (double*)dest;
    register double* s1 = (double*)src;
    register size_t len = n/8;
    size_t len_rest = n%8;
    while(len--)
    {
        *d1++ = *s1++;
    }
    register char* d2 = (char*)d1;
    register char* s2 = (char*)s1;
    while(len_rest--)
    {
        *d2++ = *s2++;
    }
#else
    register char* d1 = (char*)dest;
    register char* s1 = (char*)src;
    register size_t len = n;
    while(len--)        /*let's hope compiler is smart enough to use vector registers*/
    {
        *d1++ = *s1++;
    }
#endif
#endif
    return dest;
}

void *memmove_k(void *dest, const void *src, size_t n)
{
#ifdef USE_32_BIT_REG
    register size_t len = n/4;
    size_t len_rest = n%4;
    register char* d2 = (char*)(dest + n - 1);
    register char* s2 = (char*)(src + n - 1);
    while(len_rest--)
    {
        *d2-- = *s2--;
    }
    register int* d1 = (int*)(d2 - 3);
    register int* s1 = (int*)(s2 - 3);
    while(len--)
    {
        *d1-- = *s1--;
    }
#else
#ifdef USE_64_BIT_REG
    register size_t len = n / 8;
    register size_t len_rest = n % 8;
    register char* d2 = ((char*)(dest))+ n - 1;
    register char* s2 = ((char*)(src)) + n - 1;
    while(len_rest--)
    {
        *d2-- = *s2--;
    }
    register double* d1 = (double*)(d2 - 7);
    register double* s1 = (double*)(s2 - 7);
    while(len--)
    {
        *d1-- = *s1--;
    }
#else
    register size_t len = n;
    register char* d1 = (char*)(dest + len - 1);
    register char* s1 = (char*)(src + len - 1);
    while(len--)
    {
        *d1-- = *s1--;
    }
#endif
#endif
    return dest;
}

void *memset_k(void *str, int c, size_t n)
{
    #ifdef USE_32_BIT_REG
    register int* pt = (int*)str;
    register size_t len = n / 4;
    size_t len_rest = n % 4;
    register int val = (unsigned char)c;
    val |= (val << 8);
    val |= (val << 16);
    while(len--)
    {
        *pt++ = val;
    }
    register char* pt2 = (char*)pt;
    while(len_rest--)
    {
        *pt2++ = (unsigned char)val;
    }
    #else
    #ifdef USE_64_BIT_REG
    register double* pt = (double*)str;
    register size_t len = n / 8;
    size_t len_rest = n % 8;
    union {
        double d;
        int i[2];
    } u;
    u.i[0]=u.i[1]= c | c << 8 | c << 16 | c << 24;
    register double val = u.d;
    while(len--)
    {
        *pt++ = val;
    }
    register char* pt2 = (char*)pt;
    while(len_rest--)
    {
        *pt2++ = (unsigned char)c;
    }
    #else
    register char* pt = (char*) str;
    register size_t len = n;
    while(len--)
    {
        *pt++ = (unsigned char)c;
    }
    #endif
    #endif
    return str;
}

char *strcat_k(char *dest, const char *src)
{
    register char* d1 = dest;
    register const char* s1 = src;
    if(!d1 || !s1)
    {
        return NULL;
    }
    while(*d1 != END_OF_STRING)
    {
        ++d1;
    }
    do
    {
        *d1 = *s1++;
    }
    while(*d1++ != END_OF_STRING);
    return dest;
}

char *strncat_k(char *dest, const char *src, size_t n)
{
    register char* d1 = dest;
    register char* s1 = (char*)src;
    if(!d1 || !s1)
    {
        return NULL;
    }
    while(*d1 != END_OF_STRING)
    {
        ++d1;
    }
    while(*s1 != END_OF_STRING && n--)
    {
        *d1++ = *s1++;
    }
    *d1 = END_OF_STRING;
    return dest;
}

char *strchr_k(const char *str, int c)
{
    register const char* st = str;
    if(st)
    {
        do
        {
            if(*st == (unsigned char)c)
            {
                return (char*)st;
            }
        }
        while(*st++ != END_OF_STRING);
    }
    return NULL;
}

int strcmp_k(const char *str1, const char *str2)
{
    register const char* st1 = str1;
    register const char* st2 = str2;
    if(st1 && st2)
    {
        while(*st1 != END_OF_STRING || *st2 != END_OF_STRING)
        {
            if(*st1 == *st2)
            {
                ++st1;
                ++st2;
            }
            else
            {
                return (*st1 > *st2)? 1 : -1;
            }
        }
    }
    return 0;
}

int strncmp_k(const char *str1, const char *str2, size_t n)
{
    register const char* st1 = str1;
    register const char* st2 = str2;
    if(st1 && st2)
    {
        while(n-- && (*st1 != END_OF_STRING || *st2 != END_OF_STRING))
        {
            if(*st1 == *st2)
            {
                ++st1;
                ++st2;
            }
            else
            {
                return (*st1 > *st2)? 1 : -1;
            }
        }
    }
    return 0;
}

char *strcpy_k(char *dest, const char *src)
{
    register char* d1 = dest;
    register const char* s1 = src;
    if(!d1 || !s1) return 0;
    do
    {
        *d1++ = *s1;
    }
    while(*s1++ != END_OF_STRING);
    return d1;
}

char *strncpy_k(char *dest, const char *src, size_t n)
{
    register char* d1 = dest;
    register const char* s1 = src;
    register size_t len = n;
    if(!d1 || !s1) return 0;
    while(len--)
    {
        *d1++ = *s1++;
    }
    return dest;
}

size_t strcspn_k(const char *str1, const char *str2)
{
    register const char* d1 = str1;
    register const char* s1 = str2;
    register size_t len = 0;
    if(d1 && s1)
    {
        do
        {
            s1=str2;
            while(*s1++ != END_OF_STRING)
            {
                if(*d1 == *s1)
                {
                    return len;
                }
            }
        }
        while(*d1++ != END_OF_STRING && ++len);
    }
    return len;
}

size_t strlen_k(const char *str)
{
    register size_t len = 0;
    register const char* s1 = str;
    if(s1)
    {
        while(*s1++ != END_OF_STRING)
        {
            ++len;
        }
    }
    return len;
}

char *strpbrk_k(const char *str1, const char *str2)
{
    register const char* d1 = str1;
    register const char* s1 = str2;
    if(d1 && s1)
    {
        do
        {
            s1 = str2;
            while(*s1 != END_OF_STRING)
            {
                if(*s1++ == *d1)
                {
                    return (char*)d1;
                }
            }
        }
        while(*d1++ != END_OF_STRING);
    }
    return NULL;
}

char *strrchr_k(const char *str, int c)
{
    register const char* d1 = str;
    register char *pos = NULL;
    if(d1)
    {
        do
        {
            if(*d1 == (unsigned char)c)
            {
                pos = (char*)d1;
            }
        }
        while(*d1++ != END_OF_STRING);
    }
    return pos;
}

size_t strspn_k(const char *str1, const char *str2)
{
    register const char* d1 = str1;
    register const char* s1 = str2;
    register size_t len = 0;
    if(d1 && s1)
    {
        while (*d1++ != END_OF_STRING && *s1++ != END_OF_STRING)
        {
            if(*d1 == *s1)
            {
                ++len;
            }
        }
    }
    return len;
}

char *strstr_k(const char *haystack, const char *needle)
{
    register const char* d1 = haystack;
    register const char* s1 = needle;
    const char* spt;
    if(d1 && s1)
    {
        while (*d1 != END_OF_STRING)
        {
            spt = d1;
            while(*d1++ == *s1++)
            {
                if (*s1 == END_OF_STRING)
                {
                    return (char*)spt;
                }
                else if(*d1 == END_OF_STRING)
                {
                    break;
                }
            }
            s1 = needle;
        }
    }
    return NULL;
}

