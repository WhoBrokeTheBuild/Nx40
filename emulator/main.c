#include <stdio.h>
#include <string.h>
#include <stdint.h>

void usage() 
{
    printf(
        "usage: Nx40 FILENAME\n"
        "\n"
    );
}

typedef struct 
{
    char name[21];
} z64_header_t;

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    const char * filename = argv[1];
    FILE * file = fopen(filename, "rb");

    z64_header_t header;

    char buffer[4096];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);

    memcpy(header.name, buffer + 32, 20);
    header.name[20] = '\0';

    printf("Loading %s\n", header.name);

    fclose(file);

    return 0;
}