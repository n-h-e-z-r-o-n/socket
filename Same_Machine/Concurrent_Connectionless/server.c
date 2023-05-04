//Concurrent connectionless  server (same machine)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define PORT 8080
char buffer[1024];

void covert_to_text_file() {
    // Open JSON file for reading
    FILE *json_file = fopen("user_data.json", "r");
    if (json_file == NULL) {
        printf("Error: Could not open JSON file.\n");
    }

    // Open text file for writing
    FILE *text_file = fopen("USER_INFO.txt", "w");
    if (text_file == NULL) {
        printf("Error: Could not open text file.\n");
    }

    // Read in JSON file line by line and write to text file
    char buffer[1024];
    while (fgets(buffer, 1024, json_file)) {
        fputs(buffer, text_file);
    }

    // Close files
    fclose(json_file);
    fclose(text_file);
}

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
    covert_to_text_file();
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

char  *handle_cleint_data(char client_data[]){
  char* substrings[3];
  int  feedback;
  char data_recived[200];

  splitStringByComma(client_data, substrings, 3);
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
    char *response = "Data Uploded Successfuly to the file";
    return response;


  }else if (feedback == 1){
    printf("\nERROR: Duplicated seial number Error \n");
    char *response = " Duplicated seial-number error: Serial Number you provided exists in the file system";
    return response;

  }else if (feedback == 2){
    printf("\nERROR: Duplicate Registration number \n");
    char *response = "Duplicated Reg-number error: Registration Number you provided exist in the file";
    return response;

  }else if (feedback == 3){
    printf("\nERROR: The serial Number and Registration Number provided by client are already in the system FILE (Dublication Error)\n");
    char *response = "\nERROR: The serial-Number and Reg-Number you provide are already in the File system in the server)\n";
    return response;
  } else {
      printf("\nERROR: Unknown Error status from client infomation!\n");
    char *response = "ERROR: Unknown Error status : Try Again\n";
    return response;
  }
}

void *handle_client(void *arg) {
    struct sockaddr_in address = *(struct sockaddr_in*)arg;
    int addrlen = sizeof(address);
    char *server_massege;

    // Create a new socket file descriptor for this thread
    int new_socket;
    if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_massege = handle_cleint_data(buffer);

    // Send data to client
    sendto(new_socket, server_massege, strlen(server_massege), MSG_CONFIRM, (const struct sockaddr *) &address, addrlen);
    printf("\n --------------------------------------------------------------------------------------------------------\n");
    // Close socket
    close(new_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Bind the socket with the server address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("\n ==================================== Concurrent-Connectionless Server ==================================\n");


    printf("Server listening on port %d...\n", PORT);

    pthread_t thread_id;

    while(1) {
        // Accept client connection
        int len = recvfrom(server_fd, buffer, 1024,  MSG_WAITALL, ( struct sockaddr *) &address,  &addrlen);
        buffer[len] = '\0';
        printf("\n --------------------------------------------------------------------------------------------------------\n");
        printf("Received message from client 1: %s\n", buffer);

        // Create a new thread to handle the client request
        if(pthread_create(&thread_id, NULL, handle_client, (void*)&address) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(thread_id);
    }
    return 0;
}
