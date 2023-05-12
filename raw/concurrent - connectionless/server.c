// concurrent - connectionless server using Fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_MSG_LEN 1024

int main() {
    int server_fd, len;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in server_addr, client_addr;

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int pid = fork();

        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            len = sizeof(client_addr);
            memset(buffer, 0, sizeof(buffer));

            if (recvfrom(server_fd, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &len) < 0) {
                perror("recvfrom failed");
                exit(EXIT_FAILURE);
            }

            printf("Received message: %s\n", buffer);

            if (sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, len) < 0) {
                perror("sendto failed");
                exit(EXIT_FAILURE);
            }

            close(server_fd);
            exit(EXIT_SUCCESS);
        }

        // Parent process
        wait(NULL);
    }

    return 0;
}
