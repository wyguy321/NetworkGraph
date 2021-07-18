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

#include<sstream>

using namespace std; 

extern int globalMyID;
//last time you heard from each node. TODO: you will want to monitor this
//in order to realize when a neighbor has gotten cut off from you.
extern struct timeval globalLastHeartbeat[256];

//our all-purpose UDP socket, to be bound to 10.1.1.globalMyID, port 7777
extern int globalSocketUDP;
//pre-filled for sending to 10.1.1.0 - 255, port 7777
extern struct sockaddr_in globalNodeAddrs[256];

int sequenceNum=0;


struct sockaddr NodeAddrs;

map<int, string> graphTRANS;

map<string, map<string, string> > LSADB;

//map<string, map<string, string> > LSADB; 


map<string, string> Neighbors;

///GRAPH STUFF

int N;
int graph[256][256];
int dist[256];
bool visited[256];
int parent[256];



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



string readFile2(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return string(bytes.data(), fileSize);
}

string stripIPAddress(const string &IPADDR)
{


string s = IPADDR;

std::string delimiter = ".";
//std::string source = s.substr(0, s.find(delimiter));


s.erase(0, s.find(delimiter) + delimiter.length());

//std::string source = IPADDR.substr(0, IPADDR.find(delimiter)); 


s.erase(0, s.find(delimiter) + delimiter.length());

//std::string source = IPADDR.substr(0, IPADDR.find(delimiter));


s.erase(0, s.find(delimiter) + delimiter.length());

std::string source = s.substr(0, s.find(delimiter));

    return source;
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

void sendADDRES(string SM, string d){

int n = SM.length(); 

n++;

char secret_message[n];

strcpy(secret_message, SM.c_str()); 

  char myAddr[d.length()];



int s;

struct sockaddr_in servaddr;

strcpy(myAddr, d.c_str());

servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(7777); 
    servaddr.sin_addr.s_addr = inet_addr(myAddr);

sendto(globalSocketUDP, (const char *)secret_message, n, 
        0, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
}


void printLSAAndNeighbors(){



map<string, string>::iterator it;
///std::string formattedString;


std::cout << "PRINTING*************NEIGHBOR FOR NODE***************" << "\n";

std::cout << globalMyID << "\n";




for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
{
//formattedString.append(it->first);
    std::cout << it->first  // string (key)
              << ':'
              << it->second   // string's value 
              << std::endl ;

//formattedString.append("+");
//formattedString.append(it->second);
//formattedString.append("|");
}

std::cout << "PRINTING*************LSA FOR NODE***************"
          << "\n"
	   << globalMyID
	  << "\n"
	  << std::endl ;

map<string, map<string, string> > itt;
//map<string, string>::iterator itt;

typedef std::map<std::string, std::map<std::string, std::string> >::iterator it_type;
for(it_type iterator = LSADB.begin(); iterator != LSADB.end(); iterator++) {
    // iterator->first = key
    // iterator->second = value
    // Repeat if you also want to iterate through the second map.

  ///  std::cout << itt->first  // string (key)
     //         << ':'
       //       << itt->second   // string's value 
        //      << std::endl ;
//std::cout << "PRINTING*************LSA FOR NODE***************" << "\n";


std::cout << "Printing key FROM" << "\n";

std::cout << globalMyID << "\n";

std::cout << iterator->first << "\n"; 
map<string, string> seqCOST;

seqCOST = iterator->second;

map<string, string>::iterator itt;

std::cout << "PRINTING*************DEST + COST  FOR DEST***************"
          << "\n" 
	  <<std::endl ;


for ( itt = seqCOST.begin(); itt != seqCOST.end(); itt++ )
{
//map<string, string>::iterator it;

//formattedString.append(it->first);
    
std::cout << "PRINTING*************DEST + COST  FOR DEST***************"
          << "\n"
          <<std::endl ;

std::cout <<  "SEQUENCE:"
	      << itt->first  // string (key)
              << "COST:"
              << itt->second   // string's value 
              << std::endl ;

//formattedString.append("+");
//formattedString.append(it->second);
//formattedString.append("|");
}



}

//for ( itt = LSADB.begin(); itt != LSADB.end(); itt++ )
//{
//formattedString.append(itt->first);
  //  std::cout << itt->first  // string (key)
    //          << ':'
      //        << itt->second   // string's value 
        //      << std::endl ;
//std::cout << "PRINTING*************LSA FOR NODE***************" << "\n";


//std::cout << "Printing key FROM" << "\n";

// std::cout << itt->first; 


//map<string, string> seqCOST;


//seqCOST = itt->second;

//map<string, string>::iterator ittt;
//std::cout << "PRINTING*************SEQUENCE + COST FOR NODE***************" << "\n";	  
 //std::cout << globalMyID;
//for ( ittt = seqCOST.begin(); ittt != seqCOST.end(); ittt++ )
//{

 //std::cout << ittt->first  // string (key)
   //           << ':'
     //         << ittt->second   // string's value 
       //       << std::endl ;


//}


//formattedString.append("+");
//formattedString.append(it->second);
//formattedString.append("|");
//}


}


void sendLSA(){
sequenceNum++;

std::ostringstream s;

s << sequenceNum;

std::string converted(s.str());


map<string, string>::iterator it;
std::string formattedString;
for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
{
formattedString.append(it->first);
    std::cout << it->first  // string (key)
              << ':'
              << it->second   // string's value 
              << std::endl ;

formattedString.append("+");
formattedString.append(it->second);
formattedString.append("|");
}
std::cout << formattedString << "\n";
for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
{
//string convertedSequence(1,buffer);
	std::string address = "10.1.1.";
	address.append(it->first);
	std::cout << "Formatted string about to SEND" << "\n";
	std::cout << "Formatted string about to SEND" << "\n";
	std::cout << converted << "\n";
	std::cout << address << "\n";
	std::string sendmessage = "LSA-";
	std::ostringstream s1;

s1 << globalMyID;

std::string gmid(s1.str());
	sendmessage.append(gmid);
	sendmessage.append("-");
	sendmessage.append(converted);
	sendmessage.append("-");
	sendmessage.append(formattedString);
	std::cout << sendmessage << "\n";
	sendADDRES(sendmessage,address);
}
}

void propagateGraph(){
    int i,j,max,u,v,w,graphTranslationINDEX;
    //cout<<"Enter the number of nodes : ";
    //cin>>N;
    N = LSADB.size();
    N += Neighbors.size();
    for(i=0;i<=N;i++)
     for(j=0;j<=N;j++)
      graph[i][j]=0;
    max=N*(N+1);
    
    std::string delimiter = "+";
    
    //  stringstream LSAC(COST);
      //int lsac = 0;

      ///LSAC >> lsac;
    graphTranslationINDEX=0;
    
    
     std::ostringstream convertGlobalID;

     convertGlobalID << globalMyID;

      std::string convertedSource(convertGlobalID.str());
    map<string, int> graphTRANSIN;
    
    graphTRANSIN[convertedSource] = graphTranslationINDEX;
    graphTRANS[graphTranslationINDEX] = convertedSource;
    //graphTRANS[graphTranslationINDEX] = convertedSource.append("+0");
        ///LOOP through Neighbors
    
    std::cout << "***********Looping through neighbors from GRAPH********" << "\n";
map<string, string>::iterator it;    

    for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
    {
    //formattedString.append(it->first);
        graphTranslationINDEX++;
        
        graphTRANSIN[it->first] = graphTranslationINDEX;
        graphTRANS[graphTranslationINDEX] = it->first;
        
        std::cout << it->first  // string (key)
                  << ':'
                  << it->second   // string's value
                  << std::endl ;
        
     stringstream convertedWeight(it->first);
            

               convertedWeight >> w;
        
        u = globalMyID;
        v = graphTranslationINDEX;
        graph[u][v]=w;
        graph[v][u]=w;
       // std::ostringstream convertGlobalID;

        // convertGlobalID << globalMyID;

          //std::string convertedSource(convertGlobalID.str());
        
      //  stringstream LSAC(COST);
        //int lsac = 0;

        ///LSAC >> lsac;
        
       //graph[u][v]=w;
      //graph[v][u]=w;
        
        
    //formattedString.append("+");
    //formattedString.append(it->second);
    //formattedString.append("|");
    }
    
    //Loop through LSADB
    
    typedef std::map<std::string, std::map<std::string, std::string> >::iterator it_type;
    for(it_type iterator = LSADB.begin(); iterator != LSADB.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        // Repeat if you also want to iterate through the second map.

      ///  std::cout << itt->first  // string (key)
         //         << ':'
           //       << itt->second   // string's value
            //      << std::endl ;
    //std::cout << "PRINTING*************LSA FOR NODE***************" << "\n";


    std::cout << "***********Looping through LSA DB FROM graph********" << "\n";



    std::cout << iterator->first << "\n";
    map<string, string> seqCOST;

    seqCOST = iterator->second;

    map<string, string>::iterator itt;

    std::cout << "PRINTING*************DEST + COST  FOR DEST***************"
              << "\n"
          <<std::endl ;


    //for ( itt = seqCOST.begin(); itt != seqCOST.end(); itt++ )
    //{
        
        map<string, string>::reverse_iterator p;
       /// p = seqCOST.rbegin();
          //   printf("BEGENNING STRING!!! 1ST element %s\n",p->first.c_str());
          // printf("BEGENNING STRING!!! 2nd element %s\n",p->second.c_str());

           p =  seqCOST.rend();
            printf("BEGENNING STRING!!! 1ST element %s\n",p->first.c_str());
           printf("FINISHED STRING!!! 2nd element %s\n",p->second.c_str());

        
        if( p != seqCOST.rend() ) {
          // Do whatever with, it points to the last element
        } else {
          // map is empty
        }
        
    //map<string, string>::iterator it;

    //formattedString.append(it->first);
        
    //std::cout << "PRINTING*************DEST + COST  FOR DEST***************"
      //        << "\n"
       //       <<std::endl ;

    //std::cout <<  "SEQUENCE:"
     //         << itt->first  // string (key)
       //           << "COST:"
         //         << itt->second   // string's value
           //       << std::endl ;

    //formattedString.append("+");
    //formattedString.append(it->second);
    //formattedString.append("|");
   // }



    }

    
    //for(i=0;i<max;i++)
    //{
     //cout<<"Enter Edge and Weight : ";
     //cin>>u>>v>>w;
     //if(u==-1)    break;
     //else
     //{
       // graph[u][v]=w;
        //graph[v][u]=w;
     //}
    //}
    
    
}



void parseThroughLSANumber(string LSAString, string fromAddress){
	string localLSAString = LSAString;

std::string delimiter = "-";

std::string LSA = localLSAString.substr(0, localLSAString.find(delimiter));
localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

std::string FROMADDR = localLSAString.substr(0, localLSAString.find(delimiter));

localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

std::string SEQUENCE = localLSAString.substr(0, localLSAString.find(delimiter));

localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

std::string OTHERSTRING = localLSAString.substr(0, localLSAString.find(delimiter));

std::cout << LSA << "\n";

std::cout << FROMADDR << "\n";

std::cout << SEQUENCE << "\n";

std::cout << OTHERSTRING << "\n";

int n = localLSAString.length();

n++;

char myString[n];

strcpy(myString, localLSAString.c_str());

 if ( LSADB.find(FROMADDR) == LSADB.end() ) {
///NOT FOUND
map<string, string> seqCOST;


seqCOST[SEQUENCE] = OTHERSTRING;

LSADB[FROMADDR] = seqCOST;


}else {
///FOUND
map<string, string> seqCOST;

seqCOST = LSADB[FROMADDR];

if ( seqCOST.find(SEQUENCE) == seqCOST.end() ) {
///Not found 

seqCOST[SEQUENCE] = OTHERSTRING;

LSADB[FROMADDR] = seqCOST;
//return;
} else {
///Found Nothing to UPDATE


return;

}
}

std::ostringstream convertGlobalID;

convertGlobalID << globalMyID;

std::string convertedSource(convertGlobalID.str());


char *p = strtok(myString, "|");
while (p) {
    printf ("Token: %s\n", p);

 string LSAString(p);

LSAString.erase(0, LSAString.find(delimiter) + delimiter.length());

delimiter = "+";

std::string SOURCE = LSAString.substr(0, LSAString.find(delimiter));

LSAString.erase(0, LSAString.find(delimiter) + delimiter.length());

std::string COST = LSAString.substr(0, LSAString.find(delimiter));

std::cout << SOURCE << "\n";

std::cout << COST << "\n";


if (SOURCE == convertedSource){
//HMMM it does, lets check the our localDB and check if we need to update our neighborDB
std::cout << "SOURCE DOES EQUAL ME" << "\n";

std::string neighborFrom = stripIPAddress(fromAddress);

std::cout << neighborFrom << "\n";

std::string LOCALSOURCECOST = Neighbors[neighborFrom];

std::cout << "LOCAL NEIGHBOR SOURCE COST" << "\n";

std::cout << LOCALSOURCECOST << "\n"; 

std::cout << "LSA COST" << "\n";

std::cout << COST << "\n";

//if (SOURCECOST.length() == 0){

//std::cout << "SOURCE COST IS 0, updating local DB and sending out fresh LSA" << "\n";

if (LOCALSOURCECOST != COST){

std::cout << "HOUSTION, MY NEIGHBOR HAS PROBLEMS" << "\n";

stringstream localsourceC(LOCALSOURCECOST); 
  
    // The object has the value 12345 and stream 
    // it to the integer x 
    int lsc = 0; 
    localsourceC >> lsc; 

stringstream LSAC(COST);
int lsac = 0;

LSAC >> lsac;


std::cout << "Printing LOCAL NEIGHBORS" << "\n";


std::cout << lsc << "\n";

std::cout << "VS LSAC" << "\n";


std::cout << lsac << "\n";

if (lsac > lsc){

std::cout << "THE LSA IS different, lets update our neighbor DB, and we need to send out a notification" << "\n";


Neighbors[neighborFrom] = COST; 

sendLSA();

}else {

std::cout << "THE LSA IS different, hopefully our neighbors fix this on the next run.." << "\n";


sendLSA();

}

}


//Neighbors[neighborFrom] = 

//}

}
/////TODO PROPAGATE GRAPH


p = strtok(NULL, " ");
}


propagateGraph();
}



void listenForNeighbors(string initalTopFile, string initalLogFileName)
{
	char fromAddr[100];
	struct sockaddr_in theirAddr;
	socklen_t theirAddrLen;
	unsigned char recvBuf[1024];
		int bytesRecvd;
//printf("Top File!! %s\n", initalTopFile);
std::cout << initalTopFile << "\n";

 string line;
  ifstream myfile (initalTopFile.c_str());
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
    
std::string delimiter = " ";
std::string source = line.substr(0, line.find(delimiter)); // token is "scott"

cout << source << '\n';

line.erase(0, line.find(delimiter) + delimiter.length());

std::string cost = line.substr(0, line.find(delimiter)); 
cout << cost << '\n';

Neighbors.insert(pair<string, string>(source,cost));

}
    myfile.close();
  }

  else cout << "Unable to open file"; 


if (Neighbors.size() > 0){


std::cout << "SENDING LSA FROM COST FILE!" << "\n";

//sendLSA();
}
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
		if(strstr(fromAddr, "10.1.1."))
		{

			heardFrom = atoi(
					strchr(strchr(strchr(fromAddr,'.')+1,'.')+1,'.')+1);
			
		char* b = reinterpret_cast<char *>( &recvBuf );
			string buff(b);
			//TODO: this node can consider heardFrom to be directly connected to it; do any such logic now.
			printf("BuFF: %s\n", recvBuf);
			  if(buff.find("HEREIAM") != string::npos)
                {
			std::string mainString;
			 //printf("From address!!: %s\n", fromAddr);
			 //printf("BuFF: %s\n", recvBuf);
			string neighborToFind(fromAddr);
			neighborToFind = stripIPAddress(neighborToFind);
			 std::cout << "IP ADDRESS" << "\n";
			std::cout << neighborToFind << "\n";
			if ( Neighbors.find(neighborToFind) == Neighbors.end() ) {
			//NotFound, need to get info, record info
			std::cout << "Neighbor not found!!" << "\n";
			 std::cout << neighborToFind << "\n";
			Neighbors.insert(pair<string, string>(neighborToFind,"1"));
			
			sendLSA();
			
			//Need to broadcast the information to neighbors
			} else {
  			//Found, thats great. Do we need to check for their neighbors?
			std::cout << "FOUND!!" << "\n";
			std::cout << neighborToFind << "\n";
			//if (sequenceNum == 0){
			//std::cout << "Sequence number does equal 0, need to inform neighbors that I know" << "\n";

			//sendLSA();
			//}
			}

			}else if (buff.find("LSA") != string::npos){
			//Parse through the information & check if its a neighbor. 
			printf("INCOMING LSA!!!");
					
			parseThroughLSANumber(string(buff),string(fromAddr));

			//Re-propagate graph!!!!
 
			}
			//record that we heard from heardFrom just now.
			gettimeofday(&globalLastHeartbeat[heardFrom], 0);
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
	}
	//(should never reach here)
	close(globalSocketUDP);
}

