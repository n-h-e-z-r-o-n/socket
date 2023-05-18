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

char* user_detail_prompt() {  // Prompt user for their information --------------
      char* user_detail = malloc(sizeof(char) * 30); // allocate memory for string
      char serai_Entry[50];
      char Reg_No_Entry[50];
      char Name_Entry[50];
      printf("\n --------------------------------------------------------\n");
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

      return user_detail;
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    char* your_details = user_detail_prompt();

    sendto(client_socket, your_details, strlen(your_details), 0, (struct sockaddr *)&server_address, sizeof(server_address));


    printf("Message sent to server.\n");

    char buffer[MAX_SIZE] = {0};
    recvfrom(client_socket, buffer, MAX_SIZE, 0, NULL, NULL);

    printf("Server response: %s\n", buffer);

    close(client_socket);

    return 0;
}
