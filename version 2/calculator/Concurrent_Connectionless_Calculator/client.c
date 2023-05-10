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
    char mathematical_question[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);


    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    printf("\n\n\tEnter a mathematical expression :  ");
    fgets(mathematical_question, 1024, stdin);
    char* message = "Hello from client!";

    sendto(client_socket, mathematical_question, strlen(mathematical_question), 0, (struct sockaddr*)&server_address, sizeof(server_address));

  
    recvfrom(client_socket, mathematical_question, MAX_SIZE, 0, NULL, NULL);


    printf("\n\n\tANSWER: %s\n\n", mathematical_question);




    close(client_socket);

    return 0;
}
