#include <assert.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	char resp[MAX+20]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		n = read(sockfd, buff, sizeof(buff)); 
		if(n < 1) {
			puts("Read failed");
			break;
		}

		// print buffer which contains the client contents 
		printf("From client: %s", buff); 
		bzero(resp, MAX); 
		
		// copy server message in the buffer 
		sprintf(resp, "S: '%s'", buff);
		puts(resp);

		//while ((buff[n++] = getchar()) != '\n') 
		//	; 

		// and send that buffer to client 
		printf("Attempt write of %ld bytes: %s", strlen(resp), resp);
		n = write(sockfd, resp, strlen(resp)); 
		puts("Write completed");
		if(n == -1) {
			puts("Write failed");
		}


		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

void main_conn() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 


	// make it reusable
	const int       optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	int rtn = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
	assert(rtn == 0);   /* this is optional */

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
	//free(sockfd);
	puts("Closing connection");
} 

int main() 
{
loop:
	puts("Starting connection");
	main_conn();
	puts("Connecting stopped");
	goto loop;

	return 0;
}

