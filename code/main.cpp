#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "monitor_neighbors.h"
#include "process.h"
#include<bits/stdc++.h>
#include<iostream>
using namespace std;

void listenForNeighbors();
void* announceToNeighbors(void* unusedParam);

struct arg_struct {
    string TopFile;
    string FileName;
};
int globalMyID = 0;
//last time you heard from each node. TODO: you will want to monitor this
//in order to realize when a neighbor has gotten cut off from you.
struct timeval globalLastHeartbeat[256];

//our all-purpose UDP socket, to be bound to 10.1.1.globalMyID, port 7777
int globalSocketUDP;
//pre-filled for sending to 10.1.1.0 - 255, port 7777
struct sockaddr_in globalNodeAddrs[256];



map<string, string> PROCESS_QUEUE;



void *print_the_arguments(void *arguments)
{
   struct arg_struct *args = (struct arg_struct *)arguments;

   std::cout << args->TopFile << "\n";
    std::cout << args->FileName << "\n";
    processData(args->TopFile,args->FileName);
//pthread_exit(NULL);
  //  return NULL;
}


 
int main(int argc, char** argv)
{
	if(argc != 4)
	{
		fprintf(stderr, "Usage: %s mynodeid initialcostsfile logfile\n\n", argv[0]);
		exit(1);
	}
	
	
	//initialization: get this process's node ID, record what time it is, 
	//and set up our sockaddr_in's for sending to the other nodes.
	globalMyID = atoi(argv[1]);
	int i;
	for(i=0;i<256;i++)
	{
		gettimeofday(&globalLastHeartbeat[i], 0);
		
		char tempaddr[100];
		sprintf(tempaddr, "10.1.1.%d", i);
		memset(&globalNodeAddrs[i], 0, sizeof(globalNodeAddrs[i]));
		globalNodeAddrs[i].sin_family = AF_INET;
		globalNodeAddrs[i].sin_port = htons(7777);
		inet_pton(AF_INET, tempaddr, &globalNodeAddrs[i].sin_addr);
	}
	
	
	//TODO: read and parse initial costs file. default to cost 1 if no entry for a node. file may be empty.
	
	
	//socket() and bind() our socket. We will do all sendto()ing and recvfrom()ing on this one.
	if((globalSocketUDP=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}
	char myAddr[100];
	struct sockaddr_in bindAddr;
	sprintf(myAddr, "10.1.1.%d", globalMyID);	
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(7777);
	inet_pton(AF_INET, myAddr, &bindAddr.sin_addr);
	if(bind(globalSocketUDP, (struct sockaddr*)&bindAddr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("bind");
		close(globalSocketUDP);
		exit(1);
	}

  string initalTopFile(argv[2]);
        string logFileName(argv[3]);
	

	
	//start threads... feel free to add your own, and to remove the provided ones.
	pthread_t announcerThread;
	pthread_create(&announcerThread, 0, &announceToNeighbors, (void*)0);


	pthread_t processThread;

	int rc;

 struct arg_struct args;
    args.TopFile = initalTopFile;
    args.FileName = logFileName;

	
	pthread_t some_thread;
    

    if (pthread_create(&some_thread, NULL, &print_the_arguments, (void *)&args) != 0) {
        printf("Uh-oh!\n");
        return -1;
    }
	
	//good luck, have fun!
	listenForNeighbors(initalTopFile, logFileName);



	
	
	
}


