#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <thread>
using namespace std;

// Global variable for the client socket descripter
int clientSd;

// Thread to check for server sending messages
void checkerThread()
{
	// Buffer for the message incoming
    char receivedMessage[1500];
    while (1)
    {
    	// Clear buffer
        memset(receivedMessage, 0, sizeof(receivedMessage));

        // Try to receive a message from the server
        recv(clientSd, (char*)&receivedMessage, sizeof(receivedMessage), 0);

        if(!strcmp(receivedMessage, "exit"))
        {
            cout << "Server has quit the session" << endl;
            exit(0);
        }

        // Print for the client number and the message sent
        cout << "Client " << receivedMessage[0] << ": ";
        for(int i = 1; receivedMessage[i] != '\0'; i++)
        {
            cout << receivedMessage[i];
        }
        cout << endl;
    }
}
int main(int argc, char *argv[])
{
    // Usage
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0); 
    } 

    // Gets the IP and Port
    char *serverIp = argv[1]; 
    int port = atoi(argv[2]);

    // Create a message buffer 
    char msg[1500]; 

    // Setup a socket and connection tools 
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    clientSd = socket(AF_INET, SOCK_STREAM, 0);

    // Try to connect
    int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));

    if(status < 0)
    {
        cout << "Error connecting to server!" << endl;
        exit(0);
    }
    else
    	cout << "Connected to the server!" << endl;

    // Spawn the thread to check for incoming messages from the server
    thread t1(checkerThread);
    
    // For sending messages
    while(1)
    {
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        send(clientSd, (char*)&msg, strlen(msg), 0);

        // Clear the buffer
        memset(&msg, 0, sizeof(msg));
    }

    // Join Thread
    t1.join();
    close(clientSd); // Close Socket
    cout << "Connection closed" << endl;
    return 0;    
}