#include<stdio.h>
#define NDEBUG

#include<assert.h>
#define NDEBUG
int main (void)
{
    int n;
    printf("Enter n");
    scanf("%d", &n);
    
    if (n==0)
    {
	assert(n=0);
    }
    return 0;
}