#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROCESS_NUM 2
#define MAX_LIKES 42
#define MAX_INTERVAL 5

int main(int argc, char* argv[])
{	
	int LikesServers[PROCESS_NUM];
	int i;
	for (i = 0; i < PROCESS_NUM; i++)
	{
		LikesServers[i] = fork();
		if (LikesServers[i] == -1) 
		{
			printf("Error with creating process\n");
			exit(1);
		}
		if (LikesServers[i] == 0)
		{	
			srand(time(NULL)^(getpid()<<16));
			int lifespan = 300;
			int interval = (rand() % MAX_INTERVAL) + 1;
			while(lifespan > 0)
			{
				int likes = rand() % MAX_LIKES;
				printf("[%d] Test Likes: %d Test Interval: %d\n", getpid(), likes, interval);
				sleep(interval);
				lifespan -= interval;
			}
			exit(0);
		}
	}

	for (i = 0; i < PROCESS_NUM; i++)
	{
		wait(NULL);
	}


	return 0;
}
