// Concurrent-Connection Oriented client using fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

char* user_detail_prompt() {  // Prompt user for their information --------------
      char* user_detail = malloc(sizeof(char) * 30); // allocate memory for string
      char serai_Entry[1000];
      char Reg_No_Entry[1000];
      char Name_Entry[1000];
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

      sprintf(user_detail, "%s,%s,%s\n", serai_Entry, Reg_No_Entry, Name_Entry); // Format input in to single string

      return user_detail;
}


int main() {
    int client_fd, valread;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));



    char* your_details = user_detail_prompt();

    send(client_fd, your_details, strlen(your_details), 0);

    memset(buffer, 0, sizeof(buffer));

    read(client_fd, buffer, 1024);

    printf("Received:  %s\n", buffer);
    memset(your_details, 0, sizeof(your_details));


    return 0;
}
