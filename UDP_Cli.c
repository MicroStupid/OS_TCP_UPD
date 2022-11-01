#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
  int sockfd;
  int n, len;
  char sendline[1000], recivline[1000];

  struct sockaddr_un servaddr, cliaddr;

  if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
    perror(NULL);
    return 1;
  }

  memset(&cliaddr, 0, sizeof(cliaddr));
  cliaddr.sun_family = AF_UNIX;
  strcpy(cliaddr.sun_path, "AAAA");

  if(bind(sockfd, (struct sockaddr *) &cliaddr, SUN_LEN(&cliaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    return 1;
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sun_family = AF_UNIX;
  strcpy(servaddr.sun_path, "BBBB");

  printf("String => ");

  fgets(sendline, 1000, stdin);

  if(sendto(sockfd, sendline, strlen(sendline) + 1,
    0, (struct sockaddr *) &servaddr, SUN_LEN(&servaddr)) < 0) {
    perror(NULL);
    close(sockfd);
    return 1;
  }

  if((n = recvfrom(sockfd, recivline, 1000, 0,
    (struct sockaddr *) NULL, NULL)) < 0) {
    perror(NULL);
    close(sockfd);
    return 1;
  }

  recivline[n] = 0;
  printf("%s", recivline);
  close(sockfd);
  return 0;
}