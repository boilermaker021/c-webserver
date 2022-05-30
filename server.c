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
  char buf[100] = {0};
  sprintf(buf, "Your file descriptor is %d", client_fd);
  send(client_fd, buf, sizeof(buf), 0);
  while (1) {
    int i = 1;
  }
}


int main() {
  int status;
  struct sockaddr_in address;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(5000);
  address.sin_family = AF_INET;


  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bind(sockfd, (struct sockaddr *)&address, sizeof(address));

  listen(sockfd, 4);
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

