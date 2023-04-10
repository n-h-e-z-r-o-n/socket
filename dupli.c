#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_JSON_SIZE 1000 // Maximum size of JSON data

bool search_Duplicates(char search[]){
    FILE *jsonFile;
    char json[10000];
    char *value; // Value associated with the key
    // Open the JSON file in read mode
    jsonFile = fopen("user_data.json", "r");
    if (jsonFile == NULL) {
        printf("Failed to open the JSON file.\n");
        return false;
    } else {
        // Read the entire JSON file into a character array
        fread(json, sizeof(char), MAX_JSON_SIZE, jsonFile);
        fclose(jsonFile);

        // Search for the key in the JSON data
        value = strstr(json, search);
        if (value == NULL) {
               printf("Not Data found \n");
             	return false;
        } else {
	        printf("Value Found \n");
                return true;
              }
        }
}

int dublicate_feed(char s[], char r[]){

	bool serial_results;
	bool reg_no_results; 

	char ser_no[] = "\"serial Number\": \"";
	char reg_no[] = "\"Reg No \": \"";
	
	int status = 0;
	
	strcat(ser_no, s); // Concatenate str2 to str1
	strcat(reg_no, r); // Concatenate str2 to str1
	
	printf("\n %s \n", ser_no);
	printf("\n %s \n", reg_no);
		
	serial_results = search_Duplicates(ser_no);
	reg_no_results = search_Duplicates(reg_no);

	if (serial_results){
	  	 printf("\n Duplicated seial number \n");
	  	 status = 1;
	  	 
	   }

	if (reg_no_results){
	  	 printf("\n Duplicated Registration number \n");
	  	 if (status == 1){
		  	 status = 3;
	  	 } else {  	 
		  	 status = 2;
	  	 } 
	   }
	return status;   
	   
}




int main() {

	char keyToSearch[50]; // Key to search in the JSON data
	char Reg[50]; // Key to search in the JSON data
	printf("Enter the key to search in the JSON data: ");
	scanf("%s", keyToSearch);

	int m = dublicate_feed("Hezron", "er");
	printf("m : %d \n", m);
}



















