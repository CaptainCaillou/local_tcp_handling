/*
This code simulate sensors connected to a network. 

The TCP code is not mine, credit to :
	http://code.runnable.com/VXjZAA1cltc0LP9r/simple-tcp-client-code-for-c%2B%2B-and-socket
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

//for the timestamp
#include <chrono>
int main(int argv, char * argc [])
{
	

	
	//Variables Declaration
	struct addrinfo hints, * res;
	int status, s_send;
	int socket_id;
	
	//clear hints
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo("127.0.0.1","8888", &hints, &res);
	if(status != 0)
	{
		fprintf(stderr, "Error getaddrinfo\n");
		exit(1);
	}	 	
	
	socket_id = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(socket_id < 0)
	{
		fprintf(stderr, "Error socket \n");
		exit(2);
	}
	
	status = connect(socket_id, res->ai_addr, res->ai_addrlen);
	if(status < 0)
	{
		fprintf(stderr, "Error connect \n");
		exit(3);
	}
	
	int numbytes = 0;
	char buf[100];
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	);
	long int ms_ = ms.count();
	//sprintf(buf, "{id->'EFR1893',timestamp-> %ld ,inputs->{1->'42', 2->'51'}}",ms_ );
	sprintf(buf, "%ld",ms_ );
	
	s_send = send(socket_id,buf, 100,0);
	numbytes = recv(socket_id,buf,100,0);
	if(numbytes == -1)
	{
		fprintf(stderr, "Error receive \n");
		exit(4);
	}
	buf[numbytes] = '\0';
	ms = duration_cast< milliseconds >(
	    system_clock::now().time_since_epoch()
	);
	ms_ = ms.count();
	printf("%li \n", ms_-atol(buf));	
	freeaddrinfo(res);
	close(socket_id);	
	
	return 0;
}