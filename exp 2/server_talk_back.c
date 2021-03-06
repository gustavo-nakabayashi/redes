#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_PORT 54321
#define MAX_PENDING 5
#define MAX_LINE 256

int main()
{
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int len;
    int s, new_s;
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror(" simplex - talk : socket ");
        exit(1);
    }
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0)
    {
        perror(" simplex - talk : bind ");
        exit(1);
    }
    len = sizeof(sin);
    listen(s, MAX_PENDING);
    while (1)
    {
        if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0)
        {
            perror(" simplex - talk : accept ");
            exit(1);
        }
        while (len = recv(new_s, buf, sizeof(buf), 0))
        {
            printf(" recebido: ");
            fputs(buf, stdout);
            printf(" enviado: ");
            buf[MAX_LINE - 1] = '\0';
            len = strlen(buf) + 1;
            fputs(buf, stdout);
            send(new_s, buf, len, 0);
            printf("\n");
        }
        close(new_s);
    }
}