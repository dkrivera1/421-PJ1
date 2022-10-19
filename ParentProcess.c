#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define PROCESS_NUM 10
#define MAX_LIKES 42
#define MAX_INTERVAL 5
#define SERV_LIFESPAN 20
#define SEND_SIZE 11
#define FILEN_BUF 32
#define LEN_ID 2
#define LEN_NUM 3
#define SPACE 7
#define SUCCESS 0
#define ERROR -1
#define READ 0
#define WRITE 1



int main(int argc, char* argv[])
{	
	FILE *parent_log;
	parent_log  = fopen ("/tmp/ParentProcessStatus.log", "w");
	FILE *p_server;
	p_server  = fopen ("/tmp/PrimaryServer.log", "w");
	int LikesServers[PROCESS_NUM];
	int pipes[2];
	
	// Checks if pipe initialization results in error
	if (pipe(pipes) == ERROR)
    {
        fprintf(parent_log, "Error in pipe creation, exit 1\n");
        exit(1);
    }

	int i;
	// Initializing LikeServers through n forks
	for (i = 0; i < PROCESS_NUM; i++)
	{
		LikesServers[i] = fork();
		if (LikesServers[i] == ERROR) 
		{
			printf("Error with creating process\n");
			fprintf(parent_log, "Error in LikeServer%d, exit 1\n", i);
			exit(1);
		}
		if (LikesServers[i] == SUCCESS)
		{	
			close(pipes[READ]);
			// Initialize LikeServer(i) log file
			FILE *like_log;
			char buffer[FILEN_BUF];
			snprintf(buffer, sizeof(char) * FILEN_BUF, "/tmp/LikeServer%i.log", i);
			like_log  = fopen (buffer, "w");

			srand(time(NULL)^(getpid()<<16));
			int lifespan = SERV_LIFESPAN;
			int interval = (rand() % MAX_INTERVAL) + 1;

			// Runs until time runs out
			while (lifespan > 0)
			{
				int rand_likes = rand() % MAX_LIKES;

				// Formatting of server likes -- "Client(i) (number of likes)"
				char add_likes[SEND_SIZE] = "Client";
				char ind[LEN_ID];
				sprintf(ind, "%d", i);
				char like[LEN_NUM];
				sprintf(like, "%d", rand_likes);
				strncat(add_likes, ind, LEN_ID);
				add_likes[SPACE] = ' ';
				strncat(add_likes, like, LEN_NUM);

				printf("[%d] %s\n", getpid(), add_likes);

				// Test if write works and logs, else logs error
				if (write(pipes[WRITE], &add_likes, sizeof(add_likes)) == ERROR)
				{
					fprintf(like_log, "LikesServer%d error writing, exit 1\n", i);
					exit(1);
				}
				else
				{
					fprintf(like_log, "[%ds] LikeServer%d %d\n", SERV_LIFESPAN - lifespan, i, rand_likes);
				}

				sleep(interval);
				lifespan -= interval;
			}
			close(pipes[WRITE]);
			fprintf(parent_log, "LikesServer%d success! exit 0\n", i);
			exit(0);
		}
	}

	// Initializing Primary Likes Server
	close(pipes[WRITE]);
	int total_likes = 0;
	char likes[SEND_SIZE];

	// While read does not return an error
	while(read(pipes[READ], &likes, SEND_SIZE) > 0)
	{
		fprintf(p_server, "%s\n", likes);
		int add_likes = atoi(&likes[8]);
		total_likes += add_likes;
		fprintf(p_server, "Total %d\n", total_likes);
		printf("Total likes is %d\n", total_likes);
	}
	
	

	for (i = 0; i < PROCESS_NUM; i++)
	{
		wait(NULL);
	}


	return 0;
}
