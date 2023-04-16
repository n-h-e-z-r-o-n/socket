#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr("IP_ADDRESS_OF_REMOTE_SERVER"); // IP address of remote server

    // Connect to server
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Error in connection. Exiting...");
        exit(1);
    }

    // Send and receive data
    recv(sockfd, buffer, 1024, 0);
    printf("Data received from server: %s\n", buffer);

    strcpy(buffer, "Hello from client!");
    send(sockfd, buffer, strlen(buffer), 0);
    printf("Data sent to server: %s\n", buffer);

    // Close socket
    close(sockfd);
}
