//  Iterative-Connection-Oriented server
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
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    //	Create a socket for the server and Bind the socket to a specific port.
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for incoming connections.
    listen(server_fd, 3);

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a new client connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        printf("New client connected...\n");
        //	Read the calculation query sent by the client
        read(new_socket, buffer, 1024);

        printf("\n\tCalculation Query: %s\n", buffer);

        //	Perform the calculation using the calculator function
        char* answer =calculator(buffer);

        // Send the result back to the client using send
        send(new_socket, answer, strlen(answer), 0);

        memset(buffer, 0, sizeof(buffer)); //	Clear the buffer used for communication

        close(new_socket); // close the socket
    }

    return 0;
}
