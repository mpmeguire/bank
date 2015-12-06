#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


typedef struct
{

	char name[100];
	float balance;
	int inUse;

} account;

typedef struct
{

	pthread_t *incoming;
	pthread_t *outgoing;
	int socket_FD;
	pthread_t *next;

} thread_node;

account* start_account_session(char*, account*[]);
account* finish_account_session(account*);
account* credit_account(account*, float);
account* debit_account(account*, float);
int* add_socket_FD(int, int[]);
void start_threads(thread_node*, int);
int* remove_socket_FD(int, int[]);
void open_account(char*, account*[]);
void print_account_list(account *account_list[]);
void start_server();
void read_input(int);
void write_output(int);
