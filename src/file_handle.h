#define PATHLENGTH 2550
#define NAMELENGTH 255
#define YES 1
#define NO 0
struct dir_entry_read
{
    char name[NAMELENGTH];
    char path[PATHLENGTH];
    int no_of_dir;
    int no_of_files;
};
void create_file(char *label);
void write_to_file(char *label, struct directory_entry *entry);
void read_from_file(char *label);
void convert_filename_to_write(char *filename);
void convert_filename_to_read(char *filename);
void add_master_file(char *cd_label, char *filename);
void new_entry_add(char *label, char *filename);
void existing_entry_add(char *label, char *filename);
char *check_entry(char *label);
void search_file(char *text);
int get_no_col(void);