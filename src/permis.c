#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

#include "permis.h"

uid_t euid, ruid;

void get_root_power(void)
{
    int ret;
    
    ruid=getuid();
    euid=geteuid();
    
    #ifdef _POSIX_SAVED_IDS
    ret = seteuid (euid);
    #else
    ret = setreuid (ruid, euid);
    #endif
    if (ret < 0)
    {
         fprintf (stderr, "Couldn't set uid.\n");
         exit (ret);
    }
    
}

void leave_root_power(void)
{
    int ret;
       
    #ifdef _POSIX_SAVED_IDS
    ret = seteuid (ruid);
    #else
    ret = setreuid (euid, ruid);
    #endif
    if (ret < 0)
    {
         fprintf (stderr, "Couldn't set uid.\n");
         exit (ret);
    }
}