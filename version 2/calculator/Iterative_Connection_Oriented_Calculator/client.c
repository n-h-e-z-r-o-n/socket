//  Iterative-Connection-Oriented client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char mathematical_question[1024];

    printf("Creating Client Socket\n");
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connecting Client Socket to server address\n");

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));



    printf("\n\n\tEnter a mathematical expression :  ");
    fgets(mathematical_question, 1024, stdin);

    printf("Sending Calcluation query to the  server\n");
    send(client_fd, mathematical_question, strlen(mathematical_question), 0);

    memset(buffer, 0, sizeof(buffer));

    printf("- Receiving Calcluation answer from ther server\n");

    read(client_fd, buffer, 1024);

    printf("\n\n\tANSWER: %s\n\n", buffer);

    close(client_fd);

    return 0;
}
