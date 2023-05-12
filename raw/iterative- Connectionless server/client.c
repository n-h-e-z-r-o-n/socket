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

    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);

        if (sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent message: %s\n", buffer);

        memset(buffer, 0, sizeof(buffer));

        if (recvfrom(client_fd, buffer, sizeof(buffer), 0, NULL, NULL) < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Received message: %s\n", buffer);
    }

    return 0;
}
