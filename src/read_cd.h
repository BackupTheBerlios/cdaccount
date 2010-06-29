#define NO 0
#define YES 1
#define NAMELENGTH 255
#define PATHLENGTH 2550

typedef char string[NAMELENGTH];

typedef struct directory_entry
{
	char name[NAMELENGTH];
	char path[PATHLENGTH];
	int no_of_files;
	int no_of_dirs;
	string *file;
	struct directory_entry *dir;
	string *dir_name;
	struct directory_entry *par;
}dir_entry;

void directory_traverse (dir_entry * entry);
int is_directory(char *p, char *name);
void form_sub_dir_entry(struct directory_entry *entry, char *sub_dir_name);
void write_structure (char *path, char *name, char *filename);
void parsemount(char *mountpoint);
void see_tree(dir_entry entry);

