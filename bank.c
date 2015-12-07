
#include "bankserver.h"

void open_account(char *account_name) {
	account *new_account;
	int x;
	x = 0;

	for (x = 0; x <= 19; x++) {
		if (list[x] != NULL) {

			if (strcmp(account_name, list[x]->name) == 0) {
				printf("Account already exists.\n");
				return;
			}
		}
	}
	for (x = 0; x <= 19; x++) {
		if (list[x] == NULL) {
			new_account = (account*)malloc(sizeof(account));
			strcpy(new_account->name, account_name);
			new_account->balance = 0.0;
			new_account->inUse = 0;
			list[x] = new_account;
			printf("new account |%s| created\n", account_name);
			return;
		}
	}

	printf("Already maximum (20) accounts created.\n");
	return;

}

account* start_account_session(char *account_name) {

	int x;
	x = 0;

	for (x = 0; x <= 19; x++) {
		if (list[x] != NULL) {

			if (strcmp(account_name, list[x]->name) == 0) {
				printf("Starting session for account: |%s|\n", account_name);
				list[x]->inUse = 1;
				return list[x];
			}
		}
	}
	printf("account not found.\n");
	return list[x];
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
	int x, empty;
	
	while (1) {
		printf("ALL ACCOUNTS AND BALANCES:\n");
		empty = 1;
		for (x = 0; x <= 19; x++) {
			if (account_list[x] != NULL) {
				printf("------------------\naccount name: %s\n", account_list[x]->name);
				printf("account balance: %f\n", account_list[x]->balance);
				printf("account in use: %d\n------------------\n", account_list[x]->inUse);
				empty = 0;
			}

		}
		if (empty = 1) {
			printf("NO ACCOUNTS TO SHOW\n");
		}
		sleep(20);
	}
}

create_server(int port) {



}

void add_thread(thread_node **head, int socket_FD) {

	thread_node *pointer;
	thread_node *new_node;

	if ((*head) == NULL) {
	
		(*head) = malloc(sizeof(thread_node));
		(*head)->socket_FD = socket_FD;
		(*head)->next = NULL;
		pthread_create(&(*head)->thread, NULL, client_service_thread, socket_FD);
	}
	else {
		pointer = (*head);
		while (pointer->next != NULL) {

			pointer = pointer->next;

		}

		new_node = malloc(sizeof(thread_node));
		new_node->socket_FD = socket_FD;
		new_node->next = NULL;
		pthread_create(&new_node->thread, NULL, client_service_thread, socket_FD);
		pointer->next = new_node;

	}


}


int* remove_socket_FD(int socket_FD, int active_socket_FD_list[]) {


}



int main(int argc, char *argv[])
{
	account *active_account;
	active_account = NULL;
	pthread_mutex_init(&lock, NULL);
	/*arbitrary port number = 2101*/
	int port; port = 2101;
	char input[256];
	pthread_t print_accounts;
	pthread_create(&print_accounts, NULL, client_service_thread, socket_FD);
	thread_node *head;
	head = NULL;

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
		
		add_thread(&head, new_socket_FD);
		
		
	
		if (new_socket_FD < 0)
			error("ERROR on accept");

	
		pthread_join(head->thread, NULL);

}

char find_command(char *string) {
	/*open = o, start = s, credit = c, debit = d, balance = b, finish = f, exit = e, error = x*/
	if (strcmp(string, "open") == 0) {
		return 'o';
	}
	else if (strcmp(string, "start") == 0) {
		return 's';
	}
	else if (strcmp(string, "credit") == 0) {
		return 'c';
	}
	else if (strcmp(string, "debit") == 0) {
		return 'd';
	}
	else if (strcmp(string, "balance") == 0) {
		return 'b';
	}
	else if (strcmp(string, "finish") == 0) {
		return 'f';
	}
	else if (strcmp(string, "exit") == 0) {
		return 'e';
	}
	else return 'x';
}

void disconnect_client(int socket_FD) {

}

void client_service_thread(int new_socket_FD) {
	pthread_mutex_lock(&lock);
	char input[256], *command_arg, *input_arg, command;

	account *active_account;
	active_account = NULL;
	int num_chars_read, z;
	while (1) {
		bzero(input, 256);

		num_chars_read = read(new_socket_FD, input, 255);

		if (num_chars_read < 0) error("ERROR reading from socket");
		command_arg = strtok(input, " ");
		input_arg = strtok(NULL, " ");

		printf("command: %s\targument: %s", command_arg, input_arg);

		num_chars_read = write(new_socket_FD, "I got your message", 18);
		if (num_chars_read < 0) error("ERROR writing to socket");

		command = find_command(command_arg);
		if (active_account == NULL) {
			switch (command) {
			case 'o': open_account(input_arg);
				break;
			case 's': active_account = start_account_session(input_arg);
				break;

			case 'e': disconnect_client(new_socket_FD);
				break;
			default: printf("error\n");
			}
		}
		else {
			switch (command) {

			case 'c': active_account = credit_account(active_account, (float)atoll(input_arg));
				break;
			case 'd': active_account = debit_account(active_account, (float)atoll(input_arg));
				break;
			case 'b': account_balance(active_account);
				break;
			case 'f': finish_account_session(active_account);
				active_account = NULL;
				break;
			case 'e': disconnect_client(new_socket_FD);
				break;
			default: printf("error\n");
			}
		}


	}
	pthread_mutex_unlock(&lock);
}
