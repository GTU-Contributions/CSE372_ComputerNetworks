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

using namespace omnetpp;

class Router : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void forwardMessage(cMessage *msg);
};

Define_Module(Router);

int R_totalRouters, R_clientsPerRouter;

void Router::initialize()
{
    // Get info about the numbers of the routers and clients
    R_totalRouters = par("routersNum");
    R_clientsPerRouter = par("clientsNum");
}
void Router::handleMessage(cMessage *msg)
{
    forwardMessage(msg);
}
void Router::forwardMessage(cMessage *msg)
{
    int clientID, portID, routerID;

    char routerName[50];
    strcpy(routerName, this->getFullName());
    sscanf(routerName, "router[%d]", &routerID);

    char receivedMsg[50];
    strcpy(receivedMsg, msg->getName());

    // Forward to Server
    if(strncmp(receivedMsg, "Server", 6) == 0){
        send(msg, "s_out");
    }

    // Forward to Client or Router
    else if(strncmp(receivedMsg, "Client:", 7) == 0)
    {
        sscanf(receivedMsg, "Client: %d", &clientID);
        portID = clientID - routerID*R_clientsPerRouter;

        // If the Client is connected to the current Router
        // Forward to the Client directly
        if(portID >= 0 && portID < R_clientsPerRouter){
            send(msg, "c_out", portID);
        }
        // If the Client is connected to the Router with lower index
        // Forward to the Router with lower index
        else if(portID < 0){
            if(portID < (-R_clientsPerRouter*R_totalRouters)/2){
                send(msg, "r_out", 0);
            }
            else{
                send(msg, "r_out", 1);
            }
        }
        // If the Client is connected to the Router with higher index
        // Forward to the Router with higher index
        else if(portID > R_clientsPerRouter){
            if(portID > (R_clientsPerRouter*R_totalRouters)/2){
                send(msg, "r_out", 1);
            }
            else{
                send(msg, "r_out", 0);
            }

        }
    }

}
