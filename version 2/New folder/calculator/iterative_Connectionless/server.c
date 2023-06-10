// iterative Connectionless server calculator
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

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
    int server_fd, valread;
    struct sockaddr_in address, client_addr;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create a socket for the server and Bind the socket to a specific port.
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));


    while (1) {
        memset(buffer, 0, sizeof(buffer)); //	Clear the buffer used for communication

        //	Receive a mathematical expression query from the client
        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addrlen);

        printf("\n\tCalculation Query: %s\n", buffer);

        // 	Perform the calculation for the received expression
        char* answer =calculator(buffer);

        //	Send the answer back to the client
        sendto(server_fd, answer,  strlen(answer), 0, (struct sockaddr *)&client_addr, addrlen);
    }

    return 0;
}
