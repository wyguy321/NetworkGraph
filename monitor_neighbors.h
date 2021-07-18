#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include<bits/stdc++.h> 
#include<map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<sstream>
#include <cstddef>
#include <string>
#include <vector>


using namespace std; 

extern int globalMyID;
//last time you heard from each node. TODO: you will want to monitor this
//in order to realize when a neighbor has gotten cut off from you.
extern struct timeval globalLastHeartbeat[256];

//our all-purpose UDP socket, to be bound to 10.1.1.globalMyID, port 7777
extern int globalSocketUDP;
//pre-filled for sending to 10.1.1.0 - 255, port 7777
extern struct sockaddr_in globalNodeAddrs[256];

//vector<string> addressToAdd;



struct sockaddr NodeAddrs;


extern map<string, string> ACK_QUEUE_IN;


extern map<string, string> PROCESS_QUEUE;


//map<string, map<string, string> > LSADB; 


int addnum = 0;





//Yes, this is terrible. It's also terrible that, in Linux, a socket
//can't receive broadcast packets unless it's bound to INADDR_ANY,
//which we can't do in this assignment.
void hackyBroadcast(const char* buf, int length)
{
	int i;
	for(i=0;i<256;i++)
		if(i != globalMyID) //(although with a real broadcast you would also get the packet yourself)
			sendto(globalSocketUDP, buf, length, 0,
				  (struct sockaddr*)&globalNodeAddrs[i], sizeof(globalNodeAddrs[i]));
}

void* announceToNeighbors(void* unusedParam)
{
  struct timespec sleepFor;
  sleepFor.tv_sec = 0;
  sleepFor.tv_nsec = 300 * 1000 * 1000; //300 ms
  while(1)
  {
    hackyBroadcast("HEREIAM", 7);
    nanosleep(&sleepFor, 0);
  }
}


string returnSequenceNumber(){
    std::ostringstream s;

    s << addnum;

    std::string converted(s.str());
//s.str()
    return s.str();
;
}

void listenForNeighbors(string initalTopFile, string initalLogFileName)
{
	char fromAddr[100];
	struct sockaddr_in theirAddr;
	socklen_t theirAddrLen;
	unsigned char recvBuf[1024];
	//unsigned char nodeIdBuffer[10];
	unsigned char messageBugg[1024];
		int bytesRecvd;
//printf("Top File!! %s\n", initalTopFile);
std::cout << initalTopFile << "\n";

 
//cout << stripIPAddress("10.1.1.256");

//std::cout << stripIPAddress("10.1.1.256") << "\n";

//readFile2(initalTopFile);
//std::string content = readFile2(initalTopFile);

 //std::istringstream f(readFile2(initalTopFile));
  //  std::string line;    
   // while (std::getline(f, line)) {
     //   std::cout << line << std::endl;
    //}
    
	while(1)
	{	
		//printLSAAndNeighbors();
		//std::cout << "LSA DB!!!" << "\n";
		memset(recvBuf,'\0', 1024);
		theirAddrLen = sizeof(theirAddr);
		if ((bytesRecvd = recvfrom(globalSocketUDP, recvBuf, 1000 , 0, 
					(struct sockaddr*)&theirAddr, &theirAddrLen)) == -1)
		{
			perror("connectivity listener: recvfrom failed");
			exit(1);
		}
		
		inet_ntop(AF_INET, &theirAddr.sin_addr, fromAddr, 100);
		
		short int heardFrom = -1;
        char* b = reinterpret_cast<char *>( &recvBuf );
            string buff(b);
        string buffs(reinterpret_cast<char*>(recvBuf));
        if(strstr(fromAddr, "10.1.1.")){
            string element(fromAddr);
            element.append("-");
            element.append(returnSequenceNumber());




  
     
            addnum++;

          // std::cout << "SEND MESSAGE COMING IN!!" << "\n";
                                              // std::cout << "INCOMING CALL" << "\n";

            //                 std::cout << fromAddr << "\n";
                                                                    
            
            
            
            if (buffs.find("ACK") != string::npos){
                
               // ACK_QUEUE_IN.insert(pair<string, string>(element,buff));
                
                
            } else {
                     PROCESS_QUEUE.insert(pair<string, string>(element,buff));
            }
              //                                                   std::cout << buff << "\n";
                //                                                std::cout << "FOR NODE" << "\n";
                  //                                              std::cout << globalMyID << "\n";
                                                
            
          
            
		gettimeofday(&globalLastHeartbeat[heardFrom], 0);
            
        }else if (buffs.find("send") != string::npos) {
            
            std::cout << "Manageer!!" << "\n";

                           std::cout << fromAddr << "\n";

                           std::cout << buffs << "\n";
            std::cout << buffs.size() << "\n";
                          std::cout << "FOR NODE" << "\n";
                          std::cout << globalMyID << "\n";
                           std::cout << recvBuf << "\n";
                       std::cout << bytesRecvd << "\n";
            
            
             string element(fromAddr);
                      element.append("-");
                      element.append(returnSequenceNumber());




            
               
                      addnum++;
            
            
             
            
            
                   short int *nodeIdBuffer= ( short int *) malloc(sizeof(short int));
                                          
                                  
                                             memset(nodeIdBuffer, 0, sizeof(short int));
                                             memcpy(nodeIdBuffer,recvBuf+4,sizeof(short int));
                                             short int nodeId = -1;
                                             nodeId = ntohs(*nodeIdBuffer);
                                             char messageBuffer [100];
                                             memset(messageBuffer, 0, sizeof(char)*100);
                                
                                  int messageBytes = bytesRecvd - (4+sizeof(short int));
                                  memcpy(messageBuffer, recvBuf+4+sizeof(short int), messageBytes);
            
            std::cout << "Printing Local Byte ORder!!" << "\n";
            std::cout << nodeId << "\n";
            
            std::cout << "Printing message!!" << "\n";
                             std::cout << messageBuffer << "\n";
            string s (messageBuffer);
            string finalMessage("send");
            finalMessage.append("-");
            finalMessage.append(s);
            
            string str;          // The string
            ostringstream temp;  // 'temp' as in temporary
            temp << nodeId;
            str = temp.str();
              finalMessage.append("-");
            finalMessage.append(temp.str());
            std::cout << "Printing message about to send!!" << "\n";
            std::cout << finalMessage << "\n";
             PROCESS_QUEUE.insert(pair<string, string>(element,finalMessage));


        }
        
    }
		//Is it a packet from the manager? (see mp2 specification for more details)
		//send format: 'send'<4 ASCII bytes>, destID<net order 2 byte signed>, <some ASCII message>
		//if(!strncmp(recvBuf, "send", 4))
		//{
			//TODO send the requested message to the requested destination node
			// ...
		//}
		//'cost'<4 ASCII bytes>, destID<net order 2 byte signed> newCost<net order 4 byte signed>
		//else if(!strncmp(recvBuf, "cost", 4))
		//{
			//TODO record the cost change (remember, the link might currently be down! in that case,
			//this is the new cost you should treat it as having once it comes back up.)
			// ...
		//}
		
		//TODO now check for the various types of packets you use in your own protocol
		//else if(!strncmp(recvBuf, "your other message types", ))
		// ...
 
    //(should never reach here)
    close(globalSocketUDP);
}
    
	
