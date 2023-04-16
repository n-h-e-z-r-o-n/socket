#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define PORT 8080


void client_conneection(char str[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on port %d\n", PORT);

    // Send data to server
    const char *message = str;
    write(sockfd, message, strlen(message));

    // Read response from server
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) {
        perror("Failed to read");
        exit(EXIT_FAILURE);
    }

    printf("SERVER FEEDBACK: %s \n", buffer);

    close(sockfd); // Close the socket
}






#define MAX_SERIAL_NO_LEN 50
#define MAX_REG_NO_LEN 100
#define MAX_NAME_LEN 100
#define MAX_JSON_LEN 20000

int main(){

    char serai_Entry[MAX_SERIAL_NO_LEN];
    char  Reg_No_Entry[MAX_REG_NO_LEN];
    char Name_Entry[MAX_NAME_LEN];
    char json[MAX_JSON_LEN];
    
    
    bool fileExists = false;


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


    // Remove newline characters from input
    serai_Entry[strcspn(serai_Entry, "\n")] = 0;
    Reg_No_Entry[strcspn(Reg_No_Entry, "\n")] = 0;
    Name_Entry[strcspn(Name_Entry, "\n")] = 0;
    
 
    // Format input as JSON string
    sprintf(json, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry);

    client_conneection(json);




}








































