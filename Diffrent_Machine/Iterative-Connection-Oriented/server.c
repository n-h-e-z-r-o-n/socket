//Iterative-Connection-Oriented
// @n_h_e_z_r_o_n_
// (iterative connection-oriented server)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <arpa/inet.h>


#define PORT 8080
// Function to split a string by comma delimiter and store substrings in an array
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

int main() {
    int sockfd, new_sockfd;
    int  feedback;
    char json[20000];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    // -----------------------------   Create a socket ------------------------------------------------------------------------------
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create a socket
    if (sockfd < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET; // specifies that this is an IPv4 address
    server_addr.sin_addr.s_addr = INADDR_ANY; //  specifies that the server will listen on all available network interfaces
    server_addr.sin_port = htons(PORT); // specifies the port number that the server will listen on

    // --------------------------------- Bind the socket to the specified IP address and port ----------------------------------------
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { // Bind to well-known address
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    // ------------------------------- Listen for incoming connections ----------------------------------------------------------------
    if (listen(sockfd, 5) < 0) { // Place socket in passive mode using listen( ) with queue length of 5
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }
    printf("\n ============================================================================================================");
    printf("\n SERVER IS LISTENING ON PORT: %d\n", PORT);

    while (1) {
        client_len = sizeof(client_addr);
       // A------------------------------- ccept a new connection --------------------------------------------------------------------
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);// accept() is used to  allow the server to receive data from any client that has establishing a connection
        if (new_sockfd < 0) {
            perror("Failed to accept");
            exit(EXIT_FAILURE);
        }
        printf("\n --------------------------------------------------------------------------------------------------------\n");
        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // --------------------------------------------  Read data from client -------------------------------------------------------
        int n = read(new_sockfd, buffer, sizeof(buffer));
        if (n < 0) {
            perror("Failed to read datd from client");
            exit(EXIT_FAILURE);
        }

        printf("\nReceived  client data");

        char* substrings[3];
        splitStringByComma(buffer, substrings, 3);

        printf("\n\t1. Serial Number        : %s",substrings[0]);
        printf("\n\t2. Registration Number  : %s",substrings[1]);
        printf("\n\t3. Client Name          : %s",substrings[2]);
        printf("\n     ");



        feedback = dublicate_feed(substrings[0], substrings[1]);


      	if (feedback == 0){
      		// Format input as JSON string
      		sprintf(json, "{ \"serial Number \": \"%s\", \"Reg No \": \"%s\", \"Name \": \"%s\"}", substrings[0], substrings[1], substrings[2]);
      		write_to_file(json);
      		printf("\nSTATUS: Client Data added it FILE SYSYTEM\n");
      		// Write response to client
      		const char *response = "Data Uploded Successfuly to the file";
      		write(new_sockfd, response, strlen(response));
          printf("\n --------------------------------------------------------------------------------------------------------\n");
      		close(new_sockfd); // Close connection and return to accept()

      	}else if (feedback == 1){
      		printf("\nERROR: Duplicated seial number Error \n");
      		const char *response = " Duplicated seial-number error: Serial Number you provided exists in the file system";
      		write(new_sockfd, response, strlen(response));
          printf("\n --------------------------------------------------------------------------------------------------------\n");
      		close(new_sockfd); // Close connection and return to accept()

      	}else if (feedback == 2){
      		printf("\nERROR: Duplicate Registration number \n");
      		const char *response = "Duplicated Reg-number error: Registration Number you provided exist in the file";
      		write(new_sockfd, response, strlen(response));
          printf("\n --------------------------------------------------------------------------------------------------------\n");
      		close(new_sockfd); // Close connection and return to accept()

      	}else if (feedback == 3){
      		printf("\nERROR: The serial Number and Registration Number provided by client are already in the system FILE (Dublication Error)\n");
      		const char *response = "\nERROR: The serial-Number and Reg-Number you provide are already in the File system in the server)\n";
      		write(new_sockfd, response, strlen(response));
          printf("\n --------------------------------------------------------------------------------------------------------\n");
      		close(new_sockfd); // Close connection and return to accept()

      	} else {
      	    printf("\nERROR: Unknown Error status from client infomation!\n");
      		const char *response = "ERROR: Unknown Error status : Try Again\n";
          printf("\n --------------------------------------------------------------------------------------------------------\n");
      		write(new_sockfd, response, strlen(response));
      		close(new_sockfd); // Close connection and return to accept()
      	}
    }
    printf("\n ============================================================================================================");
    close(sockfd); // Close the server socket
    return 0;
}
