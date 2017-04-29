#include "task.h"
#include "thread_pool.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
    	{
		printf("usage : %s port\n", argv[0]);
		return 1;
    	}

    	int sockfd, connfd;
   	struct sockaddr_in servaddr, client;
    	int port = atoi(argv[1]);  //获取端口
    	//设置服务端的sockaddr_in
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_port = htons(port);
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	//创建socket
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if(sockfd < 0)
    	{
		printf("socket error\n");
		return 1;
    	}
    	//绑定
    	int ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    	if(ret < 0)
    	{
		printf("bind error\n");
		return 1;
    	}
    	//监听
    	ret = listen(sockfd, 10);
    	if(ret < 0)
    	{
		printf("listen error\n");
		return 1;
    	}
    	//创建线程池
    	threadpool<task> pool(20);
    	pool.start();  //线程池开始运行

    	while(1)
    	{
    		//printf("new connection\n");
		socklen_t len = sizeof(client);
		//接受连接
		connfd = accept(sockfd, (struct sockaddr *)&client, &len);
		task *ta = new task(connfd);
		//向线程池添加任务
		pool.append_task(ta);
    	}
    	return 0;
}