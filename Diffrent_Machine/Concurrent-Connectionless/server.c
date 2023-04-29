//Concurrent-Connectionless
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>

char buffer[1024];

void *handle_client(void *arg) {
    struct sockaddr_in address = *(struct sockaddr_in*)arg;
    int addrlen = sizeof(address);
    char *server_massege;

    // Create a new socket file descriptor for this thread
    int new_socket;
    if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_massege = " hezron  ";


    // Send data to client
    sendto(new_socket, server_massege, strlen(server_massege), MSG_CONFIRM, (const struct sockaddr *) &address, addrlen);
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    // Close socket
    close(new_socket);
    return NULL;
}


#define PORT 12345
#define MAX_JSON_SIZE 1000


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    socklen_t client_len;
    char json[MAX_JSON_SIZE];

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
    pthread_t thread_id;
    while(1) {
        // Accept client connection
        int len = recvfrom(sockfd, buffer, 1024,  MSG_WAITALL, ( struct sockaddr *) &server_addr,  &addrlen);
        buffer[len] = '\0';
        printf("\n --------------------------------------------------------------------------------------------------------\n");
        printf("Received message from client 1: %s\n", buffer);

        // Create a new thread to handle the client request
        if(pthread_create(&thread_id, NULL, handle_client, (void*)&server_addr) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
