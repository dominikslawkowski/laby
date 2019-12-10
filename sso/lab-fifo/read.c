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
    int buf;

    int number = 10;
    int i;

    char *name = "myfifo";
    int fd;
    int res;
    int bytes;
    int pid;

    SAFE_FIFO_DELETE(unlink(name));
    SAFE_FIFO_OPEN(res = mkfifo(name, O_CREAT | 0660));

    printf("read: otwieranie\n");

    SAFE(fd = open(name, O_RDONLY));

    printf("Rozpoczecie czytania\n");

    for (i = 0; i < number; i++)
    {
        SAFE(bytes = read(fd, &buf, sizeof(buf)));
        printf("(read) tresc: %d\n", buf);
    }

    printf("Zakonczenie czytania\n");

    close(fd);
    unlink(name);
    return 0;
}