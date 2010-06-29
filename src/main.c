/* Limitations of this program:
3) Some issues with filenames having long names (greater than 50 characters)
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include"read_cd.h"
#include"file_handle.h"
#include"mount.h"
#include"permis.h"

#define LABMAX 33

void action_w(char *);
int getlastcharpos(char *mountpoint, char symbol);
void action_r(char *);

int main(int argc, char *argv[])
{	
	
	int choice;
		
	while ((choice = getopt(argc, argv, "w::s:r:")) != -1) {
		switch (choice) {
		case 'w':
	
			action_w(optarg);
			break;

		case 'r':
			action_r(optarg);
			break;
		case 's':
		    
			fprintf(stderr, "Output: %s", optarg);
			search_file(optarg);
			break;
		case '?':
			if (optopt == 'r') {
				printf
				    ("Argument r requires label of CD as parameter");
			} else {
				printf("\nUnknown character or option given");
			}
			break;
		default:
			printf("\nIncorrect Arguments given, Aborting");
			abort();
		}
	}

	return 0;
}

void action_w(char *optarg)
{		
	char device_name[50], *mountpoint, mountpointpath[50], mountpointname[200], *filename=NULL;
	int lastslashpos, toumount=NO;
	
	if (optarg==NULL)
	{
	    strcpy(device_name, "/dev/sr0");
	}
	else
	{
	    strcpy(device_name, optarg);
	}
	
	mountpoint=get_mount_point(device_name);
	
	fprintf(stderr, "%s", mountpoint);
	if (*mountpoint=='\0')
	{
	    mountpoint=create_mount_point(device_name);
	    toumount=YES;
	}
	
	filename = get_cd_label(device_name);
	
	create_file(filename);
	
	lastslashpos=getlastcharpos(mountpoint, '/');
	
	#ifdef Daction_w
	//fprintf(stderr, "%d", lastslashpos);
	#endif
	
	strncpy(mountpointpath, mountpoint, lastslashpos+1);
	strcpy(mountpointname, mountpoint+lastslashpos+1);
	#ifdef DEBUG
	fprintf(stderr, "\n%s %s %s\n", __func__, mountpointpath, mountpointname); 
	#endif
	write_structure(mountpointpath, mountpointname, filename);
	if (toumount==YES)
	{
	    unmount_device(mountpoint);
	}
	add_master_file(get_cd_label(device_name), filename);
}

int getlastcharpos(char *mountpoint, char symbol)
{
    int pos, i;
    
    for (i=0; *(mountpoint+i)!='\0'; i++)
    {
	if (*(mountpoint+i)=='/')
	{
	    pos=i;
	}
    }
    return pos;
}

void action_r(char *cdname)
{
    char *filename=NULL;
    
    fprintf(stderr, "%s", cdname);
    filename=check_entry(cdname);
    fprintf(stderr, "%s", filename);
    
    if (filename==NULL)
    {
	fprintf(stderr, "No such CD record present");
    }
    else
    {
	convert_filename_to_read(filename);
	read_from_file(filename);
    }
}