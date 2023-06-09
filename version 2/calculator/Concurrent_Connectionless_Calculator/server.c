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
    printf("\t\t calculating . . . \n\n");
    char op;
    float num1, num2;
    char *ptr = expr;
    num1 = strtof(ptr, &ptr); // Extract the first operand from the expression
    printf("\t\t %f \n", num1);
    while (*ptr != '\0') {
        op = *ptr++; // Extract the operator from the expression
        num2 = strtof(ptr, &ptr); // Extract the next operand from the expression
        // Perform the calculation based on the operator
        switch(op) {
            case '+':
                num1 += num2;
                printf("\t\t%f \n\n", num2);
                break;
            case '-':
                num1 -= num2;
                break;
            case '*':
                num1 *= num2;
                break;
            case '/':
                if(num2 == 0) {
                    printf("\t\tError: division by zero");
                    exit(1);
                }
                num1 /= num2;
                break;
        }
    }
    // result

    sprintf(result, "%.2f", num1);
    printf("\n\t\tresult:  %s\n", result);
    return result;
}


int main() {
    int server_fd, len;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in server_addr, client_addr;

    printf("\n\n- Creating Server Socket\n");

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    printf("- Binding server  Server Socket to Server address\n");

    bind(server_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("- Enter a continuous loop to handle client requests\n");

    int count = 1;

    while (1) {

        printf("\n\n\tWaiting for client data...\n");
        len = sizeof(client_addr);
        memset(buffer, 0, sizeof(buffer));

        recvfrom(server_fd, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&client_addr, &len);
        printf("\t- Receiving the calculation query from the client\n");

        printf("\t- Fork a new process to handle  client request \n");
        int pid = fork();

        if (pid == 0) { // Child process
            printf("\tChild process created\n");
            printf("\tAnalyzing client's data\n");

            printf("\t\tCalculation Query: %s\n", buffer);

            char* answer =calculator(buffer);
            printf("Sleeping process %d\n",count);
            sleep(20);

            printf(" process %d continue\n",count);

            printf("\tSend the result back to the client\n", buffer);
            sendto(server_fd, answer, strlen(answer), 0, (struct sockaddr *)&client_addr, len);

            printf("\tClose the server socket and exit the child process.\n\n", buffer);
            printf(" process %d Ended\n",count);
            close(server_fd);
            exit(EXIT_SUCCESS);
        }
        count = count + 1;
    }

    return 0;
}
