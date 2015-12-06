/* A simple server in the internet domain using TCP
The port number is passed as an argument */
#include "bankserver.h"

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

void start_server() {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256], command, client_arg;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	command = NULL;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = 2101;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,
		(struct sockaddr *) &cli_addr,
		&clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	bzero(buffer, 256);

	n = read(newsockfd, buffer, 255);
	if (n < 0) error("ERROR reading from socket");
	command = strtok(buffer," ");
	client_arg = strtok(NULL, " ");
	printf("command: %s\n argument: %s\n", buffer, client_arg);


	n = write(newsockfd, "I got your message", 18);
	if (n < 0) error("ERROR writing to socket");
	close(newsockfd);
	close(sockfd);

}

int main(int argc, char *argv[])
{
	pthread_t server_thread;
	if (pthread_create(&server_thread, NULL, start_server, NULL)) {

		fprintf(stderr, "Error creating thread\n");
		return 1;

	}
	pthread_join(server_thread, NULL);
	return 0;
}

