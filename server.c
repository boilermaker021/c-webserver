#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *handle_request(void *ptr) {
  int client_fd = *((int *)ptr);
  char buf[2048] = {0};
  recv(client_fd, buf, sizeof(buf), 0);
  printf("%s\n", buf);
  
  char header[1024] = {0};
  sscanf(buf, "%63[^\n]", header);
  char url[512] = {0};
  sscanf(header, "GET %511[^ ] HTTP", url);

  char response[2048] = {0};
  sprintf(response, "HTTP/1.1 200 OK\nContent-Length:1024\r\n\r\n <h1>You are visiting %s</h1>", url);
  send(client_fd, response, sizeof(response), 0);
  
}


int main() {
  int status;
  struct sockaddr_in address;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(5902);
  address.sin_family = AF_INET;


  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Socket creation error");
  }

  status = bind(sockfd, (struct sockaddr *)&address, sizeof(address));

  if (status < 0) {
    printf("bind error");
  }

  status = listen(sockfd, 4);
  if (status < 0) {
    printf("listen error");
  }
  pthread_t *thread_array = malloc(sizeof(pthread_t) * 4);
  unsigned int thread_counter = 0;
  unsigned int thread_array_size = 4;
  int address_size = sizeof(address);
  while (1) {
    int new_fd = accept(sockfd, (struct sockaddr *)&address, &address_size);
    if (thread_counter == (thread_array_size - 1)) {
      pthread_t *new_array = malloc(sizeof(pthread_t) * thread_array_size * 2);
      memcpy(new_array, thread_array, sizeof(pthread_t) * thread_array_size);
      thread_array_size *= 2;
      free(thread_array);
      thread_array = new_array;
    }
    pthread_create(thread_array + thread_counter, NULL, handle_request, (void *)&new_fd);
    thread_counter++;
  }

  close(sockfd);
}

