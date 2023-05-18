// iterative_Connectionless server calculator 
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
    char mathematical_question[1024];

    // 	Create a socket for the server and Bind the socket to a specific port.
    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("\n\n\tEnter a mathematical expression :  ");
    fgets(mathematical_question, 1024, stdin);

    sendto(client_fd, mathematical_question, strlen(mathematical_question), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    recvfrom(client_fd, buffer, sizeof(buffer), 0, NULL, NULL);

    printf("\n\n\tANSWER: %s\n\n", buffer);

    return 0;
}
