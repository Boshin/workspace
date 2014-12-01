//Task sink
//Bind PULL socket to tcp://localhost:5558
//Collect results from workers via that socket

#include "zhelpers.h"

int main(int argc, char *argv[])
{
	//Prepare our context and socket
	void *context = zmq_ctx_new();
	void *receiver = zmq_socket(context, ZMQ_PULL);

	int ret = zmq_bind(receiver, "tcp://*:5558");
	assert(ret == 0);

	char *str = s_recv(receiver);
	printf("str_1: %s\n", str);
	free(str);

	//Start our clock now
	int64_t start_time = s_clock();

	//Process 100 confimations
	int task_nbr;

	for(task_nbr = 0; task_nbr < 100; ++task_nbr){
		char *str = s_recv(receiver);
		free(str);

		if((task_nbr / 10) * 10 == task_nbr)
			printf(":");
		else
			printf(".");

		fflush(stdout);
	}

	//Calculate and report duration of batch
	printf("\nTotal elapsed time: %d msec\n", (int)(s_clock() - start_time));

	zmq_close(receiver);
	zmq_ctx_destroy(context);

	return 0;
}
