/*#include <stdio.h>

 int  i __attribute__((aligned(8)));
 short c __attribute__((aligned(8))) = 7;
 int j __attribute__((aligned(8))) = 9;
 int k __attribute__((aligned(8))) = 0;



int main()
{
 *(((int*)&j) - 1) = 7;
 printf("%d,%d,%d,%d\n", i, c, j, k);
 printf("%p,%p,%p, %p", &i, &c, &j, &k);
 return 0;
} */



#include <stdio.h>
int z;
static int m = -1;

int main()
{
    printf("%p, %p", &m, &z);
    while(1){}
}
