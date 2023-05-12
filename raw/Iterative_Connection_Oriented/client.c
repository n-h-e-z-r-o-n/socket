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
    char *hello = "Hello from client";

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Connected to server...\n");

    send(client_fd, hello, strlen(hello), 0);
    printf("Sent message: %s\n", hello);

    memset(buffer, 0, sizeof(buffer));

    read(client_fd, buffer, 1024);
    printf("Received message: %s\n", buffer);

    close(client_fd);

    return 0;
}
