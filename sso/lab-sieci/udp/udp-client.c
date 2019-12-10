#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 80
#define PORT 9950
#define SRV_IP "127.0.0.1"

typedef struct
{
    int typ;
    char buf[BUFLEN];
} msgt;
void blad(char *s)
{
    perror(s);
    _exit(1);
}
int main(int argc, char *argv[])
{
    struct sockaddr_in adr_moj, adr_serw, adr_x;
    int s, i, slen = sizeof(adr_serw), snd, blen = sizeof(msgt), rec;
    char buf[BUFLEN];
    msgt msg;
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0)
    {
        blad("socket");
    }

    printf("Utworzenie: Klient %d\n", s);
    memset((char *)&adr_serw, 0, sizeof(adr_serw));
    adr_serw.sin_family = AF_INET;
    adr_serw.sin_port = htons(PORT);

    if (inet_aton("127.0.0.1", &adr_serw.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        _exit(1);
    }

    for (i = 0; i < 10; i++)
    {
        // Wysylanie komunikatu
        msg.typ = 1;
        sprintf(msg.buf, "%d komunikat od klienta", i);
        snd = sendto(s, &msg, blen, 0, (struct sockaddr *)&adr_serw, (socklen_t)slen);

        if (snd < 0)
        {
            blad("sendto");
        }

        rec = recvfrom(s, &msg, blen, 0, (struct sockaddr *)&adr_x, (socklen_t *)&slen);

        if (rec < 0)
        {
            blad("recvfrom");
        }

        printf("Odebrano komunikat \n");
        printf("• tresc: %s\n\n", msg.buf);
        }
    close(s);
    return 0;
}