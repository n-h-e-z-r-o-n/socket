#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345

int main() {


  char serai_Entry[50];
  char  Reg_No_Entry[100];
  char Name_Entry[100];
  char json[20000];


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

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of remote server

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in connection. Exiting...");
        exit(1);
    }

;

    // Format input as JSON string
    sprintf(buffer, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry);
    //strcpy(buffer, "Hello from client!");
    send(sockfd, buffer, strlen(buffer), 0);
    printf("Data sent to server: %s\n", buffer);


    return 0;
}
