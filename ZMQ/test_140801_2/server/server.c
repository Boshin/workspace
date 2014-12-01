//Weather update server
//Bings PUB socket to tcp://*:5556
//Publishs random weather updates

#include <zhelpers.h>

int main(int argc, char *argv[])
{
	void *context = zmq_ctx_new();
	void *publisher = zmq_socket(context, ZMQ_PUB);
	int ret = zmq_bind(publisher, "tcp://*:5556");
	assert(0 == ret);

	//ret = zmq_bind(publisher, "ipc://weather.ipc");
	//assert(0 == ret);

	//Initialize random number generator
	srandom((unsigned)time(NULL));

	while(1) {
		//Get values that will fool the boss
		int zipcode  	= randof(100000);
		int temperature = randof(215) - 80;
		int relhumidity = randof(50) + 10;

		char updateBuff[20];

		sprintf(updateBuff, "%05d %d %d", 
				zipcode, temperature, relhumidity);

		s_send(publisher, updateBuff);
	}

	zmq_close(publisher);
	zmq_ctx_destroy(context);

	return 0;
}
