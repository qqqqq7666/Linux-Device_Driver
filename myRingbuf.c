#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "myRingbuf.h"

#define DEVICE_PATH "/dev/ringbuffer"

static int flag = -1;

int open_ringbuf() {
	int flag = open(DEVICE_PATH, O_RDWR | O_CREAT, S_IRWXU);
	if(flag == -1){
		fprintf(stderr, "Failed to open device\n");
		return -1;
	}
	else
		printf("Device open\n");
	return 0;
}

int close_ringbuf(){
	if(flag != -1) {
		close(flag);
		flag = -1;
		fprintf(stderr, "Device close\n");
		return 0;
	}
	else
		return -1;
}

int enqueue(const char* buf, int len){
	open_ringbuf();
	if(flag != -1){
		ssize_t ret = write(flag, buf, len);
		if(ret == -1){
			fprintf(stderr, "Failed to enqueue\n");
			return -1;
		}
		else{
			printf("enqueue success\n");
			return 0;
		}
	}
	return -1;
}

int dequeue(char* buf, int len){
	open_ringbuf();
	if(flag != -1){
		ssize_t ret = read(flag, buf, len);
		if(ret == -1){
			fprintf(stderr, "Failed to dequeue\n");
			return -1;
		}
		else{
			printf("dequeue success\n");
			return 0;
		}
	}
	return -1;
}
