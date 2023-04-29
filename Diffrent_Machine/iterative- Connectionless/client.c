// iterative- Connectionless
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define PORT 12345

int main() {
    int sockfd, feedback;
    struct sockaddr_in server_addr;
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
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set server IP address here

    printf("Enter data to send to server: ");
    fgets(buffer, 1024, stdin);

    // Send data to server
    feedback = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (feedback == -1) {
        perror("Error in sendto. Exiting...");
        exit(1);
    }

    // Receive response from server
    feedback = recvfrom(sockfd, buffer, 1024, 0, NULL, NULL);
    if (feedback == -1) {
        perror("Error in recvfrom. Exiting...");
        exit(1);
    }

    printf("Received %d bytes from server: %s\n", feedback, buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
