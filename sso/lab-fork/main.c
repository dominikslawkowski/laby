#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SAFE(x)                                                         \
    if ((x) < 0)                                                        \
    {                                                                   \
        fprintf(stderr, "Błąd: %s w %s:%d \n", #x, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                             \
    }

int main(void)
{
    int numberOfPipes = 5245;
    int numberOfRecords = 70000;

    int pid, status, i, result;
    int fd[numberOfPipes][2];
    char buf = 'a';
    char buf2[1];
    int bytes_write, bytes_read, buff_size;

    numberOfPipes = 1;
    for (i = 0; i < numberOfPipes; i++)
    {
        SAFE(pipe(fd[i]));
        printf("%d\n", i);
    }

    SAFE(pid = fork());

    if (pid == 0)
    {
        SAFE(close(fd[0][1]));
        // sleep(2);
        for (i = 0; i < numberOfRecords; i++)
        {
            SAFE(read(fd[0][0], &buf2, sizeof(buf)));
            // printf("%d. odebrano: %c\n", i, buf);
        }

        SAFE(close(fd[0][0]));
        exit(0);
    }
    else
    {
        SAFE(close(fd[0][0]));

        for (i = 0; i < numberOfRecords; i++)
        {
            SAFE(result = write(fd[0][1], &buff_size, sizeof(buff_size)));
            if (result > -1)
            {
                buff_size += 1;
                printf("%d. wyslano bajtow: %d\n", i, buff_size);
            }
        }

        pid = wait(&status);
        SAFE(close(fd[0][1]));
        exit(0);
    }

    return 0;
}