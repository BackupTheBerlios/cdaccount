#include<stdio.h>
#include<stdlib.h>
#include<sys/mount.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include "mount.h"
#include "permis.h"
#include "read_cd.h"

char *get_mount_point(char *device)
{
    static char mountpoint[300];
    char devicefile[50];
    FILE *fptr=NULL;
    int gotmountpoint=NO;
    
    mountpoint[0]='\0';
    
    fptr=fopen("/etc/mtab", "r");
    
    if (fptr==NULL)
    {
	fprintf(stderr, "File could not be opened");
	exit(1);
    }
    
    while (!feof(fptr))
    {
	fscanf(fptr, "%s", devicefile);
	if (!strcmp(device, devicefile))
	{
	    fscanf(fptr, "%s", mountpoint);
	    parsemount(mountpoint);
	    gotmountpoint=YES;
	    break;
	}
	else
	{
	    while (getc(fptr) != '\n' && !feof(fptr)) 
		;
	}
    }
    

    #ifdef Dgetmountpoint
    fprintf(stderr, "%s %s", __func__, mountpoint);
    #endif
    fclose(fptr);
    return (mountpoint);
}
    
char *create_mount_point(char *device)
{
    static char path[500], data[500];
    FILE *mtab=NULL;
    
    strcpy(path, "/media/");
    strcat(path, get_cd_label(device));
    
    get_root_power();

    rmdir (path);
    while (mkdir(path, 01777))
    {
	if (errno==EEXIST)
	{
	    strcat(path, "_");
	}

	else if (errno==EACCES)
	{
	    strcpy(path, "CD");
	}
	else
	{
	    fprintf(stderr, "Could not create mount point");
	    perror("mkdir");
	    exit(1);
	}
		
	
    }
    
    #ifdef Dcreate_mount_point
    fprintf(stderr, "Mount directory executed:");
    #endif
    
    if (mount(device,  path, "iso9660", MS_RDONLY, data))
    {
	fprintf(stderr, "Could not mount the optical device");
	perror("mount");
	exit(1);
    }
    
    fprintf(stderr, "Mount syscall executed:");
    /*add entry to mount point */
    mtab=fopen("/etc/mtab", "a");
    if (mtab==NULL)
    {
	fprintf(stderr, "Could not enter mount point entry");
	unmount_device(path);
	exit(1);
    }
    
    convert_path_for_mtab(path);
    #ifdef Dcreate_mount_point
    fprintf(stderr, "%s %s iso9660 ro %d %d\n", device, path, geteuid(), getegid());
    #endif
    
    fprintf(mtab, "%s %s iso9660 ro %d %d\n", device, path, getuid(), getgid());
    
    leave_root_power();
    
    return path;
}
    
void convert_path_for_mtab(char *path)
{
    int i=0, j=0;
    char result[500];
    
    for (i=0; path[i]!='\0'; i++)
    {
	if (path[i]==' ')
	{
	    result[j]='\\';
	    result[j+1]='0';
	    result[j+2]='4';
	    result[j+3]='0';
	    result[j+4]='3';
	    j=j+5;
	}
	else
	{
	    result[i]=path[i];
	    j++;
	}
    }
	    
}
	
	    

int unmount_device(char *mountpoint)
{
    int ret;
    char command[500];
    
    get_root_power();
    
    ret=umount(mountpoint);
    if (ret==-1)
    {
	perror("umount");
	return (errno);
    } 
    strcpy(command, "umount ");
    strcat(command, mountpoint);
    
    system(command);
    
    if (rmdir(mountpoint)==-1)
    {
	fprintf(stderr, "Could not remove directory");
    }
    
    leave_root_power();
    
    return 0;
}


char *get_cd_label(char *device)
{
  int ifd, ix, err, lastvalid;
  static char lbl[LABMAX] = "";

    if  ((ifd = open(device,O_RDONLY)) == -1){    /* open the device */
    fprintf(stderr,"device '%s' err:%s\n",device,strerror(errno));
    exit(1);;
    }

  err = lseek(ifd,0x8028,SEEK_SET) == -1;    /* seek to the label */
  if (!err) err = (read(ifd,lbl,LABMAX - 1) == -1); /*  and try to read
it */
  if (err){
    fprintf(stderr,"device '%s' err:%s\n",device,strerror(errno));
    exit(1);
    }

  for (ix=0; ix<LABMAX; ix++) //Get rid of spaces in the end; they make it difficult to evaluate filenames
  {
      if (!(lbl[ix]==' ' || lbl[ix]=='\0'))
      {
	  lastvalid=ix;
      }
      #ifdef Dget_cd_label
      printf("%d", lastvalid);
      #endif
  }
      
  lbl[lastvalid+1]='\0';
  
  if ((int) *lbl == '\0'){                /* no label? - bad! */
    fprintf(stderr,"device '%s' err: no label found!\n",device);
    exit(1);
    }
    
  return (lbl);

}

