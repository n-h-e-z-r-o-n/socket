#include "hezron.h"
#include <stdio.h> 
#include <string.h>


int splitStringByComma(char* str, char** substrings, int maxSubstrings) {
    int numSubstrings = 0;
    char* token;
    token = strtok(str, ",");
    while (token != NULL && numSubstrings < maxSubstrings) {
        substrings[numSubstrings++] = token;
        token = strtok(NULL, ",");
    }
    return numSubstrings;
}


int checkDuplicate(char serialNumber[], char regNumber[]) {
    FILE *file;
    char line[100];
    char tempSerialNumber[100], tempRegNumber[100];
    file = fopen("data.txt", "r"); // open file in read mode
    if (file == NULL) {
        return 0; // return 0 if file cannot be opened
    }
    while (fgets(line, 100, file) != NULL) {
        sscanf(line, "%s %s", tempSerialNumber, tempRegNumber); // read serial number and registration number from file
        if (strcmp(tempSerialNumber, serialNumber) == 0 || strcmp(tempRegNumber, regNumber) == 0) {
            fclose(file);

            return 1; // return 1 if either serial number or registration number is a duplicate
        }
    }
    fclose(file);
    return 0; // return 0 if neither serial number nor registration number is a duplicate
}

char* write_to_text_file(char serial_n[], char reg_n[], char name[]){
     char* status = malloc(sizeof(char) * 110);
     int feed = checkDuplicate(serial_n, reg_n);
     if (feed){
         printf("\nDuplicate data entered: ");
         strcpy(status, "Duplicate Error: ither serial number or registration number is a duplicate");
     } else {
       FILE *file;
       file = fopen("data.txt", "a"); // open file in append mode
       fprintf(file, "%s %s %s\n", serial_n, reg_n, name); // write details to file
       fclose(file);
       printf("Details saved to file.\n");
       strcpy(status, "Details saved to file");
     }
     return status;
}

char **proc_2_1_svc(int argp1, char *argp2, struct svc_req *rqstp) // procedure 2 accepts an integer and a string and returns a float.
{
	static char *output;
	output = (char *)malloc(15000);
	
	printf("\n ======================================================================\n");
	
	
	char* substrings[3];

	splitStringByComma(argp2, substrings, 3);

	printf("\n   client data Received  ");
	printf("\n\t1. Serial Number        : %s",substrings[0]);
	printf("\n\t2. Registration Number  : %s",substrings[1]);
	printf("\n\t3. Client Name          : %s",substrings[2]);
	printf("\n\n     ");
	
	char* status = write_to_text_file(substrings[0], substrings[1], substrings[2] );
	
	sprintf(output, "%s", status);
	
	
	return &output;
}


char **proc_3_1_svc(int argp1, char *argp2, struct svc_req *rqstp) // procedure 2 accepts an integer and a string and returns a float.
{
    static char *output;
    output = (char *)malloc(1500);
    int n = strlen(output) - 1;
    printf("\nPROCEDURE 2 arguments passed to procedure 2  %s\n", argp2);
    sprintf(output, "this is s string %s", argp2);
    printf("%s", output);
    //output[n] = '\0';
    printf("%s", output);
    
    
    return &output;
}





