#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define PORT 12345
#define MAX_JSON_SIZE 1000

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
        fread(json, sizeof(char), MAX_JSON_SIZE, jsonFile);
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
int main() {
    int  feedback;
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];
    char json[20000];


    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error in socket. Exiting...");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error in bind. Exiting...");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 10);
    printf("Server is listening for incoming connections...\n");

    while (1) {
        client_len = sizeof(client_addr);
        // Accept incoming connections
        new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (new_sockfd == -1) {
            perror("Error in accept. Exiting...");
            exit(1);
        }
        printf(" \n ------------------------------------------------------------------------------------------------------- ");
        printf("\n\tConnection client ip : %s\n", inet_ntoa(client_addr.sin_addr));
        // --------------------------------------------  Read data from client -------------------------------------------------------
        int n = read(new_sockfd, buffer, sizeof(buffer));
        if (n < 0) {
            perror("Failed to read");
            exit(EXIT_FAILURE);
        }

        // Send  data to client
        //strcpy(buffer, "Hello from server!");
        //send(new_sockfd, buffer, strlen(buffer), 0);
        //printf("Data sent to client: %s\n", buffer);

        // receive data from client
        recv(new_sockfd, buffer, 1024, 0);
        printf("Data received from client: %s\n", buffer);

        char* substrings[3];
        splitStringByComma(buffer, substrings, 3);

        printf("\n\t1. Serial Number        : %s",substrings[0]);
        printf("\n\t2. Registration Number  : %s",substrings[1]);
        printf("\n\t3. Client Name          : %s",substrings[2]);
        printf("\n     ");


        feedback = dublicate_feed(substrings[0], substrings[1]);
        printf("feedback %d", feedback);


        if (feedback == 0){
            // Format input as JSON string
            sprintf(json, "{ \"serial Number \": \"%s\", \"Reg No \": \"%s\", \"Name \": \"%s\"}", substrings[0], substrings[1], substrings[2]);
            write_to_file(json);
            printf("\nSTATUS: Client Data added it FILE SYSYTEM\n");
            // Write response to client
            const char *response = "Data Uploded Successfuly to the file";
            write(new_sockfd, response, strlen(response));
            send(new_sockfd, response, strlen(response), 0);
            printf("\n\n-------------- --------------------------------------------\n");
            close(new_sockfd); // Close the new socket

        }else if (feedback == 1){
            printf("\nERROR: Duplicated seial number Error \n");
            const char *response = " Duplicated seial-number error: Serial Number you provided exists in the file system";
            write(new_sockfd, response, strlen(response));
            send(new_sockfd, response, strlen(response), 0);
            printf("\n\n-------------- --------------------------------------------\n");
            close(new_sockfd); // Close the new socket

        }else if (feedback == 2){
            printf("\nERROR: Duplicate Registration number \n");
            const char *response = "Duplicated Reg-number error: Registration Number you provided exist in the file";
            //write(new_sockfd, response, strlen(response));
            send(new_sockfd, response, strlen(response), 0);
            printf("\n\n-------------- --------------------------------------------\n");
            close(new_sockfd); // Close the new socket

        }else if (feedback == 3){
            printf("\nERROR: The serial Number and Registration Number provided by client are already in the system FILE (Dublication Error)\n");
            const char *response = "\nERROR: The serial-Number and Reg-Number you provide are already in the File system in the server)\n";
            write(new_sockfd, response, strlen(response));
            printf("\n\n-------------- --------------------------------------------\n");
            close(new_sockfd); // Close the new socket

        } else {
            printf("\nERROR: Unknown Error status from client infomation!\n");
            const char *response = "ERROR: Unknown Error status : Try Again\n";
            printf("\n\n-------------- --------------------------------------------\n");
            write(new_sockfd, response, strlen(response));
            close(new_sockfd); // Close the new socket
        }
        close(new_sockfd);   // Close the new socket
    }

    // Close the listening socket
    close(sockfd);

    return 0;
}
