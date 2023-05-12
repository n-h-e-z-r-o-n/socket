#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, valread;
    struct sockaddr_in address, client_addr;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addrlen);

        if (recv_len < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        printf("Received message: %s\n", buffer);

        if (sendto(server_fd, buffer, recv_len, 0, (struct sockaddr *)&client_addr, addrlen) < 0) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent message: %s\n", buffer);
    }

    return 0;
}
