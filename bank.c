
#include "bankserver.h"

void open_account(char *account_name, account *account_list[]) {
	account *new_account;
	int x;
	x = 0;

	for (x = 0; x <= 19; x++) {
		if (account_list[x] != NULL) {

			if (strcmp(account_name, account_list[x]->name) == 0) {
				printf("Account already exists.\n");
				return;
			}
		}
	}
	for (x = 0; x <= 19; x++) {
		if (account_list[x] == NULL) {
			new_account = (account*)malloc(sizeof(account));
			strcpy(new_account->name, account_name);
			new_account->balance = 0.0;
			new_account->inUse = 0;
			account_list[x] = new_account;
			printf("new account |%s| created\n", account_name);
			return;
		}
	}

	printf("Already maximum (20) accounts created.\n");
	return;

}

account* start_account_session(char *account_name, account *account_list[]) {

	int x;
	x = 0;

	for (x = 0; x <= 19; x++) {
		if (account_list[x] != NULL) {

			if (strcmp(account_name, account_list[x]->name) == 0) {
				printf("Starting session for account: |%s|\n", account_name);
				account_list[x]->inUse = 1;
				return account_list[x];
			}
		}
	}
	printf("account not found.\n");
	return account_list[x];
}

account* finish_account_session(account* active_account) {

	active_account->inUse = 0;
	printf("client session for account |%s| is terminated.\n", active_account->name);
	return active_account;
}

account* credit_account(account *active_account, float credit) {
	active_account->balance += credit;
	return active_account;
}

account* debit_account(account *active_account, float debit) {
	active_account->balance = active_account->balance - debit;
	return active_account;
}

void account_balance(account *active_account) {
	printf("balance for account |%s| is: $%f\n", active_account->name, active_account->balance);
	return;
}


void print_account_list(account *account_list[]) {
	int x;

	printf("ALL ACCOUNTS AND BALANCES:\n");
	for (x = 0; x <= 19; x++) {
		if (account_list[x] != NULL) {
			printf("------------------\naccount name: %s\n", account_list[x]->name);
			printf("account balance: %f\n", account_list[x]->balance);
			printf("account in use: %d\n------------------\n", account_list[x]->inUse);
		}
		else {
			printf("%d", x);
		}
	}
}

create_server(int port) {



}

int* add_socket_FD(int new_socket_FD, int active_socket_FD_list[]) {

	int x; x = 0;

	for (x = 0; x <= 4; x++) {

		if (active_socket_FD_list[x] == -1) {

			active_socket_FD_list[x] = new_socket_FD;
			printf("adding socket id %d to the list..\n", active_socket_FD_list[x]);
			return active_socket_FD_list;
		}
	}
	return active_socket_FD_list;
}

int* remove_socket_FD(int socket_FD, int active_socket_FD_list[]) {

	int x; x = 0;

	for (x = 0; x <= 4; x++) {

		if (active_socket_FD_list[x] == socket_FD) {

			active_socket_FD_list[x] = -1;
			printf("removing socket id %d from the list..\n", active_socket_FD_list[x]);
			return active_socket_FD_list;
		}
	}
	return active_socket_FD_list;
}



int main(int argc, char *argv[])
{
	account *active_account;
	active_account = NULL;
	account *list[20];
	/*arbitrary port number = 2101*/
	int port; port = 2101;
	char input[256];

	thread_node *thread_reference_head;
	thread_reference_head = malloc(sizeof(thread_node));
	thread_reference_head->incoming = NULL;
	thread_reference_head->outgoing = NULL;
	thread_reference_head->next = NULL;

	/*the following is heavily base off of http://www.linuxhowtos.org/C_C++/socket.htm */
	/* sockfd and newsockfd are for the file descriptors returned from socket/accept system call*/
	/*clilen is the size of the address of the client, needed for accept()*/
	/*num_chars_read is the number of characters read infrom read/write()*/
	int server_socket_FD, new_socket_FD, *active_socket_FD_list[4], clilen, num_chars_read;


	int x;
	x = 0;
	/*initliazing the list of accounts and list of active socket IDs*/
	for (x = 0; x <= 19; x++) {
		if (x >= 0 && x <= 4) {
			active_socket_FD_list[x] = -1;
		}
		list[x] = NULL;
	}

	/* the server will read charactes from socket connection into these buffers*/
	struct sockaddr_in server_address, client_address;

	/*1st arg = internet domain. 2nd arg = stream socket type. 3rd = always 0 because i don't know how to use it*/
	/*now sockfd will be the ID any time this socket is used.*/
	server_socket_FD = socket(AF_INET, SOCK_STREAM, 0);
	/*if this call fails, it returns -1 and throws an error.*/
	if (server_socket_FD < 0)
		error("ERROR opening socket");
	/*initializes the server address buffer to all zeroes,*/
	bzero((char *)&server_address, sizeof(server_address));

	/*initiliazing all the fields for the server address struct.*/
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	/*server address.sin_addr.s_addr is the IP address of the host. INADDR_ANY is symbolic for this address*/
	server_address.sin_addr.s_addr = INADDR_ANY;

	/*bind() takes takes three arguments, the socket file descriptor,
	the address to which is bound, and the size of the address to which it is bound. */
	bind(server_socket_FD, (struct sockaddr *) &server_address, sizeof(server_address));

	/*this will listen for (at most) 5 connections trying to contact this socket*/
	listen(server_socket_FD, 5);
	clilen = sizeof(client_address);

		
		new_socket_FD = accept(server_socket_FD, (struct sockaddr *) &client_address, &clilen);
		active_socket_FD_list[0] = add_socket_FD(new_socket_FD, active_socket_FD_list);
		
		pthread_t thread1;
		pthread_create(&thread1, NULL, client_service_thread, &new_socket_FD);

		if (new_socket_FD < 0)
			error("ERROR on accept");

	
	getch();

}

void client_service_thread(int new_socket_FD){
	char *input;
	int num_chars_read;

	bzero(input, 256);
	num_chars_read = read(new_socket_FD, input, 255);
	if (num_chars_read < 0) error("ERROR reading from socket");
	printf("Here is the message: %s", input);
	int num_chars_read;
	num_chars_read = write(new_socket_FD, "I got your message", 18);
	if (num_chars_read < 0) error("ERROR writing to socket");
}

