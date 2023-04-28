#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

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
    printf("\n User information stored in JSON file successfully!");
    //printf("\n------------------------------------------------------------\n");
	}
}

bool search_Duplicates(char search[]){
    FILE *jsonFile;
    char json[20000];
    char *value; // Value associated with the key
    // Open the JSON file in read mode
    jsonFile = fopen("user_data.json", "r");
    if (jsonFile == NULL) {
        printf("Failed to open the JSON file.\n");
        return false;
    } else {
        // Read the entire JSON file into a character array
        fread(json, sizeof(char), 20000, jsonFile);
        fclose(jsonFile);

        // Search for the key in the JSON data
        value = strstr(json, search);
        if (value == NULL) {
               //printf("\nNot Data found");
             	return false;
        } else {
	        //printf("\nValue Found");
                return true;
              }
        }
}

int dublicate_feed(char s[], char r[]){

	bool serial_results;
	bool reg_no_results;
	int status = 0;


	char ser_no[100] = "\"serial Number \": \"";
	strcat(ser_no, s); // Concatenate
	serial_results = search_Duplicates(ser_no);

	char reg_no[100] = "\"Reg No \": \"";
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

#define MAXLINE 1024
#define PORT 8080

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char json[20000];
    int  feedback;
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("\n======================================================================================================== \n");
    printf("SERVER STARTED LISTENING ON PORT %d...\n", PORT);
    printf("\n --------------------------------------------------------------------------------------------------------\n");

    while (1) {
        int len, n;

        len = sizeof(cliaddr);  // len is value/result parameter

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len); // The recvfrom function receives data from the client, and the number of bytes received is stored in the n variable
        buffer[n] = '\0'; //The buffer array is then null-terminated by adding a null character ('\0')

        char* substrings[3];
        splitStringByComma(buffer, substrings, 3);

        printf("\n\t1. Serial Number        : %s",substrings[0]);
        printf("\n\t2. Registration Number  : %s",substrings[1]);
        printf("\n\t3. Client Name          : %s",substrings[2]);
        printf("\n     ");

        feedback = dublicate_feed(substrings[0], substrings[1]);


      	if (feedback == 0){
      		// Format input as JSON string
      		write_to_file(json);
          sprintf(json, "{ \"serial Number \": \"%s\", \"Reg No \": \"%s\", \"Name \": \"%s\"}", substrings[0], substrings[1], substrings[2]);
      		printf("\nSTATUS: Client Data added it FILE SYSYTEM\n");
      		// Write response to client
      		const char *response = "Data Uploded Successfuly to the file";

          sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); // in Connectionless we use sendto insead of

          printf("\n --------------------------------------------------------------------------------------------------------\n");

      	}else if (feedback == 1){
      		printf("\nERROR: Duplicated seial number Error \n");
      		const char *response = " Duplicated seial-number error: Serial Number you provided exists in the file system";
          sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); // in Connectionless we use sendto insead of

          printf("\n --------------------------------------------------------------------------------------------------------\n");

      	}else if (feedback == 2){
      		printf("\nERROR: Duplicate Registration number \n");
      		const char *response = "Duplicated Reg-number error: Registration Number you provided exist in the file";
          sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); // in Connectionless we use sendto insead of

          printf("\n --------------------------------------------------------------------------------------------------------\n");

      	}else if (feedback == 3){
      		printf("\nERROR: The serial Number and Registration Number provided by client are already in the system FILE (Dublication Error)\n");
      		const char *response = "\nERROR: The serial-Number and Reg-Number you provide are already in the File system in the server)\n";
          sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); // in Connectionless we use sendto insead of

          printf("\n --------------------------------------------------------------------------------------------------------\n");

      	} else {
      	    printf("\nERROR: Unknown Error status from client infomation!\n");
      		const char *response = "ERROR: Unknown Error status : Try Again\n";
          printf("\n --------------------------------------------------------------------------------------------------------\n");
          sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len); // in Connectionless we use sendto insead of
      	}
    }

    close(sockfd);
    return 0;
}
