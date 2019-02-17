#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

void init_queue(struct queue *queue) {

	queue->node = malloc(sizeof(struct queue_node));
	queue->node->next = NULL;

	queue->head = queue->node;
	queue->tail = queue->node;

	pthread_mutex_init(&queue->head_l, NULL);
	pthread_mutex_init(&queue->tail_l, NULL);

	pthread_cond_init(&queue->can_enqueue, NULL);
	pthread_cond_init(&queue->can_dequeue, NULL);
}


void destroy_queue(struct queue *queue) {
	struct queue_node *node;

	pthread_mutex_destroy(&queue->head_l);
	pthread_mutex_destroy(&queue->tail_l);
	
	while(queue->head != NULL) {
		node = queue->head;
		queue->head = queue->head->next;
		free(node);
	}
	node = NULL;

	queue->head = NULL;
	queue->tail = NULL;
	queue->node = NULL;
}

void enqueue(struct queue *queue, int data) {
	struct queue_node *node = malloc(sizeof(struct queue_node));
	node->data = data;
	node->next = NULL;
	
	pthread_mutex_lock(&queue->tail_l);
	queue->tail->next = node;
	queue->tail = node;	
	pthread_mutex_unlock(&queue->tail_l);
}

int dequeue(struct queue *queue) {
	struct queue_node *node, *new_head;
	int res;

	pthread_mutex_lock(&queue->head_l);
	node = queue->head;
	new_head = node->next;
	if (new_head == NULL) {
		pthread_mutex_unlock(&queue->head_l);
		return -1;
	}
	res = new_head->data;
	queue->head = new_head;
	pthread_mutex_unlock(&queue->head_l);
	free(node);
	node = NULL;
	return res;
}
