#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/mman.h>
#include<dirent.h>
#include <sys/ioctl.h>

#include"read_cd.h"
#include"file_handle.h"

void create_file(char *label)
{

	int statret=0;
	char filename[100];
	struct stat buf;
	char kd; //choice to keep/delete file in case it is already present
	
	strcpy(filename, label);
	while (!statret)
	{
		errno=0;
		statret=stat(label, &buf);
		if (errno==ENOENT || errno==ENOTDIR)
		{
		    #ifdef Dcreate_file
		    printf("File %s can be created", label);
		    #endif
			return;
		}
		
		if (errno>0)
		{
		    fprintf(stderr, "\nError opening file %s\n", label);
		    perror("stat");
		    exit(1);
		}
		
		printf("\nThere is already a file stored for the given label. Do you want to delete(d)/keep(k) it?");
		scanf("%c", &kd);
		if (kd=='k')
		{
		    strcat(filename, "_");
		}
		else if (kd=='d')
		{
		    if(remove(filename))
		    {
			perror("remove");
			strcat(filename, "_");
		    }
		}
		
	}
	if (kd=='k')
	{
	    add_master_file(label, filename);
	}

}



void write_to_file(char *label, struct directory_entry *entry)
{
	FILE *sptr=NULL;
	dir_entry *trav=NULL;
	int i;
	
	trav=entry;
	sptr=fopen(label, "a");
	if (sptr==NULL)
	{
	    fprintf(stderr, "File could not be opened");
	    exit(1);
	}
	
	convert_filename_to_write(trav->name);
	convert_filename_to_write(trav->path);
	
	fprintf(sptr, "%s %s %d %d", trav->name, trav->path, trav->no_of_files, trav->no_of_dirs);
	
	for (i=0; i<=trav->no_of_files; i++)
	{
		convert_filename_to_write(trav->file[i]);
		//#ifdef Dwrite_to_file
		fprintf(sptr, " %s", trav->file[i]);
		//fprintf(stderr, "Record has been written %s\n", trav->file[i]);
		//#endif
	}

	for (i=0; i<=trav->no_of_dirs; i++)
	{
		convert_filename_to_write(trav->dir_name[i]);
		fprintf(sptr, " %s", trav->dir_name[i]);
	} 
	 
	fprintf(sptr, " e*nd\n");
	fclose(sptr);


	for (i=0; i<=trav->no_of_dirs; i++)
	{
	    //fprintf(stderr, "Going to directory %s no of dirs %d", trav->dir_name[i], trav->no_of_dirs);
		write_to_file (label, &trav->dir[i]);

	}
}



void convert_filename_to_write(char *filename)
{
    int i=0;
    while (filename[i]!='\0')
    {
	if (filename[i]==' ')
	{
	    filename[i]='*';
	}
	i++;
    }
}




void read_from_file(char *label)
{
	FILE *ptr=NULL;
	struct dir_entry_read def;
	int i=0, j=0, no_col, cur_pos=0;
	char next[50];
	
	string dir[10000], file[10000];
	ptr=fopen(label, "r");
		
	while (!feof(ptr))
	{
	    if (j==2)
	    {
		printf("Check out now");
	    }
	    j++;
	    
	        printf("\n");
		fscanf(ptr, "%s %s", def.name, def.path);
		
		convert_filename_to_read(def.name);
		convert_filename_to_read(def.path);
		
		fprintf(stderr, "%s", def.path);
		fprintf(stderr, "%s", def.name);
		
		fscanf(ptr, "%d %d", &def.no_of_files, &def.no_of_dir);
		#ifdef Dread_from_file
		fprintf(stderr, "%d", def.no_of_dir);
		#endif
		printf("\n");
		no_col=get_no_col();
		cur_pos=0;
		for (i=0; i<=def.no_of_files; i++)
		{
		    fscanf(ptr, "%s ", file[i]);
		    
		    convert_filename_to_read(file[i]);
		    
		    fprintf(stderr, "%55s        ", file[i]);
		    cur_pos+=63;
		    if (get_no_col()-cur_pos<63)
		    {
			cur_pos=0;
			fprintf(stderr, "\n");
		    }
		}
		printf("\n\n");
		for (i=0; i<=def.no_of_dir; i++)
		{
		    fscanf(ptr, "%s", dir[i]);
		    convert_filename_to_read(dir[i]);
		    fprintf(stderr, "%55s       ", dir[i]);
		    
		    cur_pos+=63;
		    if (get_no_col()-cur_pos<63)
		    {
			cur_pos=0;
			fprintf(stderr, "\n");
		    }
		}
		
		fscanf(ptr, "%s", next);
		
                while (strcmp(next, "e*nd") && !feof(ptr)) 
		{
		    
		    /*if (j==2)
		    fprintf(stderr, "\n%d %c\n", ch, ch); */
		}
		 
		
		printf("\n\n");
	}
	
}

int get_no_col()
{

    struct winsize ws;

    if (ioctl(0,TIOCGWINSZ,&ws)!=0)
    {
	fprintf(stderr,"TIOCGWINSZ:%s\n",strerror(errno));
	exit(1);
    }
    return ws.ws_col;
} 

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

void add_master_file(char *label, char *filename)
{
    
    fprintf(stderr, "%s %s", label, filename);
    if (check_entry(label)==NULL)
    {
	new_entry_add(label, filename);
    }
    else
    {
	existing_entry_add(label, filename);
    }
    
}
	
	
void new_entry_add(char *label, char *filename)
{
    FILE *mptr=NULL;
    
    mptr=fopen("master", "a");
    if (mptr==NULL)
    {
	printf("Entry could not be added");
	exit(1);
    }
        fprintf(stderr, "%s 1 %s\n", label, filename);

    convert_filename_to_write(label);
    convert_filename_to_write(filename);
    
    #ifdef Dnew_entry_add
    fprintf(stderr, "%s 1 %s\n", label, filename);
    #endif
    fprintf(mptr, "%s 1 %s\n", label, filename);
}

void existing_entry_add(char *label, char *filename)
{
    FILE *ptr=NULL;
    char templabel[100];
    
    ptr=fopen("master", "r+");
    if (ptr==NULL)
    {
	printf("Entry could not be added");
	exit(1);
    }
    
    convert_filename_to_write(label);
    
    while (!feof(ptr))
    {
	fscanf(ptr, "%s", templabel);

	if (strcmp(label, templabel))
	{
	    fprintf(stderr, "CD with same label exists. Cannot insert entry");
	}
	
	while (!feof(ptr) && getc(ptr)!='\n')
	    ;
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
    
    convert_filename_to_write(label);
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

void search_file(char *text)
{
  DIR *d1 = NULL;
  struct dirent *opstream = NULL;
  FILE *fp=NULL;
  char *fileaddr=NULL, *loc=NULL, cdname[500];
  int retstat, textfound=NO, fd;
  struct stat buf;

  d1 = opendir (".");
  if (errno != 0)
  {
    
    perror ("opendir");
    exit (1);
  }
  
  
  while ((opstream = readdir (d1)) != NULL)
  {
      if (!strcmp(opstream->d_name, "."))
	  ;
      else if (!strcmp(opstream->d_name, ".."))
	  ;
      else if (is_directory(".", opstream->d_name))
	  ;
      else
      {
	  fp=fopen(opstream->d_name, "r");
	  if (fp==NULL)
	  {
	      fprintf(stderr, "Could not open file for searching");
	      return;
	  }
	  
	  fscanf(fp, "%s", cdname);
	  convert_filename_to_read(cdname);
	  
	  retstat=stat(opstream->d_name, &buf);
	  if (retstat!=0)
	  {
	      perror("retstat");
	      printf("File size could not be obtained");
	      return;
	  }
	  

	  fd=fileno(fp);
	  if (fd==-1)
	  {
	      perror("fileno");
	      return;
	  }
	  
  
	  fileaddr=mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	  if (fileaddr==MAP_FAILED)
	  {
	      perror("mmap");
	      
	      printf("Could not search file %s", opstream->d_name);
	      return;
	  }
	  #ifdef Dsearch_file
	  fprintf(stderr, "\nMapping %s", opstream->d_name);
	  #endif
	  loc=strstr(fileaddr, text);
	  
	  if (loc!=NULL)
	  {
	      textfound=YES;
	      printf("Found in CD %s", cdname);
	  }
	  if (munmap(fileaddr, buf.st_size)==-1)
	  {
	      perror("munmap");
	  }
	  fileaddr=NULL;
	  fclose(fp);
	  fp=NULL;
      }
  }
  if (textfound==NO)
  {
      printf("Could not find the given text");
  }
}
	  
	  





