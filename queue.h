#include <pthread.h>
#include <stdlib.h>

struct queue_node {
	int data;
	struct queue_node *next;
};

struct queue {
	struct queue_node *head;
	pthread_mutex_t head_l;
	pthread_cond_t can_enqueue;

	struct queue_node *tail;
	pthread_mutex_t tail_l;
	pthread_cond_t can_dequeue;

	struct queue_node *node;
};

void init_queue(struct queue *queue);
void destroy_queue(struct queue *queue);
void enqueue(struct queue *queue, int data);
int dequeue(struct queue *queue);
