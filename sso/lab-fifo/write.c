#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define size 32

#define SAFE(x)                                                         \
    if ((x) < 0)                                                        \
    {                                                                   \
        fprintf(stderr, "Błąd: %s w %s:%d \n", #x, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                             \
    }

#define SAFE_FIFO_DELETE(x)                                                         \
    if ((x) < 0)                                                                    \
    {                                                                               \
        fprintf(stderr, "Proba usuniecia nieistniejacego FIFO w: %d \n", __LINE__); \
    }

#define SAFE_FIFO_OPEN(x)                                                         \
    if ((x) < 0)                                                                  \
    {                                                                             \
        fprintf(stderr, "Proba utworzenia istniejacego FIFO w: %d \n", __LINE__); \
        exit(EXIT_FAILURE);                                                       \
    }

int main(int argc, char **argv)
{
    int number = 10;
    int i;

    char *name = "myfifo";
    int fd;
    int res;
    int bytes;
    int pid;

    SAFE_FIFO_DELETE(unlink(name));
    SAFE_FIFO_OPEN(res = mkfifo(name, O_CREAT | 0660));

    printf("write: otwieranie\n");

    SAFE(fd = open(name, O_WRONLY));

    printf("Rozpoczecie pisania\n");

    for (i = 0; i < number; i++)
    {
        SAFE(bytes = write(fd, &i, sizeof(i)));
        printf("(write) tresc: %d\n", i);
    }

    printf("Zakonczenie pisania\n");

    close(fd);
    unlink(name);
    return 0;
}