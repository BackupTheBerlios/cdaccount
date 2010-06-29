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
#define LABMAX 33

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

  for (ix=0; ix<LABMAX; ix++)
  {
      if (!(lbl[ix]==' ' || lbl[ix]=='\0'))
      {
	  lastvalid=ix;
      }
      printf("%d", lastvalid);
  }
      
  lbl[lastvalid+1]='\0';
  
  if ((int) *lbl == '\0'){                /* no label? - bad! */
    fprintf(stderr,"device '%s' err: no label found!\n",device);
    exit(1);
    }
printf("%s", lbl);
  return (lbl);

}

int main(void)
{
    printf("%s", get_cd_label("/dev/sr0"));
    return 0;
}