#include<stdio.h>
#include<string.h>
int main(void)
{
    char mountpoint[250], device[]="/dev/sr0", devicefile[50];
    FILE *fptr=NULL;
    
    fptr=fopen("/etc/mtab", "r");
    
    if (fptr==NULL)
    {
	fprintf(stderr, "File could not be opened");
    }
    
    while (!feof(fptr))
    {
	fscanf(fptr, "%s", devicefile);
	if (!strcmp(device, devicefile))
	{
	    fscanf(fptr, "%s", mountpoint);
	    
	}
	else
	{
	    while (fgetc(fptr)!='\n' && !feof(fptr)) 
		;
	}
    }
    fclose(fptr);
    printf("The required mount point is: %s", mountpoint);
    return 0;
}
    