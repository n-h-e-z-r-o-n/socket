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
    printf("\t calculating . . . \n\n");
    char op;
    float num1, num2;
    char *ptr = expr;
    num1 = strtof(ptr, &ptr); // Extract the first operand from the expression
    printf("\t %f \n", num1);
    while (*ptr != '\0') {
        op = *ptr++; // Extract the operator from the expression
        num2 = strtof(ptr, &ptr); // Extract the next operand from the expression
        // Perform the calculation based on the operator
        switch(op) {
            case '+':
                num1 += num2;
                printf("\t %f \n\n", num2);
                break;
            case '-':
                num1 -= num2;
                break;
            case '*':
                num1 *= num2;
                break;
            case '/':
                if(num2 == 0) {
                    printf("\tError: division by zero");
                    exit(1);
                }
                num1 /= num2;
                break;
        }
    }
    // result

    sprintf(result, "%.2f", num1);
    printf("\n\tresult:  %s\n\n", result);
    return result;
}


int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    printf("\n\n- creating server socket\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("- binding server address to Server Socket\n");

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    printf("- Listen for incoming connections from clients.\n");

    listen(server_fd, 5);

    printf("- Enter a continuous loop to handle client requests.\n");

    int count = 1;

    while (1) {
        printf("\n\tAccepting new client connection\n");
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);


        printf("\tCreate a child process  to handle the client request \n");
        int pid = fork();

        if (pid == 0) { // Child process
            close(server_fd);

            printf("\tReceiving mathematical expression query from the client \n");
            read(new_socket, buffer, 1024);

            printf("\tCalculation Query : %s\n", buffer);

            printf("\tPerform the calculation for the received expression\n");
            printf("\tSleeping Process %d\n",count);
            sleep(10);
            printf("\tcontinuous Process %d\n",count);

            //	Perform the calculation using the calculator function
            char* answer =calculator(buffer);

            printf("\tSend the answer back to the client\n");

            send(new_socket, answer, strlen(answer), 0);

            memset(buffer, 0, sizeof(buffer)); //	Clear the buffer used for communication

            printf("\tClose the connection with the client\n\n");

            close(new_socket);
            printf("Process %d End",count);

            exit(EXIT_SUCCESS);
        }
        count = count + 1 ;
    }

    return 0;
}
