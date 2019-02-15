#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

void usage() 
{
    printf(
        "usage: romdump FILENAME\n"
        "\n"
    );
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return 0;
    }

    const char * filename = argv[1];
    FILE * file = fopen(filename, "rb");

    char buffer[4096];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    printf("Read %zu bytes\n", bytesRead);
    for (size_t i = 0; i < bytesRead; ++i) {
        if (i % 16 == 0) {
            printf("%08zX: ", i);
        }

        char c = (isalnum(buffer[i]) ? buffer[i] : ' ');
        printf("%02X %c ", (uint8_t)buffer[i], c);

        if (i % 16 == 15) {
            printf("\n");
        }
    }

    fclose(file);

    return 0;
}