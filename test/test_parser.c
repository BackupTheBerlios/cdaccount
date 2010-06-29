#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void parsemount(char *mountpoint);

int main(void)
{
    char mount[300];
    printf("Enter");
    scanf("%s", mount);
    parsemount(mount);
    printf("%s", mount);
    return 0;
}


void parsemount(char *mountpoint)
{
    int i=0, j=0, stage=0;
    char tempmountpoint[300];
    
    while (*(mountpoint+i)!='\0')
    {
	
	if (*(mountpoint+i)=='\\' && *(mountpoint+i+1)=='0' && *(mountpoint+i+2)=='4' && *(mountpoint+i+3)=='0')
	{
	    tempmountpoint[j]=' ';
	    i=i+4;
	    j++;
	    continue;
	}
	else
	{
	    tempmountpoint[j]=*(mountpoint+i);
	    j++;
	    i++;
	}
    }
    tempmountpoint[j]='\0';
    strcpy(mountpoint, tempmountpoint); 
}