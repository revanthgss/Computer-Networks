#include "networks.h"

int main(int argc, char** argv)
{
	int lfd1, lfd2, cfd;
	struct sockaddr_in servip1, servip2;
	char buff[MAXLINE];
	time_t ticks;

	lfd1 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip1, sizeof(servip1));
	servip1.sin_family = AF_INET;
	servip1.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip1.sin_port = htons(5000);

	lfd2 = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servip2, sizeof(servip2));
	servip2.sin_family = AF_INET;
	servip2.sin_addr.s_addr = inet_addr("127.0.0.1");
	servip2.sin_port = htons(5001);

	bind(lfd1, (struct sockaddr*)&servip1, sizeof(servip1));
	bind(lfd2, (struct sockaddr*)&servip2, sizeof(servip2));
	
	if(fork()>0)
	{
		close(lfd2);		
		listen(lfd1, LISTENQ);

		char addr[20];
		inet_ntop(AF_INET, &(servip1.sin_addr), addr, 20);

		printf("%s\n", addr);

		while(1)
		{
			cfd = accept(lfd1, (struct sockaddr*)NULL, NULL);

			if(cfd>0)
			{
				if(fork()==0)
				{	
					close(lfd1);
					// write some server function here, currently returns time
					snprintf(buff, sizeof(buff), "Service 1\n");
					write(cfd, buff, strlen(buff));

					close(cfd);
				}
				else
				{
					close(cfd);
				}
			}
		}
	}
	else
	{
		close(lfd1);		
		listen(lfd2, LISTENQ);

		char addr[20];
		inet_ntop(AF_INET, &(servip2.sin_addr), addr, 20);

		printf("%s\n", addr);

		while(1)
		{
			cfd = accept(lfd2, (struct sockaddr*)NULL, NULL);

			if(cfd>0)
			{
				if(fork()==0)
				{	
					close(lfd2);
					// write some server function here, currently returns time
					snprintf(buff, sizeof(buff), "Service 2\n");
					write(cfd, buff, strlen(buff));

					close(cfd);
				}
				else
				{
					close(cfd);
				}
			}
		}
	}

	return 0;
}
