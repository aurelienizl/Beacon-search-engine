#include "crawler.h"

int i = 0;

void test(char **argv)
{
    while (1)
    {
        char *page = get_page(argv[1]);
        i++;
        printf("Page %d:\n", i);
        // printf("%s\n", page);
        free(page);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        errx(1, "This program needs 1 argument");
    }
    else
    {
        for (size_t i = 0; i < 16; i++)
        {
            // Create a thread
            pthread_t thread;
            int err = pthread_create(&thread, NULL, (void *)test, argv);
            if (err != 0)
            {
                errx(1, "pthread_create error");
            }
        }
        while (1)
        {
            sleep(1);
        }
    }
}
