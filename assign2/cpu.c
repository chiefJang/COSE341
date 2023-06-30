#define ROW (100)
#define COL ROW
#include <stdio.h> 
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <sched.h>
#include <stdint.h>
#include <string.h>
#include <sys/syscall.h>

int count = 0;
int calc(){
    int matrixA[ROW][COL];
    int matrixB[ROW][COL];
    int matrixC[ROW][COL];
    int i, j, k;
    for(i=0; i < ROW; i++){
        for(j=0; j < COL; j++){
            for(k=0; k < COL; k++){
                matrixC[i][j] += matrixA[i][j] * matrixB[i][j];
            }
        }
    }

    count++;
    return count;
}
volatile sig_atomic_t interrupted = 0;
void handle_signal(int signum){
	interrupted = 1;
}

struct sched_attr{
	uint32_t size;
	uint32_t sched_policy;
	uint64_t sched_flags;
	int32_t sched_nice;
	uint32_t sched_priority;

	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};

static int sched_setattr(pid_t pid, const struct sched_attr* attr, unsigned int flags){
	return syscall(SYS_sched_setattr, pid, attr, flags);
}


int main(int argc, char* argv[]){
    	int i, num, time, result;
	struct sched_attr attr;

	memset(&attr, 0, sizeof(attr));
	attr.size = sizeof(struct sched_attr);
        //attr.sched_priority = 95;
	attr.sched_priority = 10;
	attr.sched_policy = SCHED_RR;
	result = sched_setattr(getpid(), &attr, 0);
	if(result == -1){
		perror("Error calling sched_setattr.");
	}
	num = atoi(argv[1]);
	time = atoi(argv[2]);
    	int id = 0, arr[100], rc=-1, sum=0;
	printf("Creating Process: %d\n", 0);
	for(i=1; i < num; i++){
        	rc = fork();
		arr[i] = rc;
		if(rc > 0){
       		printf("Creating Process: %d\n", i);
		}
        	if(rc == 0){
             		id = i;
            		break;
       		 }   
    	}
    struct timespec begin, current, start;
    long elapsed_time, time1;
    clock_gettime(CLOCK_MONOTONIC, &begin);
	clock_gettime(CLOCK_MONOTONIC, &start);
	signal(SIGINT, handle_signal);
    while(1){
        clock_gettime(CLOCK_MONOTONIC, &current);
        elapsed_time = ((current.tv_sec - begin.tv_sec) * 1000000000 + (current.tv_nsec - begin.tv_nsec)) / 1000000;
        time1 = ((current.tv_sec - start.tv_sec) * 1000000000 + (current.tv_nsec - start.tv_nsec)) / 1000000;
        if(elapsed_time >= 100){
            printf("PROCESS #%d count = %d %ld\n",id, count, elapsed_time);
            begin = current;
			sum += count;
			count = 0;
        }
        if(time1 >= time * 1000 || interrupted){
            break;
        }

        count = calc();
    }
   

    printf("DONE!! PROCESS #%d : %d %ld\n", id, sum, time1);
    if(rc > 0){
  		for(i=1; i < num; i++){
			int status;
			waitpid(arr[i], &status, 0);
		}
	}
	
    return 0;
}
