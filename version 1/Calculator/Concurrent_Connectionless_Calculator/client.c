// Concurrent-Connectionless client (calculator)
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
    socklen_t server_len;
    char mathematical_question[1024];
    char json[MAX_JSON_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("\nEnter a mathematical expression :  ");
    fgets(mathematical_question, 1024, stdin);

    // Send data to server
    sendto(sockfd, mathematical_question, strlen(mathematical_question), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive response from server
    recvfrom(sockfd, mathematical_question, 1024, 0, (struct sockaddr*)&server_addr, &server_len);
    printf("\nResponse\n\t\t ANSWER: %s\n\n", mathematical_question);

    // Close the socket
    close(sockfd);

    return 0;
}
