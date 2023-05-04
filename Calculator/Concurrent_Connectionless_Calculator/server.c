//Concurrent-Connectionless server (simple-calculator)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>
char buffer[1024];

float calculator(char *expr) {
    printf(" calculating \n\n");
    char op;
    float num1, num2;
    char *ptr = expr;
    num1 = strtof(ptr, &ptr); // Extract the first operand from the expression
    printf(" %f \n", num1);
    while (*ptr != '\0') {
        op = *ptr++; // Extract the operator from the expression
        num2 = strtof(ptr, &ptr); // Extract the next operand from the expression
        // Perform the calculation based on the operator
        switch(op) {
            case '+':
                num1 += num2;
                printf(" %f \n\n", num2);
                break;
            case '-':
                num1 -= num2;
                break;
            case '*':
                num1 *= num2;
                break;
            case '/':
                if(num2 == 0) {
                    printf("Error: division by zero");
                    exit(1);
                }
                num1 /= num2;
                break;
            default: //end of string
            num1 = 00.00 
                break;
        }
  }
  return num1;
}

void *handle_client(void *arg) {
    struct sockaddr_in address = *(struct sockaddr_in*)arg;
    int addrlen = sizeof(address);
    char *server_massege;
    char myString[100];

    // Create a new socket file descriptor for this thread
    int new_socket;
    if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
   //------------------------------ calculation --------------------------------
   char *end;
   char expr[100];
   while(isspace(*expr)) {expr+1;}
   end = expr + strlen(expr) - 1;
   while(end > expr && isspace(*end)) end--;
   *(end+1) = '\0';

   float result = calculator(buffer);
   sprintf(myString, "%.2f", result);
   server_massege =myString;

    // Send data to client
    sendto(new_socket, server_massege, strlen(server_massege), MSG_CONFIRM, (const struct sockaddr *) &address, addrlen);
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    // Close socket
    close(new_socket);
    return NULL;
}


#define PORT 12345
#define MAX_JSON_SIZE 1000


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    socklen_t client_len;
    char json[MAX_JSON_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in bind. Exiting...");
        exit(1);
    }

        printf("==============================Concurrent Connection less Server==========================================\n");
        printf("server is listening ...\n");
    pthread_t thread_id;
    while(1) {
        // Accept client connection
        int len = recvfrom(sockfd, buffer, 1024,  MSG_WAITALL, ( struct sockaddr *) &server_addr,  &addrlen);
        buffer[len] = '\0';
        printf("\n -------------------------------------------------------------------------------------------------------\n");
        printf("Calculation Query :   %s\n", buffer);

        // Create a new thread to handle the client request
        if(pthread_create(&thread_id, NULL, handle_client, (void*)&server_addr) < 0) {
            perror("Server crush: could not create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }
    close(sockfd);// Close the socket

    return 0;
}
