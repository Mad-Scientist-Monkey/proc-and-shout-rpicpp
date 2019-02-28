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
#include<arpa/inet.h>

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
	if (bind(socket_server,(struct sockaddr *)&addr_server, sizeof (addr_server)) < 0)
		cout << "## Error in bind." << endl;
	cout << "## Bind is ok." << endl;

	// Listen
	listen (socket_server, 3);

	// Accept incoming connection
	cout << "## Waiting for incoming connections...";
	int c = sizeof (struct sockaddr_in);
	socket_client = accept (socket_server, (struct sockaddr *)&addr_client, (socklen_t*)&c);
	if (socket_client < 0) perror ("## Accept failed.");
	cout << "Connection accepted." << endl;

	return 0;
}
