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

//
// TODO auto-generated type
//
simple Client
{
    parameters:
        int routersNum;
        int clientsNum;
    gates:
        input in;
        output out;
}
simple Router
{
    parameters:
        int routersNum;
        int clientsNum;
    gates:
        input c_in[];	// Clients input
        output c_out[];	// Clients output
        
        input s_in;		// Server input
        output s_out;	// Server output
        
        input r_in[2];	// Neighbour routers in	
        				// 1 index = left, higher index
        				// 0 index = right, lower index
        output r_out[2];// Neighbour routers out
                		// 0 index = left, higher index
        				// 1 index = right, lower index
}
simple Server
{
    parameters:
        int routersNum;
        int clientsNum;
    gates:
        input in[];
        output out[];
}
network HW1
{
    parameters:
        int msgCounter = 29; // Total 30 messages 0-29
    	@display("bgb=700,600");
    submodules:
       server: Server {
            parameters:
                @display("p=350,300");
                routersNum;
                clientsNum;
            gates:
                in[server.routersNum];
                out[server.routersNum];
        }
        router[server.routersNum]: Router {
            parameters:
            	@display("p=225,170,ri,125,125");
            	routersNum = server.routersNum;
            	clientsNum = server.clientsNum;
            gates:
                c_in[server.clientsNum];
                c_out[server.clientsNum];
        }
        client[server.clientsNum*server.routersNum]: Client {
            @display("p=100,40,ri,250,250,50");
            routersNum = server.routersNum;
            clientsNum = server.clientsNum;
        }


    connections:
		// Make routers-clients connections
		for i = 0 .. server.routersNum-1, for j = 0 .. server.clientsNum-1{
		    router[i].c_out[j]	-->	{delay=250ms;}	-->	client[j+(i*server.clientsNum)].in;
		    client[j+(i*server.clientsNum)].out	-->	{delay=250ms;}	-->	router[i].c_in[j];
		}
		
		// Make routers-routers connections
		for i = 0 .. server.routersNum-1{
		    router[i].r_out[0]	-->	{delay=250ms;} 	--> router[(i+1)%server.routersNum].r_in[1];	 
		    router[(i+1)%server.routersNum].r_out[1]	-->	{delay=250ms;} 	-->	router[i].r_in[0]; 
		    
		}
		
		// Make routers-server connections
		for i = 0 .. server.routersNum-1{
	   		server.out[i] 	-->	{delay=250ms;} 	--> router[i].s_in;
	   		router[i].s_out -->	{delay=250ms;}	--> server.in[i];
		}
}






