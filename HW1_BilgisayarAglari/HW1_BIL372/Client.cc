//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


#include <omnetpp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>

using namespace omnetpp;

class Client : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual cMessage *generateNewMessage(int msgCounter);
};

Define_Module(Client);

int C_totalRouters, C_clientsPerRouter;

void Client::initialize()
{
    // Get info about the numbers of the routers and clients
    C_totalRouters = par("routersNum");
    C_clientsPerRouter = par("clientsNum");
}
void Client::handleMessage(cMessage *msg)
{
    int currentMsgCounter, currentClientID;
    char receivedMsg[50];
    strcpy(receivedMsg, msg->getName());
    // The coming message is from another client
    if(strncmp(receivedMsg, "Client", 6) == 0){
        sscanf(receivedMsg, "Client: %d, MsgLeft: %d", &currentClientID, &currentMsgCounter);
    }
    // The coming message is from the server
    else{
        sscanf(receivedMsg, "Server, MsgLeft: %d", &currentMsgCounter);
    }

    delete msg;

    // Finish the simulation
    if(currentMsgCounter == 0){
        endSimulation();
    }

    cMessage *newMsg = generateNewMessage(currentMsgCounter-1);
    send(newMsg, "out");
}
cMessage *Client::generateNewMessage(int msgCounter)
{
    // Get current Client's ID
    int currentClientID;
    char clientName[50];
    strcpy(clientName, this->getFullName());
    sscanf(clientName, "client[%d]", &currentClientID);

    // Seed the RNG
    srand(time(NULL));
    // Select random Client or Server
    int clientID = rand()%(C_totalRouters*C_clientsPerRouter+1);

    // Prevent sending message to itself
    while(clientID == currentClientID){
        clientID = rand()%(C_totalRouters*C_clientsPerRouter+1);
    }

    char msgName[50];
    // If the random generated message will be sent to the Server
    if(clientID == (C_totalRouters*C_clientsPerRouter)){
        sprintf(msgName, "Server, MsgLeft: %d", msgCounter);
    }
    // If the random generated message will be sent to other Client
    else{
        sprintf(msgName, "Client: %d, MsgLeft: %d", clientID, msgCounter);
    }

    cMessage *msg = new cMessage(msgName);
    return msg;
}
