//Concurrent-Connection Oriented
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080

int main(int argc, char **argv) {
    int client_socket;
    struct sockaddr_in server_address;
    char message[200];
    int bytes_received;
    char serai_Entry[50];
    char  Reg_No_Entry[50];
    char Name_Entry[50];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // specify the ip address of the server
    server_address.sin_port = htons(SERVER_PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // Prompt user for name and email
    printf("\n");
    printf("-----Fill In Your Details");
    printf("\n\t 1. Enter Serail Number  :  ");
    scanf("%s", serai_Entry);

    printf("\n\t 2. Enter Reg_Number     :  ");
    scanf("%s", Reg_No_Entry);

    printf("\n\t 3. Enter Full Name      :  ");
    scanf("%s", Name_Entry);
    printf("\n\t   \n");

    // Format input as JSON string
    sprintf(message, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry);
    // Send message to server
    send(client_socket, message, strlen(message), 0);

    // Receive response from server
    memset(message, 0, 200);
    bytes_received = recv(client_socket, message, 200, 0);
    if (bytes_received == -1) {
        perror("Error receiving message");
        exit(EXIT_FAILURE);
    }
    printf("Response from server: %s\n", message);

    // Close client socket
    close(client_socket);

    return 0;
}
