//Iterative-Connection-Oriented
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
    int feedback;
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
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
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in bind. Exiting...");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 10);
    printf("Server is listening for incoming connections...\n");

    while (1) {
        client_len = sizeof(client_addr);
        // Accept incoming connections
        new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (new_sockfd == -1) {
            perror("Error in accept. Exiting...");
            exit(1);
        }

        printf("New connection established...\n");
        printf("Client IP address: %s\n", inet_ntoa(client_addr.sin_addr));

        // Read data from client
        feedback = recv(new_sockfd, buffer, 1024, 0);
        if (feedback == -1) {
            perror("Error in recv. Exiting...");
            exit(1);
        }

        printf("Received %d bytes from client: %s\n", feedback, buffer);

        // Process data
        // ...

        // Send response to client
        const char* response = "Data Uploded Successfuly to the file";
        feedback = send(new_sockfd, response, strlen(response), 0);
        if (feedback == -1) {
            perror("Error in send. Exiting...");
            exit(1);
        }

        printf("Sent %d bytes of response to client.\n", feedback);

        // Close the socket
        close(new_sockfd);
    }

    // Close the listening socket
    close(sockfd);

    return 0;
}
