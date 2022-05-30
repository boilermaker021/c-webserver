#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>



int main() {
  int client_sockfd = socket(AF_INET, SOCK_STREAM, 0);


  struct sockaddr_in address;
  inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
  address.sin_family = AF_INET;
  address.sin_port = htons(5000);

  printf("connection: %d\n", connect(client_sockfd, (struct sockaddr *)&address, sizeof(address)));
  
  char buf[100] = {0};

  recv(client_sockfd, buf, sizeof(buf), 0);
  printf("%s\n", buf);
}