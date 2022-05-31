#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "files.h"


#define PORT 5906


void *handle_request(void *ptr) {
  int client_fd = *((int *)ptr);
  char buf[2048] = {0};
  recv(client_fd, buf, sizeof(buf), 0);
  
  char header[1024] = {0};
  sscanf(buf, "%1023[^\n]", header);
  char url[512] = {0};
  sscanf(header, "GET /%511[^ ] HTTP", url);


  char *contents = NULL;
  int length = getfileasstring(url, &contents);
  if (length < 0) { 
    //file doesn't exist - HTTP error 404
  }
  else {
    
  }

  char response[2048] = {0};
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", length, contents);
  send(client_fd, response, sizeof(response), 0);
  close(client_fd);
  
}


int main() {
  int status;
  struct sockaddr_in address;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  address.sin_family = AF_INET;


  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Socket creation error: %s\n", strerror(errno));
  }

  status = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

  if (status == -1) {
    printf("Bind error: %s\n", strerror(errno));
  }

  status = listen(sockfd, 4);
  if (status < 0) {
    printf("Listen error: %s\n", strerror(errno));
  }

  pthread_t *thread_array = malloc(sizeof(pthread_t) * 4);
  unsigned int thread_counter = 0;
  unsigned int thread_array_size = 4;
  int address_size = sizeof(address);
  while (1) {
    int new_fd = accept(sockfd, (struct sockaddr *)&address, &address_size);

    //double array that holds pthreads when max amount is reached in current array
    if (thread_counter == (thread_array_size - 1)) {
      pthread_t *new_array = malloc(sizeof(pthread_t) * thread_array_size * 2);
      memcpy(new_array, thread_array, sizeof(pthread_t) * thread_array_size);
      thread_array_size *= 2;
      free(thread_array);
      thread_array = new_array;
    }

    //creates thread to handle each individial request - should switch to non-blocking sockets with thread pools later
    pthread_create(thread_array + thread_counter, NULL, handle_request, (void *)&new_fd);
    thread_counter++;
  }

  close(sockfd);
}

