//Iterative-Connection-Oriented
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_JSON_SIZE 1000


int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char json[MAX_JSON_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of the server

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in connect. Exiting...");
        exit(1);
    }
    printf("Connected to the server...\n");

    // Send data to the server
    const char *message = "Hello, server!";
    send(sockfd, message, strlen(message), 0);
    printf("Data sent to the server: %s\n", message);

    // Receive response from the server
    recv(sockfd, buffer, 1024, 0);
    printf("Response from the server: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
