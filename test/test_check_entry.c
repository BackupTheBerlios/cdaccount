#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mount.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>

void convert_filename_to_read(char *filename)
{
    int i=0;
    
    while (filename[i]!='\0')
    {
	if (filename[i]=='*')
	{
	    filename[i]=' ';
	}
	i++;
    }
}

char* check_entry(char *label)
{
    char templabel[100];
    static char tempfilename[100];
    FILE *mptr=NULL;
    int tempno;
    
    mptr=fopen("master", "r");
    if (mptr==NULL)
    {
	return (NULL);
    }
    
    convert_filename_to_(label);
    fprintf(stderr, "%s", label);
    while (!feof(mptr))
    {
	fscanf(mptr, "%s %d %s", templabel, &tempno, tempfilename);
	if (!strcmp(templabel, label))
	{	    
	    return (tempfilename);
	}
	while (fgetc(mptr)!='\n' && !feof(mptr))
	    ;
    }
    return (NULL);
}

int main(void)
{
    printf("%s", check_entry("Backup 3b"));
    return 0;
}