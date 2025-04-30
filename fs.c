#include <stdio.h>
#include <string.h>
#include "fs.h"

FileSystem fs;

// Helper function to set a bit in the bitmap
static void set_bit(int block) {
    int byte_idx = block / 8;
    int bit_idx = block % 8;
    fs.block_bitmap[byte_idx] |= (1 << bit_idx);
}

// Helper function to clear a bit in the bitmap
static void clear_bit(int block) {
    int byte_idx = block / 8;
    int bit_idx = block % 8;
    fs.block_bitmap[byte_idx] &= ~(1 << bit_idx);
}

// Helper function to check if a bit is set
static int is_bit_set(int block) {
    int byte_idx = block / 8;
    int bit_idx = block % 8;
    return (fs.block_bitmap[byte_idx] & (1 << bit_idx)) != 0;
}

void init_fs() {
    FILE *disk = fopen(DISK_FILE, "rb");
    if (!disk) {
        // No disk, create one
        disk = fopen(DISK_FILE, "wb");
        fseek(disk, DISK_SIZE - 1, SEEK_SET);
        fputc('\0', disk);
        fclose(disk);

        // Initialize filesystem structure
        memset(&fs, 0, sizeof(FileSystem));
        
        // Initialize block bitmap (reserve first block for metadata)
        set_bit(0);  // Mark metadata block as used
        
        save_fs();
    } else {
        // Load existing filesystem
        fclose(disk);
        load_fs();
    }
}

void save_fs() {
    FILE *disk = fopen(DISK_FILE, "rb+");
    fseek(disk, 0, SEEK_SET);  // Write metadata at start of file
    fwrite(&fs, sizeof(FileSystem), 1, disk);
    fclose(disk);
}

void load_fs() {
    FILE *disk = fopen(DISK_FILE, "rb");
    fseek(disk, 0, SEEK_SET);  // Read metadata from start of file
    fread(&fs, sizeof(FileSystem), 1, disk);
    fclose(disk);
}

int find_free_block(int num_blocks) {
    // Start from FIRST_DATA_BLOCK to skip metadata block
    for (int i = FIRST_DATA_BLOCK; i < NUM_BLOCKS - num_blocks; ++i) {
        int free = 1;
        for (int j = 0; j < num_blocks; ++j) {
            if (is_bit_set(i + j)) {
                free = 0;
                break;
            }
        }
        if (free) return i;
    }
    return -1;
}

int find_file(const char *name) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (fs.entries[i].used && strcmp(fs.entries[i].name, name) == 0)
            return i;
    }
    return -1;
}

void list_files() {
    printf("List of files on disk:\n");
    int found = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.entries[i].used) {
            printf("- %s (Size: %d bytes, Start Block: %d)\n",
                   fs.entries[i].name, fs.entries[i].size, fs.entries[i].start_block);
            found++;
        }
    }

    if (found == 0) {
        printf("(No files found)\n");
    }
}

int create_file(const char *name) {
    if (find_file(name) != -1) return -1; // Already exists
    
    for (int i = 0; i < MAX_FILES; ++i) {
        if (!fs.entries[i].used) {
            strncpy(fs.entries[i].name, name, MAX_FILENAME);
            fs.entries[i].size = 0;
            fs.entries[i].start_block = -1;
            fs.entries[i].used = 1;
            save_fs();
            return 0;
        }
    }
    return -1; // No space
}

int read_file(const char *name, char *buffer) {
    int index = find_file(name);
    if (index == -1) return -1;

    int block = fs.entries[index].start_block;
    int size = fs.entries[index].size;

    FILE *disk = fopen(DISK_FILE, "rb");
    fseek(disk, block * BLOCK_SIZE, SEEK_SET);
    fread(buffer, 1, size, disk);
    fclose(disk);

    buffer[size] = '\0'; // Null terminate
    return size;
}

int write_file(const char *name, const char *data, int size) {
    int index = find_file(name);
    if (index == -1) {
        printf("File not found\n");
        return -1;
    }
    if (size > MAX_FILE_SIZE) {
        printf("File too large\n");
        return -2;
    }

    // Free any previously allocated blocks
    if (fs.entries[index].start_block != -1) {
        int old_blocks = (fs.entries[index].size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        for (int i = 0; i < old_blocks; ++i) {
            clear_bit(fs.entries[index].start_block + i);
        }
    }

    int needed_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int block = find_free_block(needed_blocks);
    if (block == -1) {
        printf("Not enough space\n");
        return -3;
    }

    // Mark blocks used
    for (int i = 0; i < needed_blocks; ++i)
        set_bit(block + i);

    fs.entries[index].start_block = block;
    fs.entries[index].size = size;

    FILE *disk = fopen(DISK_FILE, "rb+");
    fseek(disk, block * BLOCK_SIZE, SEEK_SET);
    fwrite(data, 1, size, disk);
    fclose(disk);

    save_fs();
    return 0;
}

int delete_file(const char *name) {
    int index = find_file(name);
    if (index == -1) return -1;

    int start = fs.entries[index].start_block;
    int blocks = (fs.entries[index].size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (int i = 0; i < blocks; ++i) {
        clear_bit(start + i);
    }

    fs.entries[index].used = 0;
    save_fs();
    return 0;
}