//concurrent - connectionless client using fork and process
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define MAX_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    char* message = "Hello from client!";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Sendto failed");
        exit(EXIT_FAILURE);
    }

    printf("Message sent to server.\n");

    char buffer[MAX_SIZE] = {0};
    int valread;
    if ((valread = recvfrom(client_socket, buffer, MAX_SIZE, 0, NULL, NULL)) < 0) {
        perror("Recvfrom failed");
        exit(EXIT_FAILURE);
    }

    printf("Server response: %s\n", buffer);

    close(client_socket);

    return 0;
}
