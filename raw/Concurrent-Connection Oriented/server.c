// Concurrent-Connection Oriented server using fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
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

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        int pid = fork();


        if (pid == 0) { // Child process
            close(server_fd);

            valread = read(new_socket, buffer, 1024);
            printf("Received message: %s\n", buffer);

            send(new_socket, buffer, strlen(buffer), 0);
            printf("Sent message: %s\n", buffer);

            close(new_socket);
            exit(EXIT_SUCCESS);
        }

        // Parent process
        close(new_socket);
        wait(NULL);
    }

    return 0;
}
