#define NO 0
#define YES 1
#define LABMAX 33

char *get_cd_label(char *argv);
char *get_mount_point(char *device);
char *create_mount_point(char *device);
int unmount_device(char *mountpoint);
void convert_path_for_mtab(char *path);
