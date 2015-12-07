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

struct thread_node_t
{

	pthread_t thread;
	int socket_FD;
	struct thread_node_t *next;

};

typedef struct thread_node_t thread_node;

void disconnect_client(int);
pthread_mutex_t lock;
account *list[20];
account* start_account_session(char*);
account* finish_account_session(account*);
account* credit_account(account*, float);
account* debit_account(account*, float);
void add_thread(thread_node**, int);
char find_command(char*);
void start_threads(thread_node*, int);
int* remove_socket_FD(int, int[]);
void open_account(char*);
void print_account_list(account *account_list[]);
void start_server();
void client_service_thread(int);

