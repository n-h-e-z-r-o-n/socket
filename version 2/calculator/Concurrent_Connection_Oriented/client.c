// Concurrent-Connection Oriented client using fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
    int client_fd, valread;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char mathematical_question[1024];

    printf("\n\n- Creating Client Sockets\n");

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("- Connecting Client socket to server address\n");

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));


    printf("\n\n\tEnter a mathematical expression :  ");
    fgets(mathematical_question, 1024, stdin);


    printf("- Sending mathematical Question to the Server\n");

    send(client_fd, mathematical_question, strlen(mathematical_question), 0);

    memset(buffer, 0, sizeof(buffer));

    printf("- Reveiving Answer from  the server\n");

    read(client_fd, buffer, 1024);

    printf("\n\n\tANSWER: %s\n\n", buffer);

    memset(mathematical_question, 0, sizeof(mathematical_question));

    printf("- Clossing Client Sockets");
    close(client_fd);

    return 0;
}
