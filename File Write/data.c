#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
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
               //printf("Not Data found \n");
             	return false;
        } else {
	        //printf("Value Found \n");
                return true;
              }
        }
}
int dublicate_feed(char s[], char r[]){

	bool serial_results;
	bool reg_no_results; 
	int status = 0;

	
	char ser_no[] = "\"serial Number \": \"";
	strcat(ser_no, s); // Concatenate 
	serial_results = search_Duplicates(ser_no);
	
	char reg_no[] = "\"Reg No \": \"";
	strcat(reg_no, r); // Concatenate
	reg_no_results = search_Duplicates(reg_no);
        
	if (serial_results){
	  	 //printf("\n Duplicated seial number \n");
	  	 status = 1;	 
	   }

	if (reg_no_results){
	  	 //printf("\n Duplicated Registration number \n");
	  	 if (status == 1){
		  	 status = 3;
	  	 } else {  	 
		  	 status = 2;
	  	 } 
	   }
	return status;   
	   
}




void write_to_file(char New_json_Data[]){

    bool fileExists = false;
    // Check if file exists
    FILE *fptr = fopen("user_data.json", "r");
    if(fptr != NULL) {
        fileExists = true;
        fclose(fptr);
    }

    // Open file for writing
    fptr = fopen("user_data.json", fileExists ? "a" : "w");
    if(fptr == NULL) {
        printf("Error opening file For Data Write\n");
        
    } else{

    // Write JSON string to file
    if(fileExists) {
        fprintf(fptr, ",\n%s", New_json_Data);
    } else {
        fprintf(fptr, "[\n%s", New_json_Data);
    }

    // Close the file
    fclose(fptr);
    printf("\n User information stored in JSON file successfully!\n");


}}

int main() {
    char serai_Entry[50];
    char  Reg_No_Entry[50];
    char Name_Entry[50];
    char json[20000];
    int  feedback;
    
    
    


    // Prompt user for name and email
    printf("\n");
    printf("\nFill In Your Details -------(avoid spaces) \n");
    printf("\n\t1. Enter Serail Number :  ");
    scanf("%s", serai_Entry);

    printf("\n\t2. Enter Reg_Number    :  ");
    scanf("%s", Reg_No_Entry);

    printf("\n\t3. Enter Name     :  ");
    scanf("%s", Name_Entry);

    
    printf("\nseial number  %s\n", serai_Entry);
    printf("\nseial number  %s\n", Reg_No_Entry);
    printf("\nseial number  %s\n", Name_Entry);
    
    // Remove newline characters from input
    serai_Entry[strcspn(serai_Entry, "\n")] = 0;
    Reg_No_Entry[strcspn(Reg_No_Entry, "\n")] = 0;
    Name_Entry[strcspn(Name_Entry, "\n")] = 0;
    
    
    feedback = dublicate_feed(serai_Entry, Reg_No_Entry);
    
    if (feedback == 0){
	    // Format input as JSON string
	    sprintf(json, "{ \"serial Number \": \"%s\", \"Reg No \": \"%s\", \"Name \": \"%s\"}", serai_Entry, Reg_No_Entry, Name_Entry);
	    printf( "\n Data Captured \n serial Number : %s, Reg No : %s, Name : %s", serai_Entry, Reg_No_Entry, Name_Entry);
	    write_to_file(json);
    }else if (feedback == 1){
    		printf("\n Duplicated seial number \n");
    }else if (feedback == 2){
 		printf("\n Duplicated Registration number \n");    
    }else if (feedback == 3){
 		printf("\n The serial Number and Registration Number you provide are already in the system (Dublicate Error)\n");    
    } else {
            printf("\n Unknown Error !\n");  
    } 
    return 0;
}
