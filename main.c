#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"

int main(int argc, char *argv[])
{
    init_fs();

    const char *cmd = argv[1];
    const char *filename = argv[2];

    if ((strcmp(cmd, "help") == 0)) {
        printf("Usage: ./fs [create|delete|write|read] filename [data]\n       ./fs [list]\n");
    } else if (strcmp(cmd, "list") == 0)
    {
        list_files();
    }
    else if (strcmp(cmd, "create") == 0)
    {
        if (create_file(filename) == 0)
            printf("File created.\n");
        else
            printf("File creation failed.\n");
    }
    else if (strcmp(cmd, "read") == 0)
    {
        char buffer[MAX_FILE_SIZE + 1];
        int size = read_file(filename, buffer);
        if (size >= 0)
            printf("File contents: %s\n", buffer);
        else
            printf("Read failed.\n");
    }
    else if (strcmp(cmd, "write") == 0)
    {
        if (argc < 4)
        {
            printf("Missing data to write.\n");
            return 1;
        }
        if (write_file(filename, argv[3], strlen(argv[3])) == 0)
            printf("Write successful.\n");
        else
            printf("Write failed.\n");
    }
    else if (strcmp(cmd, "delete") == 0)
    {
        if (delete_file(filename) == 0)
            printf("File deleted.\n");
        else
            printf("File not found.\n");
    }
    else
    {
        printf("Unknown command.\n");
        return 1;
    }

    return 0;
}
