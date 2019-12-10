#include <stdio.h>
#include <signal.h>
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

#define SAFE_FIFO_OPEN(x)                                                         \
    if ((x) < 0)                                                                  \
    {                                                                             \
        fprintf(stderr, "Proba utworzenia istniejacego FIFO w: %d \n", __LINE__); \
        exit(EXIT_FAILURE);                                                       \
    }

void just_sleep()
{
    while (1)
    {
        sleep(1);
    }
}

static void sig_handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("odebrano SIGUSR1\n\n");
    }
    if (sig == SIGUSR2)
    {
        printf("odebrano SIGUSR2\n\n");
    }
    if (sig == SIGPIPE)
    {
        printf("odebrano SIGPIPE\n\n");
    }
    else if (sig == SIGINT)
    {
        char c;

        signal(sig, SIG_IGN);
        printf("\nNacisnales Ctrl-C.\n"
               "Czy zakonczyc program? [t/n] ");
        c = getchar();
        if (c == 't' || c == 'T')
        {
            unlink("myfifo");
            exit(0);
        }
        else
        {
            signal(SIGINT, sig_handler);
        }

        getchar();
    }
    else
    {
        fprintf(stderr, "odebrano sygnal %d\n", sig);
    }
}

void set_signal(void *handler)
{
    if (signal(SIGUSR1, sig_handler) == SIG_ERR) // obsluga bledu SIGUSR1
    {
        fprintf(stderr, "blad w odbieraniu SIGUSR1");
    }

    if (signal(SIGUSR2, sig_handler) == SIG_ERR) // obsluga bledu SIGUSR2
    {
        fprintf(stderr, "blad w odbieraniu SIGUSR2");
    }

    if (signal(SIGPIPE, sig_handler) == SIG_ERR) // obsluga bledu SIGPIPE
    {
        fprintf(stderr, "blad w odbieraniu SIGPIPE");
        just_sleep();
    }
    if (signal(SIGINT, sig_handler) == SIG_ERR) // obsluga bledu SIGINT
    {
        fprintf(stderr, "blad w odbieraniu SIGINT");
    }
}

int main(void)
{
    set_signal(sig_handler);

    int pid, i, bytes, buf2;
    char *name = "myfifo";
    int res, fd;
    char *buf = "tresc";

    SAFE_FIFO_OPEN(res = mkfifo(name, O_CREAT | 0660));

    SAFE(pid = fork());

    if (pid > 0)
    {
        SAFE(fd = open(name, O_WRONLY));
        for (i = 0; i < 4; i++)
        {
            SAFE(bytes = write(fd, &i, sizeof(i)));
            printf("(write) tresc: %d\n", i);
            sleep(1);
        }
    }
    else
    {
        SAFE(fd = open(name, O_RDONLY));
        for (i = 0; i < 2; i++)
        {
            SAFE(bytes = read(fd, &buf2, sizeof(buf2)));
            printf("(read) tresc: %d\n", buf2);
            sleep(1);
        }
        close(fd);
        wait(NULL);
    }

    while (1)
    {
        pause();
    }

    return 0;
}
