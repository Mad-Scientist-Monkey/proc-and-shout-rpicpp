//============================================================================
// Name        : ProcAndShout-Rpi.cpp
// Author      : Pablo Rego
// Version     :
// Copyright   : Copyright 2019
//============================================================================

#include <iostream>
using namespace std;

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<string.h>	//strlen
#include<unistd.h>	//write
#include<netdb.h>

string get_self_ip(){
	char hostbuffer[256];
	gethostname(hostbuffer, sizeof(hostbuffer));

	struct hostent *host = gethostbyname(hostbuffer);
	char* ip_buffer = inet_ntoa(*((struct in_addr*) host->h_addr_list[0]));
	return ip_buffer;
}

int main() {

	int socket_server, socket_client;
	struct sockaddr_in addr_server, addr_client;

	// Create the socket
	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_server == -1) cout << "## Error in socket creation." << endl;

	// Prepare the socketaddr_in structure
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = INADDR_ANY;
	addr_server.sin_port = htons (8888);

	// Bind
	if (bind(socket_server,(struct sockaddr *)&addr_server, sizeof (addr_server)) < 0){
		perror ("## Error in bind. Application will now exit.");
		return 1;
	}
	cout << "## Bind is ok." << endl;

	// Listen
	cout << "## Listening to connections ..." << endl;
	listen (socket_server, 3);

	// Accept incoming connection
	cout << "## Waiting for incoming connections..." << endl;
	int c = sizeof (struct sockaddr_in);
	while ((socket_client = accept (socket_server, (struct sockaddr *)&addr_client, (socklen_t*)&c))){
		if (socket_client < 0){
			perror ("## Accept failed. Application will now exit.");
			return 1;
		}

		// Get the address from connected client
		char *client_ip = inet_ntoa(addr_client.sin_addr);
		int  client_port = ntohs(addr_client.sin_port);

		cout << "## Connection is accepted from client " << client_ip << ":" <<
				to_string(client_port) << "." << endl;

		// Answer back message to client
		int    server_port = ntohs(addr_server.sin_port);
		string message = "Your connection request to server "+
				get_self_ip()+":"+to_string(server_port)+" was been accepted.\n";
		write(socket_client, message.c_str(), message.length());
	}

	return 0;
}
