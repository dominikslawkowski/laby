#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define BUFLEN 80
#define PORT 9950

typedef struct
{
    int typ;
    char buf[BUFLEN];
} msgt;

void show_error(char *s)
{
    perror(s);
    _exit(1);
}

int main(void)
{
    struct sockaddr_in adr_moj, adr_cli;
    int s, i, slen = sizeof(adr_cli), snd, rec, blen = sizeof(msgt);
    char buf[BUFLEN];
    msgt msg;
    gethostname(buf, sizeof(buf));
    printf("Host: %s\n", buf);

    // socket
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0)
    {
        show_error("socket");
    }

    printf("Utworzenie: Serwer %d\n", s);

    // Ustalenie adresu IP nadawcy
    memset((char *)&adr_moj, 0, sizeof(adr_moj));
    adr_moj.sin_family = AF_INET;
    adr_moj.sin_port = htons(PORT);
    adr_moj.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind
    if (bind(s, (struct sockaddr *)&adr_moj, sizeof(adr_moj)) == -1)
    {
        show_error("bind");
    }

    for (i = 0; i < 10; i++)
    {
        // Odbior komunikatu
        rec = recvfrom(s, &msg, blen, 0, (struct sockaddr *)&adr_cli, &slen);

        if (rec < 0)
        {
            show_error("recvfrom");
        }

        printf("Odebrano komunikat\n");
        printf("• tresc: %s\n\n", msg.buf);

        sprintf(msg.buf, "%d komunikat z serwera", i);
        snd = sendto(s, &msg, blen, 0, (struct sockaddr *)&adr_cli, slen);

        if (snd < 0)
        {
            show_error("sendto");
        }
        sleep(1);
    }
    close(s);
    return 0;
}