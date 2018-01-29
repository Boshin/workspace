//Weather update client
//Connect SUB socket to tcp://localhost:5556
//Collects weather updates and filter avg temp in zipcode

#include <zhelpers.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	void *context = zmq_ctx_new();
	void *subscriber = zmq_socket(context, ZMQ_SUB);
	int ret = zmq_connect(subscriber, "tcp://localhost:5556");

	assert(0 == ret);

	//Subcribe to zipcode, default is NYC, 1001
	char *filter = (argc > 1) ? argv[1] : "10001";
	ret = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, 
							filter, strlen(filter));

	assert(0 == ret);

	int update_nbr;
	long total_temp = 0;
	
	for(update_nbr = 0; update_nbr < 100; ++update_nbr)
	{
		char *str = s_recv(subscriber);
		int zipcode, temperature, relhumidity;
		sscanf(str, " %d %d %d", 
				&zipcode, &temperature, &relhumidity);
		total_temp += temperature;

		printf("update_nbr: %02d, zipcode: %05d, temperature: %03d, relhumidity: %03d\n", 
				update_nbr, zipcode, temperature, relhumidity);

		free(str);

		sleep(1);
	}

	printf("Average tempetature for zipcode '%s' was %dF\n",
			filter, (int)(total_temp / update_nbr));

	zmq_close(subscriber);
	zmq_ctx_destroy(context);

	return 0;
}
