#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#define size 32

#define SAFE(x)                                                         \
    if ((x) < 0)                                                        \
    {                                                                   \
        fprintf(stderr, "Błąd: %s w %s:%d \n", #x, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                             \
    }

#define SAFE_FIFO_OPEN(x)                                                         \
    if ((x) < 0)                                                                  \
    {                                                                             \
        fprintf(stderr, "Proba utworzenia istniejacego FIFO w: %d \n", __LINE__); \
        exit(EXIT_FAILURE);                                                       \
    }

int main(int argc, char **argv)
{
    mode_t mode = S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH;
    const char *name = "./fifo";
    int fd;
    int bytes;
    int pid;

    SAFE(mkfifo(name, mode));
    SAFE(pid = fork());

    if (pid == 0)
    {
        SAFE_FIFO_OPEN(fd = open(name, O_RDWR, mode));

        int bytes = 0;
        int counter = 0;

        printf("Rozpoczecie pisania.\n");

        while (1)
        {
            SAFE(bytes = write(fd, "a", 1));
            if (bytes == 0)
            {
                printf("Brak zapisu\n");
            }
            else
            {
                counter += bytes;
                printf("Zapisano bajtow: %d\n", counter);
            }
        }

        printf("Zakonczenie pisania.\n");
        SAFE(close(fd));
        exit(0);
    }
    else
    {
        SAFE(fd = open(name, O_RDONLY, mode));
        sleep(2);

        char buf;
        int counter = 0;
        int bytes = 0;
        int repetitionCounter = 0;

        bool next = true;
        printf("\t\t\tRozpoczecie czytania.\n");

        while (next)
        {
            sleep(1);

            printf("Podaj liczbe bajtow: ");
            scanf("%d", &repetitionCounter);
            printf("\n");

            for (int i = 0; i < repetitionCounter; i++)
            {
                SAFE(bytes = read(fd, &buf, 1));
                if (bytes == 0)
                {
                    printf("\t\t\tBrak odczytu\n");
                    next = false;
                }
                else
                {
                    counter += bytes;
                    printf("\t\t\tPrzeczytano bajtow: %d\n", counter);
                }
            }
        }

        printf("\t\t\tZakonczenie czytania.\n");
        wait(NULL);
        exit(0);
    }

    close(fd);
    unlink(name);
    return 0;
}