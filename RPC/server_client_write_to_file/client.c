#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hezron.h"




char **proc_2_1(int, char *, CLIENT *);


char* user_detail_prompt() {  // Prompt user for their information --------------
      char* user_detail = malloc(sizeof(char) * 30); // allocate memory for string
      char serai_Entry[50];
      char Reg_No_Entry[50];
      char Name_Entry[50];
      printf("\n --------------------------------------------------------\n");
      printf("\n");
      printf("         Fill In Your Details\n");
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

      sprintf(user_detail, "%s,%s,%s", serai_Entry, Reg_No_Entry, Name_Entry); // Format input in to single string

      return user_detail;
}


int main()
{
    printf("Hezron Client Has started\n");
    CLIENT *cl;
    
    int num2 = 13;
    char num_string[40] = "Maureen,nelly,ann\0";
    
    char* your_details = user_detail_prompt();
    
    
    cl = clnt_create("localhost", HEZRON, HEZRON_1, "tcp");
    if (cl == NULL)     {
        printf("An Error has occured : Unable to to connet to the server. \n Check if the server is running or check your code\n");
        exit(1);
        }

    
    char *procTwoResult = *proc_2_1(num2, your_details, cl);
    printf("Server Feedback: %s\n", procTwoResult);

 

    
    clnt_destroy(cl); //exit
    return 0;
}
