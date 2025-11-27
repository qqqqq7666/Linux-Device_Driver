#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define __NR_enqueue 460
#define __NR_dequeue 461

int main(int argc, char* argv[]){
	int32_t val, flag;

	if(!strcmp(argv[1], "enqueue") && argc == 3){
		val = atoi(argv[2]);
		flag = syscall(__NR_enqueue, val);
		if(flag != 0)
			fprintf(stderr, "failed to enqueue\nqueue is full\n");
		else
			printf("enque data %d success\n", val);
	}
	else if(!strcmp(argv[1], "dequeue") && argc == 2){
		flag = syscall(__NR_dequeue);
		if(flag != -1)
			printf("dequeue data %d success\n", flag);
		else
			fprintf(stderr, "failed to dequeue\nqueue is empty\n");
	}
	else{
		fprintf(stderr, "non-valid input\nusage : %s <enqueue | dequeue> [data]\n", argv[0]);
		return -1;
	}
	
	return 0;
}
		
