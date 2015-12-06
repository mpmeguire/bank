#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


void start_server();

typedef struct
{

	char name[100];
	float balance;
	int inUse;

} account;
