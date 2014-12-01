//Task ventilator
//Binds PUSH socket to tcp://localhost:5557
//Sends batch of tasks to workers via that socket

#include <stdio.h>
#include <zhelpers.h>

int main(int argc, char *argv[])
{
	void *context = zmq_ctx_new();

	//socket to send messages on
	void *sender = zmq_socket(context, ZMQ_PUSH);
	int ret = zmq_bind(sender, "tcp://*:5557");
	assert(0 == ret);
	ret = 0;

	//socket to send start of batch message on
	void *sink = zmq_socket(context, ZMQ_PUSH);
	ret = zmq_connect(sink, "tcp://localhost:5558");
	assert(0 == ret);
	
	printf("Press Enter when the workers are ready: ");
	getchar();
	printf("Sending task to workers...\n");

	// The first message is "0" and signals start of batch
	s_send(sink, "0");

	//Initialize random number generator
	srandom((unsigned)time(NULL));

	// Send 100 tasks
	int task_nbr = 0;
	int total_msec = 0;

	for(task_nbr = 0; task_nbr < 100; ++task_nbr) {
		int workload = 0;

		//Random workload from 1 to 100msecs
		workload = randof(100) + 1;
		total_msec += workload;
		char str[10] = {0};

		sprintf(str, "%d", workload);
		s_send(sender, str);
	}

	printf("Total expected cost: %d msec\n", total_msec);

	zmq_close(sink);
	zmq_close(sender);
	zmq_ctx_destroy(context);

	return 0;
}
