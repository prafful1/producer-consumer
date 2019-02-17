#include <stdio.h>
#include <unistd.h>
#include "queue.h"

#define PRODUCERS 7
#define CONSUMERS 3

void *produce(void *queue) {

	int i;

	for (i = 0; i < 10; i++) {
		enqueue((struct queue *)queue, i);
		printf("Added [%d] to queue\n", i);
	}

	return NULL;
}

void *consume(void *queue) {

	int res;

	while ((res = dequeue((struct queue *)queue)) != -1)
		printf("Removed [%d] from queue\n", res);

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
	pthread_join(prod_t, NULL);
	pthread_join(cons_t, NULL);
	
	destroy_queue(queue);
	free(queue);
	queue = NULL;

	return 0;
}
