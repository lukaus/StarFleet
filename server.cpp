//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux 
#include <iostream>
#include <string.h>   //strlen 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //close 
#include <arpa/inet.h>    //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <errno.h>
#include <string>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <vector>
#include <cstring>
#include "Ship.h"
#include "Projectile.h"
#include "Protocol.h"

#define SHIP_INTS (15)

using namespace std;    

#define TRUE   1 
#define FALSE  0 
#define PORT 8083 

/*
static std::vector<Ship*> ServerParseShipMessage(char * message, int message_size);
static char* ServerCrunchetizeMeCapn(std::vector<Ship*> shipArr, int& message_size);
static std::vector<Projectile*> ServerParseProjectileMessage(char* message);
static char* ServerSerializeProjectileArray(std::vector<Projectile*> projArr);
*/
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int master_socket;
    int addrlen;
    int new_socket;
    int client_socket[30];
    int max_clients = 30;
    int activity;
    int valread;
    int sd;
    int numConnected;
    int n;  
    int max_sd;  

    struct sockaddr_in address;  
        
    char buffer[1025];  //data buffer of 1K 
        
    //set of socket descriptors 
    fd_set readfds;  
        
    // Hey don't worry about this. Its nuthin
   // char *message = "";  
    
    //initialise all client_socket[] to 0 so not checked 
    for (int i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
        
    //create a master socket 
    if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
    
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    //type of socket created 
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );  
        
    //bind the socket to localhost
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", PORT);  
    
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
        
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");  
    numConnected = 0;
    while(TRUE)  
    {  
        //clear the socket set 
        FD_ZERO(&readfds);  
    
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
            
        //add child sockets to set 
        for (int i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
        
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
            
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {

            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            numConnected++;
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
            
            //send new connection greeting message 
           // if( send(new_socket, message, strlen(message), 0) != strlen(message) )  
           // {  
           //     perror("send");  
           // }  
            
           // puts("Welcome message sent successfully");  
            //add new socket to array of sockets 
            for (int i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                        
                    break;  
                }  
            }  

        }  
            
        //else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];  
            if (FD_ISSET( sd , &readfds))  
            {  

                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 99999)) == 0)  
                {
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                    
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;
                }  
                    
                //Echo back the message that came in to all clients
                else
                {  
                    //doStuff();


                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    //valread is  the number of values read
                    for(int i = valread; i > 0; i--)
                    {
                        buffer[i] = buffer[i-1];
                    }
                    buffer[0] = to_string(sd)[0];
                    buffer[valread+1] = '\0';

                    // ya done now
                    
                    std::vector<Ship*> ships = Protocol::ParseShipMessage(buffer, valread);

                    for (int i = 0; i < max_clients; i++)
                    {
                        if (client_socket[i] != 0)
                            send(client_socket[i] , buffer, strlen(buffer) , 0);
                        cerr << "Error: " << errno << endl;
                    }
                    // celery buffer
                    memset(buffer, 0, sizeof(buffer));
                }  
            }  
        }  
    }  
    close(master_socket);
    for(int i = 0; i < 30; i++)
        close(client_socket[i]);
        
    return 0;  
}  
/*
std::vector<Ship*> ServerParseShipMessage(char * message, int message_size)
{
    std::vector<Ship*> shipArray;
    int message_index = 0;
    char message_type = 'Z';
    memcpy(&message_type, &message[message_index], sizeof(char));
    message_index += sizeof(char);

    int message_length = 0;
    memcpy(&message_length, &message[message_index], sizeof(int));
    message_index += sizeof(int);

    int numberOfShips = 0;
    memcpy(&numberOfShips, &message[message_index], sizeof(int));
    message_index += sizeof(int);

    for(int i = 0; i < numberOfShips; i++)
    {
        Ship* thisShip = new Ship();

        int val;
        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setXpos(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setYpos(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsCur(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setHullPointsMax(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setTargetLock(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
        thisShip->setArmourClass(val);

        memcpy(&val, &message[message_index], sizeof(int));
        message_index += sizeof(int);
       // thisShip->setXpos(val); // attack bonus

        for(int j = 0; j < 4; j++)
        {
            memcpy(&val, &message[message_index], sizeof(int));
            message_index += sizeof(int);
            thisShip->setShieldCur((Shield)j, val);

            memcpy(&val, &message[message_index], sizeof(int));
            message_index += sizeof(int);
            thisShip->setShieldMax((Shield)j, val);
        }
        cerr << thisShip->toString() << endl;
        cerr << endl << endl << endl << endl;
    }

    cerr << "Message len: " << message_length << endl;

    return shipArray;
}
std::vector<Projectile*> ServerParseProjectileMessage(char* message)
{

}

char * ServerCrunchetizeMeCapn(std::vector<Ship*> shipArr, int &message_size)
{
    message_size = sizeof(char) + sizeof(int) + sizeof(int) + (shipArr.size() * (sizeof(int) * SHIP_INTS));
    //cerr << "message_size: " << message_size << endl;
    char* message = new char[message_size];

    char message_type = 'S';
    int message_index = 0;
    int numberOfShips = shipArr.size();

    memcpy(&message[message_index], &message_type, sizeof(char));
    message_index += sizeof(char); // skip to next byte
        
    memcpy(&message[message_index], &message_size, sizeof(int));
    message_index += sizeof(int); // skip to next byte
    
    memcpy(&message[message_index], &numberOfShips, sizeof(int));
    message_index += sizeof(int); // skip to next byte
        

    // prepare message
    for(int i = 0; i < shipArr.size(); i++)
    {
        cout << "i: " << i << endl;        
        int val = shipArr[i]->getXpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        //cout << message_index << endl;
        // 16:   y pos
        val = shipArr[i]->getYpos();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        
        //cout << message_index << endl;
        // 16:   HP (cur)
        val = shipArr[i]->getHullPointsCur();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        //cout << message_index << endl;
        // 16:   HP (tot)
        
        val = shipArr[i]->getHullPointsMax();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        //cout << message_index << endl;
        // 16:   TL
        val = shipArr[i]->getTargetLock();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        //cout << message_index << endl;
        // 16:   AC
        val = shipArr[i]->getArmourClass();
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        //cout << message_index << endl;
        // 16:   attack bonus
        val = (int) 0;
        memcpy(&message[message_index], &val, sizeof(int));
        message_index += sizeof(int);
        // 16*4: current shield values
        //cout << message_index << endl;
        for(int j = 0; j < 4; j++)
        {
            val = shipArr[i]->getShieldCur((Shield)j);
            memcpy(&message[message_index], &val, sizeof(int));
            message_index += sizeof(int);

            val = shipArr[i]->getShieldMax((Shield)j);
            memcpy(&message[message_index], &val, sizeof(int));
            message_index += sizeof(int);
        }
    }
    return message;
}

char* ServerSerializeProjectileArray(std::vector<Projectile*> projArr)
{
    char* message;

    message = new char [1];

    return message;
}
*/