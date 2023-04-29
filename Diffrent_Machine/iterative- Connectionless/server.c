//iterative- Connectionless
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define PORT 12345
#define MAX_JSON_SIZE 1000

int main() {
    int sockfd, feedback;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];
    char json[20000];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in bind. Exiting...");
        exit(1);
    }

    printf("Server is listening for incoming datagrams...\n");

    while (1) {
        client_len = sizeof(client_addr);
        // Receive incoming datagram
        feedback = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &client_len);
        if (feedback == -1) {
            perror("Error in recvfrom. Continuing...");
            continue;
        }

        printf("Received %d bytes from client %s:%d\n", feedback, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Data received from client: %s\n", buffer);

        const char* response = "Data uploaded successfully to the file";
        // Send response back to the client
        sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    }

    // Close the socket
    close(sockfd);

    return 0;
}
