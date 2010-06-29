#include<stdio.h>

int main(void)
{
    int no;
    FILE *ptr=NULL;
    char entry1[40], entry2[40];
    ptr=fopen("temp.txt", "r+");
    
    while (!feof(ptr))
    {
	fscanf(ptr, "%s %d", entry1, &no);
	no++;
	
	fseek(ptr, -1, SEEK_CUR);
	fprintf(ptr, "%d", no);
	
	return 0;
    }
	
}