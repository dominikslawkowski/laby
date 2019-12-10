#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define SAFE(x)                                                         \
    if ((x) < 0)                                                        \
    {                                                                   \
        fprintf(stderr, "Błąd: %s w %s:%d \n", #x, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                             \
    }

#define SAFE_MMAP(x)                                                    \
    if ((x) == NULL)                                                    \
    {                                                                   \
        fprintf(stderr, "Błąd: %s w %s:%d \n", #x, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                             \
    }

typedef struct
{
    char name[60];
    int balance;
} buf_t;

int main()
{
    const int key = 512435;

    int pid, k, i, shmid, stat;
    buf_t *buf;

    shmid = shmget(key, sizeof(buf_t), 0644 | IPC_CREAT);

    if (shmid < 0)
    {
        shmid = shmget(key, sizeof(buf_t), 0);
    }

    SAFE(buf = shmat(shmid, NULL, 0));

    printf("Procesy startuja\n");

    SAFE(pid = fork());

    if (pid == 0)
    {
        printf("p.pot. - przelew\n");
        buf->balance += 100; // zwiekszenie stanu konta przez proces potomny
        exit(0);
    }
    else
    {
        printf("p.rodz. - odczyt stanu konta\n");
        sleep(1);
        printf("Stan konta: %d\n", buf->balance); // sprawdzenie stanu konta przez proces macierzysty
        pid = wait(&stat);
    }

    return 0;
}