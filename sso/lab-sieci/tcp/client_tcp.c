#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#define MY_PORT 2000
#define TSIZE 32

typedef struct
{ // Komunikat
    int typ;
    char tekst[TSIZE];
} komunikat_t;

main(int argc, char *argv[])
{
    int sock, cnt, res;
    struct sockaddr_in server;
    struct hostent *hp, *gethostbyname();
    komunikat_t msg;

    // Tworzenie gniazdka
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Blad gniazdka");
        exit(1);
    }

    // Uzyskanie adresu maszyny z linii polecen
    server.sin_family = AF_INET;
    hp = gethostbyname("127.0.0.1");
    if (hp == 0)
    {
        printf("%s nieznany\n", argv[1]);
        exit(2);
    }
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    server.sin_port = htons(MY_PORT);

    // Proba polaczenia
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Polaczenie");
        exit(1);
    }
    printf("Polaczenie nawiazane\n");

    // Petla odczytu
    cnt = 0;
    memset(&msg, 0, sizeof(msg));
    msg.typ = 1;
    do
    {
        sprintf(msg.tekst, "%d komunikat od klienta", cnt);
        res = send(sock, &msg, sizeof(msg), 0);

        // printf("Zapis %d bajtow\n", res);
        cnt++;
        res = recv(sock, &msg, sizeof(msg), MSG_WAITALL);
        if (res < 0)
        {
            perror("Blad odczytu");
            break;
        }
        if (res == 0)
        {
            printf("Polaczenie zamkniete");
            break;
        }
        // printf("Msg = %d Tekst = %s\n", cnt, msg.tekst);
        printf("Odebrano komunikat \n");
        printf("• tresc: %s\n\n", msg.tekst);
    } while (cnt < 5);
}