//  Hello World client 

#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
	//  Socket to talk to server 
	char sndBuffer [] = { "hello" };
	char rcvBuffer [32] = { 0 };
	void *context = zmq_ctx_new ();
	void *client = zmq_socket (context, ZMQ_REQ);
	int rc = zmq_connect (client, "tcp://localhost:5555");
	int i;

	assert (rc == 0);

	for(i = 0; i < 10; ++i) {
		printf ("send hello: %d\n", i);
		zmq_send (client, sndBuffer, 5, 0);

		zmq_recv (client, rcvBuffer, 10, 0);
		printf ("Received world: %d\n\n", i);
	}

	zmq_close(client);
	zmq_ctx_destroy(context);
	
	return 0;
}
