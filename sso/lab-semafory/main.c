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
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/sem.h>

#define SHM_KEY 512435
#define SEM_KEY ((key_t)12345L)
#define PERM 0600

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
    char *name;
    int balance;
} buf_t;

int main(int argc, char *argv[])
{
    int value = atoi(argv[1]);  // zadana wartosc pieniedzy
    int repeat = atoi(argv[2]); // zadana liczba powtorzen

    int k, i, shmid, semid, valueHelper, pid;
    buf_t *buf;

    struct sembuf p = {0, -1, 0};
    struct sembuf v = {0, 1, 0};

    SAFE(semid = semget(SEM_KEY, 1, PERM | IPC_CREAT));

    shmid = shmget(SHM_KEY, sizeof(buf_t), 0644 | IPC_CREAT);
    SAFE(buf = shmat(shmid, NULL, 0));

    if (shmid < 0)
    {
        shmid = shmget(SHM_KEY, sizeof(buf_t), 0);
    }

    SAFE(pid = fork());

    if (pid == 0)
    {
        SAFE(semop(semid, &p, 1)); // zajecie semafora

        buf->name = "Konto nr 1";
        for (i = 0; i < repeat; i++)
        {
            valueHelper = buf->balance;
            valueHelper += value;
            buf->balance = valueHelper;
        }

        printf("Nazwa konta: %s\n", buf->name);
        printf("Stan konta: %d\n", buf->balance);

        SAFE(semop(semid, &v, 1)); // zwolnienie semafora

        exit(0);
    }
    else
    {
        SAFE(semop(semid, &p, 1)); // zajecie semafora

        buf->name = "Konto nr 1";
        for (i = 0; i < repeat; i++)
        {
            valueHelper = buf->balance;
            valueHelper += value;
            buf->balance = valueHelper;
        }

        printf("Nazwa konta: %s\n", buf->name);
        printf("Stan konta: %d\n", buf->balance);

        SAFE(semop(semid, &v, 1)); // zwolnienie semafora

        wait(NULL);
        exit(0);
    }

    return 0;
}