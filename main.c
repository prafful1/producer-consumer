#include <stdio.h>
#include <unistd.h>
#include "queue.h"

#define PRODUCERS 7
#define CONSUMERS 3

void *produce(void *queue) {

	int i = 0;

	while (1) {
		enqueue((struct queue *)queue, i);
		printf("Added [%d] to queue\n", i);
		i = (i+1)%200;

		if (i == 200) {
			pthread_cond_wait(&((struct queue *)queue)->can_enqueue, &((struct queue *)queue)->tail_l);
			pthread_cond_signal(&((struct queue *)queue)->can_dequeue);
		}
	}

	// Control never reaches here
	return NULL;
}

void *consume(void *queue) {

	int res;

	while (1) {
		res = dequeue((struct queue *)queue);
		printf("Removed [%d] from queue\n", res);
		if (res == -1) {
			pthread_cond_wait(&((struct queue *)queue)->can_dequeue, &((struct queue *)queue)->head_l);
			pthread_cond_signal(&((struct queue *)queue)->can_enqueue);
		}
	}

	// Control never reaches here
	return NULL;
}
int main() {

	pthread_t prod_t, cons_t;
	struct queue *queue;
	int res;

	queue = malloc(sizeof(struct queue));

	init_queue(queue);

	pthread_create(&prod_t, NULL, produce, queue);
	pthread_create(&cons_t, NULL, consume, queue);
	
	pthread_exit(0);
}
