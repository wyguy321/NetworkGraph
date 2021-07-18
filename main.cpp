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

map<string, vector<string> > ACK_QUEUE;

map<string, string> ACK_QUEUE_IN;

map<string, string> PROCESS_QUEUE;
map<string, string> Neighbors;


void *loop_throughACK(void* unusedParam)
{
    while(1)
    {
        try
        {
            
            map<string, string>::iterator element;
             
            
             
             for ( element = ACK_QUEUE_IN.begin(); element != ACK_QUEUE_IN.end(); element++ ){
            
                 string buff(element->second);
                 

                     std::cout << "******************************ACK Coming IN*****************************" << "\n";
                                string temp = buff;

                                   std::cout << buff << "\n";
                                string str2 = "ACK-";
                                temp.replace(temp.find(str2),str2.length(),"");
                                 std::cout << temp << "\n";

                            
                                
                                
            //stringstream ACKNUM(tokenString);


              //        int sourceIDToCompare;
                //           ACKNUM >> sourceIDToCompare;


            //if (sourceIDToCompare == globalMyID){

              std::cout << "ACK FOUND ITS HOME!!!!" << "\n";
                  string ACKAddress = returnAddressFromQueue(element->first);
                 
                   std::cout << ACKAddress << "\n";
                   //  std::cout << ACK_QUEUE << "\n";
                  if (  ACK_QUEUE.find(ACKAddress) != ACK_QUEUE.end() ) {
            std::cout << "ACK QUEUE EXISTS FOR THIS ADDRESS!!!!" << "\n";
                   
                  vector<string> messages = ACK_QUEUE.find(ACKAddress)->second;
                      if(messages.size()==1){
                      std::cout << "ACK QUEUE size is one, erasing from this address" << "\n";
                      ACK_QUEUE.erase(ACKAddress);
                          std::cout << ACK_QUEUE.size() << "\n";
                      }else {
                          vector<string>::iterator itt;
                          int index=0;
                          for(itt = messages.begin(); itt != messages.end(); itt++ )    {
                          
                                  cout <<"Looping through elements to delete!!" << endl;
                                cout << *itt << endl;
                           cout << element->second << endl;
                              
                              if (*itt == temp){
                                  cout << "IT DOES ERASING ACKKK" << endl;
                                  messages.erase(messages.begin() + index);
                                  ACK_QUEUE.at(ACKAddress) = messages;
                              }
                              
                              
                              //cout<< address << endl;
                              
                              index++;
                          }
                          
                          
                      }
                         }else {
                          std::cout << "NO ACK QUEUE FOUND IN THE ACKQUEUE IN!!!!" << "\n";
                          
                      }
             // }else {
             ACK_QUEUE_IN.erase(element->first);
             std::cout << "******************************ACK Coming OUT*****************************" << "\n";
             }
            
            
    map<string, vector<string> >::iterator it;
        struct timespec sleepFor;
        sleepFor.tv_sec = 0;
        sleepFor.tv_nsec = 1000 * 1000 * 1000; //1000 ms
          nanosleep(&sleepFor, 0);
    
    for ( it = ACK_QUEUE.begin(); it != ACK_QUEUE.end(); it++)
    {
        string address = it->first;
        vector<string> messages = it->second;
        vector<string>::iterator itt;
        
        cout << "ACK QUEUEUEUEUEUEU ADDRESS" << endl;
        cout << address << endl;
        
        
        for(itt = messages.begin(); itt != messages.end(); itt++ )    {
        
                cout << "SENDING OUT ACK:::" << endl;
              cout << *itt << endl;
         cout << "FOR NODE ADDRESS:"<< endl;
            
            cout << address << endl;
            
            string addr = stripIPAddress(address);
            if(Neighbors.find(addr) != Neighbors.end()){
                cout <<"HAVE HEARD FROM THIS ADDRESS WE WILL SEND THIS MESSAGE BACK"<< endl;
           
                
                cout  << "to ADDRESS" << endl;
                
                cout  << addr << endl;
                
                 cout  << "message!!" << endl;
            cout  << *itt << endl;
            sendADDRES(*itt,address,true);
                
            }else {
                
                cout << "HAVE NOT HEARD FROM THIS ADDRESS WE WILL TRY AGAIN" << endl;
                               
                               
                               cout  << address << endl;
            }
        }
    }
        }catch (std::bad_alloc & exception)
        {
             std::cerr << "bad_alloc detected: " << exception.what();
        }
    }
//pthread_exit(NULL);
  //  return NULL;
}




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
   // pthread_t ACKTHREAD;
  // pthread_create(&ACKTHREAD, 0, &loop_throughACK, (void*)0);

	pthread_t processThread;

	int rc;

    struct arg_struct args = {};
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


