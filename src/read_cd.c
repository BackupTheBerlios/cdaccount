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

#include"read_cd.h"
#include"file_handle.h"
#include"mount.h"


void see_tree(dir_entry entry)
{
    int i=0;
    for (i=0; i<=entry.no_of_dirs; i++)
    {
	printf("%s  ", entry.dir_name[i]);
    }
    for (i=0; i<=entry.no_of_files; i++)
    {
	printf("%s  ", entry.file[i]);
    }
    
    for (i=0; i<=entry.no_of_dirs; i++)
    {
	see_tree(entry.dir[i]);
    }
}


void
write_structure (char *path, char *name, char *file_name)
{
  dir_entry entry;
  DIR *d=NULL;
  struct dirent *dirstream=NULL;
  
  entry.no_of_files = -1;
  entry.no_of_dirs = -1;
  
  strcpy (entry.name, name);
  strcpy(entry.path, path);
  
  d=opendir(path);
  while ((dirstream=readdir(d))!=NULL)
  {
      if (!strncmp(name, dirstream->d_name, strlen(name)))
      {
	  printf("actual name: %s", dirstream->d_name);
	  break;
      }
  }
  strcpy(entry.name, dirstream->d_name);
#ifdef DEBUG 
  fprintf(stderr, "%s%s", entry.path, entry.name);
#endif
  directory_traverse (&entry);
  
  //see_tree(entry);
  write_to_file (file_name, &entry);
  
}

    
void
directory_traverse (struct directory_entry *entry)
{
  DIR *d1 = NULL;
  struct dirent *opstream = NULL;
  char entry_fullpath[1000];
  int i=0;

  
  strcpy(entry_fullpath, entry->path);
  strcat(entry_fullpath, entry->name);
  
  errno = 0;
    
  d1 = opendir (entry_fullpath);
  if (errno != 0)
  {
    
    fprintf(stderr, "%s\n%s\n", __func__, entry_fullpath);
    perror ("opendir");
    exit (1);
  }
  
  while ((opstream = readdir (d1)) != NULL)
  {
      
#ifdef Ddirectory_traverse
    fprintf(stderr, "\nfunction %s, path:%s, filename:%s\n", __func__, entry->name, opstream->d_name);
#endif
    if (!strcmp (opstream->d_name, "."))
    {
    }
    else if (!strcmp (opstream->d_name, ".."))
    {
    }
       
    else if (is_directory (entry_fullpath, opstream->d_name))
    {
	/*Add Directory entry to the data structure entry, and do the same recursively */    
      entry->no_of_dirs += 1;
      if (entry->no_of_dirs == 0)
      {
	entry->dir = (dir_entry *) malloc (10000 * sizeof (dir_entry));
	entry->dir_name=(string *) malloc (10000 * sizeof (string));
	for (i=0; i<10000; i++)
	{
		entry->dir->dir=NULL;
		entry->dir->file=NULL;
	  entry->dir[i].no_of_files=-1;
	  entry->dir[i].no_of_dirs=-1;
	}
     }
     
     form_sub_dir_entry(entry, opstream->d_name);
           
    }
        
    else
    {
	    /*Add file entry to the data structure */
      entry->no_of_files += 1;
#ifdef Ddirectory_traverse
      fprintf(stderr, "%d\n", entry->no_of_files);
#endif
      if (entry->no_of_files == 0)
      {
	      entry->file=(string *) malloc ( 10000 *sizeof(string));
	      if (entry->file==NULL)
	      {
		      fprintf(stderr, "Could not allocate enough memory for files");
	      }	      
      }
      
      strcpy(entry->file[entry->no_of_files], opstream->d_name);
    }
  }
  
  
  if (errno && !opstream)
  {
    printf("%s", __func__);
    perror ("readdir");
  }
  
  /*free up the excess memory allocated */
  for (i=0; i<=entry->no_of_dirs; i++)
  {
      entry->dir=(dir_entry*)realloc(entry->dir, (entry->no_of_dirs +1)* sizeof(dir_entry));
      entry->dir_name=(string *)realloc(entry->dir_name, (entry->no_of_dirs +1)* sizeof(string));
  }
  
  for (i=0; i<=entry->no_of_files; i++)
  {
      entry->file=(string *)realloc(entry->file, (entry->no_of_files+1)*sizeof(string));
  }
  
  for (i=0; i<=entry->no_of_dirs; i++)
  {
      //printf("%s%s", entry->dir[i].path, entry->dir[i].name);
      directory_traverse(&entry->dir[i]);
  }
  closedir(d1);
}



void form_sub_dir_entry(struct directory_entry *entry, char *sub_dir_name)
{
    
      strcpy(entry->dir[entry->no_of_dirs].name, sub_dir_name);
      strcpy(entry->dir[entry->no_of_dirs].path, entry->path);
      strcat(entry->dir[entry->no_of_dirs].path, entry->name);
      strcat(entry->dir[entry->no_of_dirs].path, "/");

      strcpy(entry->dir_name[entry->no_of_dirs], sub_dir_name);
#ifdef DEBUG  
      show_entry(entry->dir[entry->no_of_dirs]);
#endif
}

void show_entry(struct directory_entry entry)
{
    int i=0;
    
    fprintf(stderr, "\n%s\n%s", __func__, entry.name);
    fprintf(stderr, "\n%s", entry.path);
    for (i=0; i<entry.no_of_files; i++)
    {
	fprintf(stderr, "\n%s", entry.file[i]);
    }
    for (i=0; i<entry.no_of_dirs; i++)
    {
	fprintf(stderr, "\n%s", entry.dir_name[i]);
    }
}
    

int
is_directory (char *p, char *name)
{
  char full_path[1000], slash[] = "/";
  DIR *d = NULL;

  strcpy (full_path, p);
  strcat (full_path, slash);
  strcat (full_path, name);

  errno = 0;
  d = opendir (full_path);
  if (errno == ENOTDIR)
  {
    errno = 0;
    return 0;
  }
  else if (errno > 0)
  {
    puts(__func__);
    fprintf (stderr, "\n%s\n", full_path);
    perror ("opendir");
    exit (1);
  }
  else
  {
    return 1;
  }
}



void parsemount(char *mountpoint)
{
    int i=0, j=0;
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
	       