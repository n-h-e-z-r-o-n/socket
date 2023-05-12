//  Iterative-Connection-Oriented client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

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
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char *hello = "Hello from client";

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Connected to server...\n");

    char* your_details = user_detail_prompt();

    send(client_fd, your_details, strlen(your_details), 0);
    
    printf("Sent message: %s\n", hello);

    memset(buffer, 0, sizeof(buffer));

    read(client_fd, buffer, 1024);

    printf("Received message: %s\n", buffer);

    close(client_fd);

    return 0;
}
