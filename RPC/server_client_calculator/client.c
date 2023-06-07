#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hezron.h"




char **proc_2_1(int, char *, CLIENT *);



int main()
{
    char mathematical_question[1024];
    int num = 0;
    printf("Client Has started\n\n");
    CLIENT *cl;
    
    while (1){
	    printf("\n\n\tEnter a mathematical expression :  ");
	    fgets(mathematical_question, 1024, stdin);
	    
	    cl = clnt_create("localhost", HEZRON, HEZRON_1, "tcp");

	    
	    char *procTwoResult = *proc_2_1(num, mathematical_question, cl);
	    
	    printf("\tAnswer : %s\n\n", procTwoResult);

	    memset(mathematical_question, 0, sizeof(mathematical_question));
	    
	    clnt_destroy(cl); //exit
    }
    return 0;
}
