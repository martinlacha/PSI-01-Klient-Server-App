/*
 * TCP multithreading server
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <threads.h>
#include <errno.h>

#define MAXLEN 255
#define USER_INPUT_LEN 64

void reverse_string(char *buffer) {
    int len = strlen(buffer);
    char *left = buffer;
    char *right = buffer + len - 2;
	//printf("Before reverse: %s (%d)\n", buffer, strlen(buffer));
    while (left < right) {
        char tmp = *left;
        *left = *right;
        *right = tmp;
        left++;
        right--;
    }
	//printf("After reverse: %s (%d)\n", buffer, strlen(buffer));
}

int main(int argc, char* argv[]) {
	int socket_desc = 0, client_sock = 0, port = 0, optval = 1;
	char buffer[MAXLEN], user_input[USER_INPUT_LEN];
	char *p_end;
	struct sockaddr_in server, client;

	if (argc != 2) {
		perror("Invalid count program arguments");
		perror("Usage: ./server <PORT>");
		exit(EXIT_FAILURE);
	}

	//Parse input parameter
	port = strtol(argv[1], &p_end, 10);

	// Creating socket file descriptor
	printf("Creating socket.\n");
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));

	// Set socket options
	setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	// Fill in the server's information
	server.sin_family      = AF_INET; 	  // Address Family Internet IPv4
	server.sin_addr.s_addr = INADDR_ANY;  // Listen on all host's IP addresses
	server.sin_port        = htons(port); // Server's Port

	// Bind the socket with the server address (and occupy the given port)
	if (bind(socket_desc, (const struct sockaddr *) &server, sizeof(server)) < 0)
	{
		perror("Bind failed.\n");
		exit(EXIT_FAILURE);
	}
	printf("Socket binded to port %d\n", port);

	// The message is stored into 'buffer', the clien't addres into 'client'
	if (listen(socket_desc, 1) < 0) {
		perror("Error while listen\n");
		exit(EXIT_FAILURE);
	}

	int n, buffer_len;
	socklen_t len = sizeof(client);
	printf("---------------------------Start server---------------------------\n");
	printf("Stop server using shortcut: Crtl + C\n");
	while (1) {
		printf("Wait for client...\n");
		client_sock = accept(socket_desc, (struct sockaddr*)&client, &len);
		if (client_sock < 0) {
			printf("Cant accept socket(client_sock:%d).\n", client_sock);
			printf("Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		printf("New client connected (id:%d).\n", client_sock);
		memset(&buffer, '\0', MAXLEN);
		n = read(client_sock, &buffer, MAXLEN);

		// Get lenght of message from first byte of message
		buffer_len = (int) buffer[0];
		memmove(buffer, buffer + 1, buffer_len);
		buffer[buffer_len] = '\0';

		printf("Income message: leght=%d text=%s\n", buffer_len, buffer);
		reverse_string(buffer);
		n = write(client_sock, &buffer, MAXLEN);
		printf("Response msg: %s\n", buffer);
		if(n < 0) {
			printf("Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		printf("Close client socket.\n");
		close(client_sock);
		printf("---------------------------------------------------\n");
		printf("Stop server using shortcut: Crtl + C\n");
	}
	printf("Close socket.\n");
	close(socket_desc);
	return EXIT_SUCCESS;
}

/*
 * EOF
 */