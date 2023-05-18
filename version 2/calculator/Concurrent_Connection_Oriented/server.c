// Concurrent-Connection Oriented server using fork and process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

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

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 5);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        int pid = fork();


        if (pid == 0) { // Child process
            close(server_fd);

            read(new_socket, buffer, 1024);

            printf("\n\tCalculation Query: %s\n", buffer);

            //	Perform the calculation using the calculator function
            char* answer =calculator(buffer);

            send(new_socket, answer, strlen(answer), 0);

            memset(buffer, 0, sizeof(buffer)); //	Clear the buffer used for communication

            close(new_socket);

            exit(EXIT_SUCCESS);
        }

        // Parent process
        close(new_socket);
        wait(NULL);
    }

    return 0;
}
