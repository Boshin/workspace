//Task worker
//Connect PULL socket to tcp://localhost:5557
//Collects workloads from ventilator via that socket
//Connect PUSH socket to tcp://localhost:5558
//Sends results to sink via that socket

#include <zhelpers.h>

int main(int argc, char *argv[])
{
	//Socket to receive messages on
	void *context = zmq_ctx_new();
	void *receiver = zmq_socket(context, ZMQ_PULL);
	zmq_connect(receiver, "tcp://localhost:5557");

	//Socket to send messages to
	void *sender = zmq_socket(context, ZMQ_PUSH);
	zmq_connect(sender, "tcp://localhost:5558");

	//Process tasks forever
	while(1){
		char *str = s_recv(receiver);
		printf("%s\n", str);
		fflush(stdout);
		s_sleep(atoi(str));
		free(str);
		s_send(sender, "");
	}

	zmq_close(receiver);
	zmq_close(sender);
	zmq_ctx_destroy(context);

	return 0;
}
