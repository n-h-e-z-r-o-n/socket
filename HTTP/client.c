#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

  // Get the address and port of the web server
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints)); //  initialize the hints structure with zeros
  hints.ai_family = AF_INET;  // configured  IPv4 address (AF_INET) and a TCP socket
  hints.ai_socktype = SOCK_STREAM; //configured to  TCP socket
  getaddrinfo(argv[1], "80", &hints, &res); // obtain the address information for the web server specified in argv[1]

  // Create a socket
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // Connect to the web server
  connect(sockfd, res->ai_addr, res->ai_addrlen); //  establishing a TCP connection with the web server.

  // Send the HTTP request
  char http_request[1024];
  snprintf(http_request, sizeof(http_request), "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", argv[1]); //  formating the GET request with the desired URL
  send(sockfd, http_request, strlen(http_request), 0); //sending  HTTP request through the established socket connection.


  // Receive and print the HTTP response
  printf("\n-------- Receive and print the HTTP response --------------\n");
  char buffer[1024]; // used to store the HTTP request
  ssize_t bytes_received;
  //recv function to read data from the socket into the buffer
  while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
    buffer[bytes_received] = '\0'; // null-terminating
    printf("%s", buffer);
  }


  close(sockfd); // Close the socket


  freeaddrinfo(res);//freeing memory allocated for the address information
  return 0;
}
