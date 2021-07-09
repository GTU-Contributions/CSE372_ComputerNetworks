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

class Server : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual cMessage *generateNewMessage(int clientID, int msgLeft);
};

Define_Module(Server);

int DEBUG = 1;
int S_totalRouters, S_clientsPerRouter;
int clientID;

void Server::initialize()
{
    // Get info about the numbers of the routers and clients
    S_totalRouters = par("routersNum");
    S_clientsPerRouter = par("clientsNum");

    int clientID;

    // Select random client
    srand(time(NULL));
    clientID = rand()%(S_totalRouters*S_clientsPerRouter);

    // Create message
    cMessage *msg = generateNewMessage(clientID, getParentModule()->par("msgCounter"));

    // Find to which router to send the message
    int port;
    for(port = 0; port < S_totalRouters; ++port){
        if(clientID < S_clientsPerRouter){
            break;
        }
        clientID = clientID - S_clientsPerRouter;
    }

    // Send the message to the destination router
    send(msg, "out", port);
}
void Server::handleMessage(cMessage *msg)
{
    int currentMsgCounter;
    char receivedMsg[50];
    strcpy(receivedMsg, msg->getName());
    sscanf(receivedMsg, "Server, MsgLeft: %d", &currentMsgCounter);

    // Finish the simulation
    if(currentMsgCounter == 0){
        endSimulation();
    }

    int nextClientID;

    // Select random client
    srand(time(NULL));
    nextClientID = rand()%(S_totalRouters*S_clientsPerRouter);

    delete msg;

    // Create the next message
    cMessage *newMsg = generateNewMessage(nextClientID, currentMsgCounter-1);

    // Find to which router to send the message
    int port;
    for(port = 0; port < S_totalRouters; ++port){
        if(nextClientID < S_clientsPerRouter){
            break;
        }
        nextClientID = nextClientID - S_clientsPerRouter;
    }

    // Send the message to the destination router
    send(newMsg, "out", port);
}
cMessage *Server::generateNewMessage(int clientID, int msgLeft)
{
    char msgName[50];
    sprintf(msgName, "Client: %d, MsgLeft: %d", clientID, msgLeft);
    cMessage *msg = new cMessage(msgName);
    return msg;
}
