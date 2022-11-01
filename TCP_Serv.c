#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
    int sockfd, newsockfd, clilen, n;
    char recvline[1000], sendline[1000];
    struct sockaddr_in servaddr, cliaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    while (1) {
        clilen = sizeof(cliaddr);
        if ((newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        while ((n = read(newsockfd, recvline, 999)) > 0) {
            printf("Recieved message => %s", recvline);
            printf("Sending message => ");
            fflush(stdin);
            fgets(sendline, 1000, stdin);
            if ((n = write(newsockfd, sendline, strlen(sendline) + 1)) < 0) {
                perror(NULL);
                close(sockfd);
                close(newsockfd);
                exit(1);
            }
        }

        if (n < 0) {
            perror(NULL);
            close(sockfd);
            close(newsockfd);
            exit(1);
        }

        close(newsockfd);

    }
}