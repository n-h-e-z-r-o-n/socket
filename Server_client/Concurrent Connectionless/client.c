#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024] = {0};
    char buffer1[1024] = {0};
    char user_detail[300];
    char serai_Entry[50];
    char Reg_No_Entry[50];
    char Name_Entry[50];


    // Prompt user for their information
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

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr)); //  initialize servaddr to zero.

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int m, len;

    // Send data to server
    sendto(sockfd, (const char *)data_packet, strlen(data_packet),MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

    // Receive data from server
    m = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer[m] = '\0';

    printf("Server : %s\n", buffer);
    printf("\n --------------------------------------------------------------------------------------------------------\n");

    close(sockfd);
    return 0;
}
