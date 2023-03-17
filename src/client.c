/*
 * TCP klient
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLEN 255

int main(int argc, char **argv) {
	int    sockfd = 0, ret_code = 0, port = 0;
	char   buffer[MAXLEN];
	char *p_end;
	struct sockaddr_in server;
	in_addr_t server_address;

	if (argc != 4) {
		printf("Usage: %s <IP v4 address> <PORT> <String>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int msg_len = strlen(argv[3]);
	if (msg_len > MAXLEN) {
		printf("Message for send is too large. Expected %d. Current: %d\n", MAXLEN, msg_len);
		exit(EXIT_FAILURE);
	}

	// Parse port parameter
	port = strtol(argv[2], &p_end, 10);

	// Convert dotted quad notation to a sockaddr_in address structure
	if ((server_address = inet_addr(argv[1])) < 0) {
		printf("Invalid address '%s'!\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	// Create a socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed !\n");
		exit(EXIT_FAILURE);
	}

	printf("Created socket with id:%d\n", sockfd);

	// Fill in the server information
	server.sin_family      = AF_INET;     // Address Family Internet IPv4
	server.sin_port        = htons(port);        // Server's Port
	server.sin_addr.s_addr = server_address;     // Server's Address

	ret_code = connect(sockfd, (struct sockaddr_in *)&server, sizeof(server));
	if (ret_code) {
		printf("Can not connect to server (ret_code: %d).\n", ret_code);
		exit(EXIT_FAILURE);
	}
	int n = 0, last_index = strlen(argv[3]);
	memset(&buffer, '\0', MAXLEN);
	strncpy(buffer, argv[3], last_index);
	buffer[last_index] = '\n';
	printf("Sending to server(ret_code: %d): %s (len: %d)\n", sockfd, buffer, last_index);
	n = write(sockfd, &buffer, MAXLEN);
	printf("Sended chars: %d\n", n);
	printf("Waiting for response...\n");
	n = read(sockfd, &buffer, MAXLEN); 
	printf("Recieved chars: %d\n", n);
	printf("Server [%s:%d] replied: %s\n", inet_ntoa(server.sin_addr), port, buffer);
	printf("Close connection.\n");
	close(sockfd);
	return EXIT_SUCCESS;
}

/*
 * EOF
 */