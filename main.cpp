#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect input");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file != NULL)
    {
        int counter = 0;
        uint8_t buffer[512];
        FILE *outptr = NULL;
        bool jpeg = false;
        char name[8];

        while (fread(&buffer, 512, 1, file) == 1)
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                if (jpeg == true)
                {
                    fclose(outptr);
                }
                else
                {
                    jpeg = true;
                }
                sprintf(name, "%03d.jpg", counter);
                outptr = fopen(name, "w");
                if (outptr == NULL)
                {
                    fclose(file);
                    printf("error");
                    return 1;
                }
                counter++;
            }
            if (jpeg == true)
            {
                fwrite(buffer, 512, 1, outptr);
            }
        }
        fclose(file);
        if (jpeg == true)
        {
            fclose(outptr);
        }
        return 0;
    }
}