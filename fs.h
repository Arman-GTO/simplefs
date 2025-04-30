#ifndef FS_H
#define FS_H

#define DISK_FILE "disk.img"
#define DISK_SIZE 1024 * 1024 // 1MB
#define BLOCK_SIZE 512
#define MAX_FILES 128
#define MAX_FILENAME 20
#define MAX_FILE_SIZE 4096
#define NUM_BLOCKS (DISK_SIZE / BLOCK_SIZE)
#define METADATA_BLOCKS 9  // Reserve first block for metadata
#define FIRST_DATA_BLOCK METADATA_BLOCKS

typedef struct {
    char name[MAX_FILENAME];
    int size;
    int start_block;
    int used;
} FileEntry;

typedef struct {
    FileEntry entries[MAX_FILES];
    char block_bitmap[NUM_BLOCKS / 8]; // 0 = free, 1 = used (2048 bits = 256 bytes)
} FileSystem;

void init_fs();
void load_fs();
void save_fs();

void list_files();

int create_file(const char *name);
int read_file(const char *name, char *buffer);
int write_file(const char *name, const char *data, int size);
int delete_file(const char *name);

#endif
