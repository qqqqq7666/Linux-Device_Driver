#include <linux/kernel.h>
#include <linux/syscalls.h>

#define SIZE 64

static int rear = 0, front = 0, res;
static int32_t queue[SIZE];

SYSCALL_DEFINE1(enqueue, int32_t, val){
	if((rear + 1) % SIZE == front)
		return -1;
	queue[rear++] = val;
	rear %= SIZE;

	return 0;
}

SYSCALL_DEFINE0(dequeue){
	if(front == rear)
		return -1;
	res = queue[front];
	queue[front] = 0;
	front = (front + 1) % SIZE;
	return res;
}
