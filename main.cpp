void *memcpy(void *dest, const void *src, size_t n)
{
    register char* d1 = (char*)dest;
    register char* s1 = (char*)src;
    register size_t len = n;

    while(len--)
    {
        *d1++ = *s1++;
    }
    return dest;
}
