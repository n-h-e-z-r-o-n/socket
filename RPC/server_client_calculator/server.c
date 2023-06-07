#include "hezron.h"
#include <stdio.h> 
#include <string.h>


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
                printf("plus\n");
                num1 += num2;
                printf(" %f \n\n", num2);
                break;
            case '-':
                printf("minus\n");
                num1 -= num2;
                break;
            case '*':
                printf("multiplied by\n");
                num1 *= num2;
                break;
            case '/':
                printf("divid by\n");
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
    printf("\nAnswer :  %s\n", result);
    return result;
}

char **proc_2_1_svc(int argp1, char *argp2, struct svc_req *rqstp) // procedure 2 accepts an integer and a string and returns a float.
{
    static char *output;
    output = (char *)malloc(1500);
    
    printf("\n=========================================================== %s\n", argp2);
    
    printf("\n\tCalculation Query: %s\n", argp2);
    
    char* answer =calculator(argp2);
    
    sprintf(output, "%s", answer);
        
    
    return &output;
}





