#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MY_PORT 2000
#define TSIZE 32

typedef struct
{
    int typ;
    char tekst[TSIZE];
} komunikat_t;

void show_error(char *s)
{
    perror(s);
    _exit(1);
}
main()
{
    int sock, msgsock, length;
    struct sockaddr_in server;
    int rval, res, i, cnt;
    komunikat_t msg;

    // Tworzenie gniazdka
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        show_error("Blad gniazdka");
    }

    printf("Utworzenie: Serwer %d\n", sock);

    // Adres gniazdka
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    // server.sin_port = ntohs(MY_PORT);
    server.sin_port = htons(MY_PORT);
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
    {
        show_error("bind");
    }

    // Uzyskanie danych poloczenia
    length = sizeof(server);
    if (getsockname(sock, (struct sockaddr *)&server, &length))
    {
        show_error("getsocketname");
    }

    printf("Numer portu %d\n", ntohs(server.sin_port));

    // Start przyjmowania polaczen
    listen(sock, 5);
    do
    {
        printf("Czekam na polaczenie \n");
        msgsock = accept(sock, 0, 0);
        cnt = 0;
        if (sock == -1)
            perror("accept");
        else
        {
            do
            {
                // printf("Polaczenie nawiazane - %d \n", sock);

                res = recv(msgsock, &msg, sizeof(msg), MSG_WAITALL);
                if (res < 0)
                {
                    show_error("recv");
                }

                if (res == 0)
                {
                    printf("Rozlaczenie\n");
                    break;
                }
                printf("Odebrano komunikat\n");
                printf("• tresc: %s\n\n", msg.tekst);

                cnt++;
                msg.typ = 1;

                sprintf(msg.tekst, "%d komunikat z serwera", cnt);
                res = send(msgsock, &msg, sizeof(msg), 0);
                if (res < 0)
                {
                    show_error("send");
                }

                // printf("Odpowiedz wyslana - %d bajtow\n", res);
                sleep(1);
            } while (1);
            close(msgsock);
        }
    } while (1);
    printf("Koniec\n");
} /* Main */