// concurrent - connectionless server calculator using Fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_MSG_LEN 1024


char* calculator(char *expr) {
    char* result =  malloc(sizeof(char) * 50); // allocate memory for string;
    printf(" calculating . . . \n\n");
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
        }
    }
    // result

    sprintf(result, "%.2f", num1);
    printf("\n\tresult:  %s\n", result);
    return result;
}


int main() {
    int server_fd, len;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in server_addr, client_addr;

    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("\t ----- Concurrent Connection less  Server calculator -----\n");

    while (1) {
        int pid = fork();

        if (pid == 0) { // Child process
            len = sizeof(client_addr);
            memset(buffer, 0, sizeof(buffer));

            recvfrom(server_fd, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &len);

            printf("\n\tCalculation Query: %s\n", buffer);

            char* answer =calculator(buffer);


            sendto(server_fd, answer, strlen(answer), 0, (struct sockaddr *)&client_addr, len);

            close(server_fd);
            exit(EXIT_SUCCESS);
        }

        // Parent process
        wait(NULL);
    }

    return 0;
}
