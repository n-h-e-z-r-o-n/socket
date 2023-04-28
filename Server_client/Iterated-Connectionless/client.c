#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define PORT 8080

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    char user_detail[300];
    char serai_Entry[50];
    char  Reg_No_Entry[50];
    char Name_Entry[50];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Prompt user for name and email
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("------>  Fill In Your Details\N");
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

    // Format input as JSON string
    sprintf(user_detail, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry);
    char *packet_ = user_detail ;

    int n, len;
    sendto(sockfd, (const char *)packet_, strlen(packet_), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Your details have been sent to server.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("SERVER FEEDBACK: %s\n", buffer);

    printf("\n --------------------------------------------------------------------------------------------------------\n");

    close(sockfd);
    return 0;
}
