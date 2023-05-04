// Concurrent-Connectionless
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define MAX_JSON_SIZE 1000


int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_len;


    // Prompt user for their information --------------
    char buffer[1024] = {0};
    char user_detail[300];
    char serai_Entry[50];
    char Reg_No_Entry[50];
    char Name_Entry[50];
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("         Fill In Your Details\n");
    printf("\n\t 1. Enter Serail Number  :  ");
    scanf("%s", serai_Entry);

    printf("\n\t 2. Enter Reg_Number     :  ");
    scanf("%s", Reg_No_Entry);

    printf("\n\t 3. Enter Full Name      :  ");
    scanf("%s", Name_Entry);
    printf("\n\t   \n");

    // Remove newline characters from input
    serai_Entry[strcspn(serai_Entry, "\n")] = 0;
    Reg_No_Entry[strcspn(Reg_No_Entry, "\n")] = 0;
    Name_Entry[strcspn(Name_Entry, "\n")] = 0;


    sprintf(user_detail, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry); // Format input in to single string
    char *data_packet = user_detail;



    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // secify the address of the server


    // Send data to server
    sendto(sockfd, data_packet, strlen(data_packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive response from server
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&server_addr, &server_len);
    printf("Server : %s\n", buffer);
    printf("\n ------------------------------------- Concurrent _ Connectionless _ Sever  ------------------------\n");

    // Close the socket
    close(sockfd);

    return 0;
}
