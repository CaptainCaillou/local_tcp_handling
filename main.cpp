/*
Local sensors data acquisition

The TCP code is not mine, credit to :
Ahmedatia : http://code.runnable.com/VXjZZimG7Nk0smWF/simple-tcp-server-code-for-c%2B%2B-and-socket
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>


void * get_in_addr(struct sockaddr * sa)
{
	if(sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr); 
	}
	
	return &(((struct sockaddr_in6 *)sa)->sin6_addr); 
}


int main(int argc, char * argv[])
{
	int status;
	struct addrinfo hints, * res;
	int listner; 
	
	int numbytes = 0;
	char buf[100];
	long int cpt=0;
	// Before using hint you have to make sure that the data structure is empty 
	memset(& hints, 0, sizeof hints);

	// Set the attribute for hint
	hints.ai_family = AF_UNSPEC; // We don't care V4 AF_INET or 6 AF_INET6
	hints.ai_socktype = SOCK_STREAM; // TCP Socket SOCK_DGRAM 
	hints.ai_flags = AI_PASSIVE; 
	
	// Fill the res data structure and make sure that the results make sense. 
	status = getaddrinfo(NULL, "8888" , &hints, &res);
	if(status != 0)
	{
		fprintf(stderr,"getaddrinfo error: %s\n",gai_strerror(status));
	}
	
	// Create Socket and check if error occured afterwards
	listner = socket(res->ai_family,res->ai_socktype, res->ai_protocol);
	if(listner < 0 )
	{
		fprintf(stderr,"socket error: %s\n",gai_strerror(status));
	}
	
	// Bind the socket to the address of my local machine and port number 
	status = bind(listner, res->ai_addr, res->ai_addrlen); 
	if(status < 0)
	{
		fprintf(stderr,"bind: %s\n",gai_strerror(status));
	}

	status = listen(listner, 10); 
	if(status < 0)
	{
		fprintf(stderr,"listen: %s\n",gai_strerror(status));
	}
	
	// Free the res linked list after we are done with it	
	freeaddrinfo(res);
	
	
	// We should wait now for a connection to accept
	int new_conn_fd;
	struct sockaddr_storage client_addr;
	socklen_t addr_size;
	char s[INET6_ADDRSTRLEN]; // an empty string 
	
	// Calculate the size of the data structure	
	addr_size = sizeof client_addr;
	
	printf("I am now accepting connections ...\n");
	
	while(1){
		// Accept a new connection and return back the socket desciptor 
		new_conn_fd = accept(listner, (struct sockaddr *) & client_addr, &addr_size);	
		if(new_conn_fd < 0)
		{
			
			fprintf(stderr,"accept: %s\n",gai_strerror(new_conn_fd));
			continue;
		}
	
		inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *) &client_addr),s ,sizeof s); 
		printf("I am now connected to %s \n",s);
		if(status == -1)
		{
			close(new_conn_fd);
			_exit(4);
		}
		cpt++;
		numbytes = recv(new_conn_fd,buf,100,0);
		buf[numbytes] = '\0';
		printf("%li -> Received %s \n",cpt,  buf);		
		status = send(new_conn_fd,buf, 100,0);
		
		close(new_conn_fd);	
	}
	// Close the socket before we finish 
	
	return 0;
}