#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

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


void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    char data_recived[200];
    int bytes_received;
    int  feedback;

    // Receive data from client and send response
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {

        char* substrings[3];
        splitStringByComma(buffer, substrings, 3);
        printf("\n   client data Received  ");
        printf("\n\t1. Serial Number        : %s",substrings[0]);
        printf("\n\t2. Registration Number  : %s",substrings[1]);
        printf("\n\t3. Client Name          : %s",substrings[2]);
        printf("\n\n     ");

        feedback = dublicate_feed(substrings[0], substrings[1]);

        if (feedback == 0){
          // Format input as JSON string
          sprintf(data_recived, "{ \"serial Number \": \"%s\", \"Reg No \": \"%s\", \"Name \": \"%s\"}", substrings[0], substrings[1], substrings[2]);
          write_to_file(data_recived);
          printf("\nSTATUS: Client Data added it FILE SYSYTEM\n");
          // Write response to client
          const char *response = "Data Uploded Successfuly to the file";
          send(client_socket, response, strlen(response), 0);

        }else if (feedback == 1){
          printf("\nERROR: Duplicated seial number Error \n");
          const char *response = " Duplicated seial-number error: Serial Number you provided exists in the file system";
          send(client_socket, response, strlen(response), 0);
        }else if (feedback == 2){
          printf("\nERROR: Duplicate Registration number \n");
          const char *response = "Duplicated Reg-number error: Registration Number you provided exist in the file";
          send(client_socket, response, strlen(response), 0);
        }else if (feedback == 3){
          printf("\nERROR: The serial Number and Registration Number provided by client are already in the system FILE (Dublication Error)\n");
          const char *response = "\nERROR: The serial-Number and Reg-Number you provide are already in the File system in the server)\n";
          send(client_socket, response, strlen(response), 0);
        } else {
            printf("\nERROR: Unknown Error status from client infomation!\n");
          const char *response = "ERROR: Unknown Error status : Try Again\n";
          send(client_socket, response, strlen(response), 0);
        }

        //send(client_socket, "Message received", strlen("Message received"), 0);
        memset(buffer, 0, BUFFER_SIZE); //  clears out any data that may have been in the buffer previously.

    }

    // Close client socket
    printf("Client disconnected\n");
    printf("\n --------------------------------------------------------------------------------------------------------\n");

    close(client_socket);

    return NULL;
}


int main(int argc, char **argv) {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_address_length = sizeof(client_address);
    pthread_t threads[MAX_CLIENTS];
    int thread_index = 0;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    // Bind socket to server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(server_socket, 5) == -1) { //  Put socket in passive mode
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }
    printf("\n ================================= Iterative, Connectionless Server ==================================== \n");


    // Accept incoming client connections and create threads to handle them
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length))) {
        printf("\n --------------------------------------------------------------------------------------------------------\n");
        printf("Client connected:  %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port)); // Accept a new connection
        pthread_create(&threads[thread_index], NULL, client_handler, (void *)&client_socket); // Create thread
        thread_index++; // used to keep track of the number of threads created. incremented each time a new thread is created
    }

    close(server_socket); // Close server socket

    return 0;
}
