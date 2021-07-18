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





vector<string> addressToAdd;


int sequenceNum=0;


//struct sockaddr NodeAddrs;

map<int, string> graphTRANS;

map<string, string> NextHop;

extern map<string, string> PROCESS_QUEUE;


map<string, string> ACK_QUEUE;

int nodeSize=0;


map<string, map<string, string> > LSADB;

//map<string, map<string, string> > LSADB; 
  map<string, int> graphTRANSIN;

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


void countForN(){



map<string, string>::iterator it;
///std::string formattedString;


std::cout << "PRINTING*************NEIGHBOR FOR NODE***************" << "\n";

std::cout << globalMyID << "\n";



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

if ( itt != seqCOST.end() ) {
        std::cout << "PRINTING*************LAST SEQUENCE!!!!!!***************"
              << "\n"
          <<std::endl ; 
              istringstream is(itt->second);
string part;

while (getline(is, part, '|')){
  std::cout << "INCREMENTING N" << "\n";
std::cout << part << "\n";
N++;


}
}
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

void forwardLSAtoNeighbors(string fromAddress, string LSAMessage){


map<string, string>::iterator it;

istringstream iss(LSAMessage);
string partt;
std::string LSA = "";
std::string FROMADDR = "";
std::string SEQUENCE = "";
std::string OTHERSTRING = "";
int parsedString = 0;
while (getline(iss, partt, '-')){
 
 //cout << partt << endl;
if (parsedString==0){
  LSA = partt;
  std::cout << "LSA" << "\n";
  std::cout << LSA << "\n";

} else if (parsedString==1) {
  FROMADDR = "10.1.1.";
FROMADDR.append(partt);
  std::cout << "FROMADDR" << "\n";
  std::cout << FROMADDR << "\n";
  break;
} else if (parsedString==2){
SEQUENCE = partt;
std::cout << "SEQUENCE" << "\n";
  std::cout << SEQUENCE << "\n";

}else if (parsedString==3){
OTHERSTRING = partt;
std::cout << "OTHERSTRING" << "\n";
  std::cout << OTHERSTRING << "\n";
}
parsedString++;
}



for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
{
//string convertedSequence(1,buffer);
        std::string address = "10.1.1.";
        address.append(it->first);
        std::cout << "FORWARDING LSA TO NEIGHBORS" << "\n";
        std::cout << fromAddress << "\n";
	  std::cout << address << "\n";
       // std::cout << converted << "\n";
       // std::cout << address << "\n";
     //   std::string sendmessage = "LSA-";
       /// std::ostringstream s1;

//s1 << globalMyID;

///std::string gmid(s1.str());
   ///     sendmessage.append(gmid);
      //  sendmessage.append("-");
        //sendmessage.append(converted);
       // sendmessage.append("-");
       // sendmessage.append(formattedString);
       // std::cout << sendmessage << "\n";
       



	if (fromAddress != address && address != FROMADDR){ 
          std::cout << "Forwarding LSA to NEIGHBOR!!" << "\n";		
	  std::cout << LSAMessage << "\n";
	std::cout << fromAddress << "\n";
	 sendADDRES(LSAMessage,address);
	}else {
	 std::cout << "IGNORING THE BELOW!!!!!" << "\n";
	 std::cout << fromAddress << "\n";

}
}



}


void sendACK(string nextACKADRESS,string ACKADRESS,string ACKMESSAGE){
std::cout << "Sneding ACCKKK!!!!!" << "\n";

std::string address = "10.1.1.";
address.append(nextACKADRESS);
std::string forwardedMsg="ACK";
std::cout << "ADDRESSS TO SEND" << "\n";
forwardedMsg.append("-");
forwardedMsg.append(ACKADRESS);
forwardedMsg.append("-");
forwardedMsg.append(ACKMESSAGE);

std::cout << forwardedMsg << "\n";

std::cout << address << "\n";
sendADDRES(forwardedMsg,address);



}


void sendLSA(string forwardLSATOAddress,string LSAAddress){

if(forwardLSATOAddress.size()==0){
sequenceNum++;

}else {
  if (sequenceNum == 0){
    sequenceNum++;
  }
}
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
	
  if(forwardLSATOAddress.size()==0){
      address.append(it->first);
  sendADDRES(sendmessage,address);

}else {
address.append(forwardLSATOAddress);
std::string forwardedMsg="FORWARDLSATO";
std::cout << "ADDRESSS TO SEND" << "\n";
forwardedMsg.append("-");
forwardedMsg.append(LSAAddress);
forwardedMsg.append("-");
forwardedMsg.append(sendmessage);
sendADDRES(forwardedMsg,address);

    ACK_QUEUE.insert(pair<string, string>(forwardLSATOAddress,LSAAddress));
    
    

break;
}
}
}


void printPathForDest(int j)
{
  cout << "PRINTING PATH DEST" << "\n";
  //cout << graphTRANS.find(j)->second << " ";
    if (parent[j]==-1)
        return;
    printPathForDest(parent[j]);
    cout << graphTRANS.find(j)->second << " ";
    //return;
}

void printPath(int j, string orginalDest)
{


    if (parent[j]==-1){
        return;
    }
    printPath(parent[j],orginalDest);
    string hopToCompare = graphTRANS.find(0)->second;
    if (hopToCompare != graphTRANS.find(j)->second){
      string dest = NextHop.find(orginalDest)->second;
      if (dest.length() == 0){
        NextHop.at(orginalDest) = graphTRANS.find(j)->second;
      }
    }
    cout << graphTRANS.find(j)->second << " ";
    //return;
}

int minDistance()
{
    int min = 10000, minDist;
   for (int v = 0; v < N; v++)
        if (visited[v] == false && dist[v] <= min)
        {
            min = dist[v];
            minDist = v;
        }
    return minDist;
}

void dijkstraPrintPathForDest(int dest)
{
    int src = 0;
    ///cout<<"Enter the Source Node : ";
    //cin>>src;
    for (int i = 0; i < N; i++)
    {
        parent[0] = -1;
        dist[i] = 10000;
        visited[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < N-1; count++)
    {
        int u = minDistance();
        visited[u] = true;
        for (int v = 0; v < N; v++)
            if (!visited[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }
    cout<<"Src->Dest\tDistance\tPath"<<endl;
    for (int i = 1; i < N; i++)
    {
        cout<<graphTRANS.find(src)->second<<"->"<<graphTRANS.find(i)->second<<"\t\t"<<dist[i]<<"\t\t"<<graphTRANS.find(src)->second<<" ";
        printPath(i,graphTRANS.find(i)->second);
        if (dest == i){
          break;
        }

        cout<<endl;
    }
}
void dijkstra()
{
    int src = 0;
    ///cout<<"Enter the Source Node : ";
    //cin>>src;
    NextHop.clear();
    for (int i = 0; i < N; i++)
    {
        parent[0] = -1;
        dist[i] = 10000;
        visited[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < N-1; count++)
    {
        int u = minDistance();
        visited[u] = true;
        for (int v = 0; v < N; v++)
            if (!visited[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }
    }
    cout<<"Src->Dest\tDistance\tPath"<<endl;
    for (int i = 1; i < N; i++)
    {
        cout<<graphTRANS.find(src)->second<<"->"<<graphTRANS.find(i)->second<<"\t\t"<<dist[i]<<"\t\t"<<graphTRANS.find(src)->second<<" ";
        //.insert(pair<string, int>(convertedSource,graphTranslationINDEX));
        NextHop.insert(pair<string, string>(graphTRANS.find(i)->second,""));
        printPath(i,graphTRANS.find(i)->second);
        cout<<endl;
    }
}


void propagateGraph(){
    int i,j,max,u,v,w,graphTranslationINDEX;
    //cout<<"Enter the number of nodes : ";
    //cin>>N;
  //  int zeroedgraph[256][256];
    //graph = zeroedgraph;
    //memset(graph, 0, sizeof(graph));

    //countForN();
graphTRANS.clear();
  N=nodeSize;
  N++;
std::cout << "Neighbors suze" << "\n";
     std::cout << Neighbors.size() << "\n";
    std::cout << "TOTAL N SIZE" << "\n";
     N += Neighbors.size();
   
     std::cout << N << "\n";
    for(i=0;i<=N;i++)
     for(j=0;j<=N;j++)
      graph[i][j]=0;
      graph[j][i]=0;
    max=N*(N+1);
    
//memset(array, 0, sizeof(graph[0][0]) * m * n);

//int dist[256];
//bool visited[256];
//int parent[256];

memset(dist, 0, sizeof(dist));

memset(visited, 0, sizeof(visited));

memset(parent, 0, sizeof(parent));
//std::fill_n(array, elementCount, 0);    

std::string delimiter = "+";
    
    //  stringstream LSAC(COST);
      //int lsac = 0;

      ///LSAC >> lsac;
    graphTranslationINDEX=0;
    
    
     std::ostringstream convertGlobalID;

     convertGlobalID << globalMyID;

      std::string convertedSource(convertGlobalID.str());
    


graphTRANSIN.clear();
  graphTRANS.clear();

	   graphTRANSIN.insert(pair<string, int>(convertedSource,graphTranslationINDEX));
    graphTRANS.insert(pair<int,string>(graphTranslationINDEX,convertedSource));

//printLSAAndNeighbors();
	 std::cout << "***********Converted globalID!!!!!! STARTING GRAPH propagateGraph********" << "\n";
	std::cout << globalMyID << "\n";
	 std::cout << convertedSource << "\n";
	//std::cout << graphTRANS.find(graphTranslationINDEX) << "\n";

    
	 std::cout << "***********Looping through neighbors from GRAPH********" << "\n";

    //graphTRANS[graphTranslationINDEX] = convertedSource.append("+0");
        ///LOOP through Neighbors
    
    std::cout << "***********Looping through neighbors from GRAPH********" << "\n";
map<string, string>::iterator it;    

    for ( it = Neighbors.begin(); it != Neighbors.end(); it++ )
    {
    //formattedString.append(it->first);
        graphTranslationINDEX++;

        std::cout << "GRAPH TRANSLATION INDEX COUNT!!!" << "\n";
             std::cout << graphTranslationINDEX << "\n";
        graphTRANSIN.insert(pair<string, int>(it->first,graphTranslationINDEX));
       // graphTRANSIN[it->first] = graphTranslationINDEX;
        //graphTRANS[graphTranslationINDEX] = it->first;
         graphTRANS.insert(pair<int, string>(graphTranslationINDEX,string(it->first)));
        
       // std::cout << it->first  // string (key)
         //         << ':'
           //       << it->second   // string's value
             //     << std::endl ;
        
     stringstream convertedWeight(it->second);
            

               convertedWeight >> w;

               std::cout << "***********Converted weight********" << "\n";

               std::cout << w << "\n";

              std::cout << "***********Graph translation for node********" << "\n";

              std::cout << it->first << "\n";

              // std::cout << graphTRANSIN.find(it->first) << "\n";

                 std::cout << "***********ADDING SOURCE NEIGHBORS********" << "\n";


        u = 0;
         std::cout << u << "\n";

        v = graphTranslationINDEX;
        std::cout << "***********ADDING DESTINATION NEIGHBORSNEIGHBORS********" << "\n";
        std::cout << v << "\n";
        std::cout << "***********ADDING WEIGHT NEIGHBORS********" << "\n";
        std::cout << w << "\n";
        graph[u][v]=w;
        graph[v][u]=w;

std::cout << "***********TRANSLATION FROM ABOVE SOURCE NEIGHBORS********" << "\n";

std::cout << graphTRANS.find(u)->second << "\n";

std::cout << "***********TRANSLATION FROM ABOVE DEST NEIGHBORS********" << "\n";

std::cout << graphTRANS.find(v)->second << "\n";




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
    int lastGraphIndex;
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
//graphTRANSIN.insert(pair<string, int>(it->first,graphTranslationINDEX));
       // graphTRANSIN[it->first] = graphTranslationINDEX;
        //graphTRANS[graphTranslationINDEX] = it->first;
       //  graphTRANS.insert(pair<int, string>(graphTranslationINDEX,it->first));

        int sourceIndex;

 std::cout << "***********translation********" << "\n";

      if(graphTRANSIN.find(iterator->first) != graphTRANSIN.end()){
        std::cout << "***********translation DOES EXIST********" << "\n";
         sourceIndex = graphTRANSIN.find(iterator->first)->second;
            
            std::cout << "GRAPH TRANSLATION INDEX COUNT!!!" << "\n";
             std::cout << graphTranslationINDEX << "\n";

         std::cout << "USING" << "\n";
            std::cout << sourceIndex << "\n";
              std::cout << "FOR" << "\n";
            std::cout << iterator->first << "\n";


         //lastGraphIndex = graphTranslationINDEX;
         //graphTranslationINDEX = sourceIndex;

          std::cout << "GRAPHTRANSLATION!!" << "\n";
            std::cout << graphTranslationINDEX << "\n";

      } else {

        std::cout << "***********translation DOES NOT EXIST********" << "\n";


       
       // graphTranslationINDEX = lastGraphIndex;
         graphTranslationINDEX++;
        sourceIndex = graphTranslationINDEX;
  std::cout << "GRAPH TRANSLATION INDEX COUNT!!!" << "\n";
             std::cout << graphTranslationINDEX << "\n";



       std::cout << "USING" << "\n";
            std::cout << sourceIndex << "\n";
              std::cout << "FOR" << "\n";
            std::cout << graphTranslationINDEX << "\n";

      }
  
      
        
        graphTRANSIN.insert(pair<string, int>(iterator->first,graphTranslationINDEX));
        graphTRANS.insert(pair<int, string>(graphTranslationINDEX,string(iterator->first)));



    std::cout << "***********Looping through LSA DB FROM graph********" << "\n";



    std::cout << iterator->first << "\n";
    map<string, string> seqCOST;

    seqCOST = iterator->second;

    map<string, string>::iterator itt;

    //std::cout << "PRINTING*************DEST + COST  FOR DEST***************"
      //        << "\n"
        //  <<std::endl ;



    //for ( itt = seqCOST.begin(); itt != seqCOST.end(); itt++ )
    //{
        
        map<string, string>::reverse_iterator p;
	

map<string, string>::iterator itw;

    for ( itw = seqCOST.begin(); itw != seqCOST.end(); itw++ )
    {
	
  std::cout << "PRINTING*************SEQUENCSES***************"
              << "\n"
          <<std::endl ; 

           std::cout << itw->first
              << "\n"
          <<std::endl ; 
          std::cout << itw->second
              << "\n"
          <<std::endl ; 
	// std::cout << "PRINTING*************ITWE!!!!!!***************"
    //          << "\n"
      //    <<std::endl ;


	 //std::cout << itw->first  // string (key)
      //            << ':'
        //          << itw->second   // string's value
          //        << std::endl ;

   if ( itw != seqCOST.end() ) {
        std::cout << "PRINTING*************LAST SEQUENCE!!!!!!***************"
              << "\n"
          <<std::endl ; 
	// std::cout << itw->first  // string (key)
    //              << ':'
      //            << itw->second   // string's value
        //          << std::endl ;


if (itw->second.find("|") != std::string::npos) {
    std::cout << "BOZO WE HAVE A Bucket!! ||" << '\n';
}else{
   std::cout << "BOZO WE don't have a  Bucket!! ||" << '\n';
}

int n = itw->second.length();

n++;

char myString[n];

strcpy(myString, itw->second.c_str());

std::cout << "USING LSA::"
              << "\n"
          <<std::endl ; 

std::cout << itw->second
              << "\n"
          <<std::endl ; 

char *p = strtok(myString, "|");
while (p) {
    printf ("Token: %s\n", p);

 string graphstring(p);

 delimiter = "+";

istringstream is(graphstring);
string part;
int parsedString=0;
std::string SRC = "";
std::string CST = "";
while (getline(is, part, '+')){
  std::cout << "ffsdfsdfds" << "\n";

if (parsedString==0){
  SRC = part;
  cout << part << endl;
} else {
CST = part;
}
parsedString++;
}



SRC.erase(std::remove(SRC.begin(), SRC.end(), '|'), SRC.end());

CST.erase(std::remove(CST.begin(), CST.end(), '|'), CST.end());


if(SRC == convertedSource && Neighbors.find(iterator->first) != Neighbors.end()){
    std::cout << SRC<< "\n";
     std::cout << convertedSource << "\n";
  std::cout << "it DOES!! CONTINUING" << "\n";
  p = strtok(NULL, "|");
  continue;

}

std::cout << "PARSED SOURCE FROM LSA!! AFTER!!" << "\n";

std::cout << SRC << "\n";

std::cout << "PARSED COST FROM LSA!! AFTER!!" << "\n";


std::cout << CST << "\n";

std::cout << "DESTINATION!! AFTER!!" << "\n";


std::cout << convertedSource << "\n";






//== m.end()

        if (graphTRANSIN.find(SRC) == graphTRANSIN.end()){
std::cout << "***********No TRANSLATION EXISTS!!! Creating one. *******" << "\n";


      graphTranslationINDEX++;

      std::cout << "GRAPH TRANSLATION INDEX COUNT!!!" << "\n";
             std::cout << graphTranslationINDEX << "\n";
     // .insert(pair<string, string>(source,cost));


          //std::cout << "PRINTING*************LSA FOR NODE***************" << "\n";
//graphTRANSIN.insert(pair<string, int>(it->first,graphTranslationINDEX));
       // graphTRANSIN[it->first] = graphTranslationINDEX;
        //graphTRANS[graphTranslationINDEX] = it->first;
       //  graphTRANS.insert(pair<int, string>(graphTranslationINDEX,it->first));

         graphTRANSIN.insert(pair<string, int>(SRC,graphTranslationINDEX));
        //graphTRANSIN[SRC] = graphTranslationINDEX;
        graphTRANS.insert(pair<int, string>(graphTranslationINDEX,SRC));
       // graphTRANS[graphTranslationINDEX] = SRC;


 stringstream sourceWeight(CST);
            

               sourceWeight >> w;

  std::cout << "***********ADDING SOURCE LSA********" << "\n";
    u = sourceIndex;
         std::cout << u << "\n";

        v =   graphTRANSIN.find(SRC)->second;
        std::cout << "***********ADDING destination LSA********" << "\n";
          std::cout << v << "\n";

          std::cout << "***********ADDING weight LSA********" << "\n";
          std::cout << w << "\n";
    

    if (graph[u][v] != 0 || graph[v][u] != 0){

      std::cout << "HOLD THE PHONE, WE already have this element!!" << "\n";



std::cout << "***********TRANSLATION FROM ABOVE SOURCE LSA********" << "\n";

std::cout << graphTRANS.find(u)->second << "\n";

std::cout << "***********TRANSLATION FROM ABOVE DEST LSA********" << "\n";

std::cout << graphTRANS.find(v)->second << "\n";


std::cout << "***********ADDING destination LSA********" << "\n";
          std::cout << v << "\n";

          std::cout << "***********ADDING SOURCE LSA********" << "\n";
                std::cout << u << "\n";
  

  std::cout << "***********PRINTING WEIGHT********" << "\n";
                std::cout << graph[u][v] << "\n";

                std::cout << "***********PRINTING WEIGHT********" << "\n";
                std::cout << graph[v][u] << "\n";



      if (graph[u][v] > w && graph[v][u] > w){


std::cout << "***********IT DEFINETLY IS LESS, we should be good to forget. Continuing..********" << "\n";


std::cout << w << "\n";
p = strtok(NULL, "|");
continue;

      } else {



      }


  

    }
        graph[u][v]=w;
        graph[v][u]=w;
std::cout << "***********TRANSLATION FROM ABOVE SOURCE LSA********" << "\n";

std::cout << graphTRANS.find(u)->second << "\n";

std::cout << "***********TRANSLATION FROM ABOVE DEST LSA********" << "\n";

std::cout << graphTRANS.find(v)->second << "\n";


std::cout << "***********ADDING WEIGHT ELSE LSA********" << "\n";
               std::cout << w << "\n";


} else {


    stringstream sourceWeight(CST);
            

               sourceWeight >> w;


         u = sourceIndex;


if (graph[u][v] != 0 || graph[v][u] != 0){

      std::cout << "HOLD THE PHONE, WE already have this element!!" << "\n";


std::cout << "***********TRANSLATION FROM ABOVE SOURCE LSA********" << "\n";

std::cout << graphTRANS.find(u)->second << "\n";

std::cout << "***********TRANSLATION FROM ABOVE DEST LSA********" << "\n";

std::cout << graphTRANS.find(v)->second << "\n";


std::cout << "***********ADDING destination LSA********" << "\n";
          std::cout << v << "\n";

          std::cout << "***********ADDING SOURCE LSA********" << "\n";
                std::cout << u << "\n";
  

  std::cout << "***********PRINTING WEIGHT********" << "\n";
                std::cout << graph[u][v] << "\n";

                std::cout << "***********PRINTING WEIGHT********" << "\n";
                std::cout << graph[v][u] << "\n";




       if (graph[u][v] > w && graph[v][u] > w){


std::cout << "***********IT DEFINETLY IS LESS, we should be good to forget. Continuing..********" << "\n";


std::cout << w << "\n";
p = strtok(NULL, "|");
continue;

      } 

  
    }


  std::cout << "***********ADDING SOURCE ELSE LSA********" << "\n";
               std::cout << u << "\n";

        v =   graphTRANSIN.find(SRC)->second;
          std::cout << "***********ADDING DEST ELSE LSA********" << "\n";
               std::cout << v << "\n";
        graph[u][v]=w;
           graph[v][u]=w;

           std::cout << "***********ADDING WEIGHT ELSE LSA********" << "\n";
               std::cout << w << "\n";

std::cout << "***********TRANSLATION FROM ABOVE SOURCE LSA********" << "\n";

std::cout << graphTRANS.find(u)->second << "\n";

std::cout << "***********TRANSLATION FROM ABOVE DEST LSA********" << "\n";

std::cout << graphTRANS.find(v)->second << "\n";


}

p = strtok(NULL, "|");
}
}
}


//std::cout << "***********Looping through LSA DB FROM graph********" << "\n";

  //      p = seqCOST.rbegin();
             
//printf("BEGENNING STRING!!! 1ST element %s\n",p->first.c_str());
  //         printf("BEGENNING STRING!!! 2nd element %s\n",p->second.c_str());

    //       p =  seqCOST.rend();
      //      printf("BEGENNING STRING!!! 1ST element %s\n",p->first.c_str());
        //   printf("FINISHED STRING!!! 2nd element %s\n",p->second.c_str());

        

        
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

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

bool parseThroughLSANumber(string LSAString, string fromAddress){
	string localLSAString = LSAString;


std::cout << "****************STARTING LSA PARSE***************************" << "\n";


std::string delimiter = "-";
//N=0;

istringstream iss(LSAString);
string partt;
std::string LSA = "";
std::string FROMADDR = "";
std::string SEQUENCE = "";
std::string OTHERSTRING = "";
int parsedString = 0;
while (getline(iss, partt, '-')){
 
 //cout << partt << endl;
if (parsedString==0){
  LSA = partt;
  std::cout << "LSA" << "\n";
  std::cout << LSA << "\n";
} else if (parsedString==1) {
FROMADDR = partt;
  std::cout << "FROMADDR" << "\n";
  std::cout << FROMADDR << "\n";
} else if (parsedString==2){
SEQUENCE = partt;
std::cout << "SEQUENCE" << "\n";
  std::cout << SEQUENCE << "\n";

}else if (parsedString==3){
OTHERSTRING = partt;
std::cout << "OTHERSTRING" << "\n";
  std::cout << OTHERSTRING << "\n";
}
parsedString++;
}


// std::string LSA = localLSAString.substr(0, localLSAString.find(delimiter));
// localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

// std::string FROMADDR = localLSAString.substr(0, localLSAString.find(delimiter));

// localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

// std::string SEQUENCE = localLSAString.substr(0, localLSAString.find(delimiter));

// localLSAString.erase(0, localLSAString.find(delimiter) + delimiter.length());

// std::string OTHERSTRING = localLSAString.substr(0, localLSAString.find(delimiter));

std::cout << LSA << "\n";

std::cout << FROMADDR << "\n";

std::cout << SEQUENCE << "\n";

std::cout << OTHERSTRING << "\n";

int n = OTHERSTRING.length();

n++;

char myString[n];

strcpy(myString, OTHERSTRING.c_str());

 if ( LSADB.find(FROMADDR) == LSADB.end() ) {
///NOT FOUND
map<string, string> seqCOST;

nodeSize++;
std::cout << " not FoUND creating ADDRESS!!!" << "\n";
std::cout << FROMADDR << "\n";
std::cout << " not FoUND creating adress!!!" << "\n";
std::cout << SEQUENCE << "\n";

std::cout << " FOR ADDRESS" << "\n";
std::cout << FROMADDR << "\n";



   //Neighbors.insert(pair<string, string>(neighborToFind,"1"));


 seqCOST.insert(pair<string, string>(SEQUENCE,OTHERSTRING));


LSADB[FROMADDR] = seqCOST;



std::cout << "ADDING NEWLY LEARNED ADDRESS!!" << "\n";
//do {
   
    addressToAdd.push_back(FROMADDR);
  //} 


}else {
///FOUND
map<string, string> seqCOST;

seqCOST = LSADB.find(FROMADDR)->second;

if ( seqCOST.find(SEQUENCE) == seqCOST.end() ) {
///Not found 


std::cout << " not FoUND creating sequence!!!" << "\n";
std::cout << SEQUENCE << "\n";
std::cout << " not FoUND creating sequence!!!" << "\n";
std::cout << SEQUENCE << "\n";

std::cout << " FOR ADDRESS" << "\n";
std::cout << FROMADDR << "\n";




//seqCOST[SEQUENCE] = OTHERSTRING;

seqCOST.insert(pair<string, string>(SEQUENCE,OTHERSTRING));

LSADB[FROMADDR] = seqCOST;
//return;
} else {
///Found Nothing to UPDATE
std::cout << "NOTHING TO UPDATE!!" << "\n";
std::cout << FROMADDR << "\n";
std::cout << "**************** FINISHED LSA PARSE***************************" << "\n";
return false;

}
}

std::ostringstream convertGlobalID;

convertGlobalID << globalMyID;

std::string convertedSource(convertGlobalID.str());

  std::cout << "mystring to loop through" << "\n";
 std::cout << myString << "\n";

char *p = strtok(myString, "|");
while (p) {
    printf ("Token: %s\n", p);

 string LSAString(p);


//LSAString.erase(0, LSAString.find(delimiter) + delimiter.length());

//delimiter = "+";

//std::string SOURCE = LSAString.substr(0, LSAString.find(delimiter));

//LSAString.erase(0, LSAString.find(delimiter) + delimiter.length());

//std::string COST = LSAString.substr(0, LSAString.find(delimiter));


istringstream is(LSAString);
string part;
std::string SRC = "";
std::string CST = "";
int parsedString = 0;
while (getline(is, part, '+')){
  std::cout << "ffsdfsdfds" << "\n";



if (parsedString==0){
  SRC = part;
  cout << part << endl;
} else {
CST = part;
}
parsedString++;
}


std::cout << "PARSED SOURCE FROM LSA!! BEFORE!!" << "\n";

std::cout << SRC << "\n";

std::cout << "PARSED COST FROM LSA!! BEFORE!!" << "\n";


std::cout << CST << "\n";


SRC.erase(std::remove(SRC.begin(), SRC.end(), '|'), SRC.end());

CST.erase(std::remove(CST.begin(), CST.end(), '|'), CST.end());


std::cout << "PARSED SOURCE FROM LSA!! AFTER!!" << "\n";

std::cout << SRC << "\n";

std::cout << "PARSED COST FROM LSA!! AFTER!!" << "\n";


std::cout << CST << "\n";



if (SRC == convertedSource){
//HMMM it does, lets check the our localDB and check if we need to update our neighborDB
std::cout << "SOURCE DOES EQUAL ME" << "\n";

std::string neighborFrom = stripIPAddress(fromAddress);

std::cout << neighborFrom << "\n";

std::string LOCALSOURCECOST = Neighbors[neighborFrom];

std::cout << "LOCAL NEIGHBOR SOURCE COST" << "\n";

std::cout << LOCALSOURCECOST << "\n"; 

std::cout << "LSA COST" << "\n";

std::cout << CST << "\n";

//if (SOURCECOST.length() == 0){

//std::cout << "SOURCE COST IS 0, updating local DB and sending out fresh LSA" << "\n";

if (LOCALSOURCECOST != CST){

std::cout << "HOUSTION, MY NEIGHBOR HAS PROBLEMS" << "\n";

stringstream localsourceC(LOCALSOURCECOST); 
  
    // The object has the value 12345 and stream 
    // it to the integer x 
    int lsc = 0; 
    localsourceC >> lsc; 

stringstream LSAC(CST);
int lsac = 0;

LSAC >> lsac;


std::cout << "Printing LOCAL NEIGHBORS" << "\n";


std::cout << lsc << "\n";

std::cout << "VS LSAC" << "\n";


std::cout << lsac << "\n";

if (lsac > lsc){

std::cout << "THE LSA IS different, lets update our neighbor DB, and we need to send out a notification" << "\n";


Neighbors[neighborFrom] = CST; 

sendLSA("","");

}else {

std::cout << "THE LSA IS different, hopefully our neighbors fix this on the next run.." << "\n";


sendLSA("","");

}

}


//Neighbors[neighborFrom] = 

//}

} else {

  N++;
}
/////TODO PROPAGATE GRAPH


p = strtok(NULL, "|");
}


propagateGraph();

dijkstra();

std::cout << "**************** FINISHED LSA PARSE***************************" << "\n";

return true; 

}


string returnAddressFromQueue(string INCOMING){

      std::string delimiter = "-";
string buffString(INCOMING);
        
//std::cout << "Address from queue parsing!!" << "\n";

//std::cout << INCOMING << "\n";


       std::string token = buffString.substr(0, buffString.find(delimiter)); 

//buffString.erase(0, buffString.find(delimiter) + delimiter.length());
//token = buffString.substr(0, buffString.find(delimiter));
  //  buffString.erase(0, buffString.find(delimiter) + delimiter.length());


//std::cout << "Address from queue parsing!!" << "\n";

std::cout << token << "\n";

return token;
}


void loadDatafromFile(string initalTopFile){

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
    
    
    

}

void processData(string initalTopFile, string logFileName)
{
  //struct arg_struct *args = arguments;
	char fromAddr[100];
	struct sockaddr_in theirAddr;
	socklen_t theirAddrLen;
	unsigned char recvBuf[1024];
		int bytesRecvd;
    //string initalTopFile = args->TopFile;
//printf("Top File!! %s\n", initalTopFile);
std::cout << initalTopFile << "\n";
 

    //printf("%d\n", args->TopFile);
    //printf("%d\n", args ->FileName);
    loadDatafromFile(initalTopFile);



 
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
	//	memset(recvBuf,'\0', 1024);
		//theirAddrLen = sizeof(theirAddr);
		//if ((bytesRecvd = recvfrom(globalSocketUDP, recvBuf, 1000 , 0, 
			//s		(struct sockaddr*)&theirAddr, &theirAddrLen)) == -1)
		//{
			//perror("connectivity listener: recvfrom failed");
			//exit(1);
		//}
		
		//inet_ntop(AF_INET, &theirAddr.sin_addr, fromAddr, 100);
		
    map<string, string>::iterator element;
        
       
        
        for ( element = PROCESS_QUEUE.begin(); element != PROCESS_QUEUE.end(); element++ ){
            
             //  std::cout << "PROCESS QUEUE STARTING" << "\n";

    //    std::cout << element->first  // string (key)
     //   << ':'
       // << element->second   // string's value
        //<< std::endl ;


        // strcpy(recvBuf, element->messageContent.c_str());
           
            strcpy(fromAddr,  returnAddressFromQueue(element->first).c_str());
       // std::cout << "output from process:" << "\n";
             // std::cout << element->second << "\n";
		short int heardFrom = -1;
		if(strstr(fromAddr, "10.1.1.") || strstr(fromAddr, "10.0.0."))
		{

			heardFrom = atoi(
					strchr(strchr(strchr(fromAddr,'.')+1,'.')+1,'.')+1);
			
		//char* b = reinterpret_cast<char *>( &recvBuf );
			string buff(element->second);
			//TODO: this node can consider heardFrom to be directly connected to it; do any such logic now.
			//printf("BuFF: %s\n", recvBuf);
			  if(buff.find("HEREIAM") != string::npos)
                {
			std::string mainString;
			 //printf("From address!!: %s\n", fromAddr);
			 //printf("BuFF: %s\n", recvBuf);
			string neighborToFind(fromAddr);
			neighborToFind = stripIPAddress(neighborToFind);
			 //std::cout << "IP ADDRESS" << "\n";
			//std::cout << neighborToFind << "\n";
			if ( Neighbors.find(neighborToFind) == Neighbors.end() ) {
			//NotFound, need to get info, record info
			std::cout << "Neighbor not found!!" << "\n";
			 std::cout << neighborToFind << "\n";
			Neighbors.insert(pair<string, string>(neighborToFind,"1"));
			
			sendLSA("","");
			
			//Need to broadcast the information to neighbors
			} else {
  			//Found, thats great. Do we need to check for their neighbors?
			//std::cout << "FOUND!!" << "\n";
			//std::cout << neighborToFind << "\n";
			//if (sequenceNum == 0){
			//std::cout << "Sequence number does equal 0, need to inform neighbors that I know" << "\n";

			//sendLSA();
        //printf("INCOMING LSA! tessting!!!!");
			//}
			}

                }else if (buff.find("send") != string::npos){
                    
                    std::cout << "PPRTINGITNG /send" << "\n";
                       std::string delimiter = "-";
                    string tempBuff = buff;
                         buff.erase(0, buff.find(delimiter) + delimiter.length());
                        std::string Message = buff.substr(0, buff.find(delimiter));
                        buff.erase(0, buff.find(delimiter) + delimiter.length());
                     std::cout << "Message for SEND" << "\n";
                     std::cout << Message << "\n";
                    
                    std::string Node = buff.substr(0, buff.find(delimiter));
                      cout << "Value of NODE : " << "\n";
                    cout << Node << "\n";
                    
                    stringstream g(Node);
                                    
                                      // The object has the value 12345 and stream
                                      // it to the integer x
                                      int dest = 0;
                                      g >> dest;
                                    
                                      // Now the variable x holds the value 12345
                           cout << "Value of x girl friend : " << "\n";
                     cout << dest << "\n";
                    
                    char logLine[512];
                    
                    
                  
                             if (dest == globalMyID){
                                      std::cout << "ID IS HERE, should contiunue to parse" << "\n";

                                        sprintf(logLine, "receive packet message %s\n", Message.c_str());
                             } else {
                                 
                                 
                                 
                                 
                                 
                                     std::cout << "ID IS NOT HERE, should try and figure out next hop" << "\n";
                                               if (  NextHop.find(Node) == NextHop.end() ) {
                                                 std::cout << "NODE NOT FOUND SEND!!" << "\n";
                                                 std::cout << Node << "\n";
                                                   
                                                   sprintf(logLine, "unreachable dest %d\n", dest);
                                   // not found
                                 } else {
                                     string addressToSend = NextHop.find(Node)->second;
                                              std::cout <<  NextHop.find(Node)->second << "\n";
                                              std::cout << "FOUND NEXT HOP FOR SEND SHOULD BE" << "\n";
                                               std::cout << addressToSend << "\n";

                                 
                                  
                                     cout << "Log file!!! " << "\n";
                                    cout << logFileName << "\n";
                                    
                                     
                                     
                                     stringstream gg(addressToSend);
                                                                      
                                                                        // The object has the value 12345 and stream
                                                                        // it to the integer x
                                        int nexthop = 0;
                                        gg >> nexthop;
                                                                      
                                                                        // Now the variable x holds the value 12345
                                        cout << "Value of dest : " << "\n";
                                        cout << nexthop << "\n";
                                                       
                                     
                                     
                                     
                                     if(strstr(fromAddr, "10.0.0.")){
                                         ///Coming from manager
                                         
                                         sprintf(logLine, "sending packet dest %d nexthop %d message %s\n", dest, nexthop, Message.c_str());
                                       
                                         
                                     }else if (strstr(fromAddr, "10.1.1.")){
                                         ///Coming from node
                                           sprintf(logLine, "forward packet dest %d nexthop %d message %s\n", dest, nexthop, Message.c_str());
                                     }
                                     
                                   // found
                                               
                                
                                                std::string address = "10.1.1.";
                                        cout << "FBefore BUFF" << "\n";
                                     cout << tempBuff << "\n";
                               
                                    // tempBuff = ReplaceAll(tempBuff, Node, addressToSend);
                                      cout << "After BUFF" << "\n";
                                     cout << tempBuff << "\n";
                                                    address.append(addressToSend);
                                     cout << "Forwarding MESAGGE LINE!!" << "\n";
                                
                                                sendADDRES(tempBuff,address);
                                               
                                               
                                 }
                                               //FILE *my_file;
                                                //my_file = fopen(logFileName.c_str(),"wb");
                                 
                           
                                
                             }
                        std::string buffAsStdStr = logLine;
                    ofstream myfile;
                    myfile.open (logFileName.c_str(), std::ios_base::app | std::ios_base::out);
                    myfile << buffAsStdStr;
                                                     //myfile.close();
                memset(logLine,0, sizeof(logLine));
                }else if (buff.find("ACK") != string::npos){

         std::cout << "ACK Coming IN" << "\n";

           std::string delimiter = "-";
string buffString(buff);
         std::cout << buffString << "\n";
       std::string token = buffString.substr(0, buffString.find(delimiter)); 

buffString.erase(0, buffString.find(delimiter) + delimiter.length());
token = buffString.substr(0, buffString.find(delimiter)); 
        
        string tokenString(token);

stringstream ACKNUM(tokenString);


          int sourceIDToCompare;
               ACKNUM >> sourceIDToCompare;


  if (sourceIDToCompare == globalMyID){

  std::cout << "ACK FOUND ITS HOME!!!!" << "\n";


  } else {


 if (  NextHop.find(token) == NextHop.end() ) {
                std::cout << "ACK NOT FOUND Coming IN!!!!" << "\n";

                std::cout << token << "\n";
  // not found
} else {
    string addressToSend = NextHop.find(token)->second;
             std::cout <<  NextHop.find(token)->second << "\n";

              std::cout << addressToSend << "\n";

            // sendLSA(addressToSend,trans);



             sendACK(addressToSend,token,"FORWARDLSATO");
  // found
}


  }



             


      }else if (buff.find("FORWARDLSATO") != string::npos){
          ///SEND ACK-FORWARDLSATO

               std::cout << "INCOMING FORWARD LSA!!!" << "\n";
               string buffString(buff);
                std::cout << buffString << "\n";
                std::string delimiter = "-";
std::string token = buffString.substr(0, buffString.find(delimiter)); 

buffString.erase(0, buffString.find(delimiter) + delimiter.length());
token = buffString.substr(0, buffString.find(delimiter)); 

 std::cout << token << "\n";




stringstream sourceWeight(token);
            
          int sourceToCompare;
               sourceWeight >> sourceToCompare;


               if (sourceToCompare == globalMyID){
                  std::cout << "ID IS HERE, should contiunue to parse" << "\n";

                   std::size_t pos = buffString.find("LSA-");
                  std::string LSAMESSAGE = buffString.substr (pos); 
                  std::cout << LSAMESSAGE << "\n";
                  std::string t = LSAMESSAGE.substr(0, LSAMESSAGE.find(delimiter)); 

                  LSAMESSAGE.erase(0, LSAMESSAGE.find(delimiter) + delimiter.length());

                  t = LSAMESSAGE.substr(0, LSAMESSAGE.find(delimiter)); 
                  std::string address = "10.1.1.";
                  address.append(t);
                  std::cout << "PARSING LSA TO ADDRESS!!!" << "\n";
                        std::cout << address << "\n";
                        std::cout << "WITH MESSAGE" << "\n";
                        std::cout << LSAMESSAGE << "\n";
              parseThroughLSANumber(LSAMESSAGE,address);

              if (  NextHop.find(t) == NextHop.end() ) {
                std::cout << "ACK NOT FOUND FORWARDLSATO!!!!" << "\n";
                std::cout << t << "\n";
  // not found
} else {
    string addressToSend = NextHop.find(t)->second;
             std::cout <<  NextHop.find(t)->second << "\n";

              std::cout << addressToSend << "\n";

            // sendLSA(addressToSend,trans);

             sendACK(addressToSend,token,LSAMESSAGE);
  // found
}
               }else {


                if (  NextHop.find(token) == NextHop.end() ) {
                std::cout << "NOt sound!!!" << "\n";
  // not found
} else {
    string addressToSend = NextHop.find(token)->second;
             std::cout <<  NextHop.find(token)->second << "\n";
              std::cout << "FORWARDING LSA TO !!!" << "\n";
              
                 std::string address = "10.1.1.";

                  address.append(addressToSend);
                  std::cout << addressToSend << "\n";
              sendADDRES(buffString,addressToSend);
             //sendLSA(addressToSend);
  // found
}


               }


            }else if (buff.find("LSA") != string::npos){
			//Parse through the information & check if its a neighbor. 


string neighborToFind(fromAddr);
      neighborToFind = stripIPAddress(neighborToFind);
       //std::cout << "IP ADDRESS" << "\n";
      //std::cout << neighborToFind << "\n";
      if ( Neighbors.find(neighborToFind) == Neighbors.end() ) {
      //NotFound, need to get info, record info
      std::cout << "Neighbor not found!!" << "\n";
       std::cout << neighborToFind << "\n";
      Neighbors.insert(pair<string, string>(neighborToFind,"1"));
      
      sendLSA("","");

    }
			printf("INCOMING LSA!!!");

      printf("%s\n", recvBuf);
					
			bool shouldSendLSAToNeighbors = parseThroughLSANumber(string(buff),string(fromAddr));

			if (true){
			 printf("Forwarding LSA neighbors!!@@!!!");
        if (shouldSendLSAToNeighbors){
        forwardLSAtoNeighbors(string(fromAddr), string(buff));
      }
      if (addressToAdd.size() != 0){
         printf("NEED TO SEND OUT LSA TO THESE ADDRESSSES:");

         for (vector<string>::iterator t=addressToAdd.begin(); t!=addressToAdd.end(); ++t) 
    {
        cout<<*t<<endl;

        string trans(t->c_str());

        //graphTRANS.find(j)->second
      std::cout << "CHECKING TRANSLATION" << "\n";

        int g = graphTRANSIN.find(trans)->second;
//dist[i]std::cout << neighborToFind << "\n";
        std::cout << g << "\n";

          std::cout << graphTRANS.find(g)->second << "\n";
          std::cout << g << "\n";
            std::cout << "Next hop should be:" << "\n";

            if (  NextHop.find(trans) == NextHop.end() ) {
                std::cout << "NOt sound!!!" << "\n";
  // not found
} else {
    string addressToSend = NextHop.find(trans)->second;
             std::cout <<  NextHop.find(trans)->second << "\n";

              std::cout << addressToSend << "\n";

             sendLSA(addressToSend,trans);
  // found
}
           
          //NextHop.find(trans);
         // NextHop.insert()
        //printPathForDest(g);
    }
      addressToAdd.clear(); 
      }
}
		
		
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
            
         
            
            PROCESS_QUEUE.erase(element->first);
    }
	}
	//(should never reach here)
	close(globalSocketUDP);
}

