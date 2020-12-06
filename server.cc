#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "common.h"
using namespace std;

#define MAXLINE 80
int deck[52];
int user_cnt;

void *thread_func(void *arg) {
	int n;
	char buf[MAXLINE];

	int connfd = *((int*)arg);
	pthread_detach(pthread_self());
	free(arg);
	
	int user=user_cnt;
	user_cnt++;
	cout<<"your number : "<<user<<endl;

	write(connfd,&deck[user],sizeof(int));
	write(connfd,&deck[user+1],sizeof(int));
	
	close(connfd);
	return NULL;
}

int main (int argc, char *argv[]) {

		Shuffle(deck);
    int n, listenfd, connfd, caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port = atoi(argv[1]);
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }
    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);   
    if (bind(listenfd, (struct sockaddr *)&saddr, 
             sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }
    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    caddrlen = sizeof(caddr);
		int *connfdp;
		pthread_t tid;

    while (1) {
	
	connfdp = (int *)malloc(sizeof(int));

        if ((*connfdp = accept(listenfd, (struct sockaddr *)&caddr,
                            (socklen_t *)&caddrlen)) < 0) {
           printf ("accept() failed.\n");
	   free(connfdp);
           continue;
       }

	pthread_create(&tid, NULL, thread_func, connfdp);
			 
    }
}