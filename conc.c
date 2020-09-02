//
// Created by joy on 2020/8/20.
//


#include "conc.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "call.h"
#include "utils.h"





void openConnection(int port) {

    int server_fd;

    struct sockaddr_in server_addr_in;



    //
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() error. Failed to initiate a socket");
        exit(1);
    }



    //进行初始化服务器端配置

    memset(&server_addr_in, 0, sizeof(struct sockaddr_in));//全部初始化为0

    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_port = htons(port);
    server_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(server_fd, (struct sockaddr *) &server_addr_in, sizeof(server_addr_in)) == -1) {
        perror("bind() failed");
        exit(1);
    }

    printf("the server started...\n");
    printf("listen on port:%d\n", port);
    printf("waiting for client...\n");


    if (listen(server_fd, 128) == -1) {
        perror("listen failed");
        exit(1);
    }

    //接受请求，创建新线程去处理
    while (1){



        struct sockaddr_in * pSockAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

        socklen_t socket_id = sizeof(struct sockaddr_in);

        int client_fd = accept(server_fd, (struct sockaddr *) pSockAddr, &socket_id);

        if(client_fd == -1){
            perror("连接失败");

            free(pSockAddr);
            close(server_fd);
            exit(1);
        }


        int * p_client_fd = (int *)malloc(sizeof(int));
        *p_client_fd = client_fd;


        pthread_t * ppthread = (pthread_t *)malloc(sizeof(pthread_t));

        if(pthread_create(ppthread, NULL, handle, p_client_fd) != 0){
            perror("创建线程失败！！！");

            free(ppthread);
            close(client_fd);
        }
    }


}




/**
 * 简单通信协议v1.0
 *
 *
 * client       ------->       server
 *
 * xxx xxx\n      ----->
 * success/fail url\n   <------
 *
 *
 * axel -n 20 url
 *
 * 过程结束
 *
 */
void *handle(void *arg) {

    int client_fd = *((int*)arg);

    printf("当前fd:%d",client_fd);

    char buf[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];

    char * p_download_url;
    char * p_file_name;

    char * strp = NULL;

    int count = 0;
    int index = 0;


    while (1){

        //进行初始化
        memset(buf,0,sizeof(buf));
        memset(buf2,0,sizeof(buf2));


        p_download_url = NULL;
        p_file_name = NULL;
        strp = NULL;
        count = 0;
        index = 0;


        if((count = read(client_fd,buf,BUFFER_SIZE)) == -1){

            close(client_fd);
            exit(1);
        }


        buf[count] = '\0';


        rtrim(buf,'\n');
        rtrim(buf,'\r');


        printf("开始下载%s",buf);

        for (int i = 0; i < count; ++i) {
            if(buf[i] == ' '){
                index = i;
                break;
            }
        }

        if(index == 0){
            perror("输入格式错误");
            continue;
        }


        strp = buf;

        p_download_url = strsep(&strp, " ");
        p_file_name = strp;


        if(p_download_url == NULL || p_file_name == NULL){
            perror("p_download_url or p_file_name is null");
            continue;
        }


        printf("\ndownload_url:%s download_file_name:%s\n",p_download_url,p_file_name);


        if(callSystemCommand(p_download_url,p_file_name) == 0){

            //success url

            sprintf(buf2,"success http://172.86.70.211/%s",p_file_name);

            write(client_fd,buf2,strlen(buf2));

        }else{

            //fail null
            sprintf(buf2,"fail null");

            write(client_fd,buf2,strlen(buf2));
        }



    }

}



